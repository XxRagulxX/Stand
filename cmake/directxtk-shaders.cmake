# ============================================================
# Stand - DirectXTK shader (re)compilation
# ============================================================
#
# DirectXTK's shaders ship as pre-compiled D3D bytecode, embedded as C
# byte arrays in "*.inc" files under lib/directxtk/shaderscompiled/ (see
# EffectCommon.cpp, EnvironmentMapEffect.cpp, and friends, which
# #include them directly, and reference the array they declare by name -
# e.g. `EnvironmentMapEffect_VSEnvMapOneLightFresnel`). Nobody needs a
# shader compiler just to build Stand normally.
#
# But if you update DirectXTK from upstream and one of its .hlsl shaders
# actually changed, those .inc files need to be regenerated from the new
# source, or Stand keeps silently running the old compiled shader.
# STAND_COMPILE_DIRECTXTK_SHADERS (default OFF) turns that regeneration
# on: place the updated *.hlsl source under lib/directxtk/shaders/ (see
# STAND_DIRECTXTK_SHADER_SOURCES below for the exact filename each
# lib/directxtk/shaderscompiled/*.inc expects), and configuring with
# this ON recompiles every one of them with fxc (the D3D shader
# compiler) - overwriting the checked-in .inc files in that folder in
# place - on both native Windows and the Linux -> Windows cross build
# (via Wine, the same way cmake/cross-compile.cmake already runs the
# rest of the MSVC toolchain on Linux). Leave it OFF to just use the
# .inc files as they are; no shader compiler needed at all.
#
# This intentionally writes back into the source tree (lib/directxtk/
# shaderscompiled/), not the build directory - unusual for a CMake
# generated file, but the point is to produce an updated, committable
# .inc the same way you'd commit any other change to a vendored library.

option(STAND_COMPILE_DIRECTXTK_SHADERS
    "Recompile DirectXTK's shaders from HLSL source (lib/directxtk/shaders/*.hlsl) into lib/directxtk/shaderscompiled/*.inc, instead of using the already-compiled .inc files as they are. Requires fxc (from the Windows SDK; on Linux, also Wine to run it) - leave OFF unless you've changed a shader."
    OFF
)

if(NOT STAND_COMPILE_DIRECTXTK_SHADERS)
    return()
endif()

message(STATUS "STAND_COMPILE_DIRECTXTK_SHADERS is ON - DirectXTK's shaders will be recompiled from lib/directxtk/shaders/*.hlsl.")

# ------------------------------------------------------------
# Locate fxc
# ------------------------------------------------------------
# fxc.exe is a Windows PE binary (part of the Windows SDK, not the VC
# toolset msvc-wine wraps its own scripts for) - it needs Wine to run on
# Linux, same as any other unwrapped .exe.

if(CROSSCOMPILE)
    find_program(STAND_WINE_EXECUTABLE NAMES wine64 wine)

    if(NOT STAND_WINE_EXECUTABLE)
        message(FATAL_ERROR
            "STAND_COMPILE_DIRECTXTK_SHADERS is ON, but Wine wasn't found. "
            "fxc.exe (the D3D shader compiler, from the Windows SDK) is a "
            "native Windows binary and needs Wine to run here. Install "
            "wine64, or turn STAND_COMPILE_DIRECTXTK_SHADERS off."
        )
    endif()

    if(NOT DEFINED WINDOWS_KITS_ROOT)
        message(FATAL_ERROR
            "WINDOWS_KITS_ROOT is not set - this should have been defined "
            "by cmake/cross-compile.cmake before directxtk-shaders.cmake "
            "is included."
        )
    endif()

    file(GLOB STAND_FXC_CANDIDATES "${WINDOWS_KITS_ROOT}/bin/*/x64/fxc.exe")

    if(NOT STAND_FXC_CANDIDATES)
        message(FATAL_ERROR
            "fxc.exe not found under:\n"
            "  ${WINDOWS_KITS_ROOT}/bin/*/x64\n"
            "The Windows SDK installed into msvc-wine needs to include "
            "its \"D3D Compiler\" / \"Graphics Tools\" component for "
            "fxc.exe to be present, or turn STAND_COMPILE_DIRECTXTK_SHADERS off."
        )
    endif()

    list(SORT STAND_FXC_CANDIDATES)
    list(GET STAND_FXC_CANDIDATES -1 STAND_FXC_EXECUTABLE)

    set(STAND_FXC_COMMAND_PREFIX "${STAND_WINE_EXECUTABLE}")

    message(STATUS "fxc.exe: ${STAND_FXC_EXECUTABLE} (via ${STAND_WINE_EXECUTABLE})")
