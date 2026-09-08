#include "Menu/Items.hpp"

namespace Stand
{
	Button::Button(const std::string_view& name, std::function<void()> onClick, const ImVec2 size, const std::string_view& information) :
	    m_Name(name),
	    m_Information(information),
	    m_OnClick(onClick),
	    m_Size(size)
	{
	}

	// No-op - see BoolCommandItem.cpp's own comment on why.
	void Button::Draw()
	{
	}
}
