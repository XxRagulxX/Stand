#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/World/Places/TeleportTo/CommandTpToCoord.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandListStores : public CommandList
    {
    public:
        explicit CommandListStores(CommandList* parent)
            : CommandList(parent, LIT("Stores"))
        {
            createChild<CommandTpToCoord>(LIT("Ammu-Nation (Davis)"),          -663.3822f,  -935.5556f,   21.8137f);
            createChild<CommandTpToCoord>(LIT("Ammu-Nation (Downtown)"),        20.6671f,  -1107.5740f,   29.7972f);
            createChild<CommandTpToCoord>(LIT("Ammu-Nation (Eclipse Blvd)"),  -1305.8130f,   -394.2451f,   36.7073f);
            createChild<CommandTpToCoord>(LIT("Ammu-Nation (La Mesa)"),        842.7749f,   -1035.2000f,   28.2000f);
            createChild<CommandTpToCoord>(LIT("Ammu-Nation (Sandy Shores)"),  1693.4220f,    3760.1260f,   34.7071f);
            createChild<CommandTpToCoord>(LIT("Ammu-Nation (Paleto Bay)"),    -330.2924f,    6083.6630f,   31.4510f);
            createChild<CommandTpToCoord>(LIT("Ammu-Nation (Chumash)"),      -3173.3860f,    1086.5320f,   20.8420f);
            createChild<CommandTpToCoord>(LIT("Shooting Range"),              1188.6560f,    2768.1890f,   37.8700f);
            createChild<CommandTpToCoord>(LIT("Barber (Strawberry)"),          -822.3965f,   -184.0983f,   37.5630f);
            createChild<CommandTpToCoord>(LIT("Barber (Rockford Hills)"),     -816.6490f,    188.2480f,   76.4840f);
            createChild<CommandTpToCoord>(LIT("Barber (Little Seoul)"),       -1282.4910f,   -1117.6380f,    6.9946f);
            createChild<CommandTpToCoord>(LIT("Barber (Vespucci)"),          -1198.2440f,     -571.1300f,   22.7756f);
            createChild<CommandTpToCoord>(LIT("Barber (Forum Drive)"),         -30.6100f,    -152.0980f,   57.0752f);
            createChild<CommandTpToCoord>(LIT("Barber (Sandy Shores)"),       1931.2810f,    3717.5610f,   32.9028f);
            createChild<CommandTpToCoord>(LIT("Barber (Paleto Bay)"),         -279.2880f,    6230.3150f,   31.4868f);
            createChild<CommandTpToCoord>(LIT("Clothes (Binco - Strawberry)"), -155.6834f,   -304.3640f,   39.3736f);
            createChild<CommandTpToCoord>(LIT("Clothes (Sub Urban - Strawberry)"), -700.6000f, -152.2500f, 37.4145f);
            createChild<CommandTpToCoord>(LIT("Clothes (Binco - Vespucci)"), -1192.6240f,    -768.2080f,   17.3248f);
            createChild<CommandTpToCoord>(LIT("Clothes (Ponsonbys - Rockford Hills)"), -710.8700f, -152.6000f, 37.4145f);
            createChild<CommandTpToCoord>(LIT("Clothes (Ponsonbys - Superstar)"), -164.0000f,  -304.0000f,  39.7264f);
            createChild<CommandTpToCoord>(LIT("Clothes (Discount Store)"),    1193.6710f,    2700.7910f,   38.1561f);
            createChild<CommandTpToCoord>(LIT("Clothes (Suburban - Paleto)"), -296.4000f,    6208.2000f,   31.4868f);
            createChild<CommandTpToCoord>(LIT("Clothes (Binco - Forum Dr)"),   -12.1400f,    -714.1600f,   34.1432f);
            createChild<CommandTpToCoord>(LIT("Clothes (Vangelico)"),           -627.2730f,   -229.8870f,   38.0567f);
            createChild<CommandTpToCoord>(LIT("Clothes (Suburban - Hawick)"), -153.6150f,    -304.5400f,   39.3735f);
            createChild<CommandTpToCoord>(LIT("Clothes (Suburban - Forum Dr2)"), -14.2600f,  -714.1600f,   34.1432f);
            createChild<CommandTpToCoord>(LIT("Clothes (Flight Jacket)"),      -1455.6800f,   -384.8620f,   40.1678f);
            createChild<CommandTpToCoord>(LIT("Clothes (Sheriff)"),              602.5450f,   -1084.0640f,   29.3156f);
            createChild<CommandTpToCoord>(LIT("Los Santos Customs (LSIA)"),    -355.3630f,   -130.5140f,   38.4830f);
            createChild<CommandTpToCoord>(LIT("Los Santos Customs (La Mesa)"),  724.5860f,   -1088.2550f,   22.1758f);
            createChild<CommandTpToCoord>(LIT("Los Santos Customs (Strawberry)"), -204.0000f, -1328.0000f,  31.4460f);
            createChild<CommandTpToCoord>(LIT("Los Santos Customs (Harmony)"),   684.3650f,    -2018.7750f,  20.7660f);
            createChild<CommandTpToCoord>(LIT("Tattoo (Mirror Park)"),           1864.5090f,  3749.3690f,   32.5340f);
            createChild<CommandTpToCoord>(LIT("Tattoo (Chamberlain Hills)"),     -1150.3830f,  -1529.7570f,  10.6410f);
            createChild<CommandTpToCoord>(LIT("Tattoo (Little Seoul)"),          -3174.7280f,  1084.4130f,   20.9250f);
            createChild<CommandTpToCoord>(LIT("Tattoo (Hawick)"),                -302.6900f,  -589.8000f,   35.6949f);
            createChild<CommandTpToCoord>(LIT("Tattoo (Vespucci Beach)"),        -1290.9450f,  -732.3350f,   17.3248f);
            createChild<CommandTpToCoord>(LIT("Tattoo (Paleto Bay)"),            -285.5750f,   6234.7060f,   31.4868f);
        }
    };
}
