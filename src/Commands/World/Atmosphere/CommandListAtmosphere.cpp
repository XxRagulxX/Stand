#include "Commands/World/Atmosphere/CommandListAtmosphere.hpp"

#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandListSelect.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

#include <ctime>

namespace Stand
{
    namespace
    {
        static int  s_clock_speed    = 2000;
        static bool s_lock_time      = false;
        static int  s_locked_hour    = 0;
        static int  s_locked_min     = 0;
        static int  s_locked_sec     = 0;
        static bool s_use_sys_time   = false;
        static bool s_smooth_trans   = true;
        static int  s_last_sync_tick = 0;

        static int  s_weather_idx    = -1;
        static int  s_cloud_idx      = 0;

        static constexpr const char* kWeatherNames[] = {
            "extrasunny",
            "clear",
            "clouds",
            "smog",
            "foggy",
            "overcast",
            "rain",
            "thunder",
            "clearing",
            "neutral",
            "snow",
            "blizzard",
            "snowlight",
            "xmas",
            "halloween",
            "rain_halloween",
            "snow_halloween",
        };

        static constexpr const char* kWeatherLabels[] = {
            "Extra Sunny",
            "Clear",
            "Clouds",
            "Smog",
            "Foggy",
            "Overcast",
            "Rain",
            "Thunder",
            "Clearing",
            "Neutral",
            "Snow",
            "Blizzard",
            "Snowlight",
            "Xmas",
            "Clear Halloween",
            "Rainy Halloween",
            "Snowy Halloween",
        };

        static constexpr const char* kCloudHats[] = {
            nullptr,
            "altostratus",
            "Cirrus",
            "cirrocumulus",
            "Clear 01",
            "Cloudy 01",
            "Contrails",
            "Horizon",
            "horizonband1",
            "horizonband2",
            "horizonband3",
            "horsey",
            "Nimbus",
            "Puffs",
            "RAIN",
            "Snowy 01",
            "Stormy 01",
            "stratoscumulus",
            "Stripey",
            "shower",
            "Wispy",
        };

        class AtmoSectionHeader : public CommandPhysical
        {
        public:
            AtmoSectionHeader(CommandList* parent, const char* label)
                : CommandPhysical(COMMAND_ACTION, parent, Label(label, Label::TagLiteral{}), {}, NOLABEL, CMDFLAG_SECTION_HEADER) {}
        };

        class AtmoTimeSlider : public CommandSlider
        {
        public:
            explicit AtmoTimeSlider(CommandList* parent)
                : CommandSlider(parent, LIT("Time"), CMDNAMES("time"), NOLABEL, 0, 23, 12)
            { CommandTickDispatch::AddCommand(this); }

            ~AtmoTimeSlider() override { CommandTickDispatch::RemoveCommand(this); }

            void onTick() override
            {
                Click ac(CLICK_AUTO, TC_SCRIPT_YIELDABLE);
                setValueIndicator(ac, CLOCK::GET_CLOCK_HOURS());

                const int now = MISC::GET_GAME_TIMER();
                if ((now - s_last_sync_tick) > 5000)
                    NETWORK::NETWORK_OVERRIDE_CLOCK_RATE(4000 - s_clock_speed);

                if (s_use_sys_time) {
                    time_t t = time(nullptr);
                    struct tm* lt = localtime(&t);
                    NETWORK::NETWORK_OVERRIDE_CLOCK_TIME(lt->tm_hour, lt->tm_min, lt->tm_sec);
                } else if (s_lock_time) {
                    NETWORK::NETWORK_OVERRIDE_CLOCK_TIME(s_locked_hour, s_locked_min, s_locked_sec);
                }
            }

            void onChange(Click& click, int) override
            {
                if (click.isAuto()) return;
                s_locked_hour = value;
                s_locked_min  = 0;
                s_locked_sec  = 0;
                click.ensureScriptThread([h = value] {
                    CLOCK::SET_CLOCK_TIME(h, 0, 0);
                    if (s_lock_time)
                        NETWORK::NETWORK_OVERRIDE_CLOCK_TIME(h, 0, 0);
                });
            }
        };

