#pragma once
#include "Game/Entity.hpp"
#include "Rendering/GridItem.hpp"
#include "Rendering/GridItemFocusTracker.hpp"

#include <cstdint>
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
	// - No bounding-box-aware camera offset (real Stand's own
	// getEntityOrigin() offsets by the entity's own dimensions, once
	//   spawned) - this project's Entity wrapper has no bounding-box
	//   query, so a fixed forward offset is used instead; a very large
	//   or very small model may spawn too close/far rather than exactly
	//   filling the same amount of screen space every time.
	// - CommandEntityPreviews-equivalent settings (opacity, rotation
	//   speed, per-model extra offset, disable toggle) are wired through
	//   Theme::kPreviewOpaque/kPreviewRotationSpeed/kPreview*Dist/
	//   kDisableEntityPreviews; subclasses return the appropriate dist
	//   via GetAdditionalOffset() (default 0).
	// - Always rotates (real Stand's own version skips rotation for
	//   object previews specifically, since a static prop looks odd
	//   spinning) - a subclass previewing a prop can override
	//   ShouldRotate() to turn it off.
	// - Fades by the PLAYER PED's own current speed always, not only
	//   while driving a vehicle they're the driver of (real Stand's own
	//   check) - simpler, and still delivers the same "moving fast fades
	//   the preview out" effect.
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

	private:
		void TickFocused();
		void DestroyPreview();

		GridItemFocusTracker m_FocusTracker;
		std::optional<Entity> m_Preview;
		float m_RotationDegrees = 0.f;
	};
}
