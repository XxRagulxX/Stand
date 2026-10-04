#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/World/Places/WaypointTo/CommandWpToCoord.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandListWpStores : public CommandList
    {
    public:
        explicit CommandListWpStores(CommandList* parent)
            : CommandList(parent, LIT("Stores"))
        {
            createChild<CommandWpToCoord>(LIT("Ammu-Nation (Paleto Bay)"),       1697.979126f,  3753.200439f);
            createChild<CommandWpToCoord>(LIT("Ammu-Nation (Downtown)"),          245.271103f,   -45.812595f);
            createChild<CommandWpToCoord>(LIT("Ammu-Nation (La Mesa)"),           844.124817f, -1025.570679f);
            createChild<CommandWpToCoord>(LIT("Ammu-Nation (Sandy Shores)"),     -325.886658f,  6077.020020f);
            createChild<CommandWpToCoord>(LIT("Ammu-Nation (Davis)"),            -664.217773f,  -943.364624f);
            createChild<CommandWpToCoord>(LIT("Ammu-Nation (Eclipse Blvd)"),    -1313.927856f,  -390.968933f);
            createChild<CommandWpToCoord>(LIT("Ammu-Nation (Chumash)"),         -3165.230713f,  1082.855103f);
            createChild<CommandWpToCoord>(LIT("Ammu-Nation (Thompson Rd)"),      2569.611572f,   302.575989f);
            createChild<CommandWpToCoord>(LIT("Ammu-Nation (Harmony)"),         -1111.251831f,  2688.476318f);
            createChild<CommandWpToCoord>(LIT("Shooting Range (South LS)"),       811.869873f, -2149.101563f);
            createChild<CommandWpToCoord>(LIT("Shooting Range (Downtown)"),        17.680408f, -1114.287964f);
            createChild<CommandWpToCoord>(LIT("Barber (Vespucci)"),              -821.994600f,  -187.177600f);
            createChild<CommandWpToCoord>(LIT("Barber (Strawberry)"),             133.570200f, -1710.918000f);
            createChild<CommandWpToCoord>(LIT("Barber (Little Seoul)"),         -1287.082200f, -1116.557600f);
            createChild<CommandWpToCoord>(LIT("Barber (Sandy Shores)"),          1933.119100f,  3726.079000f);
            createChild<CommandWpToCoord>(LIT("Barber (Rockford Hills)"),        1208.333500f,  -470.917000f);
            createChild<CommandWpToCoord>(LIT("Barber (Forum Drive)"),            -30.744800f,  -148.492100f);
            createChild<CommandWpToCoord>(LIT("Barber (Paleto Bay)"),            -280.816500f,  6231.770500f);
            createChild<CommandWpToCoord>(LIT("Clothes (Suburban - La Mesa)"),    419.531000f,  -807.578700f);
            createChild<CommandWpToCoord>(LIT("Clothes (Binco - Vespucci)"),     -818.621800f, -1077.533000f);
            createChild<CommandWpToCoord>(LIT("Clothes (Binco - Forum Dr)"),       80.665000f, -1391.669400f);
            createChild<CommandWpToCoord>(LIT("Clothes (Suburban - Paleto)"),    1687.881200f,  4820.550000f);
            createChild<CommandWpToCoord>(LIT("Clothes (Ponsonbys - Rockford)"), -1094.048700f,  2704.170700f);
            createChild<CommandWpToCoord>(LIT("Clothes (Discount Store)"),       1197.972200f,  2704.220500f);
            createChild<CommandWpToCoord>(LIT("Clothes (Suburban - Paleto 2)"),    -0.236100f,  6516.045400f);
            createChild<CommandWpToCoord>(LIT("Clothes (Sub Urban - Del Perro)"), -715.359800f,  -155.774200f);
            createChild<CommandWpToCoord>(LIT("Clothes (Binco - Strawberry)"),   -158.219900f,  -304.966300f);
            createChild<CommandWpToCoord>(LIT("Clothes (Suburban - LSIA)"),     -1455.004500f,  -233.186200f);
            createChild<CommandWpToCoord>(LIT("Clothes (Ponsonbys - Superstar)"), -1199.809200f,  -776.688600f);
            createChild<CommandWpToCoord>(LIT("Clothes (Suburban - Mirror Park)"), 618.185700f,  2752.566700f);
            createChild<CommandWpToCoord>(LIT("Clothes (Ponsonbys - Downtown)"),   126.685300f,  -212.500300f);
            createChild<CommandWpToCoord>(LIT("Clothes (Suburban - Chumash)"),  -3168.966300f,  1055.286900f);
            createChild<CommandWpToCoord>(LIT("Los Santos Customs (LSIA)"),     -1147.202759f, -1992.451782f);
            createChild<CommandWpToCoord>(LIT("Los Santos Customs (La Mesa)"),    724.479248f, -1089.010254f);
            createChild<CommandWpToCoord>(LIT("Los Santos Customs (Strawberry)"), -354.587860f,  -135.499176f);
            createChild<CommandWpToCoord>(LIT("Los Santos Customs (Harmony)"),   1174.730347f,  2644.499023f);
            createChild<CommandWpToCoord>(LIT("Tattoo (Mirror Park)"),            321.609800f,   179.416500f);
            createChild<CommandWpToCoord>(LIT("Tattoo (Sandy Shores)"),          1861.685300f,  3750.079800f);
            createChild<CommandWpToCoord>(LIT("Tattoo (Paleto Bay)"),            -290.160300f,  6199.094700f);
            createChild<CommandWpToCoord>(LIT("Tattoo (Chamberlain Hills)"),    -1153.948100f, -1425.018600f);
            createChild<CommandWpToCoord>(LIT("Tattoo (Little Seoul)"),          1322.454700f, -1651.125200f);
            createChild<CommandWpToCoord>(LIT("Tattoo (Chumash)"),              -3169.420400f,  1074.727200f);
        }
    };
}
