#include "Commands/Vehicle/CommandLastVehicle.hpp"

#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Game/ControllerInputs.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/get_current_time_millis.hpp"
#include "Util/Label.hpp"
#include "Util/Math.hpp"

namespace Stand
{
	namespace
	{
		struct GhostDriverState
		{
			int driver = 0;

			bool initVehicle(int& veh)
			{
				auto playerPed = PLAYER::GET_PLAYER_PED(-1);
				veh = PED::GET_VEHICLE_PED_IS_IN(playerPed, true);
				if (!veh || !ENTITY::DOES_ENTITY_EXIST(veh))
					return false;
				return VEHICLE::GET_PED_IN_VEHICLE_SEAT(veh, -1, false) != playerPed;
			}

			void initDriver(int veh, int& outDriver)
			{
				if (driver && ENTITY::DOES_ENTITY_EXIST(driver))
				{
					auto vehPos = ENTITY::GET_ENTITY_COORDS(veh, true);
					ENTITY::SET_ENTITY_COORDS(driver, vehPos.x, vehPos.y, vehPos.z, false, false, false, false);
					outDriver = driver;
					return;
				}
				auto playerPed = PLAYER::GET_PLAYER_PED(-1);
				auto model = ENTITY::GET_ENTITY_MODEL(playerPed);
				auto vehPos = ENTITY::GET_ENTITY_COORDS(veh, true);
				driver = PED::CREATE_PED(4, model, vehPos.x, vehPos.y, vehPos.z, 0.0f, false, false);
				if (!driver)
				{
					outDriver = 0;
					return;
				}
				ENTITY::SET_ENTITY_HAS_GRAVITY(driver, false);
				ENTITY::SET_ENTITY_COMPLETELY_DISABLE_COLLISION(driver, false, false);
				PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(driver, true);
				ENTITY::SET_ENTITY_VISIBLE(driver, false, false);
				ENTITY::SET_ENTITY_SHOULD_FREEZE_WAITING_ON_COLLISION(driver, false);
				PED::SET_PED_INTO_VEHICLE(driver, veh, -1);
				outDriver = driver;
			}

			void cleanup(int veh)
			{
				if (driver && ENTITY::DOES_ENTITY_EXIST(driver))
					ENTITY::DELETE_ENTITY(&driver);
				driver = 0;
				if (veh && ENTITY::DOES_ENTITY_EXIST(veh))
					TASK::TASK_VEHICLE_TEMP_ACTION(PLAYER::GET_PLAYER_PED(-1), veh, 27, 60);
			}
		};

		static bool IsCtrlPressed(ControllerInputs input)
		{
			return PAD::IS_DISABLED_CONTROL_PRESSED(0, static_cast<int>(input));
		}

		static bool IsCtrlJustPressed(ControllerInputs input)
		{
			return PAD::IS_DISABLED_CONTROL_JUST_PRESSED(0, static_cast<int>(input));
		}

		static float Vec3Dist(const rage::fvector3& a, const rage::fvector3& b)
		{
			return Math::Length(a - b);
		}

		class CommandCallLastVehicle : public CommandPhysical
		{
		public:
			explicit CommandCallLastVehicle(CommandList* parent)
				: CommandPhysical(COMMAND_ACTION, parent, LIT("Teleport To Me & Drive"), { CMDNAME("calllastvehicle") })
			{
			}

			void onClick(Click& click) override
			{
				ensureYieldableScriptThread(click, [] {
					auto playerPed = PLAYER::GET_PLAYER_PED(-1);
					auto veh = PED::GET_VEHICLE_PED_IS_IN(playerPed, true);
					if (!veh || !ENTITY::DOES_ENTITY_EXIST(veh))
						return;
					if (PED::GET_VEHICLE_PED_IS_IN(playerPed, false) == veh)
						return;
					NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(veh);
					auto pos = ENTITY::GET_ENTITY_COORDS(playerPed, true);
					auto camRot = CAMERA::GET_GAMEPLAY_CAM_ROT(0);
					ENTITY::SET_ENTITY_COORDS_NO_OFFSET(veh, pos.x, pos.y, pos.z, false, false, false);
					ENTITY::SET_ENTITY_ROTATION(veh, 0.0f, 0.0f, camRot.z, 0, true);
					PED::SET_PED_INTO_VEHICLE(playerPed, veh, -1);
				});
			}
		};

		class CommandEnterLastVehicle : public CommandPhysical
		{
		public:
			explicit CommandEnterLastVehicle(CommandList* parent)
				: CommandPhysical(COMMAND_ACTION, parent, LIT("Enter"), { CMDNAME("enterlastvehicle") })
			{
			}

