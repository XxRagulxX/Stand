#pragma once
#include "Game/Entity.hpp"
#include "Rendering/StandPort/GridItem.hpp"
#include "Rendering/GridItemFocusTracker.hpp"

#include <atomic>
#include <cstdint>
#include <memory>
#include <optional>

namespace Stand::Rendering
{
	class GridItemEntityPreview : public GridItem
	{
	public:
		GridItemEntityPreview(int16_t width, int16_t height);
		~GridItemEntityPreview() override;

		void draw() override;

	protected:
		[[nodiscard]] virtual std::uint32_t GetModelHash() const = 0;
		[[nodiscard]] virtual Entity CreateEntity(const rage::fvector3& pos) const = 0;

		[[nodiscard]] virtual bool ShouldRotate() const
		{
			return true;
		}

		[[nodiscard]] virtual float GetAdditionalOffset() const
		{
			return 0.f;
		}

		// Initial z-rotation offset added to the camera yaw on first spawn.
		// Vehicles return 90.0f to match real Stand's CommandWithEntityPreview.
		[[nodiscard]] virtual float GetInitialRotationOffset() const
		{
			return 0.f;
		}

		virtual void OnPreviewTick(Entity& entity) {}

	protected:
		void ClearPreview();

	private:
		struct PreviewControl
		{
			std::atomic<bool> alive{true};
			std::atomic<bool> jobPending{false};
			std::atomic<bool> focused{false};
			std::atomic<bool> suppressed{false};
		};

		void TickFocused();
		void DestroyPreview();
		void runWatchdog(std::shared_ptr<PreviewControl> ctrl);

		GridItemFocusTracker m_FocusTracker;
		std::shared_ptr<PreviewControl> m_Control;
		std::optional<Entity> m_Preview;
		float m_RotationDegrees = 0.f;
		float m_TotalOffset = 0.f;
	};
}
