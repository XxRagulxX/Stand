#pragma once

#include <cstdint>
#include <optional>
#include <string_view>
#include <utility>
#include <vector>

namespace Stand
{
	class SimplePattern
	{
	public:
		SimplePattern(std::string_view ida_sig);

		SimplePattern(const char* ida_sig) :
		    SimplePattern(std::string_view(ida_sig))
		{
		}

		SimplePattern(SimplePattern&& other) :
		    m_Bytes(std::move(other.m_Bytes))
		{
		}

		SimplePattern(const SimplePattern& other) :
		    m_Bytes(other.m_Bytes)
		{
		}

		std::vector<std::optional<std::uint8_t>> m_Bytes;
	};
}