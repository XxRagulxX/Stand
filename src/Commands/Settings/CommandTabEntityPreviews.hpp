#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Rendering/Theme.hpp"
#include "Menu/Click.hpp"

#include <format>
#include <string>

namespace Stand
{
	class CommandDisableEntityPreviews : public CommandToggle
	{
	public:
		explicit CommandDisableEntityPreviews(CommandList* const parent)
			: CommandToggle(parent, LIT("Disable Entity Previews"), CMDNAMES("disableentitypreviews"),
				NOLABEL, false)
		{
		}

		void onChange(Click& click) override
		{
			Rendering::Theme::kDisableEntityPreviews = m_on;
		}
	};

	class CommandPreviewOpaque : public CommandToggle
	{
	public:
		explicit CommandPreviewOpaque(CommandList* const parent)
			: CommandToggle(parent, LIT("Opaque"), CMDNAMES("previewopaque"),
				NOLABEL, false)
		{
		}

		void onChange(Click& click) override
		{
			Rendering::Theme::kPreviewOpaque = m_on;
		}
	};

	class CommandPreviewRotationSpeed : public CommandSlider
	{
	public:
		explicit CommandPreviewRotationSpeed(CommandList* const parent)
			: CommandSlider(parent, LIT("Rotation Speed"), CMDNAMES("previewrotationspeed"),
				NOLABEL, 0, 400, 100, 1)
		{
		}

		std::string getValueText() const override
		{
			return std::format("{:.2f}", value / 100.f);
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kPreviewRotationSpeed = value / 100.f;
		}
	};

	class CommandPreviewPedDist : public CommandSlider
	{
	public:
		explicit CommandPreviewPedDist(CommandList* const parent)
			: CommandSlider(parent, LIT("Additional Character Distance"), CMDNAMES("previewpeddist"),
				NOLABEL, 0, 500, 0, 1)
		{
		}

		std::string getValueText() const override
		{
			return std::format("{:.2f}", value / 100.f);
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kPreviewPedDist = value / 100.f;
		}
	};

	class CommandPreviewObjDist : public CommandSlider
	{
	public:
		explicit CommandPreviewObjDist(CommandList* const parent)
			: CommandSlider(parent, LIT("Additional Object Distance"), CMDNAMES("previewobjdist"),
				NOLABEL, 0, 1000, 0, 1)
		{
		}

		std::string getValueText() const override
		{
			return std::format("{:.2f}", value / 100.f);
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kPreviewObjDist = value / 100.f;
		}
	};

	class CommandPreviewVehDist : public CommandSlider
	{
	public:
		explicit CommandPreviewVehDist(CommandList* const parent)
			: CommandSlider(parent, LIT("Additional Vehicle Distance"), CMDNAMES("previewvehdist"),
				NOLABEL, 0, 1000, 0, 1)
		{
		}

		std::string getValueText() const override
		{
			return std::format("{:.2f}", value / 100.f);
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kPreviewVehDist = value / 100.f;
		}
	};

	class CommandTabEntityPreviews : public CommandList
	{
	public:
		CommandDisableEntityPreviews* const disable;
		CommandPreviewOpaque* const opaque;
		CommandPreviewRotationSpeed* const rotationSpeed;
		CommandPreviewPedDist* const pedDist;
		CommandPreviewObjDist* const objDist;
		CommandPreviewVehDist* const vehDist;

		explicit CommandTabEntityPreviews()
			: CommandList(nullptr, LIT("Entity Previews"), CMDNAMES())
			, disable(createChild<CommandDisableEntityPreviews>())
			, opaque(createChild<CommandPreviewOpaque>())
			, rotationSpeed(createChild<CommandPreviewRotationSpeed>())
			, pedDist(createChild<CommandPreviewPedDist>())
			, objDist(createChild<CommandPreviewObjDist>())
			, vehDist(createChild<CommandPreviewVehDist>())
		{
		}
	};
}

namespace Stand::Features
{
	Stand::CommandTabEntityPreviews& GetCommandTabEntityPreviews();
}
