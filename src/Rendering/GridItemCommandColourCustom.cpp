#include "Rendering/GridItemCommandColourCustom.hpp"

#include "Commands/CommandColourCustom.hpp"
#include "Commands/Commands.hpp"
#include "Rendering/Grid.hpp"
#include "Rendering/GridItemTextInput.hpp"
#include "Rendering/GridRenderer.hpp"
#include "Rendering/MenuNavigation.hpp"
#include "Rendering/Theme.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <format>
#include <unordered_map>
#include <utility>

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kButtonSize = 22.f;
		constexpr float kValueWidth = 40.f;
		constexpr float kGap = 6.f;
		constexpr float kArrowGap = 5.f;

		// Real Stand's own ColourUtil::isContrastSufficient() threshold
		// (a WCAG-style relative-luminance contrast ratio) - see
		// GridItemColorFolder::drawText() below for where this gates
		// falling back to the normal arrow colour instead of the
		// command's own tint while focused.
		constexpr float kMinContrastRatio = 3.f;

		enum Channel
		{
			CHANNEL_R,
			CHANNEL_G,
			CHANNEL_B,
			CHANNEL_A,
		};

		float GetChannel(const ImVec4& colour, Channel channel)
		{
			switch (channel)
			{
			case CHANNEL_R:
				return colour.x;
			case CHANNEL_G:
				return colour.y;
			case CHANNEL_B:
				return colour.z;
			case CHANNEL_A:
				return colour.w;
			}
			return 0.f;
		}

		void SetChannel(ImVec4& colour, Channel channel, float value01)
		{
			switch (channel)
			{
			case CHANNEL_R:
				colour.x = value01;
				break;
			case CHANNEL_G:
				colour.y = value01;
				break;
			case CHANNEL_B:
				colour.z = value01;
				break;
			case CHANNEL_A:
				colour.w = value01;
				break;
			}
		}

		const char* ChannelLabel(Channel channel)
		{
			switch (channel)
			{
			case CHANNEL_R:
				return "R";
			case CHANNEL_G:
				return "G";
			case CHANNEL_B:
				return "B";
			case CHANNEL_A:
				return "A";
			}
			return "?";
		}

		// One R/G/B/A row - same label + value + "-"/"+" button shape as
		// GridItemCommandSlider, just stepping a 0-255 view of one channel
		// of the shared CommandColourCustom's ImVec4 (stored internally as
		// 0.0-1.0, same convention ImGui::ColorPicker4 itself uses)
		// rather than owning an CommandSlider of its own - see the file
		// comment in GridItemCommandColourCustom.hpp for why this is its own
		// small GridItem instead of one of the four channels being
		// folded into a single mega-widget.
		class GridItemColorChannel : public GridItem
		{
		public:
			GridItemColorChannel(int16_t width, int16_t height, Channel channel, CommandColourCustom* command) :
			    GridItem(GRIDITEM_INDIFFERENT, width, height),
			    m_Channel(channel),
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
				const auto* channelLabel = ChannelLabel(m_Channel);

				const auto labelSize = GridRenderer::MeasureText(channelLabel);
				GridRenderer::DrawText(x + 5.f, y + std::max(0.f, (height - labelSize.y) * 0.5f), channelLabel, Theme::kText);

				const auto valueStr = m_Command ? std::to_string(Value255()) : std::string("?");
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

			int Value255() const
			{
				return static_cast<int>(GetChannel(m_Command->GetState(), m_Channel) * 255.f + 0.5f);
			}

			void Step(int direction)
			{
				auto colour = m_Command->GetState();
				const auto current255 = static_cast<int>(GetChannel(colour, m_Channel) * 255.f + 0.5f);
				const auto stepped255 = std::clamp(current255 + direction * 5, 0, 255);
				SetChannel(colour, m_Channel, static_cast<float>(stepped255) / 255.f);
				m_Command->SetState(colour);
			}

			Channel m_Channel;
			CommandColourCustom* m_Command;
		};

		enum HsvChannel
		{
			HSV_H,
			HSV_S,
			HSV_V,
		};

		void RgbToHsv(float r, float g, float b, float& h, float& s, float& v)
		{
			const auto maxc = std::max({r, g, b});
			const auto minc = std::min({r, g, b});
			const auto delta = maxc - minc;

			v = maxc;
			s = maxc <= 0.f ? 0.f : delta / maxc;

			if (delta <= 0.f)
				h = 0.f;
			else if (maxc == r)
				h = 60.f * std::fmod((g - b) / delta, 6.f);
			else if (maxc == g)
				h = 60.f * (((b - r) / delta) + 2.f);
			else
				h = 60.f * (((r - g) / delta) + 4.f);

			if (h < 0.f)
				h += 360.f;
		}

		void HsvToRgb(float h, float s, float v, float& r, float& g, float& b)
		{
			const auto c = v * s;
			const auto hp = h / 60.f;
			const auto xComp = c * (1.f - std::fabs(std::fmod(hp, 2.f) - 1.f));
			const auto m = v - c;

			float r1 = 0.f, g1 = 0.f, b1 = 0.f;
			if (hp < 1.f)
			{
				r1 = c;
				g1 = xComp;
			}
			else if (hp < 2.f)
			{
				r1 = xComp;
				g1 = c;
			}
			else if (hp < 3.f)
			{
				g1 = c;
				b1 = xComp;
			}
			else if (hp < 4.f)
			{
				g1 = xComp;
				b1 = c;
			}
			else if (hp < 5.f)
			{
				r1 = xComp;
				b1 = c;
			}
			else
			{
				r1 = c;
				b1 = xComp;
			}

			r = r1 + m;
			g = g1 + m;
			b = b1 + m;
		}

		const char* HsvChannelLabel(HsvChannel channel)
		{
			switch (channel)
			{
			case HSV_H:
				return "H";
			case HSV_S:
				return "S";
			case HSV_V:
				return "V";
			}
			return "?";
		}

		class GridItemColorHsvChannel : public GridItem
		{
		public:
			GridItemColorHsvChannel(int16_t width, int16_t height, HsvChannel channel, CommandColourCustom* command) :
			    GridItem(GRIDITEM_INDIFFERENT, width, height),
			    m_Channel(channel),
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
				const auto* channelLabel = HsvChannelLabel(m_Channel);

				const auto labelSize = GridRenderer::MeasureText(channelLabel);
				GridRenderer::DrawText(x + 5.f, y + std::max(0.f, (height - labelSize.y) * 0.5f), channelLabel, Theme::kText);

				const auto valueStr = m_Command ? std::to_string(Value()) : std::string("?");
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

			int Value() const
			{
				const auto colour = m_Command->GetState();
				float h, s, v;
				RgbToHsv(colour.x, colour.y, colour.z, h, s, v);
				switch (m_Channel)
				{
				case HSV_H:
					return static_cast<int>(h + 0.5f);
				case HSV_S:
					return static_cast<int>(s * 100.f + 0.5f);
				case HSV_V:
					return static_cast<int>(v * 100.f + 0.5f);
				}
				return 0;
			}

			void Step(int direction)
			{
				auto colour = m_Command->GetState();
				float h, s, v;
				RgbToHsv(colour.x, colour.y, colour.z, h, s, v);

				switch (m_Channel)
				{
				case HSV_H:
					h = std::fmod(h + static_cast<float>(direction) * 5.f + 360.f, 360.f);
					break;
				case HSV_S:
					s = std::clamp(s + static_cast<float>(direction) * 0.05f, 0.f, 1.f);
					break;
				case HSV_V:
					v = std::clamp(v + static_cast<float>(direction) * 0.05f, 0.f, 1.f);
					break;
				}

				float r, g, b;
				HsvToRgb(h, s, v, r, g, b);
				colour.x = r;
				colour.y = g;
				colour.z = b;
				m_Command->SetState(colour);
			}

			HsvChannel m_Channel;
			CommandColourCustom* m_Command;
		};

		std::string ComputeColorHex(CommandColourCustom* command)
		{
			if (!command)
				return "#000000";

			const auto colour = command->GetState();
			auto toByte = [](float v) {
				return static_cast<int>(std::clamp(v, 0.f, 1.f) * 255.f + 0.5f);
			};
			return std::format("#{:02X}{:02X}{:02X}", toByte(colour.x), toByte(colour.y), toByte(colour.z));
		}

		void ApplyColorHex(CommandColourCustom* command, const std::string& text)
		{
			if (!command)
				return;

			std::string hex = text;
			if (!hex.empty() && hex.front() == '#')
				hex.erase(hex.begin());

			if (hex.size() != 6 && hex.size() != 8)
				return;

			for (char c : hex)
			{
				if (!std::isxdigit(static_cast<unsigned char>(c)))
					return;
			}

			unsigned long value;
			try
			{
				value = std::stoul(hex, nullptr, 16);
			}
			catch (const std::exception&)
			{
				return;
			}

			auto colour = command->GetState();
			if (hex.size() == 8)
			{
				colour.w = static_cast<float>(value & 0xFF) / 255.f;
				value >>= 8;
			}
			colour.z = static_cast<float>(value & 0xFF) / 255.f;
			value >>= 8;
			colour.y = static_cast<float>(value & 0xFF) / 255.f;
			value >>= 8;
			colour.x = static_cast<float>(value & 0xFF) / 255.f;

			command->SetState(colour);
		}

		class GridItemColorHex : public GridItemTextInput
		{
		public:
			explicit GridItemColorHex(int16_t width, int16_t height, CommandColourCustom* command) :
			    GridItemTextInput(width, height, "Hex", ComputeColorHex(command), [command](const std::string& text) {
				    ApplyColorHex(command, text);
			    }),
			    m_Command(command)
			{
			}

			void draw() override
			{
				if (m_Command && !isEditingText())
					SetValue(ComputeColorHex(m_Command));

				GridItemTextInput::draw();
			}

		private:
			CommandColourCustom* m_Command;
		};

		// Real Stand's own ColourUtil.hpp (origin/stand-reference) - sRGB
		// relative luminance, then the standard WCAG contrast-ratio
		// formula ((L1+0.05)/(L2+0.05), lighter over darker) - ported
		// verbatim since this project has no existing colour-contrast
		// utility of its own (checked: no other file in this codebase
		// does luminance/contrast math).
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

		// The sub-page a GridItemColorFolder row below drills into - just
		// the same four R/G/B/A channel steppers AddColorCommandRows()
		// used to push inline before this change, now on their own page
		// instead (no separate swatch/label row of its own - the folder
		// row's own tinted arrow already previews the colour, same as
		// real Stand never repeating that preview on the page it drills
		// into either).
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
				items_draft.push_back(std::make_unique<GridItemColorChannel>(Theme::kContentWidth, Theme::kContentItemHeight, CHANNEL_R, command));
				items_draft.push_back(std::make_unique<GridItemColorChannel>(Theme::kContentWidth, Theme::kContentItemHeight, CHANNEL_G, command));
				items_draft.push_back(std::make_unique<GridItemColorChannel>(Theme::kContentWidth, Theme::kContentItemHeight, CHANNEL_B, command));
				items_draft.push_back(std::make_unique<GridItemColorChannel>(Theme::kContentWidth, Theme::kContentItemHeight, CHANNEL_A, command));
				items_draft.push_back(std::make_unique<GridItemColorHsvChannel>(Theme::kContentWidth, Theme::kContentItemHeight, HSV_H, command));
				items_draft.push_back(std::make_unique<GridItemColorHsvChannel>(Theme::kContentWidth, Theme::kContentItemHeight, HSV_S, command));
				items_draft.push_back(std::make_unique<GridItemColorHsvChannel>(Theme::kContentWidth, Theme::kContentItemHeight, HSV_V, command));
				items_draft.push_back(std::make_unique<GridItemColorHex>(Theme::kContentWidth, Theme::kContentItemHeight, command));
			}

		private:
			joaat_t m_Id;
		};

		// One persistent ColorEditGrid per distinct CommandColourCustom id,
		// created on first use and reused after - a GridItemFolder-style
		// row's own m_Target must outlive the row and stay stable across
		// MenuNavigation pushes/pops (see GridItemFolder.hpp's own class
		// comment on why m_Target is always a non-owning pointer into
		// something owned elsewhere), and AddColorCommandRows() is a
		// stateless free function with no natural owner of its own to
		// keep one on. std::unordered_map guarantees pointer/reference
		// stability across insertions (node-based, no reallocation of
		// existing elements) - safe to hand out a raw pointer into it
		// that outlives this function call.
		ColorEditGrid& GetColorEditGrid(joaat_t id)
		{
			// try_emplace constructs the ColorEditGrid in place from the
			// forwarded id (only if not already present) rather than
			// constructing a temporary and moving/copying it in - Grid
			// has no need to support either.
			static std::unordered_map<joaat_t, ColorEditGrid> grids;
			return grids.try_emplace(id, id).first->second;
		}

		// A GridItemFolder-shaped row (label + right-aligned ">",
		// clicking/Enter drills into m_Target via MenuNavigation::Push())
		// whose arrow is tinted to m_Command's own current colour instead
		// of a separate swatch box - ported from real Stand's own
		// GridItemList::update() (origin/stand-reference): "if
		// (preview_colour_in_sprite && list->type == COMMAND_LIST_COLOUR)
		// ... arrowSpriteColour = commandColour" - the row's own trailing
		// arrow SPRITE gets its tint colour swapped to the command's
		// live RGBA (read via a getter, same as this reads GetState()),
		// falling back to the normal arrow colour only while this row is
		// focused AND contrast against the focus highlight would
		// otherwise be too low (IsContrastSufficient() above, same
		// ratio/threshold Stand's own ColourUtil::isContrastSufficient()
		// uses) - unfocused, the tint always applies. Real Stand also has
		// a second, separate mechanism (preview_colour_in_list) that
		// tints the entire row's own focus-highlight background instead -
		// not ported here, since only the arrow tint was asked for.
		//
		// A distinct class from GridItemFolder rather than a subclass of
		// it - GridItemFolder's own drawText() draws the label and arrow
		// in one function with no seam to override just the arrow's
		// colour, and duplicating its small draw()/drawText()/onClick()/
		// activate() shape here (this class is used only for colour
		// commands) is simpler than adding a hook to the generic
		// GridItemFolder every other folder row would need to skip.
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
					const auto c = m_Command->GetState();
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
				return m_Command ? m_Command->GetDescription() : std::string{};
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
			label = command->GetLabel();

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
}
