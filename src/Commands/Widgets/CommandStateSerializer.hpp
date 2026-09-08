#pragma once
#include "Core/IStateSerializer.hpp"

#include <vector>

// Bridges Stand::CommandPhysical's own getState()/setState()/
// applyDefaultState() triplet (already fully ported and load-bearing -
// e.g. CommandToggleNoCorrelation.cpp/CommandSlider.cpp) onto this
// project's EXISTING config save/load pipeline
// (Config/Settings.hpp's IStateSerializer roster, the same one
// Stand::Commands (Commands/Commands.cpp) already uses for the
// legacy system) - the persistence piece from the Phase 1 plan. Real
// Stand's own state saving is entirely different (a settings.json keyed
// by chat-command name, read/written through its own Command/CommandList
// tree directly, no separate serializer object) - this reuses this
// project's own mechanism instead of building a second one, the same
// choice CommandRegistry.hpp's own comment made for name lookup.
//
// Every CommandPhysical that supportsStateOperations() (CommandFlags.hpp -
// CMDFLAGS_TOGGLE/CMDFLAGS_SLIDER and friends already set this)
// auto-registers here at construction and removes itself at
// destruction, the same shape as CommandRegistry. Persisted under a
// single json key per command - CommandIssuable::command_names.front()
// (a command with no names at all, e.g. a bare CommandList node, simply
// isn't persisted; nothing today constructs a stateful command with no
// names).
namespace Stand
{
	class CommandPhysical;

	class CommandStateSerializer : private Stand::IStateSerializer
	{
	public:
		static void AddCommand(CommandPhysical* command)
		{
			GetInstance().AddCommandImpl(command);
		}

		static void RemoveCommand(CommandPhysical* command)
		{
			GetInstance().RemoveCommandImpl(command);
		}

		static void MarkDirty()
		{
			GetInstance().MarkStateDirty();
		}

	private:
		CommandStateSerializer();

		std::vector<CommandPhysical*> m_Commands;

		void AddCommandImpl(CommandPhysical* command);
		void RemoveCommandImpl(CommandPhysical* command);
		void SaveStateImpl(nlohmann::json& state) override;
		void LoadStateImpl(nlohmann::json& state) override;

		static CommandStateSerializer& GetInstance()
		{
			static CommandStateSerializer instance{};
			return instance;
		}
	};
}
