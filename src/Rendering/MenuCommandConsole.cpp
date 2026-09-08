#include "Rendering/MenuCommandConsole.hpp"

#include "Commands/CommandLegacy.hpp"
#include "Commands/Commands.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/Widgets/CommandRegistry.hpp"
#include "Menu/Click.hpp"
#include "Rendering/GridRenderer.hpp"
#include "Rendering/InputCapture.hpp"
#include "Rendering/Theme.hpp"
#include "Scripting/FiberPool.hpp"

#include <algorithm>
#include <unordered_set>
#include <windows.h>

namespace Stand::Rendering
{
	namespace
	{
		// x/y/width match MenuCommandBox's own (real Stand's own
		// CommandboxGrid: Grid((HUD_WIDTH / 2) - (width / 2), 100,
		// spacer_size)) - the two are never open at once (see this
		// class's own header comment), so there's no visual clash.
		constexpr float kWidth = 547.f;
		constexpr float kY = 100.f;
		constexpr float kTitleHeight = 24.f;
		constexpr float kInputHeight = 32.f;
		constexpr float kRowHeight = 22.f;
		constexpr float kPaddingX = 6.f;

		// Real Stand's own max_shown_matching_commands - caps the list
		// so a short, broad prefix (e.g. a single letter) doesn't fill
		// the whole screen with rows; a trailing "+N more" row (see
		// DrawText()) accounts for the rest, same as real Stand's own
		// "MORE"/"MANYMORE" rows.
		constexpr size_t kMaxShown = 8;

		// One command not yet turned into a Match - built for every
		// registered command up front (both registries), then narrowed
		// down against the typed prefix. names holds every alias (real
		// Stand's own command_names - a legacy Stand::Command only
		// ever has the one), display is what a match's own hint line
		// shows regardless of which alias matched (real Stand's own
		// getCompletionHint() always uses command_names.at(0), never the
		// alias that actually matched - mirrored here).
		struct Candidate
		{
			std::vector<std::string> names;
			std::string display; // "<name> - <label>"
			std::function<void()> activate;
		};

		std::vector<Candidate> CollectCandidates()
		{
			std::vector<Candidate> candidates;

			for (auto& [hash, command] : Commands::GetCommands())
			{
				if (command->GetName().empty())
					continue;

				candidates.push_back({
				    {command->GetName()},
				    command->GetName() + " - " + command->GetLabel(),
				    // Queued onto a script thread rather than called
				    // inline - Command::Call() (a toggle's OnEnable()/
				    // OnDisable(), a one-shot's OnCall()) commonly
				    // touches game state/natives, which must never run
				    // directly on this WndProc callback thread. Same
				    // "queue it, don't call it inline" convention
				    // GridItemCommandButton.cpp's own onClick() already
				    // uses for this exact same Command::Call() - a
				    // previous pass here called it inline by mistake.
				    [command] {
					    FiberPool::queueJob([command] {
						    command->Call();
					    });
				    },
				});
			}

			// Stand::CommandRegistry registers a command once PER ALIAS
			// (see that class's own comment - "every ... command_names
			// ... into this flat map"), all pointing at the same
			// instance - a command with more than one alias (e.g.
			// CommandGod's own "godmode"/"immortality") therefore shows
			// up more than once in this map's own iteration. Deduped by
			// pointer here so it becomes exactly one candidate (with
			// every one of its own aliases still searched via
			// candidate.names below), rather than one candidate per
			// alias.
			std::unordered_set<Stand::CommandPhysical*> seenStandCommands;
			for (auto& [hash, command] : Stand::CommandRegistry::GetCommands())
			{
				auto* physical = command->getPhysical();
				if (!physical || physical->command_names.empty())
					continue;

				if (!seenStandCommands.insert(physical).second)
					continue;

				candidates.push_back({
				    physical->command_names,
				    physical->command_names.front() + " - " + physical->getMenuName().getLocalisedUtf8(),
				    // CommandPhysical::onClick() (not the empty-stub
				    // onCommand() a much earlier pass here mistakenly
				    // gated this whole registry on) is the real generic
				    // "activate as if clicked" entry point - already
				    // proven working by CommandHotkeyDispatch and
				    // GridItemStandCommand, both of which dispatch
				    // through it the same way. Meaningful today for
				    // CommandToggleNoCorrelation-derived commands (flips
				    // + calls onEnable()/onDisable()); CommandPhysical's
				    // own default onClick() is a no-op, so a match that's
				    // e.g. a bare CommandSlider is a real, disclosed
				    // no-op on Enter here - same limitation
				    // CommandHotkeyDispatch's own doc comment already
				    // discloses for a hotkey bound to one.
				    [physical] {
					    FiberPool::queueJob([physical] {
						    // CLICK_COMMAND (real Stand's own click type for
						    // exactly this - typed into its command box),
						    // not CLICK_MENU - GridItemStandCommand.cpp's own
						    // menu-click paths deliberately don't fire a
						    // toast at all any more (the row's own checkbox
						    // already shows the new state - see that file's
						    // own comment), but this path has no such visible
						    // feedback of its own, so it still calls
						    // ensureResponse()+respond() to fire the "<name>
						    // is now enabled/disabled" toast
						    // (CommandToggleNoCorrelation::updateState()'s
						    // own generic response) - same as
						    // CommandHotkeyDispatch.cpp already does for the
						    // same reason (a hotkey can fire with the menu
						    // closed entirely).
						    Stand::Click click(Stand::CLICK_COMMAND, Stand::TC_SCRIPT_YIELDABLE);
						    physical->onClick(click);
						    click.ensureResponse();
						    click.respond();
					    });
				    },
				});
			}

			return candidates;
		}
	}

