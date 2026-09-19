#pragma once
#include <functional>
#include <string>

namespace Stand::Rendering
{
	class MenuCommandBox
	{
	public:
		static void Open(std::string commandName,
		    std::string label,
		    std::string rangeText,
		    std::string initialValue,
		    std::function<bool(const std::string&)> onSubmit,
		    std::function<std::string(const std::string&)> onType = nullptr);

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
			float labelY, labelHeight;
			float rangeY, rangeHeight;
			float errorY, errorHeight;
		};
		static Layout ComputeLayout();

		static bool s_Open;
		static std::string s_CommandName;
		static std::string s_Label;
		static std::string s_RangeText;
		static std::string s_Buffer;
		static std::function<bool(const std::string&)> s_OnSubmit;
		static std::function<std::string(const std::string&)> s_OnType;

		static bool s_ShowError;
		static unsigned long long s_ErrorShownAtMs;

		static std::string GetValueToken();
	};
}
