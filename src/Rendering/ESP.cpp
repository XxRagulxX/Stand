#include "Rendering/ESP.hpp"
#include "Commands/Settings/Game/CommandTabESP.hpp"
#include "Commands/Self/Weapons/CommandTabWeapons.hpp"
#include "Commands/CommandLegacy.hpp"
#include "World/Object.hpp"
#include "Network/Players.hpp"
#include "World/Self.hpp"
#include "Core/Pointers.hpp"
#include "Game/AllEntitiesEveryTick.hpp"
#include "Game/Pools.hpp"
#include "Rendering/GridRenderer.hpp"
#include "Scripting/Invoker.hpp"
#include "Scripting/Natives.hpp"

#include <format>
#include <string>
#include <vector>

namespace
{
	constexpr int headBone        = 31086;
	constexpr int neckBone        = 39317;
	constexpr int torsoBone       = 23553;
	constexpr int leftHandBone    = 18905;
	constexpr int rightHandBone   = 57005;
	constexpr int leftFootBone    = 14201;
	constexpr int rightFootBone   = 52301;
	constexpr int leftElbowBone   = 22711;
	constexpr int rightElbowBone  = 2992;
	constexpr int leftKneeBone    = 46078;
	constexpr int rightKneeBone   = 16335;
	constexpr int leftShoulderBone  = 61163;
	constexpr int rightShoulderBone = 28252;
}

namespace Stand::Features
{
	namespace
	{
		class CommandESPColour : public CommandLegacy
		{
			ImVec4 m_State;
		public:
			CommandESPColour(const char* name, const char* label, const char* desc, ImVec4 def)
				: CommandLegacy(name, label, desc, 0), m_State(def) {}
			ImVec4 GetState() const { return m_State; }
			void SetState(ImVec4 s) { m_State = s; MarkDirty(); }
			void OnCall() override {}
			void SaveState(nlohmann::json& v) override { v = {m_State.x, m_State.y, m_State.z, m_State.w}; }
			void LoadState(nlohmann::json& v) override
			{
				if (v.is_array()) { auto a = v.get<std::array<float, 4>>(); m_State = {a[0], a[1], a[2], a[3]}; }
			}
		};
	}

	CommandESPColour _NameColorPlayers("namecolorplayers",       "Player Name Color",     "Color of the player name label",     ImVec4{1.0f, 1.0f, 1.0f, 1.0f});
	CommandESPColour _DistanceColorPlayers("distancecolorplayers", "Player Distance Color", "Color of the player distance label", ImVec4{1.0f, 1.0f, 1.0f, 1.0f});
	CommandESPColour _SkeletonColorPlayers("skeletoncolorplayers", "Player Skeleton Color", "Color of the player skeleton lines",  ImVec4{1.0f, 1.0f, 1.0f, 1.0f});

	CommandESPColour _HashColorPeds("hashcolorpeds",         "Ped Hash Color",    "Color of the ped hash label",    ImVec4{1.0f, 1.0f, 1.0f, 1.0f});
	CommandESPColour _SkeletonColorPeds("skeletoncolorpeds", "Ped Skeleton Color", "Color of the ped skeleton lines", ImVec4{1.0f, 1.0f, 1.0f, 1.0f});

	CommandESPColour _HashColorObjects("hashcolorobjects", "Object Hash Color", "Color of the object hash label", ImVec4{1.0f, 1.0f, 1.0f, 1.0f});
}

namespace Stand
{
	namespace
	{
		constexpr float kEspTextScale      = 1.2f;
		constexpr float kSkeletonThickness = 1.5f;

		DirectX::XMFLOAT4 ToXMFLOAT4(const ImVec4& c)
		{
			return {c.x, c.y, c.z, c.w};
		}

		constexpr DirectX::XMFLOAT4 Green {0.29f, 0.69f, 0.34f, 1.f};
		constexpr DirectX::XMFLOAT4 Orange{0.69f, 0.49f, 0.29f, 1.f};
		constexpr DirectX::XMFLOAT4 Red   {0.69f, 0.29f, 0.29f, 1.f};
		constexpr DirectX::XMFLOAT4 Blue  {0.36f, 0.71f, 0.89f, 1.f};

