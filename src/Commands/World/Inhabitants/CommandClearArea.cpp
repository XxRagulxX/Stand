#include "Commands/World/Inhabitants/CommandClearArea.hpp"

#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Game/Entity.hpp"
#include "Game/Pools.hpp"
#include "Menu/Click.hpp"
#include "Rendering/MenuNavigation.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"
#include "Vehicle/Vehicle.hpp"
#include "World/Self.hpp"

#include <cmath>
#include <vector>

namespace Stand
{
    namespace
    {
        static void DrawEntityBox(Entity& ent, int r, int g, int b)
        {
            rage::fvector3 pos = ent.GetPosition();
            Hash model = ent.GetModel();
            Vector3 mn{}, mx{};
            MISC::GET_MODEL_DIMENSIONS(model, &mn, &mx);

            float h = ENTITY::GET_ENTITY_HEADING(ent.GetHandle()) * (3.14159265f / 180.0f);
            float ch = cosf(h), sh = sinf(h);

            struct P { float x, y, z; };
            auto rot = [&](float lx, float ly, float lz) -> P {
                return {
                    ch * lx + sh * ly + pos.x,
                    -sh * lx + ch * ly + pos.y,
                    lz + pos.z
                };
            };

            P c[8] = {
                rot(mn.x, mn.y, mn.z), rot(mx.x, mn.y, mn.z),
                rot(mx.x, mx.y, mn.z), rot(mn.x, mx.y, mn.z),
                rot(mn.x, mn.y, mx.z), rot(mx.x, mn.y, mx.z),
                rot(mx.x, mx.y, mx.z), rot(mn.x, mx.y, mx.z),
            };

            auto ln = [&](int a, int b2) {
                GRAPHICS::DRAW_LINE(c[a].x, c[a].y, c[a].z,
                                    c[b2].x, c[b2].y, c[b2].z, r, g, b, 255);
            };

            ln(0,1); ln(1,2); ln(2,3); ln(3,0);
            ln(4,5); ln(5,6); ln(6,7); ln(7,4);
            ln(0,4); ln(1,5); ln(2,6); ln(3,7);
        }

        static std::vector<Entity> CollectEntities(CommandClearArea* ca)
        {
            std::vector<Entity> out;
            rage::fvector3 player_pos = Self::GetPed().GetPosition();
            float dist = ca->distance->getFloatValue();
            bool ig_mission = ca->ignore_mission_entities->m_on;

            auto accept = [&](Entity ent) {
                if (!ent) return;
                if (ent.GetPosition().GetDistance(player_pos) >= dist) return;
                if (ig_mission && ent.IsMissionEntity()) return;
                out.push_back(ent);
            };

            if (ca->vehicles->m_on)
            {
                int player_veh_handle = Self::GetVehicle().GetHandle();
                for (auto veh : Pools::GetVehicles())
                {
                    if (!veh) continue;
                    if (player_veh_handle != 0 && veh.GetHandle() == player_veh_handle) continue;
                    accept(veh.As<Entity>());
                }
            }
            if (ca->peds->m_on)
            {
                for (auto ped : Pools::GetPeds())
                {
                    if (ped && !ped.IsPlayer())
                        accept(ped.As<Entity>());
                }
            }
            if (ca->objects->m_on || ca->cages->m_on)
            {
                for (auto obj : Pools::GetObjects())
                {
                    if (obj)
                        accept(obj);
                }
            }
            return out;
        }

        class CommandClearAreaAction : public CommandPhysical
        {
            CommandClearArea* m_ca;
        public:
            explicit CommandClearAreaAction(CommandList* parent)
                : CommandPhysical(COMMAND_ACTION, parent, LIT("Clear Area"), CMDNAMES("cleararea"))
                , m_ca(static_cast<CommandClearArea*>(parent))
            {}

            void onClick(Click& click) final
            {
                auto* ca = m_ca;
                click.ensureScriptThread([ca] {
                    auto entities = CollectEntities(ca);
                    for (auto& ent : entities)
                    {
                        if (ent)
                            ent.Delete();
                    }
                });
            }
        };
    }

    CommandClearArea::CommandClearArea(CommandList* parent)
        : CommandList(parent, LIT("Clear Area"))
    {
        createChild<CommandClearAreaAction>();
        distance = createChild<CommandSliderProximity>(CMDNAMES("clearproximity"), 5000);
        vehicles = createChild<CommandToggleNoCorrelation>(LIT("Vehicles"), CMDNAMES("clearvehicles"), NOLABEL, true);
        peds = createChild<CommandToggleNoCorrelation>(LIT("Pedestrians"), CMDNAMES("clearpeds"), NOLABEL, true);
        objects = createChild<CommandToggleNoCorrelation>(LIT("Objects"), CMDNAMES("clearobjects"), NOLABEL, true);
        cages = createChild<CommandToggleNoCorrelation>(LIT("Cages"), CMDNAMES("clearcages"), NOLABEL, true);
        pickups = createChild<CommandToggleNoCorrelation>(LIT("Pickups"), CMDNAMES("clearpickups"), NOLABEL, true);
        ignore_mission_entities = createChild<CommandToggleNoCorrelation>(LIT("Ignore Mission Entities"), CMDNAMES("clearnomission"));
        no_delay = createChild<CommandToggleNoCorrelation>(LIT("No Delay"), CMDNAMES("clearnodelay"));

        CommandTickDispatch::AddCommand(this);
    }

    CommandClearArea::~CommandClearArea()
    {
        CommandTickDispatch::RemoveCommand(this);
    }

    void CommandClearArea::onBecomesActiveGrid(Rendering::Grid* grid)
    {
        m_myGrid = grid;
    }

    void CommandClearArea::onTick()
    {
        if (!m_myGrid || Rendering::MenuNavigation::Current() != m_myGrid)
            return;

        auto entities = CollectEntities(this);
        for (auto& ent : entities)
        {
            if (ent)
                DrawEntityBox(ent, 0, 255, 0);
        }
    }
}
