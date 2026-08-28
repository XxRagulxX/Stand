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
# STAND_DIRECTXTK_SHADERS below for the exact filename and entry point
# each lib/directxtk/shaderscompiled/*.inc comes from), and configuring
# with this ON recompiles every one of them with fxc (the D3D shader
# compiler) - writing the resulting .inc into that folder - on both
# native Windows and the Linux -> Windows cross build (via Wine, the
# same way cmake/cross-compile.cmake already runs the rest of the MSVC
# toolchain on Linux). Leave it OFF to just use the .inc files as they
# are; no shader compiler needed at all.
#
# STAND_DIRECTXTK_SHADERS is the full list of shaders to produce - it's
# authoritative on its own (taken straight from DirectXTK's own
# Src/Shaders/CompileShaders.cmd), not derived from whatever already
# happens to exist in lib/directxtk/shaderscompiled/. That's what makes
# it possible to regenerate that folder from a clean/empty state, not
# just refresh files already sitting there.
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
# Every shader technique Stand's compiled subset of DirectXTK needs
# ------------------------------------------------------------
# Each entry is "<hlsl source, without extension>;<entry point>;<fxc
# target profile>" - e.g. the first one below means: compile
# lib/directxtk/shaders/AlphaTestEffect.hlsl's "VSAlphaTest" entry point
# as vs_4_0_level_9_1, into
# lib/directxtk/shaderscompiled/AlphaTestEffect_VSAlphaTest.inc.
#
# Taken directly from DirectXTK's own Src/Shaders/CompileShaders.cmd
# (same order, same profiles - vs/ps_4_0_level_9_1 by default,
# vs/ps_4_0 for PostProcess, ToneMap, and the handful of
# EnvironmentMapEffect entries its own script marks SM4-only), trimmed
# to the effects Stand's CMakeLists.txt actually compiles (no NormalMap/
# PBR/Debug - see DIRECTXTK_CPP_FILES). If DirectXTK adds a new
# technique to one of these effects, or Stand starts compiling another
# effect, its entry needs adding here too - EXISTS-checking every
# lib/directxtk/shaders/*.hlsl this list expects (see below) will catch
# a source file with no matching entry, but not the other way around.
set(STAND_DIRECTXTK_SHADERS
    "AlphaTestEffect\;VSAlphaTest\;vs_4_0_level_9_1"
    "AlphaTestEffect\;VSAlphaTestNoFog\;vs_4_0_level_9_1"
    "AlphaTestEffect\;VSAlphaTestVc\;vs_4_0_level_9_1"
    "AlphaTestEffect\;VSAlphaTestVcNoFog\;vs_4_0_level_9_1"
    "AlphaTestEffect\;PSAlphaTestLtGt\;ps_4_0_level_9_1"
    "AlphaTestEffect\;PSAlphaTestLtGtNoFog\;ps_4_0_level_9_1"
    "AlphaTestEffect\;PSAlphaTestEqNe\;ps_4_0_level_9_1"
    "AlphaTestEffect\;PSAlphaTestEqNeNoFog\;ps_4_0_level_9_1"
    "BasicEffect\;VSBasic\;vs_4_0_level_9_1"
    "BasicEffect\;VSBasicNoFog\;vs_4_0_level_9_1"
    "BasicEffect\;VSBasicVc\;vs_4_0_level_9_1"
    "BasicEffect\;VSBasicVcNoFog\;vs_4_0_level_9_1"
    "BasicEffect\;VSBasicTx\;vs_4_0_level_9_1"
    "BasicEffect\;VSBasicTxNoFog\;vs_4_0_level_9_1"
    "BasicEffect\;VSBasicTxVc\;vs_4_0_level_9_1"
    "BasicEffect\;VSBasicTxVcNoFog\;vs_4_0_level_9_1"
    "BasicEffect\;VSBasicVertexLighting\;vs_4_0_level_9_1"
    "BasicEffect\;VSBasicVertexLightingBn\;vs_4_0_level_9_1"
    "BasicEffect\;VSBasicVertexLightingVc\;vs_4_0_level_9_1"
    "BasicEffect\;VSBasicVertexLightingVcBn\;vs_4_0_level_9_1"
    "BasicEffect\;VSBasicVertexLightingTx\;vs_4_0_level_9_1"
    "BasicEffect\;VSBasicVertexLightingTxBn\;vs_4_0_level_9_1"
    "BasicEffect\;VSBasicVertexLightingTxVc\;vs_4_0_level_9_1"
    "BasicEffect\;VSBasicVertexLightingTxVcBn\;vs_4_0_level_9_1"
    "BasicEffect\;VSBasicOneLight\;vs_4_0_level_9_1"
    "BasicEffect\;VSBasicOneLightBn\;vs_4_0_level_9_1"
    "BasicEffect\;VSBasicOneLightVc\;vs_4_0_level_9_1"
    "BasicEffect\;VSBasicOneLightVcBn\;vs_4_0_level_9_1"
    "BasicEffect\;VSBasicOneLightTx\;vs_4_0_level_9_1"
    "BasicEffect\;VSBasicOneLightTxBn\;vs_4_0_level_9_1"
    "BasicEffect\;VSBasicOneLightTxVc\;vs_4_0_level_9_1"
    "BasicEffect\;VSBasicOneLightTxVcBn\;vs_4_0_level_9_1"
    "BasicEffect\;VSBasicPixelLighting\;vs_4_0_level_9_1"
    "BasicEffect\;VSBasicPixelLightingBn\;vs_4_0_level_9_1"
    "BasicEffect\;VSBasicPixelLightingVc\;vs_4_0_level_9_1"
    "BasicEffect\;VSBasicPixelLightingVcBn\;vs_4_0_level_9_1"
    "BasicEffect\;VSBasicPixelLightingTx\;vs_4_0_level_9_1"
    "BasicEffect\;VSBasicPixelLightingTxBn\;vs_4_0_level_9_1"
    "BasicEffect\;VSBasicPixelLightingTxVc\;vs_4_0_level_9_1"
    "BasicEffect\;VSBasicPixelLightingTxVcBn\;vs_4_0_level_9_1"
    "BasicEffect\;PSBasic\;ps_4_0_level_9_1"
    "BasicEffect\;PSBasicNoFog\;ps_4_0_level_9_1"
    "BasicEffect\;PSBasicTx\;ps_4_0_level_9_1"
    "BasicEffect\;PSBasicTxNoFog\;ps_4_0_level_9_1"
    "BasicEffect\;PSBasicVertexLighting\;ps_4_0_level_9_1"
    "BasicEffect\;PSBasicVertexLightingNoFog\;ps_4_0_level_9_1"
    "BasicEffect\;PSBasicVertexLightingTx\;ps_4_0_level_9_1"
    "BasicEffect\;PSBasicVertexLightingTxNoFog\;ps_4_0_level_9_1"
    "BasicEffect\;PSBasicPixelLighting\;ps_4_0_level_9_1"
    "BasicEffect\;PSBasicPixelLightingTx\;ps_4_0_level_9_1"
    "DualTextureEffect\;VSDualTexture\;vs_4_0_level_9_1"
    "DualTextureEffect\;VSDualTextureNoFog\;vs_4_0_level_9_1"
    "DualTextureEffect\;VSDualTextureVc\;vs_4_0_level_9_1"
    "DualTextureEffect\;VSDualTextureVcNoFog\;vs_4_0_level_9_1"
    "DualTextureEffect\;PSDualTexture\;ps_4_0_level_9_1"
    "DualTextureEffect\;PSDualTextureNoFog\;ps_4_0_level_9_1"
    "EnvironmentMapEffect\;VSEnvMap\;vs_4_0_level_9_1"
    "EnvironmentMapEffect\;VSEnvMapBn\;vs_4_0_level_9_1"
    "EnvironmentMapEffect\;VSEnvMapFresnel\;vs_4_0_level_9_1"
    "EnvironmentMapEffect\;VSEnvMapFresnelBn\;vs_4_0_level_9_1"
    "EnvironmentMapEffect\;VSEnvMapOneLight\;vs_4_0_level_9_1"
    "EnvironmentMapEffect\;VSEnvMapOneLightBn\;vs_4_0_level_9_1"
    "EnvironmentMapEffect\;VSEnvMapOneLightFresnel\;vs_4_0_level_9_1"
    "EnvironmentMapEffect\;VSEnvMapOneLightFresnelBn\;vs_4_0_level_9_1"
    "EnvironmentMapEffect\;VSEnvMapPixelLighting\;vs_4_0_level_9_1"
    "EnvironmentMapEffect\;VSEnvMapPixelLightingBn\;vs_4_0_level_9_1"
    "EnvironmentMapEffect\;VSEnvMapPixelLightingSM4\;vs_4_0"
    "EnvironmentMapEffect\;VSEnvMapPixelLightingBnSM4\;vs_4_0"
    "EnvironmentMapEffect\;PSEnvMap\;ps_4_0_level_9_1"
    "EnvironmentMapEffect\;PSEnvMapNoFog\;ps_4_0_level_9_1"
    "EnvironmentMapEffect\;PSEnvMapSpecular\;ps_4_0_level_9_1"
    "EnvironmentMapEffect\;PSEnvMapSpecularNoFog\;ps_4_0_level_9_1"
    "EnvironmentMapEffect\;PSEnvMapPixelLighting\;ps_4_0_level_9_1"
    "EnvironmentMapEffect\;PSEnvMapPixelLightingNoFog\;ps_4_0_level_9_1"
    "EnvironmentMapEffect\;PSEnvMapPixelLightingFresnel\;ps_4_0_level_9_1"
    "EnvironmentMapEffect\;PSEnvMapPixelLightingFresnelNoFog\;ps_4_0_level_9_1"
    "EnvironmentMapEffect\;PSEnvMapSpherePixelLighting\;ps_4_0_level_9_1"
    "EnvironmentMapEffect\;PSEnvMapSpherePixelLightingNoFog\;ps_4_0_level_9_1"
    "EnvironmentMapEffect\;PSEnvMapSpherePixelLightingFresnel\;ps_4_0_level_9_1"
    "EnvironmentMapEffect\;PSEnvMapSpherePixelLightingFresnelNoFog\;ps_4_0_level_9_1"
    "EnvironmentMapEffect\;PSEnvMapDualParabolaPixelLighting\;ps_4_0"
    "EnvironmentMapEffect\;PSEnvMapDualParabolaPixelLightingNoFog\;ps_4_0"
    "EnvironmentMapEffect\;PSEnvMapDualParabolaPixelLightingFresnel\;ps_4_0"
    "EnvironmentMapEffect\;PSEnvMapDualParabolaPixelLightingFresnelNoFog\;ps_4_0"
    "SkinnedEffect\;VSSkinnedVertexLightingOneBone\;vs_4_0_level_9_1"
    "SkinnedEffect\;VSSkinnedVertexLightingOneBoneBn\;vs_4_0_level_9_1"
    "SkinnedEffect\;VSSkinnedVertexLightingTwoBones\;vs_4_0_level_9_1"
    "SkinnedEffect\;VSSkinnedVertexLightingTwoBonesBn\;vs_4_0_level_9_1"
    "SkinnedEffect\;VSSkinnedVertexLightingFourBones\;vs_4_0_level_9_1"
    "SkinnedEffect\;VSSkinnedVertexLightingFourBonesBn\;vs_4_0_level_9_1"
    "SkinnedEffect\;VSSkinnedOneLightOneBone\;vs_4_0_level_9_1"
    "SkinnedEffect\;VSSkinnedOneLightOneBoneBn\;vs_4_0_level_9_1"
    "SkinnedEffect\;VSSkinnedOneLightTwoBones\;vs_4_0_level_9_1"
    "SkinnedEffect\;VSSkinnedOneLightTwoBonesBn\;vs_4_0_level_9_1"
    "SkinnedEffect\;VSSkinnedOneLightFourBones\;vs_4_0_level_9_1"
    "SkinnedEffect\;VSSkinnedOneLightFourBonesBn\;vs_4_0_level_9_1"
    "SkinnedEffect\;VSSkinnedPixelLightingOneBone\;vs_4_0_level_9_1"
    "SkinnedEffect\;VSSkinnedPixelLightingOneBoneBn\;vs_4_0_level_9_1"
    "SkinnedEffect\;VSSkinnedPixelLightingTwoBones\;vs_4_0_level_9_1"
    "SkinnedEffect\;VSSkinnedPixelLightingTwoBonesBn\;vs_4_0_level_9_1"
    "SkinnedEffect\;VSSkinnedPixelLightingFourBones\;vs_4_0_level_9_1"
    "SkinnedEffect\;VSSkinnedPixelLightingFourBonesBn\;vs_4_0_level_9_1"
    "SkinnedEffect\;PSSkinnedVertexLighting\;ps_4_0_level_9_1"
    "SkinnedEffect\;PSSkinnedVertexLightingNoFog\;ps_4_0_level_9_1"
    "SkinnedEffect\;PSSkinnedPixelLighting\;ps_4_0_level_9_1"
    "SpriteEffect\;SpriteVertexShader\;vs_4_0_level_9_1"
    "SpriteEffect\;SpritePixelShader\;ps_4_0_level_9_1"
    "DGSLEffect\;main\;vs_4_0_level_9_1"
    "DGSLEffect\;mainVc\;vs_4_0_level_9_1"
    "DGSLEffect\;main1Bones\;vs_4_0_level_9_1"
    "DGSLEffect\;main1BonesVc\;vs_4_0_level_9_1"
    "DGSLEffect\;main2Bones\;vs_4_0_level_9_1"
    "DGSLEffect\;main2BonesVc\;vs_4_0_level_9_1"
    "DGSLEffect\;main4Bones\;vs_4_0_level_9_1"
    "DGSLEffect\;main4BonesVc\;vs_4_0_level_9_1"
    "DGSLUnlit\;main\;ps_4_0_level_9_1"
    "DGSLLambert\;main\;ps_4_0_level_9_1"
    "DGSLPhong\;main\;ps_4_0_level_9_1"
    "DGSLUnlit\;mainTk\;ps_4_0_level_9_1"
    "DGSLLambert\;mainTk\;ps_4_0_level_9_1"
    "DGSLPhong\;mainTk\;ps_4_0_level_9_1"
    "DGSLUnlit\;mainTx\;ps_4_0_level_9_1"
    "DGSLLambert\;mainTx\;ps_4_0_level_9_1"
    "DGSLPhong\;mainTx\;ps_4_0_level_9_1"
    "DGSLUnlit\;mainTxTk\;ps_4_0_level_9_1"
    "DGSLLambert\;mainTxTk\;ps_4_0_level_9_1"
    "DGSLPhong\;mainTxTk\;ps_4_0_level_9_1"
    "PostProcess\;VSQuad\;vs_4_0"
    "PostProcess\;PSCopy\;ps_4_0"
    "PostProcess\;PSMonochrome\;ps_4_0"
    "PostProcess\;PSSepia\;ps_4_0"
    "PostProcess\;PSDownScale2x2\;ps_4_0"
    "PostProcess\;PSDownScale4x4\;ps_4_0"
    "PostProcess\;PSGaussianBlur5x5\;ps_4_0"
    "PostProcess\;PSBloomExtract\;ps_4_0"
    "PostProcess\;PSBloomBlur\;ps_4_0"
    "PostProcess\;PSMerge\;ps_4_0"
    "PostProcess\;PSBloomCombine\;ps_4_0"
    "ToneMap\;VSQuad\;vs_4_0"
    "ToneMap\;PSCopy\;ps_4_0"
    "ToneMap\;PSSaturate\;ps_4_0"
    "ToneMap\;PSReinhard\;ps_4_0"
    "ToneMap\;PSACESFilmic\;ps_4_0"
    "ToneMap\;PS_SRGB\;ps_4_0"
    "ToneMap\;PSSaturate_SRGB\;ps_4_0"
    "ToneMap\;PSReinhard_SRGB\;ps_4_0"
    "ToneMap\;PSACESFilmic_SRGB\;ps_4_0"
    "ToneMap\;PSHDR10\;ps_4_0"
)

