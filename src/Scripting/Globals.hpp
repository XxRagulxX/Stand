#pragma once
#include "Scripting/ScriptGlobal.hpp"

namespace Stand::Globals
{
    // ── Phone ─────────────────────────────────────────────────────────────────
    constexpr ScriptGlobal PHONE_CALL_STATE(23040);
    constexpr ScriptGlobal PHONE_CALL_IN_PROGRESS(23046);
    constexpr ScriptGlobal PHONE_INCOMING_CALL(23050);
    constexpr ScriptGlobal PHONE_CALLING_CHARACTER(8818);

    // ── Player Status 2 (MPGlobals) ───────────────────────────────────────────
    constexpr ScriptGlobal PLAYERSTATUS2(MPGlobals);
    constexpr auto PLAYERSTATUS2_OTR_TIMESTAMP              = PLAYERSTATUS2.at(58);

    // ── Freemode Global (mp_fm_events, tunables etc.) ────────────────────────
    constexpr ScriptGlobal FREEMODE_GLOBAL(262145);
    constexpr auto FREEMODE_BIKER_SUPPLIES_DELAY            = FREEMODE_GLOBAL.at(18857);
    constexpr auto FREEMODE_GR_SUPPLIES_DELAY               = FREEMODE_GLOBAL.at(21367);
    constexpr auto FREEMODE_RANDOM_EVENT_COOLDOWN           = FREEMODE_GLOBAL.at(33808);
    constexpr auto FREEMODE_RANDOM_EVENT_AVAILABILITY       = FREEMODE_GLOBAL.at(33809);
    constexpr auto FREEMODE_RANDOM_EVENT_FORCE              = FREEMODE_GLOBAL.at(33811);
    constexpr auto FREEMODE_UNLOCK_CONTENT                  = FREEMODE_GLOBAL.at(23812);
    constexpr auto FREEMODE_UNLOCK_CONTENT2                 = FREEMODE_GLOBAL.at(33811);
    constexpr auto FREEMODE_UNLOCK_SLOT                     = FREEMODE_GLOBAL.at(12027);

    // ── Gun Van ───────────────────────────────────────────────────────────────
    constexpr ScriptGlobal GUN_VAN_VEHICLE(2655288);

    // ── Garage / Property ─────────────────────────────────────────────────────
    constexpr ScriptGlobal GARAGE_SLOT_LOOKUP(1945138);
    constexpr ScriptGlobal PROPERTY_NAME_TABLE(1312440);

    // ── Heists ────────────────────────────────────────────────────────────────
    constexpr ScriptGlobal HEIST_FLEECA(1673820);
    constexpr ScriptGlobal HEIST_PRISON_BREAK(1936406);
    constexpr ScriptGlobal HEIST_HUMANE_LABS(1938374);
    constexpr ScriptGlobal HEIST_KORTZ_CENTER(1935711);
    constexpr ScriptGlobal HEIST_SERIES_A(1969071);
    constexpr ScriptGlobal HEIST_PACIFIC_STANDARD(1970385);
    constexpr ScriptGlobal HEIST_DOOMSDAY_ACT1(1970586);
    constexpr ScriptGlobal HEIST_CASINO(1973762);
    constexpr ScriptGlobal HEIST_CAYO_PERICO(1978125);
    constexpr ScriptGlobal HEIST_APARTMENT(1980496);
    constexpr ScriptGlobal HEIST_DOOMSDAY_ACT2(1980570);
    constexpr ScriptGlobal HEIST_DOOMSDAY_ACT3(1984467);
    constexpr ScriptGlobal HEIST_CONTRACT(1991386);
    constexpr ScriptGlobal HEIST_DOOMSDAY(4718592);

    // ── GPBD_FM fields (base = 1845347, same as GPBD_FM::Get()) ─────────────
    constexpr ScriptGlobal GPBD_FM_HANGAR_CRATES(1845347 + 1 + 260 + 304 + 3);
    constexpr ScriptGlobal GPBD_FM_WAREHOUSE_STOCK(1845347 + 1 + 260 + 128 + 1);

    // ── CEO / Organisation ────────────────────────────────────────────────────
    constexpr ScriptGlobal CEO_GLOBAL(2686124);

    // ── Script-global triggers (wildlife, Lester tasks etc.) ─────────────────
    constexpr ScriptGlobal GLOBAL_2709520(2709520);

    // ── FreemodeGeneral (same base as FreemodeGeneral::Get()) ────────────────
    constexpr ScriptGlobal FREEMODE_GENERAL(2733326);

    // ── Business safe-claim triggers ─────────────────────────────────────────
    constexpr ScriptGlobal SAFE_NIGHTCLUB(2708943);
    constexpr ScriptGlobal SAFE_ARCADE(2708952);
    constexpr ScriptGlobal SAFE_AGENCY(2708961);
    constexpr ScriptGlobal SAFE_SALVAGE_YARD(2708970);
    constexpr ScriptGlobal SAFE_BAIL_OFFICE(2708979);
    constexpr ScriptGlobal SAFE_GARMENT_FACTORY(2708994);
    constexpr ScriptGlobal SAFE_CAR_WASH(2709001);
}
