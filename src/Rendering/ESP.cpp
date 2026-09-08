#include "Rendering/ESP.hpp"
#include "World/Object.hpp"
#include "Commands/CommandToggleLegacy.hpp"
#include "Commands/CommandColourCustom.hpp"
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
	// Human
	constexpr int headBone = 31086;
	constexpr int neckBone = 39317;
	constexpr int torsoBone = 23553;
	constexpr int leftHandBone = 18905;
	constexpr int rightHandBone = 57005;
	constexpr int leftFootBone = 14201;
	constexpr int rightFootBone = 52301;
	constexpr int leftElbowBone = 22711;
	constexpr int rightElbowBone = 2992;
	constexpr int leftKneeBone = 46078;
	constexpr int rightKneeBone = 16335;
	constexpr int leftShoulderBone = 61163; // TODO verify all the bones
	constexpr int rightShoulderBone = 28252;
}

namespace Stand::Features
{
	// Players
	CommandToggleLegacy _ESPDrawPlayers("espdrawplayers", "Draw Players", "Should the ESP draw players?");
	CommandToggleLegacy _ESPDrawDeadPlayers("espdrawdeadplayers", "Draw Dead Players", "Should the ESP draw dead players?");

	CommandToggleLegacy _ESPName("espnameplayers", "Show Player Name", "Should the ESP draw player names?");
	CommandToggleLegacy _ESPDistance("espdistanceplayers", "Show Player Distance", "Should the ESP draw player distance?");
	CommandToggleLegacy _ESPSkeleton("espskeletonplayers", "Show Player Skeleton", "Should the ESP draw player skeletons?");

	CommandColourCustom _NameColorPlayers("namecolorplayers", "Player Name Color", "Changes the color of the name ESP for players", ImVec4{1.0f, 1.0f, 1.0f, 1.0f});
	CommandColourCustom _DistanceColorPlayers("distancecolorplayers", "Player Distance Color", "Changes the color of the distance ESP for players", ImVec4{1.0f, 1.0f, 1.0f, 1.0f});
	CommandColourCustom _SkeletonColorPlayers("skeletoncolorplayers", "Player Skeleton Color", "Changes the color of the skeleton ESP for players", ImVec4{1.0f, 1.0f, 1.0f, 1.0f});

	// Peds
	CommandToggleLegacy _ESPDrawPeds("espdrawpeds", "Draw Peds", "Should the ESP draw peds?");
	CommandToggleLegacy _ESPDrawDeadPeds("espdrawdeadpeds", "Draw Dead Peds", "Should the ESP draw dead peds?");

	CommandToggleLegacy _ESPModelPeds("espmodelspeds", "Show Ped Model", "Should the ESP draw ped models?");
	CommandToggleLegacy _ESPNetworkInfoPeds("espnetinfopeds", "Show Ped Network Info", "Should the ESP draw network info?");
	CommandToggleLegacy _ESPScriptInfoPeds("espscriptinfopeds", "Show Ped Script Info", "Should the ESP draw script info?");
	CommandToggleLegacy _ESPDistancePeds("espdistancepeds", "Show Ped Distance", "Should the ESP draw distance?");
	CommandToggleLegacy _ESPSkeletonPeds("espskeletonpeds", "Show Ped Skeleton", "Should the ESP draw the skeleton?");

	CommandColourCustom _HashColorPeds("hashcolorpeds", "Ped Hash Color", "Changes the color of the hash ESP for peds", ImVec4{1.0f, 1.0f, 1.0f, 1.0f});
	CommandColourCustom _SkeletonColorPeds("skeletoncolorpeds", "Ped Skeleton Color", "Changes the color of the skeleton ESP for peds", ImVec4{1.0f, 1.0f, 1.0f, 1.0f});

	// Objects
	CommandToggleLegacy _ESPDrawObjects("espdrawobjects", "Draw Special Objects", "Should the ESP draw special objects?");
	CommandToggleLegacy _ESPNetworkInfoObjects("espnetinfoobjects", "Show Object Network Info", "Should the ESP draw network info?");
	CommandToggleLegacy _ESPScriptInfoObjects("espscriptinfoobjects", "Show Object Script Info", "Should the ESP draw script info?");
	CommandToggleLegacy _ESPDistanceObjects("espdistanceobjects", "Show Object Distance", "Should the ESP draw distance?");

