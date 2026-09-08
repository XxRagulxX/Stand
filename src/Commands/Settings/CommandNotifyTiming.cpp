#include "Commands/CommandSliderLegacy.hpp"
#include "Rendering/NotifySettings.hpp"

#include <algorithm>
#include <cstdint>

namespace Stand::Features
{
	// Ported from real Stand's own CommandNotifyReadSpeed (Commands/
	// Online/CommandListNotifySettings.cpp on origin/stand-reference) -
	// see Notifications.cpp's own EstimateReadingTimeMs() for the formula
	// this feeds.
	class CommandNotifyReadSpeed : public CommandSliderLegacy
	{
	public:
		CommandNotifyReadSpeed() :
		    CommandSliderLegacy("readspeed", "Reading Speed (WPM)", "How many words per minute a notification's own display duration assumes you read at.", 0, 1000, 250)
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
			Rendering::NotifySettings::kReadingSpeedWpm = static_cast<std::uint16_t>(std::max(0, GetState()));
		}
	};

	static CommandNotifyReadSpeed _NotifyReadSpeed{};

	class CommandNotifyReadStartDelay : public CommandSliderLegacy
	{
	public:
		CommandNotifyReadStartDelay() :
		    CommandSliderLegacy("readstartdelay", "Reading Start Delay (ms)", "A fixed delay added to every notification's own computed reading time, before it starts counting down.", 0, 1000, 500)
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
			Rendering::NotifySettings::kReadingStartDelayMs = GetState();
		}
	};

	static CommandNotifyReadStartDelay _NotifyReadStartDelay{};

	class CommandNotifyMinDuration : public CommandSliderLegacy
	{
	public:
		CommandNotifyMinDuration() :
		    CommandSliderLegacy("notifyminduration", "Min Duration (ms)", "A notification's own computed reading time is never shown for less than this long.", 0, 60000, 2000)
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
			Rendering::NotifySettings::kMinDurationMs = GetState();
		}
	};

	static CommandNotifyMinDuration _NotifyMinDuration{};

	class CommandNotifyMaxDuration : public CommandSliderLegacy
	{
	public:
		CommandNotifyMaxDuration() :
		    CommandSliderLegacy("notifymaxduration", "Max Duration (ms)", "A notification's own computed reading time is never shown for longer than this.", 0, 60000, 10000)
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
			Rendering::NotifySettings::kMaxDurationMs = GetState();
		}
	};

	static CommandNotifyMaxDuration _NotifyMaxDuration{};
}