else()
    # Native Windows: fxc is normally already on PATH from a Developer
    # Command Prompt / vcvarsall.bat (which is also what puts INCLUDE/LIB
    # on the environment - see the matching comment in cmake/windows.cmake).
    # Fall back to a Windows SDK install's own layout if it isn't.
    find_program(STAND_FXC_EXECUTABLE
        NAMES fxc fxc.exe
        HINTS
            "$ENV{WindowsSdkVerBinPath}x64"
            "$ENV{WindowsSdkDir}bin/x64"
    )

    if(NOT STAND_FXC_EXECUTABLE)
        file(GLOB STAND_FXC_CANDIDATES
            "$ENV{ProgramFiles\(x86\)}/Windows Kits/10/bin/*/x64/fxc.exe"
            "C:/Program Files (x86)/Windows Kits/10/bin/*/x64/fxc.exe"
        )

        if(STAND_FXC_CANDIDATES)
            list(SORT STAND_FXC_CANDIDATES)
            list(GET STAND_FXC_CANDIDATES -1 STAND_FXC_EXECUTABLE)
        endif()
    endif()

    if(NOT STAND_FXC_EXECUTABLE)
        message(FATAL_ERROR
            "fxc.exe not found. Configure from a Developer Command Prompt "
            "(so the Windows SDK's tools are on PATH), install the Windows "
            "SDK's \"D3D Compiler\" component, or turn "
            "STAND_COMPILE_DIRECTXTK_SHADERS off."
        )
    endif()

    set(STAND_FXC_COMMAND_PREFIX "")

    message(STATUS "fxc.exe: ${STAND_FXC_EXECUTABLE}")
endif()

# ------------------------------------------------------------
# Which .hlsl source file each shader technique comes from
# ------------------------------------------------------------
# Every lib/directxtk/shaderscompiled/*.inc is named "<Source>_<Entry>",
# e.g. EnvironmentMapEffect_VSEnvMapOneLightFresnel.inc is the
# "VSEnvMapOneLightFresnel" technique compiled from
# lib/directxtk/shaders/EnvironmentMapEffect.hlsl. This list is exactly
# the "<Source>" part of every .inc currently checked in; a shader whose
# name doesn't start with one of these (entry point included) needs an
# entry added here first.

set(STAND_DIRECTXTK_SHADER_SOURCES
    AlphaTestEffect
    BasicEffect
    DGSLEffect
    DGSLLambert
    DGSLPhong
    DGSLUnlit
    DualTextureEffect
    EnvironmentMapEffect
    PostProcess
    SkinnedEffect
    SpriteEffect
    ToneMap
)

# Sources that DirectXTK's own Src/Shaders/CompileShaders.cmd always
# compiles at plain SM4 (vs_4_0/ps_4_0), instead of the vs/ps_4_0_level_9_1
# profile (feature-level-9.x-compatible) every other technique below
# uses. EnvironmentMapEffect is a special case of its own: only a handful
# of its own techniques need SM4 (see STAND_DIRECTXTK_SM4_ENTRIES) - the
# rest of it is still 9.1-compatible like everything else.
set(STAND_DIRECTXTK_SM4_ONLY_SOURCES
    PostProcess
    ToneMap
)

set(STAND_DIRECTXTK_SM4_ENTRIES
    VSEnvMapPixelLightingSM4
    VSEnvMapPixelLightingBnSM4
    PSEnvMapDualParabolaPixelLighting
    PSEnvMapDualParabolaPixelLightingNoFog
    PSEnvMapDualParabolaPixelLightingFresnel
    PSEnvMapDualParabolaPixelLightingFresnelNoFog
)

# ------------------------------------------------------------
# stand_compile_directxtk_shaders(target compiled_dir)
# ------------------------------------------------------------
# Adds one add_custom_command() per lib/directxtk/shaderscompiled/*.inc
# that already exists, recompiling it from its .hlsl source, and makes
# `target` depend on all of them so they're rebuilt before anything
# that #includes one gets compiled.

