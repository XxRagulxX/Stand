#include "Util/Joaat.hpp"
#include "Commands/CommandLegacy.hpp"
#include "Commands/Commands.hpp"

namespace Stand
{
	CommandLegacy::CommandLegacy(std::string name, std::string label, std::string description, int num_args) :
	    m_Name(name),
	    m_Label(label),
	    m_Description(description),
	    m_NumArgs(num_args),
	    m_Hash(Joaat(name))
	{
		Commands::AddCommand(this);
	}

	void CommandLegacy::Call()
	{
		OnCall();
	}

	void CommandLegacy::MarkDirty()
	{
		Commands::MarkDirty();
	}
}