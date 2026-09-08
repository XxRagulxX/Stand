#pragma once

#include "Commands/CommandToggleLegacy.hpp"
#include "Commands/CommandLegacy.hpp"
#include "Commands/CommandSliderFloatLegacy.hpp"
#include "Commands/CommandSliderLegacy.hpp"
#include "Commands/CommandListSelect.hpp"
#include "Commands/LoopedCommand.hpp"

namespace Stand
{
	class LuaUserInterface;

	class LuaCommand : public CommandLegacy
	{
		LuaUserInterface* m_Interface;
		int m_OnCall;

	public:
		LuaCommand(LuaUserInterface* iface, std::string name, std::string label, std::string description, int on_call);
		~LuaCommand();

		bool ShouldSaveState() const override { return false; }

	protected:
		void OnCall() override;
	};

	class LuaBoolCommand : public CommandToggleLegacy
	{
		LuaUserInterface* m_Interface;
		int m_OnEnable;
		int m_OnDisable;

	public:
		LuaBoolCommand(LuaUserInterface* iface, std::string name, std::string label, std::string description, bool default_value, int on_enable, int on_disable);
		~LuaBoolCommand();

		bool ShouldSaveState() const override { return false; }

	protected:
		void OnEnable() override;
		void OnDisable() override;
	};

	class LuaLoopedCommand : public LoopedCommand
	{
		LuaUserInterface* m_Interface;
		int m_OnTick;
		int m_OnEnable;
		int m_OnDisable;
		bool m_TickRegistered = false;

	public:
		LuaLoopedCommand(LuaUserInterface* iface, std::string name, std::string label, std::string description, int on_tick, int on_enable, int on_disable);
		~LuaLoopedCommand();

		bool ShouldSaveState() const override { return false; }

	protected:
		void OnTick() override {}
		void OnEnable() override;
		void OnDisable() override;
	};

	class LuaIntCommand : public CommandSliderLegacy
	{
		LuaUserInterface* m_Interface;
		int m_OnChange;

	public:
		LuaIntCommand(LuaUserInterface* iface, std::string name, std::string label, std::string description, std::optional<int> min, std::optional<int> max, int default_value, int on_change);
		~LuaIntCommand();

		bool ShouldSaveState() const override { return false; }

	protected:
		void OnChange() override;
	};

	class LuaFloatCommand : public CommandSliderFloatLegacy
	{
		LuaUserInterface* m_Interface;
		int m_OnChange;

	public:
		LuaFloatCommand(LuaUserInterface* iface, std::string name, std::string label, std::string description, std::optional<float> min, std::optional<float> max, float default_value, int on_change);
		~LuaFloatCommand();

		bool ShouldSaveState() const override { return false; }

	protected:
		void OnChange() override;
	};

	class LuaListCommand : public CommandListSelect
	{
		LuaUserInterface* m_Interface;
		int m_OnChange;

		std::vector<std::string> m_LabelStorage;

	public:
		LuaListCommand(LuaUserInterface* iface, std::string name, std::string label, std::string description, std::vector<std::pair<int, std::string>> entries, int default_value, int on_change);
		~LuaListCommand();

		bool ShouldSaveState() const override { return false; }

	protected:
		void OnChange() override;
	};
}
