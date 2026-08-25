#pragma once

#include "Menu/FeatureCanvas.hpp"

#include <optional>

#include "lib/soup/Window.hpp"

namespace Stand
{
	struct FeatureCanvasWindow : public FeatureCanvas
	{
		std::optional<soup::Window> w;

		void init(int width, int height, draw_t draw_func, void* userdata) final;
		void shutdown() final;
	};
}
