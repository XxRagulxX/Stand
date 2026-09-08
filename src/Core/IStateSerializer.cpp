#include "Core/IStateSerializer.hpp"

#include "Config/Settings.hpp"

namespace Stand
{
	IStateSerializer::IStateSerializer(const std::string& name) :
	    m_SerComponentName(name),
	    m_IsDirty(false)
	{
		Settings::AddComponent(this);
	}
}