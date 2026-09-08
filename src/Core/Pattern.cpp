#include "Core/Pattern.hpp"

namespace Stand
{
	static constexpr std::optional<std::uint8_t> ToHex(char c)
	{
		switch (c)
		{
		case '0': return 0x0;
		case '1': return 0x1;
		case '2': return 0x2;
		case '3': return 0x3;
		case '4': return 0x4;
		case '5': return 0x5;
		case '6': return 0x6;
		case '7': return 0x7;
		case '8': return 0x8;
		case '9': return 0x9;
		case 'a':
		case 'A': return 0xA;
		case 'b':
		case 'B': return 0xB;
		case 'c':
		case 'C': return 0xC;
		case 'd':
		case 'D': return 0xD;
		case 'e':
		case 'E': return 0xE;
		case 'f':
		case 'F': return 0xF;
		default: return std::nullopt;
		}
	}

	SimplePattern::SimplePattern(std::string_view ida_sig)
	{
		if (ida_sig.empty())
			return;

		const auto size_minus_one = ida_sig.size() - 1;

		m_Bytes.reserve(size_minus_one / 2);

		for (std::size_t i = 0; i < size_minus_one; ++i)
		{
			if (ida_sig[i] == ' ')
				continue;

			if (ida_sig[i] != '?')
			{
				auto c1 = ToHex(ida_sig[i]);
				auto c2 = ToHex(ida_sig[i + 1]);

				if (c1 && c2)
				{
					m_Bytes.emplace_back(
					    static_cast<std::uint8_t>(
					        (*c1 * 0x10) + *c2));

					++i;
				}
			}
			else
			{
				m_Bytes.push_back(std::nullopt);

				if (ida_sig[i + 1] == '?')
					++i;
			}
		}
	}
}