	CommandColourCustom _HashColorObjects("hashcolorobjects", "Object Hash Color", "Changes the color of the hash ESP for objects", ImVec4{1.0f, 1.0f, 1.0f, 1.0f});
}

namespace Stand
{
	namespace
	{
		// Palette - plain DirectX::XMFLOAT4 now rather than ImVec4/
		// ImColor (these are pure local constants, so there's no
		// reason to keep them as an ImGui type at all here).
		constexpr DirectX::XMFLOAT4 death_bg{0.117f, 0.113f, 0.172f, .75f};
		constexpr DirectX::XMFLOAT4 armor_blue_bg{0.36f, 0.71f, 0.89f, .75f};
		constexpr DirectX::XMFLOAT4 armor_blue{0.36f, 0.71f, 0.89f, 1.f};
		constexpr DirectX::XMFLOAT4 health_green_bg{0.29f, 0.69f, 0.34f, .75f};
		constexpr DirectX::XMFLOAT4 health_green{0.29f, 0.69f, 0.34f, 1.f};
		constexpr DirectX::XMFLOAT4 health_yellow_bg{0.69f, 0.49f, 0.29f, .75f};
		constexpr DirectX::XMFLOAT4 health_yellow{0.69f, 0.49f, 0.29f, 1.f};
		constexpr DirectX::XMFLOAT4 health_red_bg{0.69f, 0.29f, 0.29f, .75f};
		constexpr DirectX::XMFLOAT4 health_red{0.69f, 0.29f, 0.29f, 1.f};
		constexpr DirectX::XMFLOAT4 Green{0.29f, 0.69f, 0.34f, 1.f};
		constexpr DirectX::XMFLOAT4 Orange{0.69f, 0.49f, 0.29f, 1.f};
		constexpr DirectX::XMFLOAT4 Red{0.69f, 0.29f, 0.29f, 1.f};
		constexpr DirectX::XMFLOAT4 Blue{0.36f, 0.71f, 0.89f, 1.f};
		constexpr DirectX::XMFLOAT4 White{1.0f, 1.0f, 1.0f, 1.0f};

		// Same "1.2x the normal size" the original got via a temporary
		// ImGui::GetFont()->Scale bump - GridRenderer::DrawTextScreen's
		// own scale is a plain multiplier on the embedded font's native
		// size (no automatic resolution scaling - see its own doc
		// comment), so this is that multiplier directly.
		constexpr float kEspTextScale = 1.2f;
		constexpr float kSkeletonThickness = 1.5f;

		DirectX::XMFLOAT4 ToXMFLOAT4(const ImVec4& c)
		{
			return {c.x, c.y, c.z, c.w};
		}

		DirectX::XMFLOAT2 worldToScreen(rage::fvector3 coords)
		{
			float screen_x{}, screen_y{};

			GRAPHICS::GET_SCREEN_COORD_FROM_WORLD_COORD(coords.x, coords.y, coords.z, &screen_x, &screen_y);

			return {screen_x * (*Pointers.ScreenResX), screen_y * (*Pointers.ScreenResY)};
		}

		// One frame's worth of ESP content, collected during Draw()'s own
		// pool-iteration pass and drawn from there (lines) and from
		// DrawText() (text) - see ESP.hpp's own Draw()/DrawText() comment
		// for why text collection and text drawing are split across the
		// two passes this renderer needs (PrimitiveBatch vs SpriteBatch),
		// same reasoning as Overlay's own OverlayState.
		struct TextItem
		{
			DirectX::XMFLOAT2 pos;
			DirectX::XMFLOAT4 colour;
			std::string text;
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

			line(headBone, neckBone);

			line(neckBone, leftShoulderBone);
			line(leftShoulderBone, leftElbowBone);
			line(leftElbowBone, leftHandBone);

			line(neckBone, rightShoulderBone);
			line(rightShoulderBone, rightElbowBone);
			line(rightElbowBone, rightHandBone);

			line(neckBone, torsoBone);

			line(torsoBone, leftKneeBone);
			line(leftKneeBone, leftFootBone);

			line(torsoBone, rightKneeBone);
			line(rightKneeBone, rightFootBone);
		}

