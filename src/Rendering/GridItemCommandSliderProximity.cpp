#include "Rendering/GridItemCommandSliderProximity.hpp"

#include "Commands/Commands.hpp"
#include "Rendering/Theme.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

namespace Stand::Rendering
{
	namespace
	{
		// GTA's own MarkerTypeDebugSphere - a plain translucent sphere,
		// no texture, matching real Stand's own reasoning for using a
		// debug shape here rather than needing a real 3D model.
		constexpr int kMarkerTypeDebugSphere = 28;
	}

	GridItemCommandSliderProximity::GridItemCommandSliderProximity(int16_t width, int16_t height, joaat_t id, std::optional<std::string> labelOverride, int step) :
	    GridItemCommandSlider(width, height, id, std::move(labelOverride), step),
	    m_Id(id),
	    m_Command(Commands::GetCommand<CommandSliderLegacy>(id))
	{
	}

	void GridItemCommandSliderProximity::draw()
	{
		GridItemCommandSlider::draw();

		const bool focused = isKeyboardFocused();
		m_FocusTracker.Update(focused);

		if (!focused || !m_Command)
			return;

		const float radius = static_cast<float>(m_Command->GetState());
		if (radius <= 0.f)
			return;

		const auto pos = Self::GetPed().GetPosition();
		const auto& colour = Theme::kAccent;

		GRAPHICS::DRAW_MARKER(kMarkerTypeDebugSphere,
		    pos.x,
		    pos.y,
		    pos.z,
		    0.f,
		    0.f,
		    0.f,
		    0.f,
		    0.f,
		    0.f,
		    radius * 2.f,
		    radius * 2.f,
		    radius * 2.f,
		    static_cast<int>(colour.x * 255.f),
		    static_cast<int>(colour.y * 255.f),
		    static_cast<int>(colour.z * 255.f),
		    80,
		    false,
		    false,
		    2,
		    false,
		    nullptr,
		    nullptr,
		    false);
	}
}