	bool MenuCommandConsole::s_Open = false;
	std::string MenuCommandConsole::s_Buffer;
	std::vector<MenuCommandConsole::Match> MenuCommandConsole::s_Matches;
	int MenuCommandConsole::s_SelectedIndex = -1;

	void MenuCommandConsole::Open()
	{
		s_Buffer.clear();
		s_Matches.clear();
		s_SelectedIndex = -1;
		s_Open = true;
		InputCapture::SetTextInputActive(true);
	}

	bool MenuCommandConsole::IsOpen()
	{
		return s_Open;
	}

	void MenuCommandConsole::Close()
	{
		s_Open = false;
		s_Buffer.clear();
		s_Matches.clear();
		s_SelectedIndex = -1;
		InputCapture::SetTextInputActive(false);
	}

	void MenuCommandConsole::UpdateMatches()
	{
		s_Matches.clear();
		s_SelectedIndex = -1;

		if (s_Buffer.empty())
			return;

		auto candidates = CollectCandidates();

		// Exact match (on ANY of a candidate's own aliases) short-
		// circuits to the one result, the same as real Stand's own
		// checkCommandNameMatch NMT_HIT.
		for (auto& candidate : candidates)
		{
			if (std::ranges::find(candidate.names, s_Buffer) != candidate.names.end())
			{
				s_Matches = {{std::move(candidate.display), std::move(candidate.activate)}};
				s_SelectedIndex = 0;
				return;
			}
		}

		// Otherwise every candidate with an alias starting with (and
		// longer than) the typed text - real Stand's own NMT_GRAZED.
		for (auto& candidate : candidates)
		{
			bool grazed = std::ranges::any_of(candidate.names, [](const std::string& name) {
				return name.size() > s_Buffer.size() && name.compare(0, s_Buffer.size(), s_Buffer) == 0;
			});
			if (grazed)
				s_Matches.push_back({std::move(candidate.display), std::move(candidate.activate)});
		}

		if (s_Matches.empty())
			return;

		std::ranges::sort(s_Matches, [](const Match& a, const Match& b) {
			return a.hint < b.hint;
		});
		s_SelectedIndex = 0;
	}

	MenuCommandConsole::Layout MenuCommandConsole::ComputeLayout()
	{
		Layout layout{};
		layout.width = kWidth;
		layout.x = (Theme::kHudWidth - kWidth) * 0.5f;
		layout.y = kY;

		layout.titleHeight = kTitleHeight;
		layout.titleY = layout.y;

		layout.inputHeight = kInputHeight;
		layout.inputY = layout.titleY + layout.titleHeight;

		layout.rowHeight = kRowHeight;
		layout.firstRowY = layout.inputY + layout.inputHeight;

		return layout;
	}

	void MenuCommandConsole::Draw()
	{
		if (!s_Open)
			return;

		const auto layout = ComputeLayout();

		GridRenderer::DrawRect(layout.x, layout.titleY, layout.width, layout.titleHeight, Theme::kAccent);
		GridRenderer::DrawRect(layout.x, layout.inputY, layout.width, layout.inputHeight, Theme::kPanelBackground);

		// One row per shown match (capped at kMaxShown, same as
		// DrawText() below) plus one more for the trailing "+N more"
		// row, if there are more matches than that - the currently
		// selected match's own row gets the accent fill, everything
		// else the same muted panel background as the input row.
		const auto shown = (std::min)(s_Matches.size(), kMaxShown);
		for (size_t i = 0; i != shown; ++i)
		{
			const auto rowY = layout.firstRowY + static_cast<float>(i) * layout.rowHeight;
			GridRenderer::DrawRect(layout.x, rowY, layout.width, layout.rowHeight, (static_cast<int>(i) == s_SelectedIndex) ? Theme::kAccent : Theme::kPanelBackground);
		}
	}

