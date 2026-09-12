#pragma once
#include "Commands/Self/CommandPtfxTrail.hpp"
#include "Commands/Self/CommandPtfxTrailParticle.hpp"
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandSliderFloat.hpp"
#include "Util/Label.hpp"

namespace Stand
{
	class CommandTabPtfxTrails final : public CommandList
	{
	public:
		explicit CommandTabPtfxTrails(CommandList* parent)
			: CommandList(parent, LIT("PTFX Trails"), CMDNAMES("ptfxtrails"))
		{
			auto* particle       = createChild<CommandPtfxTrailParticle>();
			particle->m_interval = createChild<CommandSlider>(
				LIT("Interval (ms)"), CMDNAMES("trailinterval"), NOLABEL, 0, 1000, 0);
			particle->m_scale    = createChild<CommandSliderFloat>(
				LIT("Scale"), CMDNAMES("trailscale"), NOLABEL, 10, 1000, 15);
			particle->m_offset   = createChild<CommandSliderFloat>(
				LIT("Horizontal Offset"), CMDNAMES("trailoffset"), NOLABEL, -1000, 1000, 0);

			struct BoneEntry { int bone; const char* name; const char* cmd; bool on; };
			static constexpr BoneEntry kBones[] = {
				{31086, "Head",        "trailhead",   false},
				{39317, "Neck",        "trailneck",   false},
				{ 2992, "Right Elbow", "trailrelbow", false},
				{22711, "Left Elbow",  "traillelbow", false},
				{57005, "Right Hand",  "trailrhand",  true},
				{18905, "Left Hand",   "traillhand",  true},
				{11816, "Pelvis",      "trailpelvis", false},
				{36864, "Right Calf",  "trailrcalf",  false},
				{63931, "Left Calf",   "traillcalf",  false},
				{52301, "Right Foot",  "trailrfoot",  true},
				{14201, "Left Foot",   "traillfoot",  true},
			};
			for (const auto& b : kBones)
				particle->m_bones.push_back(
					createChild<CommandPtfxTrail>(b.bone, b.name, b.cmd, b.on));
		}
	};
}