# ------------------------------------------------------------
# stand_compile_directxtk_shaders(target compiled_dir)
# ------------------------------------------------------------
# Adds one add_custom_command() per entry in STAND_DIRECTXTK_SHADERS,
# compiling it into compiled_dir, and makes `target` depend on all of
# them so they're rebuilt before anything that #includes one gets
# compiled. Works regardless of what (if anything) already exists in
# compiled_dir - including nothing at all, e.g. right after clearing it
# out to force a full regeneration.

function(stand_compile_directxtk_shaders target compiled_dir)
    set(shader_src_dir "${compiled_dir}/../shaders")
    get_filename_component(shader_src_dir "${shader_src_dir}" ABSOLUTE)

    file(MAKE_DIRECTORY "${compiled_dir}")

    # Shared *.fxh fragments (Common.fxh, Lighting.fxh, ...) that the
    # top-level *.hlsl files #include - fxc resolves these itself by
    # searching the including file's own directory (same as every
    # top-level .hlsl sits in here), no /I flag needed. Listed as an
    # extra dependency on every shader below so changing a shared
    # fragment recompiles everything that could be affected, rather than
    # only the one .hlsl whose mtime CMake happens to be watching.
    file(GLOB shared_fxh_headers CONFIGURE_DEPENDS "${shader_src_dir}/*.fxh")

    set(missing_hlsl "")
    set(all_incs "")

    foreach(shader_spec IN LISTS STAND_DIRECTXTK_SHADERS)
        list(GET shader_spec 0 source)
        list(GET shader_spec 1 entry_point)
        list(GET shader_spec 2 profile)

        set(stem "${source}_${entry_point}")
        set(hlsl_path "${shader_src_dir}/${source}.hlsl")
        set(inc_path "${compiled_dir}/${stem}.inc")

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