function(stand_compile_directxtk_shaders target compiled_dir)
    set(shader_src_dir "${compiled_dir}/../shaders")
    get_filename_component(shader_src_dir "${shader_src_dir}" ABSOLUTE)

    # Shared *.fxh fragments (Common.fxh, Lighting.fxh, ...) that the
    # top-level *.hlsl files #include - fxc resolves these itself by
    # searching the including file's own directory (same as every
    # top-level .hlsl sits in here), no /I flag needed. Listed as an
    # extra dependency on every shader below so changing a shared
    # fragment recompiles everything that could be affected, rather than
    # only the one .hlsl whose mtime CMake happens to be watching.
    file(GLOB shared_fxh_headers CONFIGURE_DEPENDS "${shader_src_dir}/*.fxh")

    file(GLOB existing_incs CONFIGURE_DEPENDS "${compiled_dir}/*.inc")

    if(NOT existing_incs)
        message(FATAL_ERROR
            "STAND_COMPILE_DIRECTXTK_SHADERS is ON, but no *.inc files "
            "were found under:\n"
            "  ${compiled_dir}\n"
            "There's nothing here to tell us which shaders to recompile."
        )
    endif()

    set(missing_hlsl "")
    set(all_incs "")

    foreach(inc_path IN LISTS existing_incs)
        get_filename_component(stem "${inc_path}" NAME_WE)

        # Longest/only matching "<Source>_" prefix from the table above.
        set(matched_source "")

        foreach(candidate IN LISTS STAND_DIRECTXTK_SHADER_SOURCES)
            if(stem MATCHES "^${candidate}_(.+)$")
                set(matched_source "${candidate}")
                set(entry_point "${CMAKE_MATCH_1}")
            endif()
        endforeach()

        if(NOT matched_source)
            message(FATAL_ERROR
                "Don't know which .hlsl source '${stem}.inc' should come "
                "from - add its source file's name to "
                "STAND_DIRECTXTK_SHADER_SOURCES in cmake/directxtk-shaders.cmake."
            )
        endif()

        # Vertex vs. pixel shader: almost all of DirectXTK's entry points
        # say so themselves (VS.../PS...); the exceptions are DGSLEffect
        # (always the vertex shader, one per skinning bone count) and
        # DGSLLambert/Phong/Unlit (always the pixel shader, one per
        # lighting model) and SpriteEffect's two fixed entry points.
        if(entry_point MATCHES "^VS")
            set(shader_stage "vs")
        elseif(entry_point MATCHES "^PS")
            set(shader_stage "ps")
        elseif(matched_source STREQUAL "DGSLEffect")
            set(shader_stage "vs")
        elseif(matched_source MATCHES "^DGSL(Lambert|Phong|Unlit)$")
            set(shader_stage "ps")
        elseif(entry_point STREQUAL "SpriteVertexShader")
            set(shader_stage "vs")
        elseif(entry_point STREQUAL "SpritePixelShader")
            set(shader_stage "ps")
        else()
            message(FATAL_ERROR
                "Don't know whether '${stem}' is a vertex or pixel shader "
                "- add a case for it in cmake/directxtk-shaders.cmake."
            )
        endif()

        # Feature level: matches Src/Shaders/CompileShaders.cmd exactly -
        # everything is vs/ps_4_0_level_9_1 (feature-level-9.x compatible)
        # except the SM4-only sources and EnvironmentMapEffect's own
        # handful of SM4 entries (both above).
        if(matched_source IN_LIST STAND_DIRECTXTK_SM4_ONLY_SOURCES)
            set(profile "${shader_stage}_4_0")
        elseif(matched_source STREQUAL "EnvironmentMapEffect" AND entry_point IN_LIST STAND_DIRECTXTK_SM4_ENTRIES)
            set(profile "${shader_stage}_4_0")
        else()
            set(profile "${shader_stage}_4_0_level_9_1")
        endif()

        set(hlsl_path "${shader_src_dir}/${matched_source}.hlsl")

        if(NOT EXISTS "${hlsl_path}")
            list(APPEND missing_hlsl "${hlsl_path}")
            continue()
        endif()

        add_custom_command(
            OUTPUT "${inc_path}"
            COMMAND
                ${STAND_FXC_COMMAND_PREFIX} "${STAND_FXC_EXECUTABLE}"
                /nologo /WX /Ges /Zpc /Qstrip_reflect
                "/T${profile}"
                "/E${entry_point}"
                "/Vn${stem}"
                "/Fh${inc_path}"
                "${hlsl_path}"
            DEPENDS "${hlsl_path}" ${shared_fxh_headers}
            COMMENT "Compiling DirectXTK shader ${stem} (${profile})"
            VERBATIM
        )

        list(APPEND all_incs "${inc_path}")
    endforeach()

    if(missing_hlsl)
        list(REMOVE_DUPLICATES missing_hlsl)
        string(REPLACE ";" "\n  " missing_hlsl_display "${missing_hlsl}")
        message(FATAL_ERROR
            "STAND_COMPILE_DIRECTXTK_SHADERS is ON, but these HLSL source "
            "files are missing:\n"
            "  ${missing_hlsl_display}\n"
            "Add them under lib/directxtk/shaders/, or turn "
            "STAND_COMPILE_DIRECTXTK_SHADERS off to keep using the "
            "already-compiled .inc files."
        )
    endif()

    add_custom_target(DirectXTKShaders ALL
        DEPENDS ${all_incs}
    )

    add_dependencies(${target} DirectXTKShaders)
endfunction()
