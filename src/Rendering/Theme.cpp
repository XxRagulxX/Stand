#include "Rendering/Theme.hpp"

#include "Core/FileMgr.hpp"

#include <fstream>
#include <nlohmann/json.hpp>

namespace Stand::Rendering::Theme
{
	namespace
	{
		bool g_Loaded = false;

		nlohmann::json ColorToJson(const DirectX::XMFLOAT4& c)
		{
			return {c.x, c.y, c.z, c.w};
		}

		void JsonToColor(const nlohmann::json& j, DirectX::XMFLOAT4& c)
		{
			if (!j.is_array() || j.size() != 4)
				return;

			c.x = j[0].get<float>();
			c.y = j[1].get<float>();
			c.z = j[2].get<float>();
			c.w = j[3].get<float>();
		}
	}

	void LoadFromDisk()
	{
		if (g_Loaded)
			return;

		g_Loaded = true;

		auto file = FileMgr::GetProjectFile("./grid_theme.json");
		if (!file.Exists())
			return;

		nlohmann::json j;
		try
		{
			std::ifstream in(file.Path());
			in >> j;
		}
		catch (std::exception& e)
		{
			LOGF(WARNING, "Failed to read grid_theme.json: {}", e.what());
			return;
		}

		if (auto it = j.find("Accent"); it != j.end())
			JsonToColor(*it, kAccent);
		if (auto it = j.find("PanelBackground"); it != j.end())
			JsonToColor(*it, kPanelBackground);
		if (auto it = j.find("Text"); it != j.end())
			JsonToColor(*it, kText);
		if (auto it = j.find("ToggleOff"); it != j.end())
			JsonToColor(*it, kToggleOff);
		if (auto it = j.find("Error"); it != j.end())
			JsonToColor(*it, kError);
		if (auto it = j.find("PlaceholderText"); it != j.end())
			JsonToColor(*it, kPlaceholderText);
	}

	void SaveToDisk()
	{
		nlohmann::json j;
		j["Accent"] = ColorToJson(kAccent);
		j["PanelBackground"] = ColorToJson(kPanelBackground);
		j["Text"] = ColorToJson(kText);
		j["ToggleOff"] = ColorToJson(kToggleOff);
		j["Error"] = ColorToJson(kError);
		j["PlaceholderText"] = ColorToJson(kPlaceholderText);

		auto file = FileMgr::GetProjectFile("./grid_theme.json");
		std::ofstream out(file.Path(), std::ios::out | std::ios::trunc);
		out << j.dump(4);
	}

	Position2d GetContentOrigin()
	{
		const auto contentY = static_cast<int16_t>(kDefaultMenuOriginY + kHeaderHeight + kSpacer);

		if (!kTabsVisible)
			return Position2d{kDefaultMenuOriginX, contentY};

		switch (kTabsPosition)
		{
		case TabsPosition::Left:
			return Position2d{static_cast<int16_t>(kDefaultMenuOriginX + kSidebarWidth + kSpacer), contentY};

		case TabsPosition::Top:
			return Position2d{kDefaultMenuOriginX, static_cast<int16_t>(contentY + kSidebarEntryHeight + kSpacer)};

		case TabsPosition::Right:
		case TabsPosition::Bottom:
		default:
			return Position2d{kDefaultMenuOriginX, contentY};
		}
	}

	Position2d GetTabbedContentOrigin()
	{
		auto origin = GetContentOrigin();
		origin.y = static_cast<int16_t>(origin.y + kContentItemHeight + kSpacer);
		return origin;
	}
}
