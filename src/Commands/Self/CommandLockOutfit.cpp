#include "Commands/Self/CommandLockOutfit.hpp"

#include "Commands/Self/CommandHairHighlight.hpp"
#include "Core/types.hpp"
#include "Commands/Self/CommandOutfitComponent.hpp"
#include "Commands/Self/CommandOutfitComponentVariation.hpp"
#include "Commands/Self/CommandOutfitProp.hpp"
#include "Commands/Self/CommandOutfitPropVariation.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

#include <string>

namespace Stand
{
	CommandLockOutfit::CommandLockOutfit(CommandList* parent)
		: CommandToggleNoCorrelation(parent, LIT("Lock Outfit"), CMDNAMES("lockoutfit", "outfitlock"))
	{
		instance = this;
		player_prev_model = 0;
		CommandTickDispatch::AddCommand(this);
	}

	CommandLockOutfit::~CommandLockOutfit()
	{
		CommandTickDispatch::RemoveCommand(this);
		if (instance == this)
			instance = nullptr;
	}

	void CommandLockOutfit::onTick()
	{
		if (instance == nullptr)
			return;

		const Hash curModel = Self::GetPed().GetModel();
		const bool modelChanged = (player_prev_model != curModel);

		if (modelChanged)
		{
			if (player_prev_model != 0 && instance->m_on)
				model_outfits[player_prev_model] = buildCompactOutfit();

			player_prev_model = curModel;

			auto it = model_outfits.find(curModel);
			if (it == model_outfits.end())
			{
				instance->m_on = false;
			}
			else
			{
				instance->m_on = true;
				applyCompactOutfit(it->second);
			}

			const int ped = Self::GetPed().GetHandle();
			for (auto& [id, pair] : ped_component_commands)
				pair.first->setMaxValue(PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(ped, id) - 1);
			for (auto& [id, pair] : ped_prop_commands)
				pair.first->setMaxValue(PED::GET_NUMBER_OF_PED_PROP_DRAWABLE_VARIATIONS(ped, id) - 1);
		}

		if (modelChanged || instance->m_on)
			updateOutfitCommands();
	}

	void CommandLockOutfit::updateOutfitCommands()
	{
		const int ped = Self::GetPed().GetHandle();

		for (auto& [id, pair] : ped_component_commands)
		{
			if (instance->m_on)
			{
				const int curDrawable = PED::GET_PED_DRAWABLE_VARIATION(ped, id);
				const int curTexture  = PED::GET_PED_TEXTURE_VARIATION(ped, id);
				if (curDrawable != pair.first->value || curTexture != pair.second->value)
				{
					Click click(CLICK_AUTO, TC_SCRIPT_NOYIELD);
					pair.second->onChange(click, 0);
				}
			}
			else
			{
				Click click(CLICK_AUTO, TC_SCRIPT_NOYIELD);
				pair.first->setValueIndicator(click, PED::GET_PED_DRAWABLE_VARIATION(ped, id));
				pair.second->setValueIndicator(click, PED::GET_PED_TEXTURE_VARIATION(ped, id));
			}

			if (id != PV_COMP_HAIR)
				pair.second->updateRange();
		}

		for (auto& [id, pair] : ped_prop_commands)
		{
			if (instance->m_on)
			{
				const int curDrawable = PED::GET_PED_PROP_INDEX(ped, id, 1);
				const int curTexture  = PED::GET_PED_PROP_TEXTURE_INDEX(ped, id);
				if (curDrawable != pair.first->value || (pair.first->value != -1 && curTexture != pair.second->value))
				{
					Click click(CLICK_AUTO, TC_SCRIPT_NOYIELD);
					if (pair.first->value == -1)
						pair.first->onChange(click, 0);
					else
						pair.second->onChange(click, 0);
				}
			}
			else
			{
				Click click(CLICK_AUTO, TC_SCRIPT_NOYIELD);
				pair.first->setValueIndicator(click, PED::GET_PED_PROP_INDEX(ped, id, 1));
				pair.second->setValueIndicator(click, PED::GET_PED_PROP_TEXTURE_INDEX(ped, id));
			}
		}
	}

	void CommandLockOutfit::onClick(Click& click)
	{
		click.ensureScriptThread([this](Click& click)
		{
			if (!m_on)
				updateOutfitCommands();
			else
				model_outfits.erase(Self::GetPed().GetModel());
			toggleState(click);
		});
	}

	std::string CommandLockOutfit::getState() const
	{
		std::string state{};
		auto snapshot = model_outfits;
		if (m_on && player_prev_model != 0)
			snapshot[player_prev_model] = buildCompactOutfit();
		for (const auto& [model, outfit] : snapshot)
		{
			state.append(std::to_string(model)).push_back('<');
			for (std::size_t i = 0; i < outfit.size(); ++i)
			{
				state.append(std::to_string(outfit[i]));
				if (i + 1 < outfit.size())
					state.push_back(':');
			}
			state.push_back('>');
		}
		return state;
	}

