#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Commands/World/Watch_Dogs/CommandDedsec.hpp"
#include "Commands/World/Watch_Dogs/DedsecHacks.hpp"
#include "Game/AllEntitiesEveryTick.hpp"
#include "Menu/Click.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandListDedsec : public CommandList
    {
    private:

        class CommandDedsecSticky : public CommandSlider
        {
        public:
            explicit CommandDedsecSticky(CommandList* parent)
                : CommandSlider(parent, LIT("Sticky Selection"), CMDNAMES("dedsecsticky"), LIT("Hacks are only deselected after the specified delay in milliseconds. This is helpful for controllers."), 0, 2000, 0, 10)
            {
            }
            void onChange(Click&, int) override { AllEntitiesEveryTick::dedsec_deselect_delay = value; }
        };

        class CommandDedsecBoolToggle : public CommandToggle
        {
            bool* target;
        public:
            CommandDedsecBoolToggle(CommandList* parent, Label&& name, std::vector<CommandName>&& cmds, bool* target, bool def = true)
                : CommandToggle(parent, std::move(name), std::move(cmds), NOLABEL, def), target(target)
            {
            }
            void onEnable(Click&) override { *target = true; }
            void onDisable(Click&) override { *target = false; }
        };

        class CommandDedsecColour : public CommandList
        {
        private:
            class CommandDedsecRainbow : public CommandSlider
            {
            public:
                explicit CommandDedsecRainbow(CommandList* parent)
                    : CommandSlider(parent, LIT("Rainbow Mode"), CMDNAMES("dedsecrainbow"), LIT("Cycles the colour's hue every x milliseconds but still allows you to change the saturation, value, and opacity."), 0, 1000, 0, 1)
                {
                }
                void onChange(Click&, int) override { AllEntitiesEveryTick::dedsec_rainbow = value; }
            };

            class CommandDedsecHue : public CommandSlider
            {
                CommandDedsecColour* colour;
            public:
                explicit CommandDedsecHue(CommandList* parent, CommandDedsecColour* c)
                    : CommandSlider(parent, LIT("Hue"), {}, NOLABEL, 0, 360, AllEntitiesEveryTick::dedsec_h, 1), colour(c)
                {
                }
                void onChange(Click&, int) override;
            };

            class CommandDedsecSat : public CommandSlider
            {
                CommandDedsecColour* colour;
            public:
                explicit CommandDedsecSat(CommandList* parent, CommandDedsecColour* c)
                    : CommandSlider(parent, LIT("Saturation"), {}, NOLABEL, 0, 100, AllEntitiesEveryTick::dedsec_s, 1), colour(c)
                {
                }
                void onChange(Click&, int) override;
            };

            class CommandDedsecVal : public CommandSlider
            {
                CommandDedsecColour* colour;
            public:
                explicit CommandDedsecVal(CommandList* parent, CommandDedsecColour* c)
                    : CommandSlider(parent, LIT("Value"), {}, NOLABEL, 0, 100, AllEntitiesEveryTick::dedsec_v, 1), colour(c)
                {
                }
                void onChange(Click&, int) override;
            };

            class CommandDedsecOpacity : public CommandSlider
            {
            public:
                explicit CommandDedsecOpacity(CommandList* parent)
                    : CommandSlider(parent, LIT("Opacity"), {}, NOLABEL, 0, 255, AllEntitiesEveryTick::dedsec_a, 1)
                {
                }
                void onChange(Click&, int) override { AllEntitiesEveryTick::dedsec_a = value; }
            };

            class CommandDedsecRed : public CommandSlider
            {
                CommandDedsecColour* colour;
            public:
                explicit CommandDedsecRed(CommandList* parent, CommandDedsecColour* c)
                    : CommandSlider(parent, LIT("Red"), {}, NOLABEL, 0, 255, AllEntitiesEveryTick::dedsec_r, 1), colour(c)
                {
                }
                void onChange(Click&, int) override;
            };

            class CommandDedsecGreen : public CommandSlider
            {
                CommandDedsecColour* colour;
            public:
                explicit CommandDedsecGreen(CommandList* parent, CommandDedsecColour* c)
                    : CommandSlider(parent, LIT("Green"), {}, NOLABEL, 0, 255, AllEntitiesEveryTick::dedsec_g, 1), colour(c)
                {
                }
                void onChange(Click&, int) override;
            };

            class CommandDedsecBlue : public CommandSlider
            {
                CommandDedsecColour* colour;
            public:
                explicit CommandDedsecBlue(CommandList* parent, CommandDedsecColour* c)
                    : CommandSlider(parent, LIT("Blue"), {}, NOLABEL, 0, 255, AllEntitiesEveryTick::dedsec_b, 1), colour(c)
                {
                }
                void onChange(Click&, int) override;
            };

            static void hsvToRgb(int h, int s, int v, int& r, int& g, int& b)
            {
                float hf = (float)h;
                float sf = (float)s / 100.0f;
                float vf = (float)v / 100.0f;
                if (sf == 0.0f)
                {
                    int iv = (int)(vf * 255.0f);
                    r = g = b = iv;
                    return;
                }
                float sector = hf / 60.0f;
                int i = (int)sector;
                float f = sector - (float)i;
                float p = vf * (1.0f - sf);
                float q = vf * (1.0f - sf * f);
                float t = vf * (1.0f - sf * (1.0f - f));
                float rf, gf, bf;
                switch (i % 6)
                {
                case 0: rf = vf; gf = t;  bf = p;  break;
                case 1: rf = q;  gf = vf; bf = p;  break;
                case 2: rf = p;  gf = vf; bf = t;  break;
                case 3: rf = p;  gf = q;  bf = vf; break;
                case 4: rf = t;  gf = p;  bf = vf; break;
                default: rf = vf; gf = p;  bf = q;  break;
                }
                r = (int)(rf * 255.0f);
                g = (int)(gf * 255.0f);
                b = (int)(bf * 255.0f);
            }

            static void rgbToHsv(int r, int g, int b, int& h, int& s, int& v)
            {
                float rf = (float)r / 255.0f;
                float gf = (float)g / 255.0f;
                float bf = (float)b / 255.0f;
                float cmax = rf > gf ? (rf > bf ? rf : bf) : (gf > bf ? gf : bf);
                float cmin = rf < gf ? (rf < bf ? rf : bf) : (gf < bf ? gf : bf);
                float delta = cmax - cmin;
                v = (int)(cmax * 100.0f);
                if (cmax == 0.0f) { h = 0; s = 0; return; }
                s = (int)(delta / cmax * 100.0f);
                if (delta == 0.0f) { h = 0; return; }
                float hf;
                if (cmax == rf)
                    hf = 60.0f * (gf - bf) / delta;
                else if (cmax == gf)
                    hf = 60.0f * ((bf - rf) / delta + 2.0f);
                else
                    hf = 60.0f * ((rf - gf) / delta + 4.0f);
                if (hf < 0.0f) hf += 360.0f;
                h = (int)hf;
            }

        public:
            CommandDedsecHue* hue = nullptr;
            CommandDedsecSat* sat = nullptr;
            CommandDedsecVal* val = nullptr;
            CommandDedsecRed* red = nullptr;
            CommandDedsecGreen* green = nullptr;
            CommandDedsecBlue* blue = nullptr;

            void syncRgbFromHsv()
            {
                if (!hue || !sat || !val || !red || !green || !blue) return;
                int r, g, b;
                hsvToRgb(hue->value, sat->value, val->value, r, g, b);
                red->setValueIndicator(Click(CLICK_AUTO, TC_OTHER), r);
                green->setValueIndicator(Click(CLICK_AUTO, TC_OTHER), g);
                blue->setValueIndicator(Click(CLICK_AUTO, TC_OTHER), b);
                AllEntitiesEveryTick::dedsec_r = r;
                AllEntitiesEveryTick::dedsec_g = g;
                AllEntitiesEveryTick::dedsec_b = b;
            }

            void syncHsvFromRgb()
            {
                if (!hue || !sat || !val || !red || !green || !blue) return;
                int h, s, v;
                rgbToHsv(red->value, green->value, blue->value, h, s, v);
                hue->setValueIndicator(Click(CLICK_AUTO, TC_OTHER), h);
                sat->setValueIndicator(Click(CLICK_AUTO, TC_OTHER), s);
                val->setValueIndicator(Click(CLICK_AUTO, TC_OTHER), v);
                AllEntitiesEveryTick::dedsec_h = h;
                AllEntitiesEveryTick::dedsec_s = s;
                AllEntitiesEveryTick::dedsec_v = v;
                AllEntitiesEveryTick::dedsec_r = red->value;
                AllEntitiesEveryTick::dedsec_g = green->value;
                AllEntitiesEveryTick::dedsec_b = blue->value;
            }

            explicit CommandDedsecColour(CommandList* parent)
                : CommandList(parent, LIT("Colour"), CMDNAMES("dedsechex"), NOLABEL)
            {
                createChild<CommandDedsecRainbow>();
                hue = createChild<CommandDedsecHue>(this);
                sat = createChild<CommandDedsecSat>(this);
                val = createChild<CommandDedsecVal>(this);
                createChild<CommandDedsecOpacity>();
                red = createChild<CommandDedsecRed>(this);
                green = createChild<CommandDedsecGreen>(this);
                blue = createChild<CommandDedsecBlue>(this);
            }
        };

        class CommandDedsecPhoneDelay : public CommandSlider
        {
        public:
            explicit CommandDedsecPhoneDelay(CommandList* parent)
                : CommandSlider(parent, LIT("Phone Open Deadline"), CMDNAMES("dedsecdelay"), LIT("Allows you to open your phone despite having a hacking target if you release the key within the given amount of milliseconds."), 0, 60000, 200, 10)
            {
            }
            void onChange(Click&, int) override { AllEntitiesEveryTick::phone_input_delay = value; }
        };

        class CommandDedsecHackToggle : public CommandToggle
        {
            bool* target;
        public:
            CommandDedsecHackToggle(CommandList* parent, Label&& name, bool* target, bool def = true)
                : CommandToggle(parent, std::move(name), {}, NOLABEL, def), target(target)
            {
            }
            void onEnable(Click&) override { *target = true; }
            void onDisable(Click&) override { *target = false; }
        };

    public:
        explicit CommandListDedsec(CommandList* parent)
            : CommandList(parent, LIT("Watch Dogs-Like World Hacking"), CMDNAMES("watchdogshacking"), NOLABEL)
        {
            createChild<CommandDedsec>();
            createChild<CommandDedsecSticky>();

            {
                auto passive = createChild<CommandList>(LIT("When No Target Selected..."), CMDNAMES(), NOLABEL);
                passive->createChild<CommandDedsecBoolToggle>(LIT("Show Reticle"), CMDNAMES("dedsecpassivereticle"), &AllEntitiesEveryTick::dedsec_passive_reticle, true);
                passive->createChild<CommandDedsecBoolToggle>(LIT("Show Line"), CMDNAMES("dedsecpassiveline"), &AllEntitiesEveryTick::dedsec_passive_line, true);
                passive->createChild<CommandDedsecBoolToggle>(LIT("Show Box"), CMDNAMES("dedsecpassivebox"), &AllEntitiesEveryTick::dedsec_passive_box, false);
            }

            {
                auto active = createChild<CommandList>(LIT("When Target Selected..."), CMDNAMES(), NOLABEL);
                active->createChild<CommandDedsecBoolToggle>(LIT("Show Reticle"), CMDNAMES("dedsecactivereticle"), &AllEntitiesEveryTick::dedsec_active_reticle, true);
                active->createChild<CommandDedsecBoolToggle>(LIT("Show Line"), CMDNAMES("dedsecactiveline"), &AllEntitiesEveryTick::dedsec_active_line, true);
                active->createChild<CommandDedsecBoolToggle>(LIT("Show Box"), CMDNAMES("dedsecactivebox"), &AllEntitiesEveryTick::dedsec_active_box, true);
            }

            createChild<CommandDedsecColour>();
            createChild<CommandDedsecPhoneDelay>();

            {
                auto hacks = createChild<CommandList>(LIT("Hacks"), CMDNAMES(), LIT("Allows you to configure which hacks appear on your screen."));

                {
                    auto general = hacks->createChild<CommandList>(LIT("General Actions"), CMDNAMES(), NOLABEL);
                    general->createChild<CommandDedsecHackToggle>(LIT("Freeze"), &DedsecHack::freeze.enabled, true);
                    general->createChild<CommandDedsecHackToggle>(LIT("Unfreeze"), &DedsecHack::unfreeze.enabled, true);
                    general->createChild<CommandDedsecHackToggle>(LIT("Explode"), &DedsecHack::explode.enabled, true);
                    general->createChild<CommandDedsecHackToggle>(LIT("Disarm"), &DedsecHack::disarm.enabled, true);
                    general->createChild<CommandDedsecHackToggle>(LIT("Delete"), &DedsecHack::del.enabled, true);
                }

                {
                    auto vehicle = hacks->createChild<CommandList>(LIT("Vehicle Actions"), CMDNAMES(), NOLABEL);
                    vehicle->createChild<CommandDedsecHackToggle>(LIT("Destroy"), &DedsecHack::destroy.enabled, true);
                    vehicle->createChild<CommandDedsecHackToggle>(LIT("Drive"), &DedsecHack::drive.enabled, true);
                    vehicle->createChild<CommandDedsecHackToggle>(LIT("Enter"), &DedsecHack::enter.enabled, true);
                    vehicle->createChild<CommandDedsecHackToggle>(LIT("Empty"), &DedsecHack::empty.enabled, true);
                    vehicle->createChild<CommandDedsecHackToggle>(LIT("Ignite"), &DedsecHack::ignite.enabled, false);
                    vehicle->createChild<CommandDedsecHackToggle>(LIT("Slingshot"), &DedsecHack::slingshot.enabled, true);
                    vehicle->createChild<CommandDedsecHackToggle>(LIT("In Stand"), &DedsecHack::menu_player_veh.enabled, true);
                }

                {
                    auto npc_ped = hacks->createChild<CommandList>(LIT("NPC Actions"), CMDNAMES(), NOLABEL);
                    npc_ped->createChild<CommandDedsecHackToggle>(LIT("Burn"), &DedsecHack::burn.enabled, true);
                    npc_ped->createChild<CommandDedsecHackToggle>(LIT("Flee"), &DedsecHack::flee.enabled, true);
                    npc_ped->createChild<CommandDedsecHackToggle>(LIT("Cower"), &DedsecHack::cower.enabled, true);
                    npc_ped->createChild<CommandDedsecHackToggle>(LIT("Revive"), &DedsecHack::revive.enabled, true);
                }

                {
                    auto player = hacks->createChild<CommandList>(LIT("Player Actions"), CMDNAMES(), NOLABEL);
                    player->createChild<CommandDedsecHackToggle>(LIT("Kill"), &DedsecHack::kill.enabled, false);
                    player->createChild<CommandDedsecHackToggle>(LIT("Cage"), &DedsecHack::cage.enabled, false);
                    player->createChild<CommandDedsecHackToggle>(LIT("In Stand"), &DedsecHack::menu_player.enabled, true);
                }
            }
        }
    };

    inline void CommandListDedsec::CommandDedsecColour::CommandDedsecHue::onChange(Click&, int)
    {
        AllEntitiesEveryTick::dedsec_h = value;
        colour->syncRgbFromHsv();
    }

    inline void CommandListDedsec::CommandDedsecColour::CommandDedsecSat::onChange(Click&, int)
    {
        AllEntitiesEveryTick::dedsec_s = value;
        colour->syncRgbFromHsv();
    }

    inline void CommandListDedsec::CommandDedsecColour::CommandDedsecVal::onChange(Click&, int)
    {
        AllEntitiesEveryTick::dedsec_v = value;
        colour->syncRgbFromHsv();
    }

    inline void CommandListDedsec::CommandDedsecColour::CommandDedsecRed::onChange(Click&, int)
    {
        AllEntitiesEveryTick::dedsec_r = value;
        colour->syncHsvFromRgb();
    }

    inline void CommandListDedsec::CommandDedsecColour::CommandDedsecGreen::onChange(Click&, int)
    {
        AllEntitiesEveryTick::dedsec_g = value;
        colour->syncHsvFromRgb();
    }

    inline void CommandListDedsec::CommandDedsecColour::CommandDedsecBlue::onChange(Click&, int)
    {
        AllEntitiesEveryTick::dedsec_b = value;
        colour->syncHsvFromRgb();
    }
}
