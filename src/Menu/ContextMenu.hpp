#pragma once
#include "Commands/Widgets/CommandList.hpp"

namespace Stand
{
	class CommandCtxHotkeys;

	struct ContextMenu
	{
		inline static std::unique_ptr<CommandList> root = {};
		inline static CommandList* view = nullptr;
		inline static CommandPhysical* pending_target = nullptr;

		[[nodiscard]] static Command* getTarget() noexcept;
		[[nodiscard]] static CommandPhysical* getTargetPhysical() noexcept;

		[[nodiscard]] static bool isAvailable() noexcept;
		static void toggleIfAvailable(ThreadContext thread_context);
		static void toggle(ThreadContext thread_context);
		[[nodiscard]] static bool isOpen() noexcept;
		static void close(ThreadContext thread_context);
		static void open(ThreadContext thread_context, CommandPhysical* target_override = nullptr);
		static CommandCtxHotkeys* getHotkeysList();
		static void openIntoHotkeysList(ThreadContext thread_context, CommandPhysical* target_override = nullptr);

		static void doSaveState(Click& click);
		static void doLoadState(Click& click);
		static void doApplyDefaultState(Click& click);
		static void doApplyDefaultStateToChildren(Click& click);
		static void doMin(Click& click);
		static void doMax(Click& click);
	};
}
