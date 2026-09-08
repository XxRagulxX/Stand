#include "Commands/CommandToggleLegacy.hpp"
#include "Commands/CommandSliderLegacy.hpp"
#include "Rendering/NotifySettings.hpp"

namespace Stand::Features
{
	// Ported from real Stand's own CommandNotifyInvertFlow (Commands/
	// Online/CommandListNotifySettings.cpp on origin/stand-reference) -
	// only takes effect while NotifySettings::kType == StandCustomPosition,
	// same as Notify Position above (see Notifications.cpp's own
	// ComputeLayout()).
	class CommandNotifyInvertFlow : public CommandToggleLegacy
	{
	public:
		CommandNotifyInvertFlow() :
		    CommandToggleLegacy("notifyinvertflow",
		        "Invert Flow",
		        "Stacks newer notifications upward from the anchor point instead of downward. Only takes effect with type set to \"Stand, Custom Position\".",
		        false)
		{
		}

	protected:
		void OnEnable() override
		{
			Rendering::NotifySettings::kInvertFlow = true;
		}

		void OnDisable() override
		{
			Rendering::NotifySettings::kInvertFlow = false;
		}
	};

	static CommandNotifyInvertFlow _NotifyInvertFlow{};

	// Ported from real Stand's own CommandNotifyWidth - the toast card's
	// own width, replacing this project's previous hardcoded 350 (see
	// NotifySettings::kWidth's own comment).
	class CommandNotifyWidth : public CommandSliderLegacy
	{
	public:
		CommandNotifyWidth() :
		    CommandSliderLegacy("notifywidth", "Width", "The notification card's own width.", 0, 15000, 400)
		{
		}

	protected:
		void OnChange() override
		{
			Sync();
		}

		void LoadState(nlohmann::json& value) override
		{
			CommandSliderLegacy::LoadState(value);
			Sync();
		}

	private:
		void Sync()
		{
			Rendering::NotifySettings::kWidth = static_cast<float>(GetState());
		}
	};

	static CommandNotifyWidth _NotifyWidth{};

	// Ported from real Stand's own CommandNotifyPadding - the gap between
	// the anchor point/stacked cards.
	class CommandNotifyPadding : public CommandSliderLegacy
	{
	public:
		CommandNotifyPadding() :
		    CommandSliderLegacy("notifypadding", "Padding", "The gap between the anchor point and stacked notification cards.", 0, 15000, 10)
		{
		}

	protected:
		void OnChange() override
		{
			Sync();
		}

		void LoadState(nlohmann::json& value) override
		{
			CommandSliderLegacy::LoadState(value);
			Sync();
		}

	private:
		void Sync()
		{
			Rendering::NotifySettings::kPadding = static_cast<float>(GetState());
		}
	};

	static CommandNotifyPadding _NotifyPadding{};
}
