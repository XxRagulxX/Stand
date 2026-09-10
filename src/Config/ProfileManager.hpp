#pragma once
#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

namespace Stand::ProfileManager
{
	std::filesystem::path GetProfilesPath();
	uint32_t GetGeneration();

	void SaveProfile(const std::string& name);
	void LoadProfile(const std::string& name);
	void DeleteProfile(const std::string& name);
	std::vector<std::string> ListProfiles();
}
