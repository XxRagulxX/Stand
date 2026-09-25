#include "Commands/Vehicle/Doors/CommandListDoors.hpp"
#include "Commands/Vehicle/Doors/CommandVehDoorOpen.hpp"
#include "Commands/Widgets/CommandList.hpp"

namespace Stand
{
    CommandListDoors::CommandListDoors(CommandList* parent)
        : CommandList(parent, LIT("Open/Close Doors"), CMDNAMES_0())
    {
        createChild<CommandVehDoorOpen>(-1, LIT("All Doors"),   CMDNAMES("openvehdoors", "openvehicledoors"));
        createChild<CommandVehDoorOpen>( 0, LIT("Front Left"),  CMDNAMES_0());
        createChild<CommandVehDoorOpen>( 1, LIT("Front Right"), CMDNAMES_0());
        createChild<CommandVehDoorOpen>( 2, LIT("Back Left"),   CMDNAMES_0());
        createChild<CommandVehDoorOpen>( 3, LIT("Back Right"),  CMDNAMES_0());
        createChild<CommandVehDoorOpen>( 4, LIT("Hood"),        CMDNAMES_0());
        createChild<CommandVehDoorOpen>( 5, LIT("Trunk"),       CMDNAMES_0());
        createChild<CommandVehDoorOpen>( 6, LIT("Back"),        CMDNAMES_0());
        createChild<CommandVehDoorOpen>( 7, LIT("Back 2"),      CMDNAMES_0());
    }
}
