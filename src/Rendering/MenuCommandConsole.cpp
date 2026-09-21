#include "Rendering/MenuCommandConsole.hpp"
#include "Commands/Extra/CommandFindVehicle.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/Widgets/CommandRegistry.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
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
		constexpr float kWidth = 547.f;
		constexpr float kY = 100.f;
		constexpr float kTitleHeight = 24.f;
		constexpr float kInputHeight = 32.f;
		constexpr float kRowHeight = 22.f;
		constexpr float kPaddingX = 6.f;
		constexpr size_t kMaxShown = 8;

		struct Candidate
		{
			std::vector<std::string> names;
			std::string label;   // plain label, e.g. "Set Wanted Level"
			std::string display; // "name - label" for prefix-match rows
			std::function<void()> activate;
			std::function<void(const std::string&)> activate_with_args;
		};

		std::vector<Candidate> CollectCandidates()
		{
			std::vector<Candidate> candidates;
			std::unordered_set<Stand::CommandPhysical*> seenStandCommands;
			for (auto& [hash, command] : Stand::CommandRegistry::GetCommands())
			{
				auto* physical = command->getPhysical();
				if (!physical || physical->command_names.empty())
					continue;

				if (!seenStandCommands.insert(physical).second)
					continue;

				const std::string lbl = physical->getMenuName().getLocalisedUtf8();
				const std::string disp = physical->command_names.front() + " - " + lbl;

				if (auto* slider = dynamic_cast<Stand::CommandSlider*>(physical))
				{
					candidates.push_back({
					    physical->command_names,
					    lbl,
					    disp,
					    [physical] {
						    FiberPool::queueJob([physical] {
							    Stand::Click click(Stand::CLICK_COMMAND, Stand::TC_SCRIPT_YIELDABLE);
							    physical->onClick(click);
							    click.ensureResponse();
							    click.respond();
						    });
					    },
					    [slider](const std::string& args) {
						    FiberPool::queueJob([slider, args] {
							    Stand::Click click(Stand::CLICK_COMMAND, Stand::TC_SCRIPT_YIELDABLE);
							    slider->setState(click, args);
							    click.ensureResponse();
							    click.respond();
						    });
					    },
					});
					continue;
				}

				if (auto* fv = dynamic_cast<Stand::CommandFindVehicle*>(physical))
				{
					candidates.push_back({
					    physical->command_names,
					    lbl,
					    disp,
					    [physical] {
						    FiberPool::queueJob([physical] {
							    Stand::Click click(Stand::CLICK_COMMAND, Stand::TC_SCRIPT_YIELDABLE);
							    physical->onClick(click);
							    click.ensureResponse();
							    click.respond();
						    });
					    },
					    fv->getArgsActivator(),
					});
					continue;
				}

				candidates.push_back({
				    physical->command_names,
				    lbl,
				    disp,
				    [physical] {
					    FiberPool::queueJob([physical] {
						    Stand::Click click(Stand::CLICK_COMMAND, Stand::TC_SCRIPT_YIELDABLE);
						    physical->onClick(click);
						    click.ensureResponse();
						    click.respond();
					    });
				    },
				    nullptr,
				});
			}

			return candidates;
		}
	}

	bool MenuCommandConsole::s_Open = false;
	std::string MenuCommandConsole::s_Buffer;
	std::vector<MenuCommandConsole::Match> MenuCommandConsole::s_Matches;
	int MenuCommandConsole::s_SelectedIndex = -1;
	std::function<void(const std::string&)> MenuCommandConsole::s_DirectCallback;
	std::string MenuCommandConsole::s_DirectHint;

	void MenuCommandConsole::Open()
	{
		s_Buffer.clear();
		s_DirectCallback = nullptr;
		s_DirectHint.clear();
		s_Matches.clear();
		s_SelectedIndex = -1;
		s_Open = true;
		InputCapture::SetTextInputActive(true);
	}

	void MenuCommandConsole::Open(std::string prefill, std::function<void(const std::string&)> callback, std::string hint)
	{
		s_Buffer = std::move(prefill);
		s_DirectCallback = std::move(callback);
		s_DirectHint = hint.empty() ? s_Buffer : std::move(hint);
		s_Matches.clear();
		s_SelectedIndex = -1;
		s_Open = true;
		InputCapture::SetTextInputActive(true);
		UpdateMatches();
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
		s_DirectCallback = nullptr;
		s_DirectHint.clear();
		InputCapture::SetTextInputActive(false);
	}

	void MenuCommandConsole::UpdateMatches()
	{
		s_Matches.clear();
		s_SelectedIndex = -1;

		if (s_DirectCallback)
		{
			const auto spacePos = s_Buffer.find(' ');
			if (spacePos != std::string::npos)
			{
				const auto argsPart = s_Buffer.substr(spacePos + 1);

				if (!argsPart.empty())
				{
					auto cb = s_DirectCallback;
					s_Matches = {{s_DirectHint, [cb, argsPart] { cb(argsPart); }, {}}};
					s_SelectedIndex = 0;
				}
			}
			return;
		}

		if (s_Buffer.empty())
			return;

		auto candidates = CollectCandidates();

		const auto spacePos = s_Buffer.find(' ');
		if (spacePos != std::string::npos)
		{
			const std::string cmdPart = s_Buffer.substr(0, spacePos);
			const std::string argsPart = s_Buffer.substr(spacePos + 1);
			for (auto& candidate : candidates)
			{
				if (candidate.activate_with_args
				    && std::ranges::find(candidate.names, cmdPart) != candidate.names.end())
				{
					s_Matches = {{candidate.label, [fn = std::move(candidate.activate_with_args), argsPart] {
					    fn(argsPart);
					}}};
					s_SelectedIndex = 0;
					return;
				}
			}
			return;
		}

		for (auto& candidate : candidates)
		{
			if (std::ranges::find(candidate.names, s_Buffer) != candidate.names.end())
			{
				s_Matches = {{std::move(candidate.display), std::move(candidate.activate), candidate.names.front()}};
				s_SelectedIndex = 0;
				return;
			}
		}

		for (auto& candidate : candidates)
		{
			bool grazed = std::ranges::any_of(candidate.names, [](const std::string& name) {
				return name.size() > s_Buffer.size() && name.compare(0, s_Buffer.size(), s_Buffer) == 0;
			});
			if (grazed)
				s_Matches.push_back({std::move(candidate.display), std::move(candidate.activate), candidate.names.front()});
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

		const char* title = "Command Console";
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

		case VK_TAB:
			if (s_SelectedIndex >= 0 && static_cast<size_t>(s_SelectedIndex) < s_Matches.size())
			{
				const auto& name = s_Matches[s_SelectedIndex].name;
				if (!name.empty())
				{
					s_Buffer = name + " ";
					UpdateMatches();
				}
			}
			break;

		default:
			break;
		}
	}

	void MenuCommandConsole::HandleChar(wchar_t c)
	{
		if (!s_Open)
			return;

		if ((c == u'u' || c == u'U') && (GetKeyState('U') & 0x8000) != 0)
			return;

		if (c >= 0x20 && c < 0x7f && s_Buffer.size() < 64)
		{
			s_Buffer.push_back(static_cast<char>(c));
			UpdateMatches();
		}
	}
}
