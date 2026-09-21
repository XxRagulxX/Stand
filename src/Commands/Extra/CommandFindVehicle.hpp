#pragma once
#include "Commands/Widgets/CommandPhysical.hpp"

#include <functional>
#include <string>

namespace Stand
{
	class CommandFindVehicle : public CommandPhysical
	{
	public:
		using CommandPhysical::CommandPhysical;

		virtual std::function<void(const std::string&)> getArgsActivator() const
		{
			return nullptr;
		}
	};
}
