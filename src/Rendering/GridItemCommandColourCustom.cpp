#include "Rendering/GridItemCommandColourCustom.hpp"

#include "Rendering/StandPort/Grid.hpp"
#include "Rendering/StandPort/CommandColourCustom.hpp"
#include "Commands/Commands.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Rendering/StandPort/Position2d.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/GridRenderer.hpp"
#include "Rendering/MenuNavigation.hpp"
#include "Rendering/Theme.hpp"

#include <cmath>
#include <unordered_map>
#include <utility>

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kArrowGap = 5.f;

		constexpr float kMinContrastRatio = 3.f;

		float RelativeLuminance(const DirectX::XMFLOAT4& c)
		{
			auto linearize = [](float v) {
				return v <= 0.03928f ? v / 12.92f : std::pow((v + 0.055f) / 1.055f, 2.4f);
			};
			return 0.2126f * linearize(c.x) + 0.7152f * linearize(c.y) + 0.0722f * linearize(c.z);
		}

		bool IsContrastSufficient(const DirectX::XMFLOAT4& a, const DirectX::XMFLOAT4& b)
		{
			const float la = RelativeLuminance(a) + 0.05f;
			const float lb = RelativeLuminance(b) + 0.05f;
			const float ratio = la > lb ? la / lb : lb / la;
			return ratio > kMinContrastRatio;
		}

		class ColorEditGrid : public Grid
		{
		public:
			explicit ColorEditGrid(joaat_t id) :
			    Grid(Theme::GetContentOrigin(), 0),
			    m_Id(id)
			{
			}

		protected:
			void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override
			{
				auto* command = Commands::GetCommand<CommandColourCustom>(m_Id);
				if (!command)
					return;
				for (auto& child : command->children)
				{
					if (child->type == COMMAND_DIVIDER)
						continue;
					if (auto* phys = dynamic_cast<CommandPhysical*>(child.get()))
						items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, Theme::kContentItemHeight, phys));
				}
			}

		private:
			joaat_t m_Id;
		};

		ColorEditGrid& GetColorEditGrid(joaat_t id)
		{
			static std::unordered_map<joaat_t, ColorEditGrid> grids;
			return grids.try_emplace(id, id).first->second;
		}

		class GridItemColorFolder : public GridItem
		{
		public:
			GridItemColorFolder(int16_t width, int16_t height, std::string label, Grid* target, CommandColourCustom* command) :
			    GridItem(GRIDITEM_INDIFFERENT, width, height),
			    m_Label(std::move(label)),
			    m_Target(target),
			    m_Command(command)
			{
			}

			bool isFocusable() const override
			{
				return true;
			}

			void draw() override
			{
				if (isKeyboardFocused())
					GridRenderer::DrawRect(x, y, width, height, Theme::kAccent);
			}

			void drawText() override
			{
				const auto labelSize = GridRenderer::MeasureText(m_Label.c_str());
				GridRenderer::DrawText(x + 5.f, y + std::max(0.f, (height - labelSize.y) * 0.5f), m_Label.c_str(), Theme::kText);

				DirectX::XMFLOAT4 arrowColour = Theme::kText;
				if (m_Command)
				{
					const auto c = m_Command->getRGBA();
					const DirectX::XMFLOAT4 commandColour{c.x, c.y, c.z, c.w};
					if (!isKeyboardFocused() || IsContrastSufficient(commandColour, Theme::kAccent))
						arrowColour = commandColour;
				}

				const auto arrowSize = GridRenderer::MeasureText(">");
				GridRenderer::DrawText(x + width - arrowSize.x - kArrowGap, y + std::max(0.f, (height - arrowSize.y) * 0.5f), ">", arrowColour);
			}

			void onClick(int16_t, int16_t) override
			{
				activate();
			}

			void activate() override
			{
				MenuNavigation::Push(m_Label, m_Target);
			}

			[[nodiscard]] std::string GetDescription() const override
			{
				return m_Command ? m_Command->help_text.getLocalisedUtf8() : std::string{};
			}

		private:
			std::string m_Label;
			Grid* m_Target;
			CommandColourCustom* m_Command;
		};
	}

	void AddColorCommandRows(std::vector<std::unique_ptr<GridItem>>& items_draft, int16_t width, joaat_t id, std::optional<std::string> labelOverride)
	{
		auto* command = Commands::GetCommand<CommandColourCustom>(id);

		std::string label = "Unknown!";
		if (labelOverride.has_value())
			label = *labelOverride;
		else if (command)
			label = command->menu_name.getLocalisedUtf8();

		items_draft.push_back(std::make_unique<GridItemColorFolder>(width, Theme::kContentItemHeight, std::move(label), &GetColorEditGrid(id), command));
	}

	void AddConditionalColorCommandRows(Grid& grid,
	    std::vector<std::unique_ptr<GridItem>>& items_draft,
	    int16_t width,
	    joaat_t id,
	    std::function<bool()> condition,
	    std::optional<std::string> labelOverride)
	{
		if (grid.watchCondition(condition))
			AddColorCommandRows(items_draft, width, id, std::move(labelOverride));
	}

	class ColorEditGridByPtr : public Grid
	{
	public:
		explicit ColorEditGridByPtr(CommandColourCustom* cmd) :
		    Grid(Theme::GetContentOrigin(), 0),
		    m_Cmd(cmd)
		{
		}

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override
		{
			if (!m_Cmd)
				return;
			for (auto& child : m_Cmd->children)
			{
				if (child->type == COMMAND_DIVIDER)
					continue;
				if (auto* phys = dynamic_cast<CommandPhysical*>(child.get()))
					items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, Theme::kContentItemHeight, phys));
			}
		}

	private:
		CommandColourCustom* m_Cmd;
	};

	ColorEditGridByPtr& GetColorEditGridByPtr(CommandColourCustom* command)
	{
		static std::unordered_map<CommandColourCustom*, ColorEditGridByPtr> grids;
		return grids.try_emplace(command, command).first->second;
	}

	void AddColorCommandRows(std::vector<std::unique_ptr<GridItem>>& items_draft,
	    int16_t width,
	    CommandColourCustom* command,
	    std::optional<std::string> labelOverride)
	{
		std::string label = "Unknown!";
		if (labelOverride.has_value())
			label = *labelOverride;
		else if (command)
			label = command->menu_name.getLocalisedUtf8();

		items_draft.push_back(std::make_unique<GridItemColorFolder>(width, Theme::kContentItemHeight, std::move(label), &GetColorEditGridByPtr(command), command));
	}

	void AddConditionalColorCommandRows(Grid& grid,
	    std::vector<std::unique_ptr<GridItem>>& items_draft,
	    int16_t width,
	    CommandColourCustom* command,
	    std::function<bool()> condition,
	    std::optional<std::string> labelOverride)
	{
		if (grid.watchCondition(condition))
			AddColorCommandRows(items_draft, width, command, std::move(labelOverride));
	}
}