		DirectX::XMFLOAT2 worldToScreen(rage::fvector3 coords)
		{
			float sx{}, sy{};
			GRAPHICS::GET_SCREEN_COORD_FROM_WORLD_COORD(coords.x, coords.y, coords.z, &sx, &sy);
			return {sx * (*Pointers.ScreenResX), sy * (*Pointers.ScreenResY)};
		}

		DirectX::XMFLOAT4 distanceColour(float d)
		{
			if (d < 100.f)  return Green;
			if (d < 300.f)  return Orange;
			return Red;
		}

		struct TextItem
		{
			DirectX::XMFLOAT2 pos;
			DirectX::XMFLOAT4 colour;
			std::string       text;
			float             scale = kEspTextScale;
		};
		std::vector<TextItem> g_TextItems;

		void PushText(DirectX::XMFLOAT2 pos, const DirectX::XMFLOAT4& colour, std::string text, float scale = kEspTextScale)
		{
			g_TextItems.push_back({pos, colour, std::move(text), scale});
		}

		void DrawSkeleton(Ped ped, const DirectX::XMFLOAT4& colour)
		{
			if (!ped.IsValid())
				return;

			using Rendering::GridRenderer;

			auto line = [&](int a, int b) {
				const auto p0 = worldToScreen(ped.GetBonePosition(a));
				const auto p1 = worldToScreen(ped.GetBonePosition(b));
				GridRenderer::DrawLineScreen(p0.x, p0.y, p1.x, p1.y, colour, kSkeletonThickness);
			};

			line(headBone,         neckBone);
			line(neckBone,         leftShoulderBone);
			line(leftShoulderBone, leftElbowBone);
			line(leftElbowBone,    leftHandBone);
			line(neckBone,         rightShoulderBone);
			line(rightShoulderBone, rightElbowBone);
			line(rightElbowBone,   rightHandBone);
			line(neckBone,         torsoBone);
			line(torsoBone,        leftKneeBone);
			line(leftKneeBone,     leftFootBone);
			line(torsoBone,        rightKneeBone);
			line(rightKneeBone,    rightFootBone);
		}

		void DrawPlayerBox(Ped ped, const DirectX::XMFLOAT4& colour)
		{
			DirectX::XMFLOAT2 topPx = worldToScreen(ped.GetBonePosition(headBone));
			DirectX::XMFLOAT2 botPx = worldToScreen(ped.GetBonePosition(rightFootBone));
			if (topPx.x <= 0.f || botPx.x <= 0.f || botPx.y <= topPx.y) return;
			const float h = botPx.y - topPx.y;
			const float w = h * 0.4f;
			const float cx = (topPx.x + botPx.x) * 0.5f;
			constexpr float kThick = 1.5f;
			using Rendering::GridRenderer;
			GridRenderer::DrawLineScreen(cx - w, topPx.y, cx + w, topPx.y, colour, kThick);
			GridRenderer::DrawLineScreen(cx - w, botPx.y, cx + w, botPx.y, colour, kThick);
			GridRenderer::DrawLineScreen(cx - w, topPx.y, cx - w, botPx.y, colour, kThick);
			GridRenderer::DrawLineScreen(cx + w, topPx.y, cx + w, botPx.y, colour, kThick);
		}

		void DrawPlayerLine(Ped ped, const DirectX::XMFLOAT4& colour)
		{
			DirectX::XMFLOAT2 pedPx = worldToScreen(ped.GetBonePosition(torsoBone));
			if (pedPx.x <= 0.f) return;
			using Rendering::GridRenderer;
			GridRenderer::DrawLineScreen(
				*Pointers.ScreenResX * 0.5f, static_cast<float>(*Pointers.ScreenResY),
				pedPx.x, pedPx.y, colour, 1.5f);
		}

