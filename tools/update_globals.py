#!/usr/bin/env python3
"""
update_globals.py — Sync ScriptGlobal.hpp #define / SOUP_TUNABLE values
                    with GTA V Enhanced decompiled scripts.

Usage:
    python3 update_globals.py \
        --header  src/Scripting/ScriptGlobal.hpp \
        --scripts /path/to/GTA-V-Decompiled-Scripts-Enhanced/decompiled_scripts \
        [--output  out/ScriptGlobal.hpp]     # default: overwrite --header
        [--dry-run]                          # print changes, do not write

Algorithm per standalone #define NAME VALUE  (whole-number RHS, not an expression):
  1. Verified map  — hand-checked against element-size analysis → applied directly.
  2. If VALUE found in Enhanced scripts            → keep VALUE, mark 'found'.
  3. Else search [-200, +3000] for best candidate:
       score = total_refs * log(1+num_scripts) / (1 + abs(delta)/200)
       (proximity bonus so +3 beats a distant popular global)
  4. No candidate → leave VALUE, mark 'unchanged'.

Skipped automatically:
  • Lines inside  #pragma region ScriptGenie_Tunables  (these are tunable INDEX
    offsets from GLOBAL_TUNABLE_BASE, not absolute Global_ addresses).
  • SOUP_TUNABLE field-offset values < 10 000  (struct member indices).
"""

import re, os, sys, math, argparse
from collections import Counter, defaultdict

SCRIPTS_EXTS = {'.c'}

# Hand-verified: old absolute value → confirmed Enhanced value.
# Evidence: array element-size matching in freemode.c, ScriptGlobals.cpp port,
# or high ref counts in targeted scripts.
VERIFIED_MAP: dict[int, int] = {
    # GPBD_* (element-size verified in freemode.c)
    1845298: 1845347,   # GPBD_FM           elm 884
    1882652: 1882797,   # GPBD_FM_2         elm 321
    1892925: 1893070,   # GPBD_FM_3         elm 615
    1912667: 1912812,   # GPBD_FM_4         elm 322
    1877238: 1877383,   # GPBD_FM_HeistPlanning elm 77
    1873491: 1873636,   # GPBD_MissionName
    1874004: 1874149,   # GPBD_Kicking
    1928305: 1928782,   # GPBD_SCTV         elm 199
    1983730: 1985024,   # GPBD_Flow         elm 149
    1981269: 1982548,   # GPBD_HeistIsland  elm 53
    1980404: 1980409,   # GPBD_NetHeistPlanningGeneric
    # GSBD_* (all +3, confirmed by elm sizes)
    2649161: 2649164,   # GSBD
    2650425: 2650428,   # GSBD_BlockB
    2652581: 2652584,   # GSBD_BlockC
    2655292: 2655295,   # GSBD_MissionRequest
    2655949: 2655952,   # GSBD_MissionList
    2656159: 2656162,   # GSBD_ExclusionAreas
    2656384: 2656387,   # GSBD_ActivitySelector
    2657306: 2657309,   # GSBD_Betting
    2657887: 2657890,   # GSBD_HoldUp
    2658058: 2658061,   # GSBD_FM_events2
    1835555: 1835603,   # GSBD_FM
    1877205: 1877350,   # GSBD_Kicking
    1882200: 1882345,   # GSBD_RandomEvents
    1935929: 1936066,   # GSBD_HeistPlanning
    1943836: 1944313,   # GSBD_PropertyInstances
    1922845: 1923118,   # GSBD_FM_events / GlobalServerBD_FM_events
    4515360: 4516837,   # GSBD_SyncedInteractions
    4514687: 4515492,   # GPBD_Interactions
    # GlobalplayerBD / MPGlobals block (+3)
    2658293: 2658296,   # GlobalplayerBD
    2673273: 2673276,   # MPGlobals
    2733190: 2733326,   # MPGlobalsAmbience
    # Misc confirmed
    2700281: 2700299,   # g_AMC_playerBD
    2640100: 2640101,   # g_TransitionSpawnData
    2685152: 2685155,   # g_sTransitionSessionData
    2647284: 2647287,   # GLOBAL_BLIPS
    1950702: 1951218,   # g_SimpleInteriorData
    1679116: 1679169,   # GLOBAL_TIMERS
    1677465: 1677608,   # GLOBAL_MAINTRANSID
    1677555: 1677605,   # GLOBAL_PLAYERSTATUS1_BASE
    1836511: 1836803,   # GLOBAL_LBD_PLAYER_COUNT
    1836747: 1837086,   # GLOBAL_RECLAIM_DISABLED
    1964670: 1965230,   # GLOBAL_NANO_DRONE (top global in am_mp_drone.c)
    1964632: 1965192,   # GLOBAL_NANO_DRONE_NET_TIMER (38 below NANO_DRONE)
    1972286: 1972752,   # GLOBAL_PASSIVE_KILL_COOLDOWN
    1583771: 1583778,   # GLOBAL_PVS (= MPSV in ScriptGlobals.cpp)
    2712186: 2712196,   # GLOBAL_AM_PI_MENU_SUBMENU
    2712187: 2712197,   # GLOBAL_AM_PI_MENU_OPTION_ENABLED (+1 from SUBMENU)
    # Derived defines with inline C comments (regex can't parse expressions)
    1892925: 1893070,   # GLOBAL_PLAYERREMOTE_BASE (= GPBD_FM_3)
    1845298: 1845347,   # GLOBAL_PLAYER_STAT_BASE  (= GPBD_FM)
    2658293: 2658296,   # GLOBAL_PLAYERSTATUS2_BASE (= GlobalplayerBD)
    881:     884,        # GLOBAL_PLAYER_STAT_SIZE (elm_size changed 881→884)
    # Globals.hpp standalone entries
    1970058: 1970385,   # HEIST_PACIFIC_STANDARD (1554 refs in 308 scripts)
}

