#include "Commands/Extra/CommandListInfoOverlay.hpp"

#include <chrono>
#include <ctime>
#include <format>

#include "Commands/Widgets/CommandListSelect.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Core/Pointers.hpp"
#include "Game/Pools.hpp"
#include "Network/GSBD_BlockB.hpp"
#include "Network/Players.hpp"
#include "Rendering/Overlay.hpp"
#include "Rendering/StandPort/CommandDivider.hpp"
#include "Scripting/Natives.hpp"
#include "Scripting/scrThread.hpp"
#include "World/Self.hpp"

namespace Stand
{
	static bool show_in_game_time    = false;
	static bool show_real_world_time = false;
	static bool show_speedometer     = false;
	static bool show_tps             = false;
	static bool show_dps             = false;

	static bool show_players         = false;
	static bool show_host            = false;
	static bool show_script_host     = false;

	static bool show_altitude        = false;
	static bool show_ground_distance = false;
	static bool show_position        = false;
	static bool show_rotation_ent    = false;
	static bool show_rotation_cam    = false;
	static bool show_peds            = false;
	static bool show_vehs            = false;
	static bool show_objs            = false;
	static bool show_threads         = false;

	static uint8_t s_Ref = 0;
	static CommandListInfoOverlay* s_Instance = nullptr;

	class CommandToggleInfo : public CommandToggle
	{
		bool* const m_Ptr;

	public:
		explicit CommandToggleInfo(CommandList* parent, bool* ptr, Label&& menu_name, std::vector<CommandName>&& command_names = {})
			: CommandToggle(parent, std::move(menu_name), std::move(command_names)), m_Ptr(ptr)
		{
		}

		void onEnable(Click& click) override
		{
			*m_Ptr = true;
			if (++s_Ref == 1)
				CommandTickDispatch::AddCommand(s_Instance);
		}

		void onDisable(Click& click) override
		{
			*m_Ptr = false;
			if (--s_Ref == 0)
				CommandTickDispatch::RemoveCommand(s_Instance);
		}

		~CommandToggleInfo() override
		{
			if (*m_Ptr)
			{
				*m_Ptr = false;
				if (--s_Ref == 0)
					CommandTickDispatch::RemoveCommand(s_Instance);
			}
		}
	};

	class CommandOverlayPosition : public CommandListSelect
	{
	public:
		explicit CommandOverlayPosition(CommandList* parent)
			: CommandListSelect(parent, LIT("Position"), CMDNAMES("overlaypos"), NOLABEL,
			    {
			        {0, LIT("Top Left")},
			        {1, LIT("Top Right")},
			        {2, LIT("Bottom Left")},
			        {3, LIT("Bottom Right")},
			        {4, LIT("Free")},
			    },
			    0)
		{
		}

		void onChange(Click& click, long long) override
		{
			Overlay::s_Position = static_cast<OverlayPosition>(value);
		}
	};

	CommandListInfoOverlay::CommandListInfoOverlay(CommandList* const parent)
		: CommandList(parent, LIT("Info Overlay"), CMDNAMES("hudinfo", "infooverlay", "infoverlay"))
	{
		s_Instance = this;

		createChild<CommandOverlayPosition>();

		createChild<CommandDivider>(LIT("Info"));
		createChild<CommandToggleInfo>(&show_in_game_time,    LIT("In-Game Time"),    CMDNAMES("infoclock"));
		createChild<CommandToggleInfo>(&show_real_world_time, LIT("Real World Time"), CMDNAMES("infotime"));
		createChild<CommandToggleInfo>(&show_speedometer,     LIT("Speedometer"),     CMDNAMES("infospeed"));
		createChild<CommandToggleInfo>(&show_tps,             LIT("TPS"),             CMDNAMES("infotps", "infofps"));
		createChild<CommandToggleInfo>(&show_dps,             LIT("DPS"),             CMDNAMES("infodps"));

		createChild<CommandDivider>(LIT("Online"));
		createChild<CommandToggleInfo>(&show_players,         LIT("Players"),         CMDNAMES("infoplayers"));
		createChild<CommandToggleInfo>(&show_host,            LIT("Host"),            CMDNAMES("infohost"));
		createChild<CommandToggleInfo>(&show_script_host,     LIT("Script Host"),     CMDNAMES("infoscripthost"));

		createChild<CommandDivider>(LIT("World"));
		createChild<CommandToggleInfo>(&show_altitude,        LIT("Altitude"),         CMDNAMES("infoaltitude"));
		createChild<CommandToggleInfo>(&show_ground_distance, LIT("Ground Distance"),  CMDNAMES("infogrounddistance"));
		createChild<CommandToggleInfo>(&show_position,        LIT("Position"),         CMDNAMES("infoposition"));
		createChild<CommandToggleInfo>(&show_rotation_ent,    LIT("Entity Rotation"),  CMDNAMES("inforotationentity"));
		createChild<CommandToggleInfo>(&show_rotation_cam,    LIT("Camera Rotation"),  CMDNAMES("inforotationcamera"));
		createChild<CommandToggleInfo>(&show_peds,            LIT("Peds"),             CMDNAMES("infopeds"));
		createChild<CommandToggleInfo>(&show_vehs,            LIT("Vehicles"),         CMDNAMES("infovehicles"));
		createChild<CommandToggleInfo>(&show_objs,            LIT("Objects"),          CMDNAMES("infoobjects"));

		createChild<CommandDivider>(LIT("Game"));
		createChild<CommandToggleInfo>(&show_threads,         LIT("Script Threads"),   CMDNAMES("infoscriptthreads", "infothreads"));
	}

