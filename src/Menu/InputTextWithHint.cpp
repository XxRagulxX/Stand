#include "Menu/Items.hpp"

namespace Stand
{
	InputTextWithHint::InputTextWithHint(std::string label, std::string hint, std::string* buf, std::function<void()> cb) :
	    m_Id(std::move(label)),
	    m_Hint(std::move(hint)),
	    m_Buf(buf),
	    m_Callback(std::move(cb))
	{
	}

	// No-op - see BoolCommandItem.cpp's own comment on why.
	void InputTextWithHint::Draw()
	{
	}
}
