#pragma once

#include <string>

namespace Stand
{
	class AbstractPlayer
	{
	public:
		int m_id;

		explicit AbstractPlayer(int id) : m_id(id) {}

		[[nodiscard]] std::string getName() const { return {}; }
	};
}
