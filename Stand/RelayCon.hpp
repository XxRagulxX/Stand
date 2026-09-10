#pragma once

#include <soup/CertStore.hpp>
#include <soup/Scheduler.hpp>
#include <soup/WebSocketConnection.hpp>

#include "fwddecl.hpp"
#include "RecursiveSpinlock.hpp"

namespace Stand
{
#pragma pack(push, 1)
	class RelayCon : public soup::Scheduler
	{
	public:
		HANDLE thread = INVALID_HANDLE_VALUE;
		soup::SharedPtr<soup::CertStore> faketls_cert;
		soup::SharedPtr<soup::WebSocketConnection> sock;

		[[nodiscard]] bool isRunning() const noexcept;
		void init();

	private:
		void run();
		void setRecvHandler(soup::WebSocketConnection& s) SOUP_EXCAL;
		void processMessage(const std::string& message) SOUP_EXCAL;
		[[nodiscard]] Command* getCommand(const std::string& target);

	public:
		RecursiveSpinlock send_mtx;
		void sendLine(const std::string& str);
		void sendLineAsync(std::string&& str);

		void sendLang();

		void cleanup() noexcept;
		void close();
	};
#pragma pack(pop)

	inline RelayCon g_relay{};
}
