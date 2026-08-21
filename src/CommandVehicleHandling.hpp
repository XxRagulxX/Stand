#pragma once

#include "CommandSliderFloat.hpp"

#include <unordered_map>

#include "AbstractEntity.hpp"
#include "AbstractModel.hpp"
#include "atoffset.hpp"
#include "CHandlingData.hpp"
#include "gta_vehicle.hpp"

namespace Stand
{
	template <typename T, typename Base>
	class CommandVehicleHandling : public Base
	{
	private:
		uintptr_t value_offset = 0;
		std::unordered_map<hash_t, T> default_values{};

		[[nodiscard]] T* getAddr() const noexcept
		{
			if (auto v = g_player_veh.getCVehicle())
			{
				const auto data = v->handling_data;

				auto* const parent = this->parent->template as<CommandListSubhandling>();

				if (parent->handling_type == HANDLING_TYPE_INVALID)
				{
					return atoffset<T>(data, value_offset);
				}

				if (auto subhandling = data->getSubhandlingData(parent->handling_type))
				{
					return atoffset<T>(subhandling, value_offset);
				}
			}
			return nullptr;
		}

		void setVisualValueOnly(T value)
		{
			Click click(CLICK_BULK, TC_SCRIPT_NOYIELD);
			if constexpr (std::is_same_v<T, float>)
			{
				this->setValueIndicator(click,static_cast<int>(value * Base::getPrecisionScalar()));
			}
			else
			{
				this->setValueIndicator(click, value);
			}
		}

	public:
		explicit CommandVehicleHandling(CommandList* const parent, uintptr_t offset, Label&& menu_name)
			: Base(parent, std::move(menu_name), { CMDNAME("vh") }, NOLABEL, -100000000, 100000000, 0, 5, CMDFLAGS_SLIDER | CMDFLAG_TEMPORARY | CMDFLAG_FEATURELIST_FINISHLIST), value_offset(offset)
		{
			Base::addSuffixToCommandNames(this->menu_name.getEnglishForCommandName());
			if constexpr (std::is_same_v<Base, CommandSliderFloat>)
			{
				Base::step_size = 500;
				Base::precision = 4;
			}
		}

		void onChange(Click& click, int prev_value) final
		{
			if (auto addr = getAddr())
			{
				const hash_t this_hash = g_player_veh.getModel();

				if (!default_values.contains(this_hash))
				{
					default_values.emplace(this_hash, *addr);
				}

				*addr = Base::template getTValue<T>();
			}
		}

		void applyDefaultState() final
		{
			if (!g_player_veh.isValid())
			{
				return;
			}

			const auto entry = default_values.find(g_player_veh.getModel());

			if (entry != default_values.end())
			{
				setVisualValueOnly(entry->second);

				Click click(CLICK_BULK,TC_APPLYDEFAULTSTATE);

				onChange(click, 0);
			}
		}

		void onTickInGameViewport() final
		{
			if (auto addr = getAddr())
			{
				setVisualValueOnly(*addr);

				this->setMinValue(-100000000);
				this->setMaxValue(100000000);
				return;
			}
			//this->setVisualValueOnly(0);
			this->setMinValue(0);
			this->setMaxValue(0);
		}

		void onTickInWebViewport() final
		{
			Base::onTickInWebViewportImplRedirect();
		}

		void onPreScriptedAccess() final
		{
			if (!Base::isInViewport())
			{
				onTickInGameViewport();
			}
		}
	};

	using CommandVehicleHandlingFloat = CommandVehicleHandling<float, CommandSliderFloat>;
	using CommandVehicleHandlingInt = CommandVehicleHandling<int, CommandSlider>;
}