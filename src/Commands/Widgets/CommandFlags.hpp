#pragma once
#include "Game/typedecl.hpp"

// Real Stand's CommandFlags.hpp also carries flags for systems this
// project doesn't have (feature-list edition gating, saved/starred
// command iteration, the hotkeys-list UI, search) - kept only the ones
// that mean something without those: whether a command's state persists
// (CMDFLAG_SUPPORTS_STATE_OPERATIONS/CMDFLAG_NO_SAVED_STATE, matching
// this project's own CommandToggle/CommandSliderFloat GetState/SetState split),
// CMDFLAG_TEMPORARY, and CMDFLAG_CONCEALED (used internally by Command
// itself). The CMDFLAGS_* per-widget convenience macros are kept under
// the same names so a command ported from Stand (e.g. CMDFLAGS_TOGGLE)
// compiles unchanged, even though most now just resolve to 0 or a single
// flag rather than Stand's own wider combination.
// commandflags_t is defined in Game/typedecl.hpp (size depends on CMDFLAG_BITS).

#define CMDFLAG_TEMPORARY (Stand::commandflags_t)0b1
#define CMDFLAG_SUPPORTS_STATE_OPERATIONS (Stand::commandflags_t)0b10
#define CMDFLAG_NO_SAVED_STATE (Stand::commandflags_t)0b100
#define CMDFLAG_CONCEALED (Stand::commandflags_t)0b1000

#define CMDFLAGS_ACTION 0
#define CMDFLAGS_ACTION_NAME CMDFLAGS_ACTION
#define CMDFLAGS_ACTION_SCRIPT CMDFLAGS_ACTION
#define CMDFLAGS_ACTION_SLIDER 0
#define CMDFLAGS_ACTION_VALUE CMDFLAG_SUPPORTS_STATE_OPERATIONS
#define CMDFLAGS_COLOUR_CUSTOM CMDFLAG_SUPPORTS_STATE_OPERATIONS
#define CMDFLAGS_COLOUR CMDFLAGS_COLOUR_CUSTOM
#define CMDFLAGS_LIST 0
#define CMDFLAGS_LIST_ACTION 0
#define CMDFLAGS_LIST_SELECT CMDFLAG_SUPPORTS_STATE_OPERATIONS
#define CMDFLAGS_READONLY_VALUE 0
#define CMDFLAGS_SLIDER CMDFLAG_SUPPORTS_STATE_OPERATIONS
#define CMDFLAGS_SLIDER_TEXT CMDFLAG_SUPPORTS_STATE_OPERATIONS
#define CMDFLAGS_TOGGLE_NO_CORRELATION CMDFLAG_SUPPORTS_STATE_OPERATIONS
#define CMDFLAGS_TOGGLE CMDFLAGS_TOGGLE_NO_CORRELATION
#define CMDFLAGS_TOGGLE_CUSTOM 0
