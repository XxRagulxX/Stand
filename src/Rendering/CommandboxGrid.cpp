#include "Rendering/CommandboxGrid.hpp"

#include "Rendering/StandPort/ColourUtil.hpp"
#include "Rendering/Commandbox.hpp"
#include "Commands/CommandExtraInfo.hpp"
#include "Core/ExceptionHandler.hpp"
#include "Rendering/StandPort/GridItemColourBox.hpp"
#include "Rendering/StandPort/GridItemColourBoxBordered.hpp"
#include "Rendering/StandPort/GridItemPrimaryText.hpp"
#include "Rendering/StandPort/GridItemText.hpp"
#include "Rendering/StandPort/GridItemCommandboxInput.hpp"
#include "Rendering/StandPort/GridItemTextBigCentre.hpp"
#include "Rendering/StandPort/GridItemTextBigCentreBordered.hpp"
#include "Rendering/Gui.hpp"
#include "Rendering/huddecl.hpp"
#include "Input/InputStub.hpp"
#include "Util/lang.hpp"
#include "Util/StringUtils.hpp"
#include "Rendering/UnicodePrivateUse.hpp"
#include "Rendering/TextColour.hpp"

namespace Stand
{
	using namespace Rendering;

	CommandboxGrid::CommandboxGrid()
		: Rendering::Grid((HUD_WIDTH / 2) - (width / 2), 100, spacer_size)
	{
	}

	void CommandboxGrid::clearCache() noexcept
	{
		cache_input.clear();
		cache_hasData = false;
		cache_data.clear();
	}

	static Rendering::GridItem* addExtra(std::vector<std::unique_ptr<Rendering::GridItem>>& items_draft, uint8_t offset, std::string&& text, int16_t width, int16_t height, uint8_t priority, Alignment alignment_relative_to_last = ALIGN_TOP_RIGHT, Rendering::GridItem* force_alignment_to = nullptr)
	{
		return ((g_commandbox.colour_selector_cursor == (CommandboxGrid::num_colours + offset))
			? items_draft.emplace_back(std::make_unique<GridItemTextBigCentreBordered>(std::move(text), width, height, priority, alignment_relative_to_last, force_alignment_to)).get()
			: items_draft.emplace_back(std::make_unique<GridItemTextBigCentre>(std::move(text), width, height, priority, alignment_relative_to_last, force_alignment_to)).get());
	}

