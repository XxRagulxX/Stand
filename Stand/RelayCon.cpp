#include "RelayCon.hpp"

#include <fmt/core.h>

#include <soup/netStatus.hpp>
#include <soup/sha256.hpp>
#include <soup/truHostTask.hpp>

#include "Auth.hpp"
#include "Exceptional.hpp"
#include "FiberPool.hpp"
#include "Gui.hpp"
#include "lang.hpp"
#include "StringUtils.hpp"
#include "Util.hpp"

#define RELAY_PORT 25769

namespace Stand
{
	bool RelayCon::isRunning() const noexcept
	{
		return thread != INVALID_HANDLE_VALUE;
	}

	void RelayCon::init()
	{
		if (!isRunning())
		{
			thread = Exceptional::createExceptionalThread([]
			{
				THREAD_NAME("RelayCon");
				g_relay.run();
				CloseHandle(g_relay.thread);
				g_relay.thread = INVALID_HANDLE_VALUE;
			});
		}
	}

	void RelayCon::run()
	{
		Util::toast(LANG_FMT("RELAY_C", "relay"), TOAST_ABOVE_MAP);
		{
			const auto& tht = *soup::Scheduler::add<soup::truHostTask>(
				SOUP_IPV4_NWE(198, 251, 89, 45),
				soup::string::bin2hexLower(soup::sha256::hash(g_auth.activation_key_to_try)),
				faketls_cert
			);
			soup::Scheduler::tick();
			while (!tht.isWorkDone() && !g_gui.isUnloadPending())
			{
				::Sleep(100);
				soup::Scheduler::tick();
			}
			if (!tht.out_sock)
			{
				if (!tht.ctrl_sock)
				{
					Util::toast(LANG_FMT("RELAY_E", soup::netStatusToString(tht.relay_connector.getStatus())), TOAST_ABOVE_MAP);
				}
				else
				{
					Util::toast(LOC("RELAY_L"), TOAST_ABOVE_MAP);
				}
				return;
			}
			sock = tht.out_sock;
		}
		setRecvHandler(*sock);
		//g_logger.log(fmt::format("Web interface is connected via {}", sock->peer.toString()));
		Util::toast(LOC("WEB_T"), TOAST_ABOVE_MAP);
		sendLang();
		g_gui.sendRootListToWeb();
		soup::Scheduler::run();

		if (!g_gui.isUnloadPending())
		{
			Util::toast(LOC("WEB_T_L"), TOAST_ABOVE_MAP);
		}

		close();
		cleanup();
	}

	void RelayCon::setRecvHandler(soup::WebSocketConnection& s) SOUP_EXCAL
	{
		s.wsRecv([](soup::WebSocketConnection& s, soup::WebSocketMessage&& msg, soup::Capture&& cap)
		{
			const auto self = cap.get<RelayCon*>();
			self->processMessage(msg.data);
			self->setRecvHandler(s);
		}, this);
	}

	void RelayCon::processMessage(const std::string& message) SOUP_EXCAL
	{
		if (g_gui.isUnloadPending())
		{
			return;
		}
		//g_logger.log(message);
		const auto& message_substr_0_2 = message.substr(0, 2);
		if (message_substr_0_2 == "k ")
		{
			Command* const command = getCommand(message.substr(2));
			if (command != nullptr)
			{
				FiberPool::queueJob([command]
				{
					Click click(CLICK_WEB, TC_SCRIPT_YIELDABLE);
					command->getPhysical()->onClick(click);
				});
			}
		}
		else if (message_substr_0_2 == "p ")
		{
			Command* const command = g_gui.root_list->resolveCommandWeb(message.substr(2));
			if (command == nullptr)
			{
				sendLine(std::move(std::string("toast Failed to resolve ").append(message.substr(2))));
			}
			else
			{
				Click click(CLICK_WEB, TC_OTHER);
				command->getPhysical()->onClick(click);
			}
		}
		else if (message_substr_0_2 == "c ")
		{
			std::string data = message.substr(2);
			FiberPool::queueJob([data{ std::move(data) }]() mutable
			{
				Click click(CLICK_WEB_COMMAND, TC_OTHER);
				g_gui.triggerCommands(std::move(data), click);
			});
		}
		else if (message_substr_0_2 == "s ")
		{
			size_t i = message.find(':');
			if (i != std::string::npos)
			{
				Command* const command = getCommand(message.substr(2, i - 2));
				if (command != nullptr)
				{
					std::string data = message.substr(i + 1);
					FiberPool::queueJob([command, data{ std::move(data) }]
					{
						Click click(CLICK_WEB, TC_SCRIPT_YIELDABLE);
						command->getPhysical()->setState(click, data);
					});
				}
			}
		}
		else if (message.length() > 17)
		{
			if (message.substr(0, 17) == "notify_above_map ")
			{
				Util::toast(message.substr(17), TOAST_DEFAULT);
			}
		}
	}

	Command* RelayCon::getCommand(const std::string& target)
	{
		Command* const command = (g_gui.web_focus == nullptr ? g_gui.root_list.get() : g_gui.web_focus)->resolveChildByMenuNameWebString(target);
		if (command == nullptr)
		{
			sendLine(std::move(std::string("toast Unknown command: ").append(target)));
		}
		return command;
	}

	void RelayCon::sendLine(const std::string& str)
	{
		EXCEPTIONAL_LOCK(send_mtx)
		if (sock)
		{
			soup::ServerWebService::wsSendText(*sock, str);
		}
		EXCEPTIONAL_UNLOCK(send_mtx)
	}

	void RelayCon::sendLineAsync(std::string&& str)
	{
		Exceptional::createManagedExceptionalThread(__FUNCTION__, [this, str{ std::move(str) }]() mutable
		{
			sendLine(str);
		});
	}

	void RelayCon::sendLang()
	{
		auto web_lang = Lang::active_id;
		if (!Lang::isSupportedByWebInterface(web_lang))
		{
			web_lang = LANG_EN;
		}
		sendLine(std::move(std::string("lang ").append(Lang::idToCodeA(web_lang))));
	}

	void RelayCon::cleanup() noexcept
	{
		g_gui.web_focus = nullptr;
	}

	void RelayCon::close()
	{
		if (sock)
		{
			sock->close();
		}
	}
}
