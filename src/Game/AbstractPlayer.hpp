#pragma once

#include <string>
#include <vector>

namespace Stand
{
	class AbstractPlayer
	{
	public:
		int m_id;

		explicit AbstractPlayer(int id) : m_id(id) {}

		[[nodiscard]] static std::vector<AbstractPlayer> listAll() { return {}; }

		[[nodiscard]] std::string getName() const { return {}; }
		[[nodiscard]] std::wstring getNameW() const { return {}; }
		[[nodiscard]] std::wstring getNameW(bool with_flags) const { return {}; }
	};
}