	void CommandboxGrid::populate(std::vector<std::unique_ptr<Rendering::GridItem>>& items_draft)
	{
		items_draft.emplace_back(std::make_unique<GridItemPrimaryText>(width, 24, LANG_GET_W("CMDPRMPT")));

		auto text = g_commandbox.text.substr(0, g_commandbox.cursor);
		text.push_back(L'|');
		text.append(g_commandbox.text.substr(g_commandbox.cursor));
		items_draft.emplace_back(std::make_unique<GridItemCommandboxInput>(StringUtils::utf16_to_utf8(std::move(text)), width, 2));

		if (!g_commandbox.isColourSelectorActive())
		{
			std::wstring current_command{};
			auto res = StringUtils::explode_with_delimiting(g_commandbox.text, L';');
			if (!res.empty())
			{
				current_command = res.back();
			}
			if (g_commandbox.cursor >= (g_commandbox.text.length() - current_command.length()))
			{
				UnicodePrivateUse::toGta(current_command);
				std::wstring command_name{ current_command };
				std::wstring args{};
				if (Gui::parseCommand(command_name, args))
				{
					if (command_name != cache_input)
					{
						if (command_name != cacheupdate_input)
						{
							cache_mtx.lock();
							if (cacheupdate_running)
							{
								cacheupdate_next_input = std::move(command_name);
								cache_mtx.unlock();
							}
							else
							{
								cacheupdate_input = std::move(command_name);
								cacheupdate_running = true;
								cache_mtx.unlock();
								cacheupdate_thread.awaitCompletion();
								cacheupdate_thread.start([](soup::Capture&&)
								{
									__try
									{
										bool loop = false;
										do
										{
#if COMPACT_COMMAND_NAMES
											auto data = g_gui.findCommandsWhereCommandNameStartsWithAsWeakrefs(StringUtils::utf16_to_utf8(g_commandbox_grid.cacheupdate_input));
#else
											auto data = g_gui.findCommandsWhereCommandNameStartsWithAsWeakrefs(g_commandbox_grid.cacheupdate_input);
#endif
											g_commandbox_grid.cache_mtx.lock();
											{
												g_commandbox_grid.cache_data = std::move(data);
												g_commandbox_grid.cache_input = std::move(g_commandbox_grid.cacheupdate_input);
												g_commandbox_grid.cache_hasData = true;

												loop = !g_commandbox_grid.cacheupdate_next_input.empty();
												g_commandbox_grid.cacheupdate_running = loop;
												if (loop)
												{
													g_commandbox_grid.cacheupdate_input = std::move(g_commandbox_grid.cacheupdate_next_input);
													g_commandbox_grid.cacheupdate_next_input.clear();
												}
											}
											g_commandbox_grid.cache_mtx.unlock();
											g_commandbox_grid.update();
										} while (loop);
									}
									__EXCEPTIONAL()
									{
									}
								});
							}
						}
						if (!cache_hasData)
						{
							return;
						}
						command_name = cache_input;
					}
					bool collapsed = false;
					g_commandbox_grid.cache_mtx.lock();
					auto commands{ cache_data };
					g_commandbox_grid.cache_mtx.unlock();
					while (true)
					{
						if (commands.empty())
						{
							items_draft.emplace_back(std::make_unique<GridItemText>(LANG_FMT("CMDUNK", StringUtils::utf16_to_utf8(command_name)), width, 0, 3));
						}
						else if (commands.size() > 1)
						{
							if (command_name.length() > 2 || collapsed)
							{
								size_t shown = 0;
								for (const auto& command : commands)
								{
									EXCEPTIONAL_LOCK_READ(g_gui.root_mtx)
									if (auto cmd = command.getPointer())
									{
										items_draft.emplace_back(std::make_unique<GridItemText>(StringUtils::utf16_to_utf8(cmd->getCompletionHint()), width, 0, 3));
									}
									else
									{
										items_draft.emplace_back(std::make_unique<GridItemText>(LANG_GET("NPHYS"), width, 0, 3));
									}
									EXCEPTIONAL_UNLOCK_READ(g_gui.root_mtx)
									if (++shown == max_shown_matching_commands)
									{
										break;
									}
								}
								if (size_t more = (commands.size() - shown); more != 0)
								{
									if (commands.size() > 100)
									{
										items_draft.emplace_back(std::make_unique<GridItemText>(LANG_GET("MANYMORE"), width, 0, 3));
									}
									else
									{
										items_draft.emplace_back(std::make_unique<GridItemText>(LANG_FMT("MORE", more), width, 0, 3));
									}
								}
							}
						}
						else
						{
							CommandExtraInfo info{};
							EXCEPTIONAL_LOCK_READ(g_gui.root_mtx)
							if (auto cmd = commands.at(0).getPointer())
							{
								cmd->getExtraInfo(info, args);
							}
							else
							{
								info.completed_hint = LANG_GET_W("NPHYS");
							}
							EXCEPTIONAL_UNLOCK_READ(g_gui.root_mtx)
							if (info.collapse && !collapsed)
							{
								auto space_off = current_command.find(' ');
								if (space_off != std::string::npos)
								{
									command_name = current_command;
									command_name.erase(space_off, 1);
									space_off = command_name.find(' ');
									if (space_off != std::string::npos && command_name.length() > space_off + 1L)
									{
										args = command_name.substr(space_off + 1L);
									}
									else
									{
										args.clear();
									}
									StringUtils::to_lower(command_name);
#if COMPACT_COMMAND_NAMES
									commands = g_gui.findCommandsWhereCommandNameStartsWithAsWeakrefs(StringUtils::utf16_to_utf8(command_name));
#else
									commands = g_gui.findCommandsWhereCommandNameStartsWithAsWeakrefs(command_name);
#endif
									collapsed = true;
									continue;
								}
							}

							if (current_command.find(L' ') != std::string::npos)
							{
								const long long chars_left = (info.char_limit - args.length());
								if (info.char_limit != 0 && (!info.colour_selector || chars_left < 3))
								{
									items_draft.emplace_back(std::make_unique<GridItemText>(fmt::format(
										fmt::runtime(LANG_GET("CHRCNT")),
										fmt::arg("cur", args.length()),
										fmt::arg("max", info.char_limit)
									), width, 0, 4));
									break;
								}
								if (info.colour_selector)
								{
									items_draft.emplace_back(std::make_unique<GridItemText>(LANG_FMT(
										"FRMTSEL_AVAIL",
										Hotkey('U', true, false, false).toBracketedString()
									), width, 0, 4));
									break;
								}
							}
							if (!info.completed_hint.empty())
							{
								items_draft.emplace_back(std::make_unique<GridItemText>(StringUtils::utf16_to_utf8(std::move(info.completed_hint)), width, 0, 3));
								break;
							}
						}
						break;
					}
				}
				else
				{
					clearCache();
				}
			}
		}
		else
		{
			Rendering::GridItem* first_colour;
			uint8_t draw_cursor = 0;
			for (const auto& colour : TextColour::all)
			{
				DirectX::SimpleMath::Color dxcol{ float(colour.r) / 255, float(colour.g) / 255, float(colour.b) / 255, 1.0f };
				DirectX::SimpleMath::Color border_colour{ 1.0f, 1.0f, 1.0f, 1.0f };
				Rendering::GridItem* item;
				if (draw_cursor == g_commandbox.colour_selector_cursor)
				{
					ColourUtil::negateIfInsufficientContrast(border_colour, dxcol);
					item = items_draft.emplace_back(std::make_unique<GridItemColourBoxBordered>(4, draw_cursor == 0 ? ALIGN_BOTTOM_LEFT : ALIGN_TOP_RIGHT, std::move(dxcol), std::move(border_colour))).get();
				}
				else
				{
					item = items_draft.emplace_back(std::make_unique<GridItemColourBox>(4, draw_cursor == 0 ? ALIGN_BOTTOM_LEFT : ALIGN_TOP_RIGHT, std::move(dxcol))).get();
				}
				if (draw_cursor == 0)
				{
					first_colour = item;
				}
				++draw_cursor;
			}

#define ADD_EXTRA(...) addExtra(items_draft, __VA_ARGS__);

			Rendering::GridItem* reset = ADD_EXTRA(0, StringUtils::utf16_to_utf8(UnicodePrivateUse::rs), COLOURS_WIDTH(1), colour_size, 4, ALIGN_BOTTOM_LEFT, first_colour);
			ADD_EXTRA(1, StringUtils::utf16_to_utf8(UnicodePrivateUse::wanted_star), COLOURS_WIDTH(1), colour_size, 4);
			ADD_EXTRA(2, StringUtils::utf16_to_utf8(UnicodePrivateUse::lock), COLOURS_WIDTH(1), colour_size, 4);
			ADD_EXTRA(3, StringUtils::utf16_to_utf8(UnicodePrivateUse::rs_verified), COLOURS_WIDTH(2), colour_size, 4);
			ADD_EXTRA(4, StringUtils::utf16_to_utf8(UnicodePrivateUse::rs_created), COLOURS_WIDTH(2), colour_size, 4);
			ADD_EXTRA(5, StringUtils::utf16_to_utf8(UnicodePrivateUse::blank_box), COLOURS_WIDTH(2), colour_size, 4);
			ADD_EXTRA(6, StringUtils::utf16_to_utf8(UnicodePrivateUse::reset), COLOURS_WIDTH(1), colour_size, 4);
			ADD_EXTRA(7, StringUtils::utf16_to_utf8(UnicodePrivateUse::newline), COLOURS_WIDTH(1), colour_size, 4);

			std::string bottom_text{};
			if (g_commandbox.colour_selector_cursor == (CommandboxGrid::num_colours + 6))
			{
				bottom_text = LANG_GET("FRMTSEL_H_R");
			}
			else if (g_commandbox.colour_selector_cursor == (CommandboxGrid::num_colours + 7))
			{
				bottom_text = LANG_GET("FRMTSEL_H_N");
			}
			else
			{
				bottom_text = LANG_FMT(
					"FRMTSEL_H",
					Input::vk_to_string(VK_RETURN)
				);
			}
			items_draft.emplace_back(std::make_unique<GridItemText>(std::move(bottom_text), width, 0, 5, ALIGN_BOTTOM_LEFT, reset));
		}
	}
}
