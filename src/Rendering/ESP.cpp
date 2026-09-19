#include "Rendering/ESP.hpp"
#include "Commands/ESP/CommandTabESP.hpp"
#include "Commands/Self/Weapons/CommandTabWeapons.hpp"
#include "Commands/CommandColourCustom.hpp"
#include "World/Object.hpp"
#include "Network/Players.hpp"
#include "World/Self.hpp"
#include "Core/Pointers.hpp"
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
	CommandColourCustom _NameColorPlayers("namecolorplayers",     "Player Name Color",     "Color of the player name label",     ImVec4{1.0f, 1.0f, 1.0f, 1.0f});
	CommandColourCustom _DistanceColorPlayers("distancecolorplayers", "Player Distance Color", "Color of the player distance label", ImVec4{1.0f, 1.0f, 1.0f, 1.0f});
	CommandColourCustom _SkeletonColorPlayers("skeletoncolorplayers", "Player Skeleton Color", "Color of the player skeleton lines",  ImVec4{1.0f, 1.0f, 1.0f, 1.0f});

	CommandColourCustom _HashColorPeds("hashcolorpeds",       "Ped Hash Color",    "Color of the ped hash label",    ImVec4{1.0f, 1.0f, 1.0f, 1.0f});
	CommandColourCustom _SkeletonColorPeds("skeletoncolorpeds", "Ped Skeleton Color", "Color of the ped skeleton lines", ImVec4{1.0f, 1.0f, 1.0f, 1.0f});

	CommandColourCustom _HashColorObjects("hashcolorobjects", "Object Hash Color", "Color of the object hash label", ImVec4{1.0f, 1.0f, 1.0f, 1.0f});
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
		};
		std::vector<TextItem> g_TextItems;

		void PushText(DirectX::XMFLOAT2 pos, const DirectX::XMFLOAT4& colour, std::string text)
		{
			g_TextItems.push_back({pos, colour, std::move(text)});
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
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
		}
	}

	void ESP::DrawText()
	{
		using Rendering::GridRenderer;
		for (auto& item : g_TextItems)
			GridRenderer::DrawTextScreen(item.pos.x, item.pos.y, item.text.c_str(), item.colour, kEspTextScale);
	}
}
