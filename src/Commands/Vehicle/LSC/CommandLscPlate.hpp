#pragma once
#include "Commands/Vehicle/LSC/CommandVehicleColour.hpp"
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Menu/Click.hpp"
#include "Rendering/MenuCommandBox.hpp"
#include "Scripting/Natives.hpp"
#include "Util/get_current_time_millis.hpp"

#include <cstdio>
#include <cstdlib>
#include <string>

namespace Stand
{
    namespace
    {
        class CommandPlateType : public CommandSlider
        {
            static const char* PlateTypeName(int v) {
                switch (v) {
                case 0:  return "Blue-on-White 1";
                case 1:  return "Blue-on-White 2";
                case 2:  return "Blue-on-White 3";
                case 3:  return "Yellow-on-Black";
                case 4:  return "Yellow-on-Blue";
                case 5:  return "North Yankton";
                default: return "";
                }
            }
        public:
            explicit CommandPlateType(CommandList* parent)
                : CommandSlider(parent, LIT("Plate Type"), CMDNAMES("platetype"), NOLABEL, 0, 5, 0)
            {
                CommandTickDispatch::AddCommand(this);
            }

            ~CommandPlateType() override { CommandTickDispatch::RemoveCommand(this); }

            std::string getValueText() const override { return PlateTypeName(value); }

            void onTick() override {
                int veh = LscGetVehicle();
                if (!veh) return;
                Click c(CLICK_AUTO, TC_SCRIPT_NOYIELD);
                setValueIndicator(c, VEHICLE::GET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(veh));
            }

            void onChange(Click& click, int) override {
                if (click.isAuto()) return;
                int v = value;
                click.ensureScriptThread([v] {
                    int veh = LscGetVehicle(); if (!veh) return;
                    VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(veh, v);
                });
            }
        };

        class CommandPlateRandomise : public CommandPhysical
        {
        public:
            explicit CommandPlateRandomise(CommandList* parent)
                : CommandPhysical(COMMAND_ACTION, parent, LIT("Randomise Plate"), CMDNAMES("platerandomise"), NOLABEL) {}

            void onClick(Click& click) override {
                click.ensureScriptThread([] {
                    int veh = LscGetVehicle(); if (!veh) return;
                    static const char kChars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
                    char plate[9] = {};
                    for (int i = 0; i < 8; ++i)
                        plate[i] = kChars[rand() % (sizeof(kChars) - 1)];
                    VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, plate);
                });
            }
        };

        class CommandListScrollingText : public CommandList
        {
        public:
            std::string m_scrollText;
            int m_intervalMs = 250;

            explicit CommandListScrollingText(CommandList* parent)
                : CommandList(parent, LIT("Scrolling Text"), CMDNAMES("platescroll", "scrollplatetext"))
            {
                struct CommandScrollingToggle : public CommandToggle {
                    int m_scrollPos = 0;
                    time_t m_lastTick = 0;
                    explicit CommandScrollingToggle(CommandList* p)
                        : CommandToggle(p, LIT("Scrolling Text"), CMDNAMES_0()) {}

                    void onChange(Click& click) override {
                        if (!m_on) return;
                        m_scrollPos = 0;
                        m_lastTick = 0;
                        onChangeToggleScriptTickEventHandler(click, [this] {
                            if (!m_on) return false;
                            int veh = LscGetVehicle(); if (!veh) return true;
                            auto* list = static_cast<CommandListScrollingText*>(this->parent);
                            std::string text = list->m_scrollText;
                            if (text.empty()) {
                                const char* p = VEHICLE::GET_VEHICLE_NUMBER_PLATE_TEXT(veh);
                                text = (p && *p) ? p : "PLATE";
                            }
                            auto now = get_current_time_millis();
                            if (now - m_lastTick < (time_t)list->m_intervalMs) return true;
                            m_lastTick = now;
                            std::string padded = text + "        ";
                            int pos = m_scrollPos % (int)padded.size();
                            VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, padded.substr(pos, 8).c_str());
                            ++m_scrollPos;
                            return true;
                        });
                    }
                };
                createChild<CommandScrollingToggle>();

                struct CommandScrollingTextSetter : public CommandPhysical {
                    explicit CommandScrollingTextSetter(CommandList* p)
                        : CommandPhysical(COMMAND_ACTION, p, LIT("Text"), CMDNAMES_0(), NOLABEL, CMDFLAG_TEXT_INPUT) {}

                    void onClick(Click& click) override {
                        auto* list = static_cast<CommandListScrollingText*>(this->parent);
                        Rendering::MenuCommandBox::Open(
                            "platescrolltext",
                            "Scrolling Text",
                            "Text to scroll across the plate",
                            list->m_scrollText,
                            [list, this](const std::string& text) -> bool {
                                list->m_scrollText = text;
                                setMenuName(text.empty() ? LIT("Text") : Label("Text: " + text, Label::TagLiteral{}));
                                return true;
                            }
                        );
                    }
                };
                createChild<CommandScrollingTextSetter>();