	void MenuCommandConsole::DrawText()
	{
		if (!s_Open)
			return;

		const auto layout = ComputeLayout();

		const char* title = "CommandLegacy Console";
		const auto titleSize = GridRenderer::MeasureText(title, Theme::kSmallTextScale);
		GridRenderer::DrawText(layout.x + kPaddingX,
		    layout.titleY + std::max(0.f, (layout.titleHeight - titleSize.y) * 0.5f),
		    title,
		    Theme::kText,
		    Theme::kSmallTextScale);

		const auto displayLine = s_Buffer + "|";
		const auto lineSize = GridRenderer::MeasureText(displayLine.c_str());
		GridRenderer::DrawText(layout.x + kPaddingX, layout.inputY + std::max(0.f, (layout.inputHeight - lineSize.y) * 0.5f), displayLine.c_str(), Theme::kText);

		if (s_Buffer.empty())
			return;

		if (s_Matches.empty())
		{
			const char* unknown = "Unknown command.";
			const auto unknownSize = GridRenderer::MeasureText(unknown, Theme::kSmallTextScale);
			GridRenderer::DrawText(layout.x + kPaddingX,
			    layout.firstRowY + std::max(0.f, (layout.rowHeight - unknownSize.y) * 0.5f),
			    unknown,
			    Theme::kError,
			    Theme::kSmallTextScale);
			return;
		}

		// "<name> - <label>", exactly CommandIssuable::getCompletionHint()'s
		// own format on real Stand (confirmed against origin/stand-
		// reference) - e.g. "godmode - Immortality".
		const auto shown = (std::min)(s_Matches.size(), kMaxShown);
		for (size_t i = 0; i != shown; ++i)
		{
			const auto rowY = layout.firstRowY + static_cast<float>(i) * layout.rowHeight;
			const auto hintSize = GridRenderer::MeasureText(s_Matches[i].hint.c_str(), Theme::kSmallTextScale);
			GridRenderer::DrawText(layout.x + kPaddingX,
			    rowY + std::max(0.f, (layout.rowHeight - hintSize.y) * 0.5f),
			    s_Matches[i].hint.c_str(),
			    Theme::kText,
			    Theme::kSmallTextScale);
		}

		if (const auto more = s_Matches.size() - shown; more != 0)
		{
			const auto moreText = "+ " + std::to_string(more) + " more";
			const auto rowY = layout.firstRowY + static_cast<float>(shown) * layout.rowHeight;
			const auto moreSize = GridRenderer::MeasureText(moreText.c_str(), Theme::kSmallTextScale);
			GridRenderer::DrawText(layout.x + kPaddingX,
			    rowY + std::max(0.f, (layout.rowHeight - moreSize.y) * 0.5f),
			    moreText.c_str(),
			    Theme::kPlaceholderText,
			    Theme::kSmallTextScale);
		}
	}

	void MenuCommandConsole::HandleKey(unsigned int vkCode)
	{
		if (!s_Open)
			return;

		switch (vkCode)
		{
		case VK_RETURN:
			if (s_SelectedIndex >= 0 && static_cast<size_t>(s_SelectedIndex) < s_Matches.size())
			{
				if (auto activate = s_Matches[s_SelectedIndex].activate)
					activate();
				Close();
			}
			break;

		case VK_ESCAPE:
			Close();
			break;

		case VK_BACK:
			if (!s_Buffer.empty())
			{
				s_Buffer.pop_back();
				UpdateMatches();
			}
			break;

		case VK_UP:
		case VK_NUMPAD8:
			if (!s_Matches.empty() && s_SelectedIndex > 0)
				--s_SelectedIndex;
			break;

		case VK_DOWN:
		case VK_NUMPAD2:
			if (!s_Matches.empty() && static_cast<size_t>(s_SelectedIndex + 1) < (std::min)(s_Matches.size(), kMaxShown))
				++s_SelectedIndex;
			break;

		default:
			break;
		}
	}

	void MenuCommandConsole::HandleChar(wchar_t c)
	{
		if (!s_Open)
			return;

		// Reported bug: the 'U' keypress that opens this (see the
		// ungated AddWindowProcedureCallback in GridRenderer::Init())
		// generates its own WM_CHAR right after the WM_KEYDOWN that
		// opened it - same physical keystroke, so without this, every
		// open typed a leading "u" straight into the buffer. Real
		// Stand's own Commandbox has the identical problem for its own
		// (user-configurable) open hotkey and solves it the same way
		// (see origin/stand-reference's own Renderer.cpp WM_CHAR
		// handler, which checks the typed char against Input::scheme.
		// key_command_box) - narrowed here to this project's own single
		// fixed 'U' key: while the physical U key is still being held
		// down from the keystroke that opened this, its own char is
		// swallowed rather than typed. GetKeyState() (not GetAsyncKeyState())
		// matches MenuGrid.cpp's own existing convention for a
		// physical-key-still-down check from within a WndProc callback.
		if ((c == u'u' || c == u'U') && (GetKeyState('U') & 0x8000) != 0)
			return;

		// Printable ASCII only, same restriction MenuCommandBox::HandleChar()
		// applies (see that file's own comment).
		if (c >= 0x20 && c < 0x7f && s_Buffer.size() < 64)
		{
			s_Buffer.push_back(static_cast<char>(c));
			UpdateMatches();
		}
	}
}
