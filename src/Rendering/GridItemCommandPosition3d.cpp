#include "Rendering/GridItemCommandPosition3d.hpp"

#include "Commands/Commands.hpp"
#include "Rendering/Grid.hpp"
#include "Rendering/GridItemButton.hpp"
#include "Rendering/GridItemText.hpp"
#include "Rendering/GridRenderer.hpp"
#include "World/Self.hpp"
#include "Rendering/Theme.hpp"
#include "Commands/CommandPosition3d.hpp"
#include "Game/vector.hpp"

#include <algorithm>
#include <format>

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kButtonSize = 22.f;
		constexpr float kValueWidth = 56.f;
		constexpr float kGap = 6.f;
		constexpr float kStep = 0.5f;

		enum Axis
		{
			AXIS_X,
			AXIS_Y,
			AXIS_Z,
		};

		float GetAxis(const rage::fvector3& v, Axis axis)
		{
			switch (axis)
			{
			case AXIS_X:
				return v.x;
			case AXIS_Y:
				return v.y;
			case AXIS_Z:
				return v.z;
			}
			return 0.f;
		}

		void SetAxis(rage::fvector3& v, Axis axis, float value)
		{
			switch (axis)
			{
			case AXIS_X:
				v.x = value;
				break;
			case AXIS_Y:
				v.y = value;
				break;
			case AXIS_Z:
				v.z = value;
				break;
			}
		}

		const char* AxisLabel(Axis axis)
		{
			switch (axis)
			{
			case AXIS_X:
				return "X";
			case AXIS_Y:
				return "Y";
			case AXIS_Z:
				return "Z";
			}
			return "?";
		}

		// One X/Y/Z row - same label + value + "-"/"+" button shape as
		// GridItemCommandSliderFloat, just stepping one axis of the shared
		// CommandPosition3d's rage::fvector3 rather than owning a
		// CommandSliderFloat of its own - see the file comment in
		// GridItemCommandPosition3d.hpp for why this is its own small
		// GridItem instead of one mega-widget.
		class GridItemVector3Axis : public GridItem
		{
		public:
			GridItemVector3Axis(int16_t width, int16_t height, Axis axis, CommandPosition3d* command) :
			    GridItem(GRIDITEM_INDIFFERENT, width, height),
			    m_Axis(axis),
			    m_Command(command)
			{
			}

			bool isFocusable() const override
			{
				return true;
			}

			bool onArrow(int delta) override
			{
				if (!m_Command)
					return false;

				Step(delta > 0 ? 1 : -1);
				return true;
			}

			void draw() override
			{
				if (isKeyboardFocused())
					GridRenderer::DrawRect(x, y, width, height, Theme::kAccent);
			}

			void drawText() override
			{
				const auto layout = ComputeLayout();
				const auto* axisLabel = AxisLabel(m_Axis);

				const auto labelSize = GridRenderer::MeasureText(axisLabel);
				GridRenderer::DrawText(x + 5.f, y + std::max(0.f, (height - labelSize.y) * 0.5f), axisLabel, Theme::kText);

				const auto valueStr = m_Command ? std::format("{:.2f}", GetAxis(m_Command->GetState(), m_Axis)) : std::string("?");
				const auto valueSize = GridRenderer::MeasureText(valueStr.c_str());
				GridRenderer::DrawText(layout.valueX + std::max(0.f, (layout.valueWidth - valueSize.x) * 0.5f),
				    y + std::max(0.f, (height - valueSize.y) * 0.5f),
				    valueStr.c_str(),
				    m_Command ? Theme::kText : Theme::kError);

				const auto minusSize = GridRenderer::MeasureText("<");
				GridRenderer::DrawText(layout.minusX + std::max(0.f, (layout.buttonSize - minusSize.x) * 0.5f),
				    y + std::max(0.f, (height - minusSize.y) * 0.5f),
				    "<",
				    Theme::kText);

				const auto plusSize = GridRenderer::MeasureText(">");
				GridRenderer::DrawText(layout.plusX + std::max(0.f, (layout.buttonSize - plusSize.x) * 0.5f),
				    y + std::max(0.f, (height - plusSize.y) * 0.5f),
				    ">",
				    Theme::kText);
			}

			void onClick(int16_t cursorX, int16_t) override
			{
				if (!m_Command)
					return;

				const auto layout = ComputeLayout();
				if (cursorX >= layout.plusX && cursorX < layout.plusX + layout.buttonSize)
					Step(1);
				else if (cursorX >= layout.minusX && cursorX < layout.minusX + layout.buttonSize)
					Step(-1);
			}

		private:
			struct Layout
			{
				float valueX;
				float valueWidth;
				float minusX;
				float plusX;
				float buttonSize;
			};

			Layout ComputeLayout() const
			{
				Layout layout;
				layout.buttonSize = kButtonSize;
				layout.valueWidth = kValueWidth;
				layout.plusX = x + width - kButtonSize;
				layout.valueX = layout.plusX - kGap - kValueWidth;
				layout.minusX = layout.valueX - kGap - kButtonSize;
				return layout;
			}

			void Step(int direction)
			{
				auto v = m_Command->GetState();
				SetAxis(v, m_Axis, GetAxis(v, m_Axis) + static_cast<float>(direction) * kStep);
				m_Command->SetState(v);
			}

			Axis m_Axis;
			CommandPosition3d* m_Command;
		};
	}

	void AddVector3CommandRows(Grid& grid, std::vector<std::unique_ptr<GridItem>>& items_draft, int16_t width, joaat_t id, std::optional<std::string> labelOverride)
	{
		auto* command = Commands::GetCommand<CommandPosition3d>(id);

		std::string label = "Unknown!";
		if (labelOverride.has_value())
			label = *labelOverride;
		else if (command)
			label = command->GetLabel();

		items_draft.push_back(std::make_unique<GridItemText>(width, Theme::kContentItemHeight, std::move(label), command ? Theme::kText : Theme::kError));
		if (grid.watchCondition([] {
			    return static_cast<bool>(Self::GetPed());
		    }))
		{
			items_draft.push_back(std::make_unique<GridItemButton>(width, Theme::kContentItemHeight, "Current", [command] {
				if (command && Self::GetPed())
					command->SetState(Self::GetPed().GetPosition());
			}));
		}
		items_draft.push_back(std::make_unique<GridItemVector3Axis>(width, Theme::kContentItemHeight, AXIS_X, command));
		items_draft.push_back(std::make_unique<GridItemVector3Axis>(width, Theme::kContentItemHeight, AXIS_Y, command));
		items_draft.push_back(std::make_unique<GridItemVector3Axis>(width, Theme::kContentItemHeight, AXIS_Z, command));
	}
}
