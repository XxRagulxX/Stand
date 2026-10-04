#include "Commands/World/Places/Interiors/CommandListInteriors.hpp"
#include "Commands/World/Places/Interiors/CommandInteriorBunker.hpp"
#include "Commands/World/Places/Interiors/CommandInteriorVehware.hpp"
#include "Commands/Widgets/CommandListSelect.hpp"
#include "Commands/Stand/CommandToggleNoCorrelation.hpp"
#include "Menu/Click.hpp"
#include "Util/Label.hpp"

namespace
{
    using namespace Stand;

    class InteriorStyleSelect final : public CommandListSelect
    {
        CommandInteriorCustomisable* const m_interior;
    public:
        InteriorStyleSelect(CommandList* parent, Label&& name, std::vector<CommandName>&& cmd_names,
                            std::vector<std::pair<long long, Label>>&& options, long long def_val,
                            CommandInteriorCustomisable* interior)
            : CommandListSelect(parent, std::move(name), std::move(cmd_names), NOLABEL,
                                std::move(options), def_val,
                                CMDFLAGS_LIST_SELECT & ~CMDFLAG_SUPPORTS_STATE_OPERATIONS)
            , m_interior(interior)
        {}

        void onChange(Click&, long long) override
        {
            m_interior->onOptionChanged();
        }
    };

    class InteriorToggle final : public CommandToggleNoCorrelation
    {
        CommandInteriorCustomisable* const m_interior;
    public:
        InteriorToggle(CommandList* parent, Label&& name, std::vector<CommandName>&& cmd_names,
                       CommandInteriorCustomisable* interior, bool def_on)
            : CommandToggleNoCorrelation(parent, std::move(name), std::move(cmd_names), NOLABEL, def_on,
                                         CMDFLAGS_TOGGLE & ~CMDFLAG_SUPPORTS_STATE_OPERATIONS)
            , m_interior(interior)
        {}

        void onChange(Click&) override
        {
            m_interior->onOptionChanged();
        }
    };
}

namespace Stand
{
    static Vector3 v3(float x, float y, float z)
    {
        Vector3 v{};
        v.x = x; v.y = y; v.z = z;
        return v;
    }