        class AtmoClockSync : public CommandPhysical
        {
        public:
            explicit AtmoClockSync(CommandList* parent)
                : CommandPhysical(COMMAND_ACTION, parent, LIT("Use Session Time"),
                    CMDNAMES("syncclock", "synclock", "clocksync", "synctime", "timesync"), NOLABEL)
            {}

            void onClick(Click& click) override
            {
                click.ensureScriptThread([] {
                    s_last_sync_tick = MISC::GET_GAME_TIMER();
                    NETWORK::NETWORK_CLEAR_CLOCK_TIME_OVERRIDE();
                });
            }
        };

        class AtmoClockLock : public CommandToggle
        {
        public:
            explicit AtmoClockLock(CommandList* parent)
                : CommandToggle(parent, LIT("Lock Time"), CMDNAMES("locktime", "pausetime"))
            {}

            void onChange(Click& click) final
            {
                s_lock_time = m_on;
                if (m_on) {
                    s_locked_hour = CLOCK::GET_CLOCK_HOURS();
                    s_locked_min  = CLOCK::GET_CLOCK_MINUTES();
                    s_locked_sec  = 0;
                    if (s_use_sys_time) {
                        s_use_sys_time = false;
                    }
                }
            }
        };

        class AtmoClockSys : public CommandToggle
        {
        public:
            explicit AtmoClockSys(CommandList* parent)
                : CommandToggle(parent, LIT("Use System Time"), CMDNAMES("systime", "systemtime"))
            {}

            void onChange(Click& click) final
            {
                s_use_sys_time = m_on;
                if (m_on && s_lock_time) {
                    s_lock_time = false;
                }
            }
        };

        class AtmoClockSpeed : public CommandSlider
        {
        public:
            explicit AtmoClockSpeed(CommandList* parent)
                : CommandSlider(parent, LIT("Speed"), CMDNAMES("clockspeed"), NOLABEL, 1, 3999, 2000, 50)
            {}

            void onChange(Click& click, int) override { s_clock_speed = value; }
        };

        class AtmoSmoothTrans : public CommandToggle
        {
        public:
            explicit AtmoSmoothTrans(CommandList* parent)
                : CommandToggle(parent, LIT("Smooth Transition"), CMDNAMES("timesmoothing"), NOLABEL, true)
            {}

            void onChange(Click& click) final { s_smooth_trans = m_on; }
        };

        class AtmoClockPreset : public CommandPhysical
        {
            int m_hour;
        public:
            AtmoClockPreset(CommandList* parent, const char* label, int hour)
                : CommandPhysical(COMMAND_ACTION, parent,
                    Label(label, Label::TagLiteral{}), CMDNAMES_0(), NOLABEL)
                , m_hour(hour)
            {}

            void onClick(Click& click) override
            {
                const int h = m_hour;
                click.ensureScriptThread([h] {
                    s_locked_hour = h;
                    s_locked_min  = 0;
                    s_locked_sec  = 0;
                    CLOCK::SET_CLOCK_TIME(h, 0, 0);
                    if (s_lock_time)
                        NETWORK::NETWORK_OVERRIDE_CLOCK_TIME(h, 0, 0);
                });
            }
        };

        class AtmoWeatherTick : public CommandPhysical
        {
        public:
            explicit AtmoWeatherTick(CommandList* parent)
                : CommandPhysical(COMMAND_ACTION, parent, LIT(""), CMDNAMES_0(), NOLABEL, CMDFLAG_CONCEALED)
            { CommandTickDispatch::AddCommand(this); }

            ~AtmoWeatherTick() override { CommandTickDispatch::RemoveCommand(this); }

            void onTick() override
            {
                if (s_weather_idx >= 0 && s_weather_idx < 17)
                    MISC::SET_OVERRIDE_WEATHER(kWeatherNames[s_weather_idx]);
            }
        };

        class AtmoWeather : public CommandListSelect
        {
            static std::vector<std::pair<long long, Label>> buildOptions()
            {
                std::vector<std::pair<long long, Label>> opts;
                opts.emplace_back(-1LL, LIT("Don't Override"));
                for (int i = 0; i < 17; ++i)
                    opts.emplace_back((long long)i, Label(kWeatherLabels[i], Label::TagLiteral{}));
                return opts;
            }

