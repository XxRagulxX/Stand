#pragma once
#include "Commands/Online/CommandPerm.hpp"
#include "Core/ThreadContext.hpp"
#include "Menu/ClickType.hpp"
#include "Util/Label.hpp"

#include <functional>
#include <string>

namespace Stand
{
	class CommandPhysical;
	enum ClickResponseType : uint8_t
	{
		RESPONSE_CUSTOM,
		RESPONSE_GENERIC,
		RESPONSE_NONE,
	};

	enum Sound : uint8_t
	{
		SOUND_SELECT,
		SOUND_YES,
		SOUND_NO,
		SOUND_ERROR,
	};

	class Click
	{
	public:
		const ClickType type;
		ThreadContext thread_context;
		Sound sound_feedback = SOUND_SELECT;
		Label response = NOLABEL;
		ClickResponseType response_type = RESPONSE_GENERIC;
		bool response_acknowledged = false;

		explicit Click(ClickType type) noexcept;
		explicit Click(ClickType type, ThreadContext thread_context) noexcept;

		Click(const Click& b) noexcept;
		Click(Click&& b) noexcept;

		~Click();

		[[nodiscard]] Click derive(ClickType type) const noexcept;
		[[nodiscard]] Click deriveAuto() const noexcept;

		void ensureYieldableScriptThread(std::function<void()>&& func) const;
		void ensureScriptThread(std::function<void()>&& func) const;
		void ensureScriptThread(std::function<void(Click&)>&& func);
		void ensureWorkerContext(std::function<void()>&& func) const;

		[[nodiscard]] bool isMenu() const noexcept;
		[[nodiscard]] bool isAuto() const noexcept;
		[[nodiscard]] bool isStand() const noexcept;
		[[nodiscard]] bool isBulk() const noexcept;

		[[nodiscard]] bool hasResponse() const noexcept;
		[[nodiscard]] bool canHaveResponse() const noexcept;
		[[nodiscard]] bool canHaveGenericResponse() const noexcept;
		[[nodiscard]] bool canHaveGenericErrorResponse() const noexcept;
		void setResponse(Label&& response) noexcept;
		void setResponse(Label&& response) const;
		void showCommandBoxIfPossible(std::wstring prefill) const noexcept;
		void setGenericResponse(Label&& response);
		void setNoResponse() noexcept;

		void ensureResponse();
		[[nodiscard]] Label getResponse();
		void forgetResponse() noexcept;
		void respond();

		[[nodiscard]] bool isWeb() const noexcept { return false; }
		inline static bool self_chat_commands_friends_perms = false;
		inline static bool auto_proceed_warnings_for_commands = false;
		inline static bool auto_proceed_warnings_for_hotkeys = false;
		void stopInputIfAllowed() const noexcept {}
		[[nodiscard]] CommandPerm getEffectivePermissions() const { return COMMANDPERM_USERONLY; }
		[[nodiscard]] CommandPhysical* getCommandPhysicalFromArg(const std::wstring&) const { return nullptr; }
	};
}
