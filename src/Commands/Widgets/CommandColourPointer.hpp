#pragma once
#include "Commands/CommandColourCustom.hpp"

#include <utility>

namespace Stand::StandWidgets
{
	// Ported from real Stand's own CommandColourPointer/
	// CommandColourPointerDirectx (Commands/Widgets/CommandColourPointer.hpp,
	// CommandColourPointerDirectx.hpp on origin/stand-reference) - a colour
	// widget backed by an existing ImVec4* rather than owning its own
	// storage, same "one-way sync onto an already-virtual OnChange() hook"
	// shape as CommandTogglePointer.hpp/CommandSliderPointer.hpp (see
	// either one's own doc comment for the shared reasoning - reads *ptr
	// once at construction as the initial state, writes back to it on
	// every change). Real Stand splits this into two classes only because
	// it has two different external colour storage types of its own (a
	// Colour struct, and DirectX::SimpleMath::Color) - this project's own
	// CommandColourCustom always stores ImVec4, so one class here covers
	// what both of those cover there.
	class CommandColourPointer : public CommandColourCustom
	{
	public:
		CommandColourPointer(std::string name, std::string label, std::string description, ImVec4* ptr) :
		    CommandColourCustom(std::move(name), std::move(label), std::move(description), ptr ? *ptr : ImVec4(1.f, 1.f, 1.f, 1.f)),
		    m_Ptr(ptr)
		{
		}

	protected:
		void OnChange() override
		{
			if (m_Ptr)
				*m_Ptr = GetState();
		}

	private:
		ImVec4* m_Ptr;
	};
}