    CommandListInteriors::CommandListInteriors(CommandList* parent)
        : CommandList(parent, LIT("Interiors"))
    {
        {
            auto list = createChild<CommandList>(LIT("Bunker"));
            auto bunker = list->createChild<CommandInteriorBunker>();
            bunker->style = list->createChild<InteriorStyleSelect>(
                LIT("Style"), CMDNAMES("bunkerstyle"),
                std::vector<std::pair<long long, Label>>{{0, LIT("1")}, {1, LIT("2")}, {2, LIT("3")}},
                1,
                bunker
            );
            bunker->security = list->createChild<InteriorToggle>(
                LIT("Security"), CMDNAMES("bunkersecurity"), bunker, true);
            bunker->equipment_upgrade = list->createChild<InteriorToggle>(
                LIT("Equipment Upgrade"), CMDNAMES("bunkerequipmentupgrade"), bunker, true);
            bunker->gun_locker = list->createChild<InteriorToggle>(
                LIT("Gun Locker"), CMDNAMES("bunkergunlocker"), bunker, true);
            bunker->personal_quarters = list->createChild<InteriorToggle>(
                LIT("Personal Quarters"), CMDNAMES("bunkerpersonalquarters"), bunker, true);
            bunker->gun_range = list->createChild<InteriorToggle>(
                LIT("Gun Range"), CMDNAMES("bunkergunrange"), bunker, true);
        }

        {
            auto list = createChild<CommandList>(LIT("Vehicle Warehouse"));
            auto vehware = list->createChild<CommandInteriorVehware>();
            vehware->style = list->createChild<InteriorStyleSelect>(
                LIT("Style"), CMDNAMES("vehwarestyle"),
                std::vector<std::pair<long long, Label>>{
                    {2, LIT("Branded")},
                    {1, LIT("Urban")},
                    {0, LIT("Basic (Colours 1)")},
                    {-1, LIT("Basic (Colours 2)")},
                    {-2, LIT("Basic (Colours 3)")},
                    {-3, LIT("Basic (Colours 4)")}
                },
                1,
                vehware
            );
        }

        lscm        = createChild<CommandInterior>(LIT("LS Car Meet"),                   CMDNAMES("tpcarmeet"),                                    v3(-2000.0f,   1113.4f,   25.7f),   true);
        createChild<CommandInterior>(             LIT("IAA Facility"),                   CMDNAMES("tpfacility"),                                   v3( 2047.0f,   2942.0f,  -61.9f),   true);
        server_room = createChild<CommandInterior>(LIT("Server Room"),                   CMDNAMES("tpserverroom", "tpserveroom"),                   v3( 2168.0f,   2920.0f,  -84.0f),   true);
        createChild<CommandInterior>(             LIT("Bogdan's Submarine"),             CMDNAMES("tpsubmarine"),                                  v3(  514.19403f, 4838.989f, -62.587917f), true);
        createChild<CommandInterior>(             LIT("Humane Labs"),                    CMDNAMES("tphumanelabs"),                                  v3( 3616.85f,   3738.5544f, 28.69009f));
        createChild<CommandInterior>(             LIT("FIB (Bureau Raid)"),              CMDNAMES("tpburntfib"),                                   v3(  115.09295f, -747.57446f, 234.15239f));
        createChild<CommandInterior>(             LIT("FIB"),                            CMDNAMES("tpfib"),                                        v3(  129.75044f, -763.3821f, 242.1519f));
        createChild<CommandInterior>(             LIT("IAA"),                            CMDNAMES("tpiaa"),                                        v3(  114.059616f, -619.2508f, 206.04666f));
        ranch       = createChild<CommandInterior>(LIT("La Fuente Blanca"),             CMDNAMES("tpranch"),                                      v3( 1399.973f,  1148.756f, 113.3336f));
        createChild<CommandInterior>(             LIT("Split Sides West Comedy Club"),   CMDNAMES("tpcomedy"),                                     v3(  377.253662f, -998.272339f, -98.999947f));
        bahamamamas = createChild<CommandInterior>(LIT("Bahama Mamas"),                  CMDNAMES("tpbahamamamas"),                                 v3(-1388.0013f,  -618.41967f,  30.819599f));
        createChild<CommandInterior>(             LIT("Torture Room"),                   CMDNAMES("tptortureroom"),                                v3(  144.77847f, -2201.925f,    4.68802f));
        createChild<CommandInterior>(             LIT("Motel Room"),                     CMDNAMES("tpmotelroom"),                                  v3(  152.2604f,  -1004.47107f, -99.00476f));
        createChild<CommandInterior>(             LIT("Cinema"),                         CMDNAMES("tpcinema"),                                     v3(-1425.5645f,   -244.3f,     16.8053f));
        createChild<CommandInterior>(             LIT("Recycling Plant"),                CMDNAMES("tprecyling"),                                   v3( -598.6379f,  -1608.399f,   26.0108f));
        createChild<CommandInterior>(             LIT("Omega's Garage"),                 CMDNAMES("tpomega"),                                      v3( 2330.9282f,  2574.3071f,   46.680347f));
        createChild<CommandInterior>(             LIT("Lester's House"),                 CMDNAMES("tplester"),                                     v3( 1273.9f,    -1719.305f,   54.77141f));
        therapy     = createChild<CommandInterior>(LIT("Friedlander's Office"),          CMDNAMES("tptherapy"),                                    v3(-1908.024f,   -573.4244f,   19.09722f));
        sol         = createChild<CommandInterior>(LIT("Solomon's Office"),              CMDNAMES("tpsolomon"),                                    v3(-1005.663f,   -478.3461f,   49.0265f));
        createChild<CommandInterior>(             LIT("Floyd's House"),                  CMDNAMES("tpfloyd"),                                      v3(-1152.3326f, -1518.9675f,   10.632729f));
        createChild<CommandInterior>(             LIT("Janitor's House"),                CMDNAMES("tpjanitor"),                                    v3( -111.7116f,   -11.912f,    69.5196f));
        benny       = createChild<CommandInterior>(LIT("Benny's Original Motor Works"),  CMDNAMES("tpbenny"),                                      v3( -210.29391f, -1324.2723f,   30.890385f), true);
        chopshop    = createChild<CommandInterior>(LIT("Hayes Autos"),                   CMDNAMES("tpchopshop"),                                   v3(  479.0568f,  -1316.825f,   28.2038f));
        tequilala   = createChild<CommandInterior>(LIT("Tequi-La-La"),                   CMDNAMES("tprockclub", "tptequilala"),                    v3( -556.5089f,   286.3181f,   81.1763f));
        createChild<CommandInterior>(             LIT("Foundry"),                        CMDNAMES("tpfoundry"),                                    v3( 1100.1342f,  -2002.1356f,   30.063913f));
        udg         = createChild<CommandInterior>(LIT("Union Depository Garage"),       CMDNAMES("tpcarpark"),                                    v3(  -16.29585f,  -684.0385f,   33.50832f));
        eps         = createChild<CommandInterior>(LIT("Epsilon Storage Room"),          CMDNAMES("tpepsilonstorage"),                             v3(  245.1564f,   370.211f,   104.7382f));
        createChild<CommandInterior>(             LIT("Character Creation Room"),        CMDNAMES(),                                               v3(  402.5164f, -1002.847f,   -99.2587f));
        createChild<CommandInterior>(             LIT("Mission End Carpark"),            CMDNAMES(),                                               v3(  405.9228f,  -954.1149f,  -99.6627f));
        createChild<CommandInterior>(             LIT("Nightclub Garage"),               CMDNAMES(),                                               v3(-1505.783f,  -3012.587f,   -79.9999f), true);
        createChild<CommandInterior>(             LIT("Weed Farm"),                      CMDNAMES("tpweedfarm"),                                   v3( 1042.3301f,  -3199.142f,   -38.161537f), true);
        createChild<CommandInterior>(             LIT("Stilt House"),                    CMDNAMES("tpstilthouse"),                                 v3(  372.6707f,   405.5235f,  144.5326f), true);
        createChild<CommandInterior>(             LIT("High-End Apartment"),             CMDNAMES(),                                               v3( -282.0588f,  -955.17f,     85.3036f));
        createChild<CommandInterior>(             LIT("Mid-End Apartment"),              CMDNAMES(),                                               v3(  342.7946f,  -997.4225f,   -99.7444f));
        createChild<CommandInterior>(             LIT("Low-End Apartment"),              CMDNAMES(),                                               v3(  260.3268f,  -997.4298f,  -100.0086f));
        createChild<CommandInterior>(             LIT("High-End Garage"),                CMDNAMES(),                                               v3(  228.6161f,  -992.053f,    -99.9999f));
        createChild<CommandInterior>(             LIT("Mid-End Garage"),                 CMDNAMES(),                                               v3(  199.9716f,  -999.6678f,  -100.0f));
        createChild<CommandInterior>(             LIT("Low-End Garage"),                 CMDNAMES(),                                               v3(  173.1165f, -1003.28f,    -99.9999f));
        createChild<CommandInterior>(             LIT("Large Warehouse"),                CMDNAMES(),                                               v3( 1010.008f,  -3100.0f,     -39.9999f), true);
        createChild<CommandInterior>(             LIT("Medium Warehouse"),               CMDNAMES(),                                               v3( 1059.995f,  -3100.0f,     -39.9999f), true);
        createChild<CommandInterior>(             LIT("Small Warehouse"),                CMDNAMES(),                                               v3( 1094.997f,  -3100.012f,   -39.9999f), true);
    }
}