        public:
            explicit AtmoWeather(CommandList* parent)
                : CommandListSelect(parent, LIT("Override Weather"), CMDNAMES("weather", "myweather"),
                    LIT("Overrides the weather locally."), buildOptions(), -1)
            {}

            void onChange(Click& click, long long prev) override
            {
                s_weather_idx = (int)value;
                if (value == -1)
                    MISC::CLEAR_OVERRIDE_WEATHER();
            }
        };

        class AtmoCloudsTick : public CommandPhysical
        {
        public:
            explicit AtmoCloudsTick(CommandList* parent)
                : CommandPhysical(COMMAND_ACTION, parent, LIT(""), CMDNAMES_0(), NOLABEL, CMDFLAG_CONCEALED)
            { CommandTickDispatch::AddCommand(this); }

            ~AtmoCloudsTick() override { CommandTickDispatch::RemoveCommand(this); }

            void onTick() override
            {
                if (s_cloud_idx > 0 && s_cloud_idx < 21 && kCloudHats[s_cloud_idx])
                    MISC::LOAD_CLOUD_HAT(kCloudHats[s_cloud_idx], 0.f);
            }
        };

        class AtmoClouds : public CommandListSelect
        {
            static std::vector<std::pair<long long, Label>> buildOptions()
            {
                std::vector<std::pair<long long, Label>> opts;
                opts.emplace_back(0LL, LIT("Don't Manipulate"));
                for (int i = 1; i < 21; ++i)
                    opts.emplace_back((long long)i, Label(kCloudHats[i], Label::TagLiteral{}));
                return opts;
            }

        public:
            explicit AtmoClouds(CommandList* parent)
                : CommandListSelect(parent, LIT("Clouds"), CMDNAMES("clouds"),
                    LIT("This will only affect your game."), buildOptions(), 0)
            {}

            void onChange(Click& click, long long prev) override
            {
                const int prev_idx = (int)prev;
                const int new_idx  = (int)value;
                if (prev_idx > 0 && prev_idx < 21 && kCloudHats[prev_idx])
                    MISC::UNLOAD_CLOUD_HAT(kCloudHats[prev_idx], 0.f);
                s_cloud_idx = new_idx;
                if (new_idx == 0)
                    MISC::UNLOAD_ALL_CLOUD_HATS();
                else if (kCloudHats[new_idx]) {
                    MISC::PRELOAD_CLOUD_HAT(kCloudHats[new_idx]);
                    MISC::LOAD_CLOUD_HAT(kCloudHats[new_idx], 0.f);
                }
            }
        };

        class AtmoDisableSkybox : public CommandToggle
        {
        public:
            explicit AtmoDisableSkybox(CommandList* parent)
                : CommandToggle(parent, LIT("Disable Skybox"), CMDNAMES("nosky"),
                    LIT("This will only affect your game."))
            {}

            void onChange(Click& click) final
            {
                if (m_on)
                    GRAPHICS::SET_TIMECYCLE_MODIFIER("no sky");
                else
                    GRAPHICS::CLEAR_TIMECYCLE_MODIFIER();
            }
        };
    }

    CommandListAtmosphere::CommandListAtmosphere(CommandList* parent)
        : CommandList(parent, LIT("Atmosphere"), CMDNAMES("atmosphere"))
    {
        {
            auto* clock = createChild<CommandList>(LIT("Clock"), CMDNAMES("clock"),
                LIT("This will only affect your game."));
            clock->createChild<AtmoTimeSlider>();
            clock->createChild<AtmoClockSync>();
            clock->createChild<AtmoClockLock>();
            clock->createChild<AtmoClockSys>();
            clock->createChild<AtmoClockSpeed>();
            clock->createChild<AtmoSmoothTrans>();
            clock->createChild<AtmoSectionHeader>("Presets");
            clock->createChild<AtmoClockPreset>("Noon",      12);
            clock->createChild<AtmoClockPreset>("Morning",    6);
            clock->createChild<AtmoClockPreset>("Midnight",   0);
            clock->createChild<AtmoClockPreset>("Afternoon", 18);
        }

        createChild<AtmoWeather>();
        createChild<AtmoWeatherTick>();

        createChild<AtmoClouds>();
        createChild<AtmoCloudsTick>();

        createChild<AtmoDisableSkybox>();
    }
}
