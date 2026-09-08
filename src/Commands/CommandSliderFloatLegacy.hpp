#pragma once
#include "Commands/CommandLegacy.hpp"

#include <functional>
#include <utility>

namespace Stand
{
	class CommandSliderFloatLegacy : public CommandLegacy
	{
	protected:
		virtual void OnChange() {};
		virtual void OnCall() override;
		virtual void SaveState(nlohmann::json& value) override;
		virtual void LoadState(nlohmann::json& value) override;

		float m_State = 0;
		std::optional<float> m_Min;
		std::optional<float> m_Max;
		std::function<std::pair<float, float>()> m_DynamicRange;

	public:
		CommandSliderFloatLegacy(std::string name, std::string label, std::string description, std::optional<float> min = std::nullopt, std::optional<float> max = std::nullopt, float def_val = 0.0f);
		float GetState();
		void SetState(float state);
		std::optional<float> GetMinimum();
		std::optional<float> GetMaximum();
		void SetDynamicRange(std::function<std::pair<float, float>()> dynamicRange);
	};
}