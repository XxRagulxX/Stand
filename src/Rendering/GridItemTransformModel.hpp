#pragma once
#include "Commands/Self/CommandTransformModel.hpp"
#include "Rendering/GridItem.hpp"

#include <atomic>
#include <memory>

namespace Stand::Rendering
{
	class GridItemTransformModel : public GridItem
	{
	public:
		GridItemTransformModel(int16_t width, int16_t height, Stand::CommandTransformModel* cmd);
		~GridItemTransformModel() override;

		void draw() override;
		void drawText() override;
		void onClick(int16_t, int16_t) override;

		bool isFocusable() const override { return true; }
		void activate() override;

	private:
		Stand::CommandTransformModel* m_Cmd;

		struct PreviewFiber
		{
			std::atomic<bool> active{true};
		};
		std::shared_ptr<PreviewFiber> m_Fiber;
		bool m_WasFocused = false;

		void StartPreview();
		void StopPreview();
	};
}