		void DrawPlayer(const CommandTabESP& esp, Player plyr)
		{
			if (!plyr.IsValid() || !plyr.GetPed().IsValid() || plyr == Self::GetPlayer())
				return;
			if (worldToScreen(plyr.GetPed().GetBonePosition(torsoBone)).x == 0)
				return;
			if (plyr.GetPed().IsDead() && !esp.drawDeadPlayers->m_on)
				return;

			const float dist = Self::GetPed().GetPosition().GetDistance(
			    plyr.GetPed().GetBonePosition(torsoBone));

			if (esp.namePlayers->m_on)
			{
				PushText(worldToScreen(plyr.GetPed().GetBonePosition(headBone)),
				    plyr == Players::GetSelected()
				        ? Blue
				        : ToXMFLOAT4(Features::_NameColorPlayers.GetState()),
				    plyr.GetName());
			}

			if (esp.distancePlayers->m_on)
			{
				const auto pos = worldToScreen(plyr.GetPed().GetBonePosition(headBone));
				PushText({pos.x, pos.y + 20},
				    ToXMFLOAT4(Features::_DistanceColorPlayers.GetState()),
				    std::to_string(static_cast<int>(dist)) + "m");
			}

			if (esp.skeletonPlayers->m_on && dist < 250.f)
				DrawSkeleton(plyr.GetPed(), ToXMFLOAT4(Features::_SkeletonColorPlayers.GetState()));
		}

		void DrawPed(const CommandTabESP& esp, Ped ped)
		{
			if (!ped.IsValid() || ped.IsPlayer() || ped == Self::GetPlayer().GetPed())
				return;
			if (worldToScreen(ped.GetBonePosition(torsoBone)).x == 0)
				return;
			if (ped.IsDead() && !esp.drawDeadPeds->m_on)
				return;

			float dist = 0.f;
			if (auto local = Self::GetPed())
				dist = local.GetPosition().GetDistance(ped.GetBonePosition(torsoBone));

			std::string info;

			if (esp.modelPeds->m_on)
				info += std::format("0x{:08X} ", static_cast<joaat_t>(ped.GetModel()));

			if (esp.netInfoPeds->m_on && ped.IsNetworked())
			{
				auto owner = Player(ped.GetOwner());
				info += std::format("{} {} ", ped.GetNetworkObjectId(), owner.GetName());
			}

			if (esp.scriptInfoPeds->m_on)
			{
				if (auto script = ENTITY::GET_ENTITY_SCRIPT(ped.GetHandle(), nullptr))
					info += std::format("{} ", script);
			}

			if (!info.empty())
				PushText(worldToScreen(ped.GetBonePosition(headBone)),
				    ToXMFLOAT4(Features::_HashColorPeds.GetState()), info);

			if (esp.distancePeds->m_on)
			{
				const auto pos = worldToScreen(ped.GetBonePosition(headBone));
				PushText({pos.x, pos.y + 20},
				    distanceColour(dist),
				    std::to_string(static_cast<int>(dist)) + "m");
			}

			if (esp.skeletonPeds->m_on && dist < 250.f)
				DrawSkeleton(ped, ToXMFLOAT4(Features::_SkeletonColorPeds.GetState()));
		}

		void DrawObject(const CommandTabESP& esp, Object object)
		{
			if (!object.IsValid())
				return;

			const bool isCamera       = object.IsCamera();
			const bool isJammer       = object.IsSignalJammer();
			const bool isMission      = object.IsMissionEntity();
			if (!isCamera && !isJammer && !isMission)
				return;

			float dist = 0.f;
			if (auto local = Self::GetPed())
				dist = local.GetPosition().GetDistance(object.GetPosition());

			DirectX::XMFLOAT4 colour = ToXMFLOAT4(Features::_HashColorObjects.GetState());
			std::string info = std::format("0x{:08X} ", static_cast<joaat_t>(object.GetModel()));

			if (esp.netInfoObjects->m_on && object.IsNetworked())
			{
				auto owner = Player(object.GetOwner());
				info += std::format("{} {} ", object.GetNetworkObjectId(), owner.GetName());
			}

			if (esp.scriptInfoObjects->m_on)
			{
				if (auto script = ENTITY::GET_ENTITY_SCRIPT(object.GetHandle(), nullptr))
					info += std::format("{} ", script);
			}

			if (isCamera)       { colour = Red; info += "(Camera)"; }
			else if (isJammer)  { colour = Red; info += "(Jammer)"; }
			else if (isMission) {               info += "(Mission)"; }

			PushText(worldToScreen(object.GetPosition()), colour, info);

			if (esp.distanceObjects->m_on)
			{
				const auto pos = worldToScreen(object.GetPosition());
				PushText({pos.x, pos.y + 20},
				    distanceColour(dist),
				    std::to_string(static_cast<int>(dist)) + "m");
			}
		}
	}

