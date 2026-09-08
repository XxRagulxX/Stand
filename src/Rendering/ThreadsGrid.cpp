#include "Rendering/ThreadsGrid.hpp"

#include "Core/Pointers.hpp"
#include "Network/CNetGamePlayer.hpp"
#include "Rendering/GridItemButton.hpp"
#include "Rendering/GridItemFolder.hpp"
#include "Rendering/GridItemLiveText.hpp"
#include "Rendering/GridItemLocalList.hpp"
#include "Rendering/GridItemSelectList.hpp"
#include "Rendering/GridItemText.hpp"
#include "Rendering/Theme.hpp"
#include "Scripting/CGameScriptHandlerNetComponent.hpp"
#include "Scripting/FiberPool.hpp"
#include "Scripting/GtaThread.hpp"
#include "Scripting/Natives.hpp"
#include "Scripting/Scripts.hpp"
#include "Scripting/scrProgram.hpp"
#include "Scripting/scrThread.hpp"
#include "Util/Joaat.hpp"

#include <format>

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kSectionHeaderH = Theme::kContentItemHeight;
		constexpr float kItemH = Theme::kContentItemHeight;

		// Same "skip KILLED/zero-stack threads" filter the original's
		// own combo applies - shared between the picker's rows() and
		// onSelect so both walk Pointers.ScriptThreads in the same order
		// (index from rows() only means anything against the identical
		// filtered walk - see GridItemSelectList's own class comment).
		bool IsSelectableThread(rage::scrThread* thread)
		{
			return thread && thread->m_Context.m_State != rage::scrThread::State::KILLED && thread->m_Context.m_StackSize != 0;
		}
	}

	// Origin (1438, 622) - offset further down than every other content
	// Grid's usual (1438, 587) origin to clear ScriptsGrid's own tab
	// strip, same as every other GridTabbed target does (see
	// GridTabbed.hpp's own class comment and VehicleSpawnNewGrid's
	// identical offset).
	ThreadsGrid::ThreadsGrid() :
	    Grid(Theme::GetTabbedContentOrigin(), 0)
	{
	}

	bool ThreadsGrid::IsThreadStillValid(rage::scrThread* thread, rage::scrProgram* program)
	{
		// Mirrors MenuScripts.cpp's own per-frame validity check exactly
		// (the one gating its "Threads" tab body every frame) - not
		// re-confirming thread is still actually present in
		// Pointers.ScriptThreads, since the original doesn't either.
		return thread && program && program->IsValid() && program->m_RefCount != 0;
	}

	void ThreadsGrid::SyncThread()
	{
		if (m_CurThread && !IsThreadStillValid(m_CurThread, m_CurProgram))
		{
			m_CurThread = nullptr;
			m_CurProgram = nullptr;
			m_BytecodeContent.SetProgram(nullptr);
			invalidate();
		}
	}

	void ThreadsGrid::draw()
	{
		SyncThread();
		Grid::draw();
	}

	void ThreadsGrid::drawText()
	{
		SyncThread();
		Grid::drawText();
	}

	GridItem* ThreadsGrid::findItemAt(int16_t cursorX, int16_t cursorY)
	{
		SyncThread();
		return Grid::findItemAt(cursorX, cursorY);
	}

	void ThreadsGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Thread", Theme::kText));

		if (!Pointers.ScriptThreads || Pointers.ScriptThreads->size() == 0)
		{
			items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kItemH, "None", Theme::kError));
			return;
		}

		items_draft.push_back(std::make_unique<GridItemSelectList>(
		    Theme::kContentWidth,
		    "Search",
		    [] {
			    std::vector<std::string> rows;
			    if (!Pointers.ScriptThreads)
				    return rows;

			    for (auto* thread : *Pointers.ScriptThreads)
				    if (IsSelectableThread(thread))
					    rows.push_back(std::format("{} (ID {})", thread->m_ScriptName, thread->m_Context.m_ThreadId));

			    return rows;
		    },
		    [this](size_t index, const std::string&, bool, bool, bool) {
			    if (!Pointers.ScriptThreads)
				    return;

			    std::vector<rage::scrThread*> filtered;
			    for (auto* thread : *Pointers.ScriptThreads)
				    if (IsSelectableThread(thread))
					    filtered.push_back(thread);

			    if (index >= filtered.size())
				    return;

			    m_CurThread = filtered[index];
			    m_CurProgram = Scripts::FindScriptProgram(m_CurThread->m_ScriptHash);

			    if (!IsThreadStillValid(m_CurThread, m_CurProgram))
			    {
				    m_CurThread = nullptr;
				    m_CurProgram = nullptr;
			    }

			    m_BytecodeContent.SetProgram(m_CurProgram);
			    invalidate();
		    }));

		if (!m_CurThread || !m_CurProgram)
			return;

		// Categories - Bytecode, at the top once a thread is selected
		// (same "folders go first" convention every other page in this
		// project follows).
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Categories", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Bytecode", &m_BytecodeContent));

		// State - writes straight into the live thread struct's own
		// State field, same as the original's own bound ImGui::Combo.
		items_draft.push_back(std::make_unique<GridItemLocalList>(Theme::kContentWidth,
		    kItemH,
		    "State",
		    std::vector<std::string>{"Idle", "Running", "Killed", "Paused", "Unk4"},
		    static_cast<int>(m_CurThread->m_Context.m_State),
		    [this](int index) {
			    m_CurThread->m_Context.m_State = static_cast<rage::scrThread::State>(index);
		    }));

		if (m_CurThread->m_Context.m_State == rage::scrThread::State::KILLED)
		{
			items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kItemH, std::format("Exit Reason: {}", m_CurThread->m_ErrorMessage), Theme::kText));
			return;
		}

		items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Kill", [this] {
			m_CurThread->Kill();
			m_CurThread->m_Context.m_State = rage::scrThread::State::KILLED;
			invalidate();
		}));

		items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Log Labels", [this] {
			auto* program = m_CurProgram;
			FiberPool::queueJob([program] {
				for (std::uint32_t i = 0; i < program->m_StringsCount; i++)
				{
					if (auto* str = program->GetString(i))
					{
						if (HUD::DOES_TEXT_LABEL_EXIST(str))
							LOGF(INFO, "{} - {} (0x{:X}): {}", i, str, Joaat(str), HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(str));
					}
				}
			});
		}));

		// Info
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Info", Theme::kText));

		// Host/Take Control - only for a net-scripted thread (GtaThread's
		// own m_NetComponent), and only shown at all once selected, since
		// "taking control" while already host is a no-op - simpler than
		// the original's own disabled-but-still-visible button (this
		// menu has no disabled-but-clickable state - see
		// StatEditorGrid.hpp's identical note on its own Write/Force
		// Write split).
		if (auto* netComponent = reinterpret_cast<GtaThread*>(m_CurThread)->m_NetComponent)
		{
			items_draft.push_back(std::make_unique<GridItemLiveText>(
			    Theme::kContentWidth,
			    kItemH,
			    [this] {
				    auto* nc = reinterpret_cast<GtaThread*>(m_CurThread)->m_NetComponent;
				    auto* host = nc ? nc->GetHost() : nullptr;
				    return std::format("Host: {}", host ? host->GetName() : "(none)");
			    },
			    Theme::kText));

			if (!netComponent->IsLocalPlayerHost())
			{
				items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Take Control", [this] {
					auto* thread = m_CurThread;
					FiberPool::queueJob([thread] {
						Scripts::ForceScriptHost(thread);
					});
				}));
			}
		}

		// Program Counter/Stack Pointer change as the script actually
		// runs - GridItemLiveText re-reads them every drawText() call
		// (see that class's own doc comment); every other field below is
		// effectively fixed once a script starts, so a plain GridItemText
		// (read once, at populate() time) is enough for those.
		items_draft.push_back(std::make_unique<GridItemLiveText>(
		    Theme::kContentWidth,
		    kItemH,
		    [this] {
			    return std::format("Program Counter: 0x{:X}", m_CurThread->m_Context.m_ProgramCounter);
		    },
		    Theme::kText));
		items_draft.push_back(std::make_unique<GridItemLiveText>(
		    Theme::kContentWidth,
		    kItemH,
		    [this] {
			    return std::format("Stack Pointer: 0x{:X}", m_CurThread->m_Context.m_StackPointer);
		    },
		    Theme::kText));
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kItemH, std::format("Thread ID: {}", m_CurThread->m_Context.m_ThreadId), Theme::kText));
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kItemH, std::format("Stack Size: {}", m_CurThread->m_Context.m_StackSize), Theme::kText));
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kItemH, std::format("Code Size: {}", m_CurProgram->m_CodeSize), Theme::kText));
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kItemH, std::format("Arg Count: {}", m_CurProgram->m_ArgCount), Theme::kText));
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kItemH, std::format("Local Count: {}", m_CurProgram->m_LocalCount), Theme::kText));
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kItemH, std::format("Global Count: {}", m_CurProgram->m_GlobalCount), Theme::kText));
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kItemH, std::format("Native Count: {}", m_CurProgram->m_NativeCount), Theme::kText));
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kItemH, std::format("String Count: {}", m_CurProgram->m_StringsCount), Theme::kText));
	}
}