# Values to never update (no confident match found).
SKIP_VALUES: set[int] = {
    1976846,  # GPBD_CasinoHeist — uncertain; 1978125 already used for HEIST_CAYO_PERICO
}

FIELD_OFFSET_THRESHOLD = 10_000  # SOUP_TUNABLE field offsets


def build_index(scripts_dir: str) -> tuple[Counter, dict]:
    total: Counter[int] = Counter()
    by_script: dict[int, set] = defaultdict(set)
    for fname in os.listdir(scripts_dir):
        if not any(fname.endswith(e) for e in SCRIPTS_EXTS):
            continue
        fpath = os.path.join(scripts_dir, fname)
        try:
            with open(fpath, 'r', errors='replace') as f:
                content = f.read()
        except OSError:
            continue
        seen = set()
        for m in re.finditer(r'Global_(\d+)', content):
            v = int(m.group(1))
            total[v] += 1
            seen.add(v)
        for v in seen:
            by_script[v].add(fname)
    return total, by_script


def candidate_score(val: int, delta: int, total: Counter, by_script: dict) -> float:
    refs = total.get(val, 0)
    scripts = len(by_script.get(val, set()))
    proximity = 1.0 / (1.0 + abs(delta) / 200.0)
    return refs * math.log1p(scripts) * proximity


_ALREADY_ENHANCED: frozenset[int] = frozenset(VERIFIED_MAP.values())

