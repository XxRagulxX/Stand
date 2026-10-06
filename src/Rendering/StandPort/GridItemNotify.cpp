#include "Rendering/StandPort/GridItemNotify.hpp"

#include "Rendering/GridRenderer.hpp"
#include "Rendering/NotifySettings.hpp"
#include "Rendering/Theme.hpp"
#include "Util/get_current_time_millis.hpp"

#include <algorithm>

namespace Stand::Rendering
{
	static constexpr int16_t kBorderWidth = 3;

	std::vector<GridItemNotify*> GridItemNotify::s_Instances;

	void GridItemNotify::Freeze()
	{
		NotifySettings::kFrozenSince = Stand::get_current_time_millis();
		for (auto* inst : s_Instances)
			inst->frozen = true;
	}

	void GridItemNotify::Unfreeze()
	{
		NotifySettings::kFrozenSince = 0;
	}

	bool GridItemNotify::IsFrozen() noexcept
	{
		return NotifySettings::isFrozen();
	}

	GridItemNotify::GridItemNotify(std::string&& text, time_t show_until, time_t flash_until, std::function<void()> on_expire)
		: GridItem(
		      GRIDITEM_INDIFFERENT,
		      int16_t(NotifySettings::kWidth),
		      0,
		      0,
		      (NotifySettings::kInvertFlow && NotifySettings::kType != NotifySettings::Type::StandNextToMap)
		          ? ALIGN_BOTTOM_LEFT
		          : ALIGN_TOP_CENTRE),
		  show_until(show_until),
		  flash_until(flash_until),
		  frozen(NotifySettings::isFrozen()),
		  flashing(!IS_DEADLINE_REACHED(flash_until)),
		  text(GridRenderer::WrapText(std::move(text), Theme::kSmallTextScale, float(int16_t(NotifySettings::kWidth) - 10))),
		  m_OnExpire(std::move(on_expire))
	{
		height = int16_t(GridRenderer::MeasureText(this->text.c_str(), Theme::kSmallTextScale).y + 2.f * NotifySettings::kPadding);
		s_Instances.push_back(this);
	}

	GridItemNotify::~GridItemNotify()
	{
		s_Instances.erase(std::remove(s_Instances.begin(), s_Instances.end(), this), s_Instances.end());
	}

	void GridItemNotify::draw()
	{
		if (show_until != 0 && !frozen && IS_DEADLINE_REACHED(show_until))
		{
			if (m_OnExpire)
				m_OnExpire();
		}

		GridRenderer::DrawRect(float(x - kBorderWidth), float(y), float(kBorderWidth), float(height), NotifySettings::kBorderColour);

		if (!frozen)
			flashing = !IS_DEADLINE_REACHED(flash_until);

		GridRenderer::DrawRect(float(x), float(y), float(width), float(height),
		    flashing ? NotifySettings::kFlashColour : NotifySettings::kBackgroundColour);

		drawText();
	}

	void GridItemNotify::drawText()
	{
		GridRenderer::DrawText(
		    float(x) + 5.f + NotifySettings::kPadding,
		    float(y) + NotifySettings::kPadding,
		    text.c_str(),
		    Theme::kUnfocusedText,
		    Theme::kSmallTextScale);
	}
}