		//TODO : Very bare bones currently, expand and possibly refactor
		void DrawPlayer(Player plyr)
		{
			if (!plyr.IsValid() || !plyr.GetPed().IsValid() || plyr == Self::GetPlayer()
			    || worldToScreen(plyr.GetPed().GetBonePosition(torsoBone)).x == 0
			    || (plyr.GetPed().IsDead() && !Features::_ESPDrawDeadPlayers.GetState()))
				return;

			float distanceToPlayer = Self::GetPed().GetPosition().GetDistance(plyr.GetPed().GetBonePosition(torsoBone));
			DirectX::XMFLOAT4 colorBasedOnDistance = Red;

			if (distanceToPlayer < 100.f)
				colorBasedOnDistance = Green;
			else if (distanceToPlayer > 100.f && distanceToPlayer < 300.f)
				colorBasedOnDistance = Orange;
			else if (distanceToPlayer > 300.f)
				colorBasedOnDistance = Red;

			if (Features::_ESPName.GetState())
			{
				PushText(worldToScreen(plyr.GetPed().GetBonePosition(headBone)),
				    plyr == Players::GetSelected() ? Blue : ToXMFLOAT4(Features::_NameColorPlayers.GetState()),
				    plyr.GetName());
			}

			if (Features::_ESPDistance.GetState())
			{
				std::string distanceStr = std::to_string((int)Self::GetPed().GetPosition().GetDistance(plyr.GetPed().GetBonePosition(torsoBone))) + "m";
				const auto pos = worldToScreen(plyr.GetPed().GetBonePosition(headBone));
				PushText({pos.x, pos.y + 20}, colorBasedOnDistance, distanceStr);
			}

			//TODO Boxes, Distance colors, Friendlies, Tracers, Health bars

			if (Features::_ESPSkeleton.GetState() /* && !plyr.GetPed().IsAnimal() */) // yes, this is neccesary.
			{
				if (distanceToPlayer < 250.f)
				{
					DrawSkeleton(plyr.GetPed(), ToXMFLOAT4(Features::_SkeletonColorPlayers.GetState()));
				}
			}
		}

		void DrawPed(Ped ped)
		{
			if (!ped.IsValid() || ped.IsPlayer() || ped == Self::GetPlayer().GetPed() || worldToScreen(ped.GetBonePosition(torsoBone)).x == 0 || (ped.IsDead() && !Features::_ESPDrawDeadPeds.GetState()))
				return;

			float distanceToPed = 0.0f;

			if (auto local = Self::GetPed())
				distanceToPed = local.GetPosition().GetDistance(ped.GetBonePosition(torsoBone));

			DirectX::XMFLOAT4 colorBasedOnDistance = Red;

			if (distanceToPed < 100.f)
				colorBasedOnDistance = Green;
			else if (distanceToPed > 100.f && distanceToPed < 300.f)
				colorBasedOnDistance = Orange;
			else if (distanceToPed > 300.f)
				colorBasedOnDistance = Red;

			std::string info = "";

			if (Features::_ESPModelPeds.GetState())
			{
				//if (auto it = Data::g_PedModels.find(ped.GetModel()); it != Data::g_PedModels.end())
				//	info += std::format("{} ", it->second);
				//else
				info += std::format("0x{:08X} ", (joaat_t)ped.GetModel());
			}

			if (Features::_ESPNetworkInfoPeds.GetState() && ped.IsNetworked())
			{
				auto owner = Player(ped.GetOwner());
				auto id = ped.GetNetworkObjectId();

				info += std::format("{} {} ", id, owner.GetName());
			}

			if (Features::_ESPScriptInfoPeds.GetState())
			{
				if (auto script = ENTITY::GET_ENTITY_SCRIPT(ped.GetHandle(), nullptr))
				{
					info += std::format("{} ", script);
				}
			}

			if (!info.empty())
				PushText(worldToScreen(ped.GetBonePosition(headBone)), ToXMFLOAT4(Features::_HashColorPeds.GetState()), info);

			if (Features::_ESPDistancePeds.GetState())
			{
				std::string distanceStr = std::to_string((int)distanceToPed) + "m";
				const auto pos = worldToScreen(ped.GetBonePosition(headBone));
				PushText({pos.x, pos.y + 20}, colorBasedOnDistance, distanceStr);
			}

			//TODO Boxes, Distance colors, Tracers, Health bars

			if (Features::_ESPSkeletonPeds.GetState() /* && !ped.IsAnimal() */)
			{
				if (distanceToPed < 250.f)
				{
					DrawSkeleton(ped, ToXMFLOAT4(Features::_SkeletonColorPeds.GetState()));
				}
			}
		}

