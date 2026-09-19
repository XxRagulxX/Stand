#pragma once
#include "Commands/Self/Bodyguards/CommandBodyguardModel.hpp"
#include "Commands/Self/Bodyguards/CommandBodyguardWeapon.hpp"
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Core/FileMgr.hpp"
#include "Menu/Click.hpp"
#include "Menu/Folder.hpp"
#include "Util/Label.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>

namespace Stand
{
	class CommandBodyguardPresetEntry : public CommandPhysical
	{
		CommandBodyguardModel* m_model;
		CommandBodyguardPrimaryWeapon* m_primary;
		CommandBodyguardSecondaryWeapon* m_secondary;
		CommandSlider* m_count;
		std::filesystem::path m_file;
	public:
		CommandBodyguardPresetEntry(CommandList* parent, Label&& label,
			CommandBodyguardModel* model, CommandBodyguardPrimaryWeapon* primary,
			CommandBodyguardSecondaryWeapon* secondary, CommandSlider* count,
			std::filesystem::path file)
			: CommandPhysical(COMMAND_ACTION, parent, std::move(label), {}, NOLABEL)
			, m_model(model), m_primary(primary), m_secondary(secondary), m_count(count)
			, m_file(std::move(file))
		{}

		void onClick(Click&) override
		{
			std::ifstream f(m_file);
			if (!f.is_open()) return;
			nlohmann::json j;
			f >> j;
			if (j.contains("model"))     m_model->SetModel(j["model"].get<Hash>());
			if (j.contains("primary"))   m_primary->SetWeapon(j["primary"].get<Hash>());
			if (j.contains("secondary")) m_secondary->SetWeapon(j["secondary"].get<Hash>());
			if (j.contains("count"))     m_count->value = std::clamp(j["count"].get<int>(), 1, 7);
		}
	};

	class CommandBodyguardPresetsSave : public CommandPhysical
	{
		CommandBodyguardModel* m_model;
		CommandBodyguardPrimaryWeapon* m_primary;
		CommandBodyguardSecondaryWeapon* m_secondary;
		CommandSlider* m_count;
		std::filesystem::path m_folder;
	public:
		CommandBodyguardPresetsSave(CommandList* parent,
			CommandBodyguardModel* model, CommandBodyguardPrimaryWeapon* primary,
			CommandBodyguardSecondaryWeapon* secondary, CommandSlider* count,
			std::filesystem::path folder)
			: CommandPhysical(COMMAND_ACTION, parent, LIT("Save Preset"), CMDNAMES("bodyguardpresetssave"), NOLABEL)
			, m_model(model), m_primary(primary), m_secondary(secondary), m_count(count)
			, m_folder(std::move(folder))
		{}

		void onCommand(Click& click, std::wstring& args) override
		{
			if (args.empty()) return;
			const std::string name(args.begin(), args.end());
			const auto file = m_folder / (name + ".json");

			nlohmann::json j;
			j["model"]     = m_model->GetModel();
			j["primary"]   = m_primary->GetWeapon();
			j["secondary"] = m_secondary->GetWeapon();
			j["count"]     = m_count->value;

			std::ofstream f(file, std::ios::out | std::ios::trunc);
			if (!f.is_open()) return;
			f << j.dump(4);

			parent->createChild<CommandBodyguardPresetEntry>(
				LIT(name), m_model, m_primary, m_secondary, m_count, file);
		}
	};

	class CommandBodyguardPresets : public CommandList
	{
	public:
		CommandBodyguardPresets(CommandList* parent,
			CommandBodyguardModel* model, CommandBodyguardPrimaryWeapon* primary,
			CommandBodyguardSecondaryWeapon* secondary, CommandSlider* count)
			: CommandList(parent, LIT("Presets"), CMDNAMES("bodyguardpresets"))
		{
			const auto folder = FileMgr::GetProjectFolder("./bodyguard_presets/");
			createChild<CommandBodyguardPresetsSave>(model, primary, secondary, count, folder.Path());

			if (std::filesystem::exists(folder.Path()))
			{
				for (const auto& entry : std::filesystem::directory_iterator(folder.Path()))
				{
					if (entry.path().extension() == ".json")
					{
						const std::string name = entry.path().stem().string();
						createChild<CommandBodyguardPresetEntry>(
							LIT(name), model, primary, secondary, count, entry.path());
					}
				}
			}
		}
	};
}