	static void ESP_DrawImpl(CommandTabESP& esp);

	void ESP::Draw()
	{
		g_TextItems.clear();

		if (!NativeInvoker::AreHandlersCached()
		    || CAMERA::IS_SCREEN_FADED_OUT()
		    || HUD::IS_WARNING_MESSAGE_ACTIVE()
		    || HUD::IS_PAUSE_MENU_ACTIVE()
		    || NETWORK::NETWORK_IS_IN_MP_CUTSCENE())
			return;

		auto& esp = Features::GetCommandTabESP();

		__try
		{
			ESP_DrawImpl(esp);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
		}
	}

	static void ESP_DrawImpl(CommandTabESP& esp)
	{
			if (esp.drawPlayers->m_on)
			{
				for (auto& [id, player] : Players::GetPlayers())
					DrawPlayer(esp, player);
			}

			if (esp.drawPeds->m_on && GetPedPool())
			{
				for (Ped ped : Pools::GetPeds())
				{
					if (ped && ped.GetPointer<void*>())
						DrawPed(esp, ped);
				}
			}

			if (esp.drawObjects->m_on && GetObjectPool())
			{
				for (auto obj : Pools::GetObjects())
				{
					if (obj)
						DrawObject(esp, obj.As<Object>());
				}
			}

			if (AllEntitiesEveryTick::npc_bone_esp && GetPedPool())
			{
				const DirectX::XMFLOAT4 npcEspColour{
				    AllEntitiesEveryTick::npc_esp_colour_r / 255.f,
				    AllEntitiesEveryTick::npc_esp_colour_g / 255.f,
				    AllEntitiesEveryTick::npc_esp_colour_b / 255.f,
				    0.78431f
				};
				for (Ped ped : Pools::GetPeds())
				{
					if (!ped || ped.IsPlayer()) continue;
					if (AllEntitiesEveryTick::npc_bone_esp_exclude_dead && ped.IsDead()) continue;
					if (worldToScreen(ped.GetBonePosition(torsoBone)).x == 0) continue;
					DrawSkeleton(ped, npcEspColour);
				}
			}

			if (AllEntitiesEveryTick::player_esp_bone || AllEntitiesEveryTick::player_esp_name
			    || AllEntitiesEveryTick::player_esp_box || AllEntitiesEveryTick::player_esp_line)
			{
				using AEET = AllEntitiesEveryTick;
				const DirectX::XMFLOAT4 defaultEspColour{
				    AEET::player_esp_colour_r / 255.f,
				    AEET::player_esp_colour_g / 255.f,
				    AEET::player_esp_colour_b / 255.f,
				    0.78431f
				};
				auto tagColour = [](const AEET::PlayerEspTagEntry& t) {
					return DirectX::XMFLOAT4{t.r / 255.f, t.g / 255.f, t.b / 255.f, 0.78431f};
				};
				for (auto& [id, player] : Players::GetPlayers())
				{
					if (!player.IsValid() || player == Self::GetPlayer()) continue;
					Ped ped = player.GetPed();
					if (!ped.IsValid()) continue;
					const int h = ped.GetHandle();
					if (worldToScreen(ped.GetBonePosition(torsoBone)).x == 0.f) continue;
					const float dist = Self::GetPed().GetPosition().GetDistance(ped.GetBonePosition(torsoBone));
					const int pid = player.GetId();

					DirectX::XMFLOAT4 espColour = defaultEspColour;
					if      (AEET::player_esp_tag_dead.use        && PED::IS_PED_DEAD_OR_DYING(h, TRUE))                  espColour = tagColour(AEET::player_esp_tag_dead);
					else if (AEET::player_esp_tag_invulnerable.use && PLAYER::GET_PLAYER_INVINCIBLE(pid))                 espColour = tagColour(AEET::player_esp_tag_invulnerable);
					else if (AEET::player_esp_tag_modder.use       && player.IsModder())                                   espColour = tagColour(AEET::player_esp_tag_modder);
					else if (AEET::player_esp_tag_invisible.use    && !ENTITY::IS_ENTITY_VISIBLE(h))                      espColour = tagColour(AEET::player_esp_tag_invisible);

					if (AEET::player_esp_bone)
						DrawSkeleton(ped, espColour);
					if (AEET::player_esp_name && dist <= static_cast<float>(AEET::player_esp_name_max_dist))
					{
						const float min_s  = AEET::player_esp_name_min_scale / 100.f;
						const float max_s  = AEET::player_esp_name_max_scale / 100.f;
						const float max_d  = static_cast<float>(AEET::player_esp_name_max_dist);
						const float mul    = max_s - min_s;
						float       ts     = max_d > 0.f ? (mul / max_d) * dist : 0.f;
						if (AEET::player_esp_name_invert_scale)
							ts = (mul + 1.f) / (ts + 1.f) - 1.f;
						PushText(worldToScreen(ped.GetBonePosition(headBone)), espColour, player.GetName(), ts + min_s);
					}
					if (AEET::player_esp_box && dist <= static_cast<float>(AEET::player_esp_box_max_dist))
						DrawPlayerBox(ped, espColour);
					if (AEET::player_esp_line && dist <= static_cast<float>(AEET::player_esp_line_max_dist))
						DrawPlayerLine(ped, espColour);
				}
			}

			{
				auto& weapons = Features::GetCommandTabWeapons();
				auto* aimbotToggle = weapons.aimbot->toggle;
				if (aimbotToggle->m_on && aimbotToggle->box && aimbotToggle->box->m_on)
				{
					const int target = aimbotToggle->m_currentTarget.load(std::memory_order_acquire);
					if (target != 0)
					{
						DirectX::XMFLOAT2 topPx{}, botPx{};
						if (aimbotToggle->m_targetIsPed.load(std::memory_order_relaxed))
						{
							topPx = worldToScreen(PED::GET_PED_BONE_COORDS(target, headBone,       0.f, 0.f, 0.f));
							botPx = worldToScreen(PED::GET_PED_BONE_COORDS(target, rightFootBone, 0.f, 0.f, 0.f));
						}
						else
						{
							const Vector3 c = ENTITY::GET_ENTITY_COORDS(target, true);
							topPx = worldToScreen({c.x, c.y, c.z + 1.5f});
							botPx = worldToScreen({c.x, c.y, c.z - 1.0f});
						}
						if (topPx.x > 0.f && botPx.x > 0.f && botPx.y > topPx.y)
						{
							const float h  = botPx.y - topPx.y;
							const float w  = h * 0.4f;
							const float cx = (topPx.x + botPx.x) * 0.5f;
							constexpr DirectX::XMFLOAT4 kPink{1.f, 0.f, 0.5f, 0.86f};
							constexpr float kThick = 1.5f;
							using Rendering::GridRenderer;
							GridRenderer::DrawLineScreen(cx - w, topPx.y, cx + w, topPx.y, kPink, kThick);
							GridRenderer::DrawLineScreen(cx - w, botPx.y, cx + w, botPx.y, kPink, kThick);
							GridRenderer::DrawLineScreen(cx - w, topPx.y, cx - w, botPx.y, kPink, kThick);
							GridRenderer::DrawLineScreen(cx + w, topPx.y, cx + w, botPx.y, kPink, kThick);
						}
					}
				}
			}
	}

	void ESP::DrawText()
	{
		using Rendering::GridRenderer;
		for (auto& item : g_TextItems)
			GridRenderer::DrawTextScreen(item.pos.x, item.pos.y, item.text.c_str(), item.colour, item.scale);
	}
}