	std::string CommandLockOutfit::getDefaultState() const
	{
		return {};
	}

	void CommandLockOutfit::setState(Click& click, const std::string& state)
	{
		m_on = false;
		model_outfits.clear();
		if (state.empty())
		{
			player_prev_model = 0;
			return;
		}
		std::string s = state;
		std::size_t modelDelPos;
		while ((modelDelPos = s.find('>')) != std::string::npos)
		{
			std::string modelData = s.substr(0, modelDelPos);
			s.erase(0, modelDelPos + 1);
			const std::size_t outfitDelPos = modelData.find('<');
			const auto model = (Hash)std::stoull(modelData.substr(0, outfitDelPos));
			modelData.erase(0, outfitDelPos + 1);
			model_outfits[model] = {};
			std::size_t partDelPos;
			std::size_t i = 0;
			while ((partDelPos = modelData.find(':')) != std::string::npos)
			{
				model_outfits[model][i++] = (std::int16_t)std::stoi(modelData.substr(0, partDelPos));
				modelData.erase(0, partDelPos + 1);
			}
			if (!modelData.empty())
				model_outfits[model][i++] = (std::int16_t)std::stoi(modelData);

			while (i < COMPACT_OUTFIT_LEGACY_SIZE)
			{
				model_outfits[model][i] = (std::int16_t)((i % 2) ? 0 : -1);
				++i;
			}
			while (i < COMPACT_OUTFIT_SIZE)
				model_outfits[model][i++] = -1;
		}
		player_prev_model = 0;
	}

	void CommandLockOutfit::applyDefaultState()
	{
		m_on = false;
		model_outfits.clear();
		player_prev_model = 0;
	}

	void CommandLockOutfit::setHairColourFromCommands()
	{
		const auto& hairPair = ped_component_commands[PV_COMP_HAIR];
		const int colour = hairPair.second->value;
		int highlight = hair_highlight->value;
		if (highlight == -1)
			highlight = colour;
		PED::SET_PED_HAIR_TINT(Self::GetPed().GetHandle(), colour, highlight);
	}

	std::array<std::int16_t, COMPACT_OUTFIT_SIZE> CommandLockOutfit::buildCompactOutfit()
	{
		std::array<std::int16_t, COMPACT_OUTFIT_SIZE> out{};
		std::size_t i = 0;
		for (const auto& c : ped_components)
		{
			const auto& pair = ped_component_commands[c.id];
			out[i++] = (std::int16_t)pair.first->value;
			out[i++] = (std::int16_t)pair.second->value;
		}
		for (const auto& p : ped_props)
		{
			const auto& pair = ped_prop_commands[p.id];
			out[i++] = (std::int16_t)pair.first->value;
			out[i++] = (std::int16_t)pair.second->value;
		}
		out[i] = (std::int16_t)hair_highlight->value;
		return out;
	}

	void CommandLockOutfit::applyCompactOutfit(const std::array<std::int16_t, COMPACT_OUTFIT_SIZE>& outfit)
	{
		const int ped = Self::GetPed().GetHandle();
		std::size_t i = 0;
		for (const auto& c : ped_components)
		{
			const int drawable = (int)outfit[i++];
			const int texture  = (int)outfit[i++];
			const auto& pair = ped_component_commands[c.id];
			Click click(CLICK_AUTO, TC_SCRIPT_NOYIELD);
			pair.first->setValueIndicator(click, drawable);
			pair.second->setValueIndicator(click, texture);
			if (c.id == PV_COMP_HAIR)
			{
				PED::SET_PED_COMPONENT_VARIATION(ped, PV_COMP_HAIR, drawable, 0, 0);
			}
			else
			{
				PED::SET_PED_COMPONENT_VARIATION(ped, c.id, drawable, texture, PED::GET_PED_PALETTE_VARIATION(ped, c.id));
			}
		}
		for (const auto& p : ped_props)
		{
			const int drawable = (int)outfit[i++];
			const int texture  = (int)outfit[i++];
			const auto& pair = ped_prop_commands[p.id];
			Click click(CLICK_AUTO, TC_SCRIPT_NOYIELD);
			pair.first->setValueIndicator(click, drawable);
			pair.second->setValueIndicator(click, texture);
			if (drawable == -1)
				PED::CLEAR_PED_PROP(ped, p.id, 1);
			else
				PED::SET_PED_PROP_INDEX(ped, p.id, drawable, texture, TRUE, 1);
		}
		const int highlightVal = (int)outfit[i];
		if (highlightVal >= 0 && hair_highlight)
		{
			Click click(CLICK_AUTO, TC_SCRIPT_NOYIELD);
			hair_highlight->setValueIndicator(click, highlightVal);
			setHairColourFromCommands();
		}
	}
}
