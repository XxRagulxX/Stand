#include "Scripting/ScriptData.hpp"
#include "Scripting/scrProgram.hpp"

namespace Stand
{
	ScriptData::ScriptData(rage::scrProgram* program)
	{
		auto pages = new uint8_t*[program->GetNumCodePages()];

		for (auto i = 0u; i < program->GetNumCodePages(); i++)
		{
			pages[i] = new uint8_t[program->GetCodePageSize(i)];
			std::memcpy(pages[i], program->GetCodePage(i), program->GetCodePageSize(i));
		}

		m_Data = pages;
		m_Pages = program->GetNumCodePages();
		m_Size = program->GetFullCodeSize();
		m_Name = program->m_Name;
	}

	ScriptData::~ScriptData()
	{
		for (auto i = 0u; i < m_Pages; i++)
		{
			delete[] m_Data[i];
		}

		delete[] m_Data;
		m_Data = nullptr;
		m_Name = nullptr;
	}

	std::uint8_t* ScriptData::GetCodeLocation(ScriptData* data, int idx)
	{
		return &data->GetData()[idx >> 14][idx & 0x3FFF];
	}

	std::optional<std::uint32_t> ScriptData::GetCodeLocationByPattern(ScriptData* data, const SimplePattern& pattern)
	{
		if (!data || pattern.m_Bytes.empty())
			return std::nullopt;

		const auto codeSize = static_cast<std::size_t>(data->GetSize());

		const auto patternSize = pattern.m_Bytes.size();

		if (patternSize > codeSize)
			return std::nullopt;

		for (std::size_t i = 0;
		    i + patternSize <= codeSize;
		    ++i)
		{
			bool found = true;

			for (std::size_t j = 0;
			    j < patternSize;
			    ++j)
			{
				if (pattern.m_Bytes[j].has_value())
				{
					const auto loc = static_cast<std::uint32_t>(i + j);

					if (pattern.m_Bytes[j].value()
					    != *GetCodeLocation(data, loc))
					{
						found = false;
						break;
					}
				}
			}

			if (found)
				return static_cast<std::uint32_t>(i);
		}

		return std::nullopt;
	}
}