                struct CommandScrollingInterval : public CommandSlider {
                    explicit CommandScrollingInterval(CommandList* p)
                        : CommandSlider(p, LIT("Interval (ms)"), CMDNAMES_0(), NOLABEL, 50, 2000, 250) {}

                    void onChange(Click& click, int) override {
                        if (click.isAuto()) return;
                        static_cast<CommandListScrollingText*>(this->parent)->m_intervalMs = value;
                    }
                };
                createChild<CommandScrollingInterval>();
            }
        };

        class CommandPlateSpeed : public CommandToggle
        {
            std::string m_savedPlate;
            time_t      m_lastTick = 0;
        public:
            explicit CommandPlateSpeed(CommandList* parent)
                : CommandToggle(parent, LIT("Speedometer"), CMDNAMES("platespeedometer", "platespeed")) {}

            void onChange(Click& click) override {
                if (!m_on) {
                    if (!m_savedPlate.empty()) {
                        std::string plate = m_savedPlate;
                        click.ensureScriptThread([plate] {
                            int veh = LscGetVehicle(); if (!veh) return;
                            VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, plate.c_str());
                        });
                        m_savedPlate.clear();
                    }
                    return;
                }
                m_lastTick = 0;
                onChangeToggleScriptTickEventHandler(click, [this] {
                    if (!m_on) return false;
                    int veh = LscGetVehicle();
                    if (!veh) return true;
                    if (m_savedPlate.empty()) {
                        const char* p = VEHICLE::GET_VEHICLE_NUMBER_PLATE_TEXT(veh);
                        m_savedPlate = (p && *p) ? p : "";
                    }
                    auto now = get_current_time_millis();
                    if (now - m_lastTick < 500) return true;
                    m_lastTick = now;
                    float spd = ENTITY::GET_ENTITY_SPEED(veh) * 3.6f;
                    char buf[16];
                    std::snprintf(buf, sizeof(buf), "%.0f KMH", spd);
                    VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, buf);
                    return true;
                });
            }
        };

        class CommandPlateTps : public CommandToggle
        {
            std::string m_savedPlate;
            time_t      m_lastTick  = 0;
            int         m_frames    = 0;
            time_t      m_fpsTimer  = 0;
            int         m_fps       = 0;
        public:
            explicit CommandPlateTps(CommandList* parent)
                : CommandToggle(parent, LIT("Ticks Per Second"), CMDNAMES("platetps")) {}

            void onChange(Click& click) override {
                if (!m_on) {
                    if (!m_savedPlate.empty()) {
                        std::string plate = m_savedPlate;
                        click.ensureScriptThread([plate] {
                            int veh = LscGetVehicle(); if (!veh) return;
                            VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, plate.c_str());
                        });
                        m_savedPlate.clear();
                    }
                    return;
                }
                m_lastTick = m_fpsTimer = get_current_time_millis();
                m_frames   = 0;
                m_fps      = 0;
                onChangeToggleScriptTickEventHandler(click, [this] {
                    if (!m_on) return false;
                    int veh = LscGetVehicle();
                    if (!veh) return true;
                    if (m_savedPlate.empty()) {
                        const char* p = VEHICLE::GET_VEHICLE_NUMBER_PLATE_TEXT(veh);
                        m_savedPlate = (p && *p) ? p : "";
                    }
                    auto now = get_current_time_millis();
                    ++m_frames;
                    if (now - m_fpsTimer >= 1000) {
                        m_fps     = m_frames;
                        m_frames  = 0;
                        m_fpsTimer = now;
                    }
                    if (now - m_lastTick < 500) return true;
                    m_lastTick = now;
                    char buf[16];
                    std::snprintf(buf, sizeof(buf), "%d FPS", m_fps);
                    VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, buf);
                    return true;
                });
            }
        };

        class CommandLockPlate : public CommandToggle
        {
            std::string m_lockedPlate;
        public:
            explicit CommandLockPlate(CommandList* parent)
                : CommandToggle(parent, LIT("Lock License Plate Text"), CMDNAMES("lockplate")) {}

            void onChange(Click& click) override {
                if (!m_on) { m_lockedPlate.clear(); return; }
                onChangeToggleScriptTickEventHandler(click, [this] {
                    if (!m_on) return false;
                    int veh = LscGetVehicle();
                    if (!veh) return true;
                    if (m_lockedPlate.empty()) {
                        const char* p = VEHICLE::GET_VEHICLE_NUMBER_PLATE_TEXT(veh);
                        m_lockedPlate = (p && *p) ? p : "LOCKED";
                    }
                    VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, m_lockedPlate.c_str());
                    return true;
                });
            }
        };
    }
}