		void DrawObject(Object object)
		{
			if (!object.IsValid())
				return;

			bool is_camera = object.IsCamera();
			bool is_signal_jammer = object.IsSignalJammer();
			bool is_mission_object = object.IsMissionEntity();

			if (!is_camera && !is_signal_jammer && !is_mission_object)
				return;

			float distanceToObject = 0.0f;

			if (auto local = Self::GetPed())
				distanceToObject = local.GetPosition().GetDistance(object.GetPosition());

			DirectX::XMFLOAT4 colorBasedOnDistance = Red;

			if (distanceToObject < 100.f)
				colorBasedOnDistance = Green;
			else if (distanceToObject > 100.f && distanceToObject < 300.f)
				colorBasedOnDistance = Orange;
			else if (distanceToObject > 300.f)
				colorBasedOnDistance = Red;

			Vector3 coords = object.GetPosition();
			DirectX::XMFLOAT4 colour = ToXMFLOAT4(Features::_HashColorObjects.GetState());
			std::string info = std::format("0x{:08X} ", (joaat_t)object.GetModel());

			if (Features::_ESPNetworkInfoObjects.GetState() && object.IsNetworked())
			{
				auto owner = Player(object.GetOwner());
				auto id = object.GetNetworkObjectId();

				info += std::format("{} {} ", id, owner.GetName());
			}

			if (Features::_ESPScriptInfoObjects.GetState())
			{
				if (auto script = ENTITY::GET_ENTITY_SCRIPT(object.GetHandle(), nullptr))
				{
					info += std::format("{} ", script);
				}
			}

			if (is_camera)
			{
				colour = Red;
				info += " (Camera)";
			}
			else if (is_signal_jammer)
			{
				colour = Red;
				info += " (Jammer)";
			}
			else if (is_mission_object)
			{
				info += " (Mission)";
			}

			PushText(worldToScreen(coords), colour, info);

			if (Features::_ESPDistanceObjects.GetState())
			{
				std::string distanceStr = std::to_string((int)distanceToObject) + "m";
				const auto pos = worldToScreen(object.GetPosition());
				PushText({pos.x, pos.y + 20}, colorBasedOnDistance, distanceStr);
			}
		}
	}

	void ESP::Draw()
	{
		g_TextItems.clear();

		if (!NativeInvoker::AreHandlersCached() || CAMERA::IS_SCREEN_FADED_OUT() || HUD::IS_WARNING_MESSAGE_ACTIVE() || HUD::IS_PAUSE_MENU_ACTIVE() || NETWORK::NETWORK_IS_IN_MP_CUTSCENE())
			return;

		// lots of race conditions and other things make this particularly crash-prone
		__try
		{
			if (Features::_ESPDrawPlayers.GetState())
			{
				for (auto& [id, player] : Players::GetPlayers())
				{
					DrawPlayer(player);
				}
			}
			if (Features::_ESPDrawPeds.GetState() && GetPedPool())
			{
				for (Ped ped : Pools::GetPeds())
				{
					if (ped && ped.GetPointer<void*>())
						DrawPed(ped);
				}
			}
			if (Features::_ESPDrawObjects.GetState() && GetObjectPool())
			{
				for (auto obj : Pools::GetObjects())
				{
					if (obj)
						DrawObject(obj.As<Object>());
				}
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
		}

		// Skeleton lines were already drawn directly (via
		// GridRenderer::DrawLineScreen) from inside DrawSkeleton() above -
		// only text is deferred to DrawText(), same split as everywhere
		// else in this system (PrimitiveBatch vs SpriteBatch passes).
	}

	void ESP::DrawText()
	{
		using Rendering::GridRenderer;

		for (auto& item : g_TextItems)
			GridRenderer::DrawTextScreen(item.pos.x, item.pos.y, item.text.c_str(), item.colour, kEspTextScale);
	}
}
