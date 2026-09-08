#include "Rendering/TextWrap.hpp"

#include "Rendering/GridRenderer.hpp"

#include <sstream>

namespace Stand::Rendering
{
	std::vector<std::string> WrapText(const std::string& text, float maxWidth, float scale)
	{
		std::vector<std::string> lines;
		std::string currentLine;

		std::istringstream words(text);
		std::string word;
		while (words >> word)
		{
			std::string candidate = currentLine.empty() ? word : currentLine + " " + word;
			if (!currentLine.empty() && GridRenderer::MeasureText(candidate.c_str(), scale).x > maxWidth)
			{
				lines.push_back(currentLine);
				currentLine = word;
			}
			else
			{
				currentLine = candidate;
			}
		}

		if (!currentLine.empty())
			lines.push_back(currentLine);

		return lines;
	}
}