			void onClick(Click& click) override
			{
				ensureYieldableScriptThread(click, [] {
					auto playerPed = PLAYER::GET_PLAYER_PED(-1);
					auto veh = PED::GET_VEHICLE_PED_IS_IN(playerPed, true);
					if (!veh || !ENTITY::DOES_ENTITY_EXIST(veh))
						return;
					if (PED::GET_VEHICLE_PED_IS_IN(playerPed, false) == veh)
						return;
					int maxPassengers = VEHICLE::GET_VEHICLE_MAX_NUMBER_OF_PASSENGERS(veh);
					int seat = -1;
					for (int i = -1; i < maxPassengers; i++)
					{
						if (!VEHICLE::GET_PED_IN_VEHICLE_SEAT(veh, i, false))
						{
							seat = i;
							break;
						}
					}
					PED::SET_PED_INTO_VEHICLE(playerPed, veh, seat);
				});
			}
		};

		class CommandRc : public CommandToggle
		{
			GhostDriverState m_ghost{};
			int m_lastVeh = 0;
			int m_lightsState = 1;
			time_t m_lastTurn = 0;

		public:
			explicit CommandRc(CommandList* parent)
				: CommandToggle(parent, LIT("Remote Control"), { CMDNAME("rc"), CMDNAME("remotecontrol") })
			{
			}

			~CommandRc() override
			{
				CommandTickDispatch::RemoveCommand(this);
			}

			void onEnable(Click& click) override
			{
				CommandTickDispatch::AddCommand(this);
			}

			void onDisable(Click& click) override
			{
				CommandTickDispatch::RemoveCommand(this);
				doCleanup();
			}

			void onTick() override
			{
				int veh = 0;
				if (!m_ghost.initVehicle(veh))
				{
					doCleanup();
					return;
				}

				int driver = 0;
				m_ghost.initDriver(veh, driver);
				if (!driver)
					return;

				m_lastVeh = veh;
				NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(veh);
				VEHICLE::SET_VEHICLE_ENGINE_ON(veh, true, true, false);
				VEHICLE::SET_BIKE_ON_STAND(veh, 0.0f, 0.0f);

				auto pos = ENTITY::GET_ENTITY_COORDS(veh, true);
				CAMERA::SET_IN_VEHICLE_CAM_STATE_THIS_UPDATE(veh, 0);

				if (IsCtrlJustPressed(ControllerInputs::INPUT_VEH_CIN_CAM))
				{
					Vector3 nodeCoords{};
					float roadHeading = 0.0f;
					PATH::GET_CLOSEST_VEHICLE_NODE_WITH_HEADING(pos.x, pos.y, pos.z, &nodeCoords, &roadHeading, 1, 3, 0);
					ENTITY::SET_ENTITY_HEADING(veh, roadHeading);
					ENTITY::SET_ENTITY_COORDS(veh, nodeCoords.x, nodeCoords.y, nodeCoords.z, true, false, false, true);
				}

				if (IsCtrlJustPressed(ControllerInputs::INPUT_VEH_HEADLIGHT))
				{
					if (++m_lightsState > 2)
						m_lightsState = 0;
					VEHICLE::SET_VEHICLE_USE_PLAYER_LIGHT_SETTINGS(veh, false);
					if (m_lightsState == 0)
						VEHICLE::SET_VEHICLE_LIGHTS(veh, 1);
					else if (m_lightsState == 2)
					{
						VEHICLE::SET_VEHICLE_LIGHTS(veh, 2);
						VEHICLE::SET_VEHICLE_FULLBEAM(veh, true);
					}
					else
					{
						VEHICLE::SET_VEHICLE_LIGHTS(veh, 2);
						VEHICLE::SET_VEHICLE_FULLBEAM(veh, false);
					}
				}

				if (IsCtrlPressed(ControllerInputs::INPUT_VEH_HANDBRAKE))
				{
					if (IsCtrlPressed(ControllerInputs::INPUT_VEH_MOVE_LEFT_ONLY))
						TASK::TASK_VEHICLE_TEMP_ACTION(driver, veh, 11, 60);
					else if (IsCtrlPressed(ControllerInputs::INPUT_VEH_MOVE_RIGHT_ONLY))
						TASK::TASK_VEHICLE_TEMP_ACTION(driver, veh, 10, 60);
					else
						TASK::TASK_VEHICLE_TEMP_ACTION(driver, veh, 6, 60);
				}
				else if (IsCtrlPressed(ControllerInputs::INPUT_VEH_ACCELERATE))
				{
					if (IsCtrlPressed(ControllerInputs::INPUT_VEH_BRAKE))
						TASK::TASK_VEHICLE_TEMP_ACTION(driver, veh, 30, 60);
					else if (IsCtrlPressed(ControllerInputs::INPUT_VEH_MOVE_LEFT_ONLY))
					{
						m_lastTurn = get_current_time_millis();
						TASK::TASK_VEHICLE_TEMP_ACTION(driver, veh, 7, 60);
					}
					else if (IsCtrlPressed(ControllerInputs::INPUT_VEH_MOVE_RIGHT_ONLY))
					{
						m_lastTurn = get_current_time_millis();
						TASK::TASK_VEHICLE_TEMP_ACTION(driver, veh, 8, 60);
					}
					else
					{
						if (GET_MILLIS_SINCE(m_lastTurn) > 1000)
							TASK::TASK_VEHICLE_TEMP_ACTION(driver, veh, 23, 60);
						else
							TASK::TASK_VEHICLE_TEMP_ACTION(driver, veh, 9, 60);
					}
				}
				else if (IsCtrlPressed(ControllerInputs::INPUT_VEH_BRAKE))
				{
					if (IsCtrlPressed(ControllerInputs::INPUT_VEH_MOVE_LEFT_ONLY))
						TASK::TASK_VEHICLE_TEMP_ACTION(driver, veh, 13, 60);
					else if (IsCtrlPressed(ControllerInputs::INPUT_VEH_MOVE_RIGHT_ONLY))
						TASK::TASK_VEHICLE_TEMP_ACTION(driver, veh, 14, 60);
					else
						TASK::TASK_VEHICLE_TEMP_ACTION(driver, veh, 28, 60);
				}
				else if (IsCtrlPressed(ControllerInputs::INPUT_VEH_MOVE_LEFT_ONLY))
					TASK::TASK_VEHICLE_TEMP_ACTION(driver, veh, 11, 60);
				else if (IsCtrlPressed(ControllerInputs::INPUT_VEH_MOVE_RIGHT_ONLY))
					TASK::TASK_VEHICLE_TEMP_ACTION(driver, veh, 10, 60);
				else
					TASK::TASK_VEHICLE_TEMP_ACTION(driver, veh, 27, 60);
			}

