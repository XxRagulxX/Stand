#include "Game/netIpAddress.hpp"

#include <stdexcept>

#include <soup/rand.hpp>
#include <soup/string.hpp>

namespace rage
{
	netIpAddress::operator std::string() const
	{
		return toString<std::string>();
	}

	netIpAddress::operator std::wstring() const
	{
		return toString<std::wstring>();
	}

	netIpAddress netIpAddress::fromString(const std::string& str)
	{
		auto fields = soup::string::explode(str, '.');
		if (fields.size() == 4)
		{
			uint8_t field_vals[4] = {};
			uint8_t i = 0;
			for (const auto& field : fields)
			{
				auto field_val = std::stoul(field);
				if (field_val > 255)
				{
					goto malformatted_ip_str;
				}
				field_vals[i++] = uint8_t(field_val);
			}
			return netIpAddress(field_vals[0], field_vals[1], field_vals[2], field_vals[3]);
		}
		else if (fields.size() != 1)
		{
		malformatted_ip_str:
			throw std::invalid_argument("");
		}
		return std::stoul(str);
	}

	netIpAddress netIpAddress::fromString(const std::wstring& str)
	{
		auto fields = soup::string::explode(str, L'.');
		if (fields.size() == 4)
		{
			uint8_t field_vals[4] = {};
			uint8_t i = 0;
			for (const auto& field : fields)
			{
				auto field_val = std::stoul(field);
				if (field_val > 255)
				{
					goto malformatted_ip_str;
				}
				field_vals[i++] = uint8_t(field_val);
			}
			return netIpAddress(field_vals[0], field_vals[1], field_vals[2], field_vals[3]);
		}
		else if (fields.size() != 1)
		{
		malformatted_ip_str:
			throw std::invalid_argument("");
		}
		return std::stoul(str);
	}

	netIpAddress netIpAddress::random()
	{
		return soup::rand.t<uint32_t>(0, 0xFFFFFFFF);
	}

	std::string netIpAddress::getGeoIpField(Stand::GeoIpField field)
	{
		return "";
	}
}
