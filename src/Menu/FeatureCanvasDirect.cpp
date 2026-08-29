#include "Menu/FeatureCanvasDirect.hpp"

#include "lib/soup/ObfusString.hpp"

#include "Commands/Widgets/CommandPhysical.hpp" // HANDLER_CHECK, HANDLER_END
#include "Core/regular_event.hpp"
#include "Rendering/RenderTarget.hpp"

namespace Stand
{
	void FeatureCanvasDirect::init(int width, int height, draw_t draw_func, void* userdata)
	{
		enabled = true;
		rePresentEvent::registerHandler([this, draw_func, userdata]
		{
			HANDLER_CHECK(enabled)
			RenderTargetH rt;
			draw_func(userdata, rt);
			HANDLER_END
		}, soup::ObfusString("A Stand Gadget"));
	}

	void FeatureCanvasDirect::shutdown()
	{
		enabled = false;
	}
}