def find_enhanced(old_val: int, total: Counter, by_script: dict) -> tuple[int, str]:
    if old_val in SKIP_VALUES:
        return old_val, 'skip-manual'
    if old_val in VERIFIED_MAP:
        return VERIFIED_MAP[old_val], 'verified'
    if old_val in _ALREADY_ENHANCED:
        return old_val, 'already-enhanced'
    if old_val < FIELD_OFFSET_THRESHOLD:
        return old_val, 'skip-offset'
    if total.get(old_val, 0) > 0:
        return old_val, 'found'

    best_val, best_score = old_val, -1.0
    for delta in range(-200, 3001):
        v = old_val + delta
        if v <= 0:
            continue
        s = candidate_score(v, delta, total, by_script)
        if s > best_score:
            best_score = s
            best_val = v

    if best_score <= 0:
        return old_val, 'unchanged'
    return best_val, 'heuristic'


# ── Regex patterns ────────────────────────────────────────────────────────────
DEFINE_BASE_RE = re.compile(r'^(#define\s+\w+\s+(?:/\*[^*]*\*/\s*)?)(\d+)\s*$')
TUNABLE_RE = re.compile(r'^(inline\s+SOUP_TUNABLE\s*\([^)]+\)\s*=\s*)(\d+)\s*;')
PRAGMA_REGION_RE = re.compile(r'#pragma\s+region\s+(\w+)')
PRAGMA_ENDREGION_RE = re.compile(r'#pragma\s+endregion')

# Regions whose defines are relative offsets — skip them
SKIP_REGIONS = {'ScriptGenie_Tunables'}


def update_header(src: str, total: Counter, by_script: dict) -> tuple[str, list]:
    lines = src.splitlines(keepends=True)
    out, changes = [], []
    in_skip_region = False

    for line in lines:
        stripped = line.rstrip('\n')

        m_region = PRAGMA_REGION_RE.search(stripped)
        m_endregion = PRAGMA_ENDREGION_RE.search(stripped)
        if m_region:
            in_skip_region = m_region.group(1) in SKIP_REGIONS
        elif m_endregion:
            in_skip_region = False

        if in_skip_region:
            out.append(line)
            continue

        m_base = DEFINE_BASE_RE.match(stripped)
        m_tun = TUNABLE_RE.match(stripped)

        if m_base:
            prefix, old_str = m_base.group(1), m_base.group(2)
            old_val = int(old_str)
            new_val, conf = find_enhanced(old_val, total, by_script)
            if new_val != old_val:
                changes.append((old_val, new_val, conf, stripped))
            out.append(f'{prefix}{new_val}\n')

        elif m_tun:
            prefix, old_str = m_tun.group(1), m_tun.group(2)
            old_val = int(old_str)
            new_val, conf = find_enhanced(old_val, total, by_script)
            if new_val != old_val:
                changes.append((old_val, new_val, conf, stripped))
            out.append(f'{prefix}{new_val};\n')

        else:
            out.append(line)

    return ''.join(out), changes


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--header',  required=True)
    ap.add_argument('--scripts', required=True)
    ap.add_argument('--output',  default=None)
    ap.add_argument('--dry-run', action='store_true')
    args = ap.parse_args()

    out_path = args.output or args.header

    print(f'[update_globals] Scanning {args.scripts} ...', file=sys.stderr)
    total, by_script = build_index(args.scripts)
    print(f'[update_globals] {len(total)} unique globals in index', file=sys.stderr)

    with open(args.header, 'r') as f:
        src = f.read()

    updated, changes = update_header(src, total, by_script)

    if changes:
        print(f'\n{"Old":>12}  {"New":>12}  {"Conf":<22}  Name')
        print('-' * 90)
        for old, new, conf, line in changes:
            name = re.search(r'(?:#define\s+|SOUP_TUNABLE\([^,]+,\s*)(\w+)', line)
            name_str = name.group(1) if name else '?'
            print(f'{old:>12}  {new:>12}  {conf:<22}  {name_str}')
    else:
        print('No changes needed.')

    if not args.dry_run:
        with open(out_path, 'w') as f:
            f.write(updated)
        print(f'\n[update_globals] Written → {out_path}', file=sys.stderr)
    else:
        print('\n[dry-run] No files written.', file=sys.stderr)


if __name__ == '__main__':
    main()
