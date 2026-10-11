#pragma once

#include <string>

#include "Util/is_number_char.hpp"

namespace Stand
{
	template <typename Char>
	inline int alphanum_impl(const Char* l, const Char* r)
	{
		enum mode_t { STRING, NUMBER } mode = STRING;

		while (*l && *r)
		{
			if (mode == STRING)
			{
				Char l_char, r_char;
				while ((l_char = *l) && (r_char = *r))
				{
					const bool l_digit = is_number_char(l_char), r_digit = is_number_char(r_char);
					if (l_digit && r_digit)
					{
						mode = NUMBER;
						break;
					}
					if (l_digit) return -1;
					if (r_digit) return +1;
					const int diff = l_char - r_char;
					if (diff != 0) return diff;
					++l;
					++r;
				}
			}
			else
			{
				unsigned long long l_int = 0;
				while (*l && is_number_char(*l))
				{
					l_int = l_int * 10 + *l - '0';
					++l;
				}

				unsigned long long r_int = 0;
				while (*r && is_number_char(*r))
				{
					r_int = r_int * 10 + *r - '0';
					++r;
				}

				const long long diff = l_int - r_int;
				if (diff != 0)
					return diff < 0 ? -1 : +1;

				mode = STRING;
			}
		}

		if (*r) return -1;
		if (*l) return +1;
		return 0;
	}

	inline bool alphanum_less(const std::string& l, const std::string& r)
	{
		return alphanum_impl(l.c_str(), r.c_str()) < 0;
	}

	inline bool alphanum_less(const std::wstring& l, const std::wstring& r)
	{
		return alphanum_impl(l.c_str(), r.c_str()) < 0;
	}
}
