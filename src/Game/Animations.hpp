#pragma once
#include "Core/types.hpp"

#include <cstdint>
#include <vector>

namespace Stand
{
	enum class ForcedMovementType : uint8_t
	{
		NONE,
		FORCE_STILL,
		FORCE_MOVEMENT
	};

	struct SimpleAnimation
	{
		const char* display_name;
		const char* raw_name;
		ForcedMovementType movement_override = ForcedMovementType::NONE;
	};

	struct ComplexAnimation
	{
		const char* display_name;
		const char* anim_dict;
		const char* anim_name;
		ForcedMovementType movement_override = ForcedMovementType::NONE;
	};

	struct ObjectData
	{
		Hash model;
		int bone;
		float ox, oy, oz;
		float rx, ry, rz;
	};

	struct ComplexObjectAnimation : ComplexAnimation
	{
		ObjectData object_data;
	};

	namespace PedBoneId
	{
		constexpr int SKEL_ROOT       = 0;
		constexpr int SKEL_Spine2     = 24818;
		constexpr int SKEL_Spine3     = 24817;
		constexpr int PH_R_Hand       = 57005;
		constexpr int PH_L_Hand       = 26610;
		constexpr int IK_R_Hand       = 36029;
		constexpr int SKEL_R_Hand     = 28422;
		constexpr int SKEL_L_Hand     = 18905;
		constexpr int SKEL_R_Finger20 = 4137;
		constexpr int SKEL_L_Finger42 = 4538;
	}

	struct Animations
	{
		static const std::vector<SimpleAnimation>          scenarios;
		static const std::vector<ComplexAnimation>         dances;
		static const std::vector<ComplexAnimation>         animations;
		static const std::vector<ComplexObjectAnimation>   object_animations;

		static inline bool   looped                       = true;
		static inline bool   partial_movement             = false;
		static inline bool   last_animation_was_partial   = false;
		static inline Object last_object_attachment       = 0;
		static inline const ComplexAnimation* last_complex_animation = nullptr;

		static void loadDict(const char* dict);
		static void unloadDict(const char* dict);
		static void reset();
	};
}
