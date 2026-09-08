#pragma once
#include "Util/Joaat.hpp"
#include "Core/IStateSerializer.hpp"

namespace Stand
{
	class CommandLegacy;
	class LoopedCommand;
	class CommandToggleLegacy;

	class Commands :
	    private IStateSerializer
	{
	private:
		std::unordered_map<joaat_t, CommandLegacy*> m_Commands;
		std::vector<LoopedCommand*> m_LoopedCommands;
		std::vector<CommandToggleLegacy*> m_BoolCommands;
		Commands();

	public:
		static void AddCommand(CommandLegacy* command)
		{
			GetInstance().AddCommandImpl(command);
		}

		static void AddBoolCommand(CommandToggleLegacy* command)
		{
			GetInstance().AddBoolCommandImpl(command);
		}

		static void AddLoopedCommand(LoopedCommand* command)
		{
			GetInstance().AddLoopedCommandImpl(command);
		}
		
		static void RemoveCommand(CommandLegacy* command)
		{
			GetInstance().RemoveCommandImpl(command);
		}

		static void RunLoopedCommands()
		{
			GetInstance().RunLoopedCommandsImpl();
		}

		static void EnableBoolCommands()
		{
			GetInstance().EnableBoolCommandsImpl();
		}

		template<typename T = CommandLegacy>
		static T* GetCommand(joaat_t hash)
		{
			return reinterpret_cast<T*>(GetInstance().GetCommandImpl(hash));
		}

		static std::unordered_map<joaat_t, CommandLegacy*>& GetCommands()
		{
			return GetInstance().m_Commands;
		};

		static std::vector<LoopedCommand*> GetLoopedCommands()
		{
			return GetInstance().m_LoopedCommands;
		}

		static void MarkDirty()
		{
			GetInstance().MarkStateDirty();
		}

		static void Shutdown()
		{
			GetInstance().ShutdownImpl();
		}

		static void RunScript();

	private:
		void AddCommandImpl(CommandLegacy* command);
		void AddBoolCommandImpl(CommandToggleLegacy* command);
		void AddLoopedCommandImpl(LoopedCommand* command);
		void RemoveCommandImpl(CommandLegacy* command);
		void EnableBoolCommandsImpl();
		void RunLoopedCommandsImpl();
		CommandLegacy* GetCommandImpl(joaat_t hash);
		virtual void SaveStateImpl(nlohmann::json& state) override;
		virtual void LoadStateImpl(nlohmann::json& state) override;
		void ShutdownImpl();

		static Commands& GetInstance()
		{
			static Commands instance{};
			return instance;
		}
	};
}