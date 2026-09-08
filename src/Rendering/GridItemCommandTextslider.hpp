#pragma once
#include "Rendering/GridItem.hpp"
#include "Util/Joaat.hpp"
#include "Commands/CommandTextslider.hpp"

#include <optional>
#include <string>

namespace Stand::Rendering
{
	class GridItemCommandTextslider : public GridItem
	{
	public:
		GridItemCommandTextslider(int16_t width, int16_t height, joaat_t id, std::optional<std::string> labelOverride = std::nullopt);

		void draw() override;
		void drawText() override;
		void onClick(int16_t cursorX, int16_t cursorY) override;

		bool isFocusable() const override
		{
			return true;
		}

		bool onArrow(int delta) override;

		[[nodiscard]] std::string GetDescription() const override;

	private:
		void Cycle(int direction);

		struct Layout
		{
			float valueX;
			float valueWidth;
			float prevX;
			float nextX;
			float buttonSize;
		};
		Layout ComputeLayout() const;

		const std::string& Label() const;
		const char* CurrentOptionText() const;
		float MaxItemWidth() const;

		CommandTextslider* m_Command;
		std::optional<std::string> m_LabelOverride;
		mutable std::optional<float> m_MaxItemWidth;
	};
}
