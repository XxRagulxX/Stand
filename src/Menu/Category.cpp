#include "Menu/Category.hpp"

#include "Rendering/GridRenderer.hpp"

namespace Stand
{
	void Category::Draw()
	{
		for (auto& item : m_Items)
			item->Draw();
	}

	int Category::GetLength()
	{
		if (m_Length.has_value())
			return m_Length.value();

		m_Length = static_cast<int>(std::max(Rendering::GridRenderer::MeasureText(m_Name.c_str()).x + 25.0f, 75.0f));
		return m_Length.value();
	}
}
