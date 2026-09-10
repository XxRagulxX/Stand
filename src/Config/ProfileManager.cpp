#include "Config/ProfileManager.hpp"

#include "Commands/CommandLegacy.hpp"
#include "Commands/Commands.hpp"

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <nlohmann/json.hpp>

namespace Stand::ProfileManager
{
	namespace
	{
		uint32_t s_Generation = 0;

		void EnsureFolder()
		{
			std::filesystem::create_directories(GetProfilesPath());
		}
	}

	std::filesystem::path GetProfilesPath()
	{
		return std::filesystem::path(std::getenv("appdata")) / "StandEnhanced" / "Profiles";
	}

	uint32_t GetGeneration()
	{
		return s_Generation;
	}

	void SaveProfile(const std::string& name)
	{
		if (name.empty())
			return;

		EnsureFolder();

		nlohmann::json state = nlohmann::json::object();
		for (auto& [hash, command] : Commands::GetCommands())
		{
			if (!command->ShouldSaveState())
				continue;
			state[command->GetName()] = nlohmann::json::object();
			command->SaveState(state[command->GetName()]);
		}

		auto path = GetProfilesPath() / (name + ".json");
		std::ofstream file(path, std::ios::out | std::ios::trunc);
		file << state.dump(4);

		++s_Generation;
	}

	void LoadProfile(const std::string& name)
	{
		auto path = GetProfilesPath() / (name + ".json");
		if (!std::filesystem::exists(path))
			return;

		std::ifstream file(path);
		nlohmann::json state;
		try
		{
			file >> state;
		}
		catch (...)
		{
			return;
		}

		for (auto& [hash, command] : Commands::GetCommands())
		{
			if (!command->ShouldSaveState())
				continue;
			if (state.contains(command->GetName()))
				command->LoadState(state[command->GetName()]);
		}

		Commands::MarkDirty();
	}

	void DeleteProfile(const std::string& name)
	{
		auto path = GetProfilesPath() / (name + ".json");
		std::filesystem::remove(path);
		++s_Generation;
	}

	std::vector<std::string> ListProfiles()
	{
		EnsureFolder();

		std::vector<std::string> result;
		for (auto& entry : std::filesystem::directory_iterator(GetProfilesPath()))
		{
			if (entry.path().extension() == ".json")
				result.push_back(entry.path().stem().string());
		}
		std::sort(result.begin(), result.end());
		return result;
	}
}
