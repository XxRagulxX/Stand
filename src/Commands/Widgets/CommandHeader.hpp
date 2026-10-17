#pragma once

#include "Commands/Widgets/CommandListSelect.hpp"

namespace Stand
{
	class CommandHeader final : public CommandListSelect
	{
	public:
		explicit CommandHeader(CommandList* const parent);
		~CommandHeader();

		void onChange(Click& click, long long prev_value) final;
		void onTick() final;

		void setState(Click& click, const std::string& state) final;

		bool applyValue() const;

	private:
		size_t m_lastFrameCount = SIZE_MAX;
	};
}
