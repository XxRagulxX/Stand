#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Menu/Click.hpp"
#include "Util/Label.hpp"

#include <atomic>
#include <functional>
#include <vector>

namespace Stand
{
    struct RGB { int r, g, b; };
    struct HSV { float h, s, v; };

    RGB HsvToRgb(HSV c);
    HSV RgbToHsv(RGB c);

    int LscGetVehicle();
    RGB GetVehicleColourRgb(int veh, bool primary);

    enum class ColourTarget { Primary, Secondary, Pearlescent, Interior, Wheel, Neon, TyreSmoke };

    void AddStdColours(CommandList* parent, ColourTarget tgt, bool searchable = false);

    RGB GetNeonColourRgb(int veh);
    RGB GetTyreSmokeColourRgb(int veh);

    class CommandVehRainbow : public CommandSlider
    {
        std::function<void(RGB)> m_setter;
        time_t m_lastTick = 0;
        float  m_hue      = 0.f;
        bool   m_ticking  = false;
    public:
        CommandVehRainbow(CommandList* parent, std::vector<CommandName> cmds,
                          std::function<void(RGB)> setter);
        ~CommandVehRainbow() override;
        void onChange(Click& click, int prev_value) override;
        void onTick() override;
    };

    class CommandColourList : public CommandList
    {
    public:
        std::atomic<uint32_t> m_listColour{0};
        using CommandList::CommandList;
    };

    class CommandVehicleColour : public CommandColourList
    {
    public:
        CommandVehicleColour(CommandList* parent, Label name,
                             std::vector<CommandName> cmdnames,
                             ColourTarget target);
    };

    class CommandNeonColour : public CommandColourList
    {
    public:
        explicit CommandNeonColour(CommandList* parent);
    };

    class CommandTyreSmokeColour : public CommandColourList
    {
    public:
        explicit CommandTyreSmokeColour(CommandList* parent);
    };
}
