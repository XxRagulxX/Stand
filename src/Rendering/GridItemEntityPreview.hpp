#pragma once
#include "Game/Entity.hpp"
#include "Rendering/GridItem.hpp"
#include "Rendering/GridItemFocusTracker.hpp"

#include <atomic>
#include <cstdint>
#include <memory>
#include <optional>

namespace Stand::Rendering
{
	// Ported from real Stand's own CommandWithEntityPreview<T> (Commands/
	// Widgets/CommandWithEntityPreview.hpp on origin/stand-reference) - a
	// mixin that spawns a live ghost preview entity near the camera while
	// its own row stays focused (fading it based on player speed, slowly
	// rotating it), and cleans it up the moment focus moves elsewhere.
	// Reimplemented here as a GridItem base (not a Command mixin - see
	// GridItemFocusTracker.hpp's own class comment for why focus lives on
	// the GridItem side in this project, not the Command side) that a
	// concrete row subclasses, implementing GetModelHash()/CreateEntity()
	// the same way real Stand's own getModelHash()/createPreviewEntity()
	// pure virtuals work.
	//
	// Scoped down from real Stand's own version - each gap disclosed
	// where it matters:
	// - No model-type auto-detection (ped/vehicle/object/water-animal) -
	//   the subclass already knows which Stand::Ped/Vehicle/Object::
	//   Create() to call from CreateEntity(), so there's nothing to
	//   detect (real Stand needs this since its own createPreviewEntity()
	//   is a single shared implementation across every model type).
	// - No model-type auto-detection (ped/vehicle/object/water-animal) -
	//   the subclass already knows which entity type to create from
	//   CreateEntity(), so there's nothing to detect.
	// - CommandEntityPreviews-equivalent settings (opacity, rotation
	//   speed, per-model extra offset, disable toggle) are wired through
	//   Theme::kPreviewOpaque/kPreviewRotationSpeed/kPreview*Dist/
	//   kDisableEntityPreviews; subclasses return the appropriate dist
	//   via GetAdditionalOffset() (default 0).
	// - Always rotates (real Stand's own version skips rotation for
	//   object previews specifically) - a subclass can override
	//   ShouldRotate() to disable it.
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