	void CommandListInfoOverlay::onTick()
	{
		{
			static auto s_LastTick = std::chrono::steady_clock::now();
			const auto now         = std::chrono::steady_clock::now();
			const float dt         = std::chrono::duration<float>(now - s_LastTick).count();
			s_LastTick             = now;
			if (dt > 0.f)
			{
				const float instant = 1.f / dt;
				Overlay::s_Tps      = Overlay::s_Tps <= 0.f ? instant : Overlay::s_Tps * 0.9f + instant * 0.1f;
			}
		}

		Overlay::s_Lines.clear();

		if (show_in_game_time)
		{
			Overlay::s_Lines.push_back({std::format("{:02}:{:02}:{:02}",
				CLOCK::GET_CLOCK_HOURS(), CLOCK::GET_CLOCK_MINUTES(), CLOCK::GET_CLOCK_SECONDS())});
		}

		if (show_real_world_time)
		{
			std::time_t t  = std::time(nullptr);
			std::tm* now   = std::localtime(&t);
			Overlay::s_Lines.push_back({std::format("{:02}:{:02}:{:02}", now->tm_hour, now->tm_min, now->tm_sec)});
		}

		if (show_speedometer)
		{
			Ped ped     = Self::GetPed();
			float speed = ped ? ped.GetSpeed() : 0.f;
			Overlay::s_Lines.push_back({std::format("Speed: {:.1f} m/s", speed)});
		}

		if (show_tps)
			Overlay::s_Lines.push_back({std::format("TPS: {:.0f}", Overlay::s_Tps)});

		if (show_dps)
			Overlay::s_Lines.push_back({std::format("DPS: {:.0f}", Overlay::s_Dps)});

		if (show_players)
		{
			const auto count = Players::GetPlayers().size();
			Overlay::s_Lines.push_back({std::format("Players: {}", count)});
		}

		if (show_host)
		{
			const char* name = nullptr;
			for (auto& [id, player] : Players::GetPlayers())
			{
				if (player.IsHost())
				{
					name = player.GetName();
					break;
				}
			}
			Overlay::s_Lines.push_back({std::format("Host: {}", name ? name : "N/A")});
		}

		if (show_script_host)
		{
			if (auto* gsbd = GSBD_BlockB::Get())
			{
				Player sh = gsbd->ScriptHost;
				const char* name = sh ? sh.GetName() : nullptr;
				Overlay::s_Lines.push_back({std::format("Script Host: {}", name ? name : "N/A")});
			}
		}

		if (show_altitude)
		{
			Ped ped = Self::GetPed();
			if (ped)
			{
				auto pos = ped.GetPosition();
				Overlay::s_Lines.push_back({std::format("Altitude: {:.2f}", pos.z)});
			}
		}

		if (show_ground_distance)
		{
			Ped ped = Self::GetPed();
			if (ped)
			{
				auto pos      = ped.GetPosition();
				float groundZ = 0.f;
				MISC::GET_GROUND_Z_FOR_3D_COORD(pos.x, pos.y, pos.z, &groundZ, false, false);
				Overlay::s_Lines.push_back({std::format("Ground Dist: {:.2f}", pos.z - groundZ)});
			}
		}

		if (show_position)
		{
			Ped ped = Self::GetPed();
			if (ped)
			{
				auto pos = ped.GetPosition();
				Overlay::s_Lines.push_back({std::format("X: {:.2f}", pos.x)});
				Overlay::s_Lines.push_back({std::format("Y: {:.2f}", pos.y)});
				Overlay::s_Lines.push_back({std::format("Z: {:.2f}", pos.z)});
			}
		}

		if (show_rotation_ent)
		{
			Ped ped = Self::GetPed();
			if (ped)
			{
				auto rot = ped.GetRotation();
				Overlay::s_Lines.push_back({std::format("Rot X: {:.2f}", rot.x)});
				Overlay::s_Lines.push_back({std::format("Rot Y: {:.2f}", rot.y)});
				Overlay::s_Lines.push_back({std::format("Rot Z: {:.2f}", rot.z)});
			}
		}

		if (show_rotation_cam)
		{
			auto rot = CAMERA::GET_FINAL_RENDERED_CAM_ROT(2);
			Overlay::s_Lines.push_back({std::format("Cam X: {:.2f}", rot.x)});
			Overlay::s_Lines.push_back({std::format("Cam Y: {:.2f}", rot.y)});
			Overlay::s_Lines.push_back({std::format("Cam Z: {:.2f}", rot.z)});
		}

		if (show_peds)
		{
			size_t count = 0;
			for ([[maybe_unused]] auto _ : Pools::GetPeds())
				++count;
			Overlay::s_Lines.push_back({std::format("Peds: {}", count)});
		}

		if (show_vehs)
		{
			size_t count = 0;
			for ([[maybe_unused]] auto _ : Pools::GetVehicles())
				++count;
			Overlay::s_Lines.push_back({std::format("Vehicles: {}", count)});
		}

		if (show_objs)
		{
			size_t count = 0;
			for ([[maybe_unused]] auto _ : Pools::GetObjects())
				++count;
			Overlay::s_Lines.push_back({std::format("Objects: {}", count)});
		}

		if (show_threads)
		{
			uint32_t count = 0;
			if (Pointers.ScriptThreads)
			{
				for (auto* thread : *Pointers.ScriptThreads)
				{
					if (thread && thread->m_Context.m_ThreadId)
						++count;
				}
			}
			Overlay::s_Lines.push_back({std::format("Threads: {}", count)});
		}
	}
}