		private:
			void doCleanup()
			{
				m_ghost.cleanup(m_lastVeh);
				m_lastVeh = 0;
				m_lightsState = 1;
				m_lastTurn = 0;
			}
		};

		class CommandVehFollow : public CommandToggle
		{
			GhostDriverState m_ghost{};
			int m_lastVeh = 0;
			time_t m_forwardSampleStart = 0;
			rage::fvector3 m_forwardSamplePos{};
			bool m_cantAdvanceForward = false;
			time_t m_backwardSampleStart = 0;
			rage::fvector3 m_backwardSamplePos{};
			bool m_cantAdvanceBackwards = false;
			uint8_t m_stopping = 0;

		public:
			explicit CommandVehFollow(CommandList* parent)
				: CommandToggle(parent, LIT("Follow Me"), { CMDNAME("vehfollow") })
			{
			}

			~CommandVehFollow() override
			{
				CommandTickDispatch::RemoveCommand(this);
			}

			void onEnable(Click& click) override
			{
				CommandTickDispatch::AddCommand(this);
			}

			void onDisable(Click& click) override
			{
				CommandTickDispatch::RemoveCommand(this);
				doCleanup();
			}

			void onTick() override
			{
				int veh = 0;
				if (!m_ghost.initVehicle(veh))
				{
					doCleanup();
					return;
				}
				m_lastVeh = veh;

				auto vehPos = ENTITY::GET_ENTITY_COORDS(veh, true);
				float gz = 0.0f;
				MISC::GET_GROUND_Z_FOR_3D_COORD(vehPos.x, vehPos.y, vehPos.z, &gz, false, false);
				if ((vehPos.z - gz) >= 1.0f)
					return;

				int driver = 0;
				m_ghost.initDriver(veh, driver);
				if (!driver)
					return;

				auto playerPed = PLAYER::GET_PLAYER_PED(-1);
				auto playerPos = ENTITY::GET_ENTITY_COORDS(playerPed, true);
				float distToPlayer = Vec3Dist(vehPos, playerPos);

				auto vehModel = ENTITY::GET_ENTITY_MODEL(veh);
				Vector3 minDim{}, maxDim{};
				MISC::GET_MODEL_DIMENSIONS(vehModel, &minDim, &maxDim);
				float vehSize = std::max(std::fabs(maxDim.x - minDim.x), std::fabs(maxDim.y - minDim.y));

				if (m_stopping == 0 && distToPlayer > vehSize)
				{
					if (VEHICLE::IS_THIS_MODEL_A_BIKE(vehModel) && !VEHICLE::IS_VEHICLE_ON_ALL_WHEELS(veh))
						VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(veh, 5.0f);
					VEHICLE::SET_VEHICLE_ENGINE_ON(veh, true, true, false);

					auto vehRot = ENTITY::GET_ENTITY_ROTATION(veh, 0);
					auto fwd = Math::RotationToDirection({0.0f, 0.0f, vehRot.z});
					auto rgt = Math::RotationToDirection({0.0f, 0.0f, vehRot.z + 90.0f});

					rage::fvector3 toPlayer{playerPos.x - vehPos.x, playerPos.y - vehPos.y, 0.0f};
					float toPlayerLen = Math::Length(toPlayer);

					float dotFwd = 0.0f, dotRight = 0.0f;
					if (toPlayerLen > 0.001f)
					{
						float inv = 1.0f / toPlayerLen;
						dotFwd = (toPlayer.x * fwd.x + toPlayer.y * fwd.y) * inv;
						dotRight = (toPlayer.x * rgt.x + toPlayer.y * rgt.y) * inv;
					}

					float d = std::acos(std::max(-1.0f, std::min(1.0f, dotFwd))) * (180.0f / 3.14159265f);
					bool isRight = dotRight > 0.0f;
					bool isSide = std::fabs(dotRight) > std::fabs(dotFwd);
					bool isFront = dotFwd > 0.0f || distToPlayer > 15.0f;

					if ((isFront && !m_cantAdvanceForward) || m_cantAdvanceBackwards)
					{
						if (d > 30.0f)
						{
							if (isRight)
								TASK::TASK_VEHICLE_TEMP_ACTION(driver, veh, 7, 60);
							else
								TASK::TASK_VEHICLE_TEMP_ACTION(driver, veh, 8, 60);
						}
						else
							TASK::TASK_VEHICLE_TEMP_ACTION(driver, veh, 9, 60);

						if (GET_MILLIS_SINCE(m_forwardSampleStart) > 1000)
						{
							if (m_forwardSampleStart != 0)
								m_cantAdvanceForward = Vec3Dist(m_forwardSamplePos, vehPos) < 0.5f;
							m_forwardSampleStart = get_current_time_millis();
							m_forwardSamplePos = vehPos;
						}
						if (GET_MILLIS_SINCE(m_backwardSampleStart) > 2000)
							m_cantAdvanceBackwards = false;
					}
					else
					{
						if (isSide || m_cantAdvanceForward || (d > 150.0f && d < 170.0f))
						{
							if (!isSide && !m_cantAdvanceForward && distToPlayer > 4.0f)
							{
								m_cantAdvanceForward = true;
								m_forwardSampleStart = get_current_time_millis() + 1000;
							}
							if (isRight ^ m_cantAdvanceForward)
								TASK::TASK_VEHICLE_TEMP_ACTION(driver, veh, 13, 60);
							else
								TASK::TASK_VEHICLE_TEMP_ACTION(driver, veh, 14, 60);
						}
						else
							TASK::TASK_VEHICLE_TEMP_ACTION(driver, veh, 28, 60);

						if (GET_MILLIS_SINCE(m_forwardSampleStart) > 2000)
							m_cantAdvanceForward = false;
						if (GET_MILLIS_SINCE(m_backwardSampleStart) > 1000)
						{
							if (m_backwardSampleStart != 0)
								m_cantAdvanceBackwards = Vec3Dist(m_backwardSamplePos, vehPos) < 0.5f;
							m_backwardSampleStart = get_current_time_millis();
							m_backwardSamplePos = vehPos;
						}
					}
				}
				else
				{
					TASK::TASK_VEHICLE_TEMP_ACTION(driver, veh, 27, 60);
					if (distToPlayer > vehSize)
						++m_stopping;
					else if (m_stopping != 0)
						++m_stopping;
				}
			}

		private:
			void doCleanup()
			{
				m_ghost.cleanup(m_lastVeh);
				m_lastVeh = 0;
				m_forwardSampleStart = 0;
				m_backwardSampleStart = 0;
				m_cantAdvanceForward = false;
				m_cantAdvanceBackwards = false;
				m_stopping = 0;
			}
		};
	}

	CommandListLastVehicle::CommandListLastVehicle(CommandList* parent)
		: CommandList(parent, LIT("Last Vehicle"), { CMDNAME("lastvehicle") })
	{
		createChild<CommandCallLastVehicle>();
		createChild<CommandEnterLastVehicle>();
		createChild<CommandRc>();
		createChild<CommandVehFollow>();
	}
}
