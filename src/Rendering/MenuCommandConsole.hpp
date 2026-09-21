#pragma once
#include <functional>
#include <string>
#include <vector>

namespace Stand::Rendering
{
	class MenuCommandConsole
	{
	public:
		static void Open();
		static void Open(std::string prefill, std::function<void(const std::string&)> callback, std::string hint = {});
		static bool IsOpen();
		static void Close();
		static void Draw();
		static void DrawText();
		static void HandleKey(unsigned int vkCode);
		static void HandleChar(wchar_t c);

	private:
		struct Layout
		{
			float x, y, width;
			float titleY, titleHeight;
			float inputY, inputHeight;
			float firstRowY, rowHeight;
		};
		static Layout ComputeLayout();

		struct Match
		{
			std::string hint; // "<name> - <label>", ready to draw as-is
			std::function<void()> activate;
			std::string name;
		};

		static void UpdateMatches();	
		static bool s_Open;
		static std::string s_Buffer;
		static std::vector<Match> s_Matches;
		static int s_SelectedIndex; // -1 if s_Matches is empty
		static std::function<void(const std::string&)> s_DirectCallback;
		static std::string s_DirectHint;
	};
}
