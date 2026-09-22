#include "Commands/Vehicle/PersonalVehicles/CommandListCurrentPV.hpp"

#include "Commands/Vehicle/PersonalVehicles/CommandCurrentPV.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Menu/Click.hpp"
#include "Scripting/FiberPool.hpp"
#include "Util/Label.hpp"

namespace Stand
{
	namespace
	{
		class CommandCallCurrentPV : public CommandPhysical
		{
		public:
			explicit CommandCallCurrentPV(CommandList* parent)
				: CommandPhysical(COMMAND_ACTION, parent, LIT("Teleport To Me & Drive"), { CMDNAME("callpersonalvehicle") })
			{
			}

			void onClick(Click& click) override
			{
				FiberPool::queueJob([] { Features::CallPersonalVehicle(); });
			}
		};

		class CommandEnterCurrentPV : public CommandPhysical
		{
		public:
			explicit CommandEnterCurrentPV(CommandList* parent)
				: CommandPhysical(COMMAND_ACTION, parent, LIT("Enter"), { CMDNAME("enterpersonalvehicle") })
			{
			}

			void onClick(Click& click) override
			{
				FiberPool::queueJob([] { Features::EnterPersonalVehicle(); });
			}
		};

		class CommandReturnCurrentPV : public CommandPhysical
		{
		public:
			explicit CommandReturnCurrentPV(CommandList* parent)
				: CommandPhysical(COMMAND_ACTION, parent, LIT("Return To Storage"), { CMDNAME("returnpv") })
			{
			}

			void onClick(Click& click) override
			{
				FiberPool::queueJob([] { Features::ReturnPVToStorage(); });
			}
		};

		class CommandImaniRC : public CommandPhysical
		{
		public:
			explicit CommandImaniRC(CommandList* parent)
				: CommandPhysical(COMMAND_ACTION, parent, LIT("Imani Tech Remote Control"), { CMDNAME("imanirc") })
			{
			}

			void onClick(Click& click) override
			{
				FiberPool::queueJob([] { Features::ImaniTechRemoteControl(); });
			}
		};
	}

	CommandListCurrentPV::CommandListCurrentPV(CommandList* parent)
		: CommandList(parent, LIT("Current Personal Vehicle"), { CMDNAME("currentpv") })
	{
		createChild<CommandCallCurrentPV>();
		createChild<CommandEnterCurrentPV>();
		createChild<CommandReturnCurrentPV>();
		createChild<CommandImaniRC>();
	}
}
