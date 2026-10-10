#include "Rendering/StandPort/GridItemQrcode.hpp"

#include <soup/QrCode.hpp>
#include <soup/RenderTarget.hpp>
#include <soup/Rgb.hpp>

#include "Rendering/GridRenderer.hpp"
#include "Rendering/StandPort/ColourUtil.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Rendering
{
	namespace
	{
		struct RenderTargetQrcode : public soup::RenderTarget
		{
			RenderTargetQrcode()
				: soup::RenderTarget(
				      static_cast<unsigned int>(Theme::kHudWidth),
				      static_cast<unsigned int>(Theme::kHudHeight))
			{
			}

			void drawRect(int x, int y, unsigned int w, unsigned int h, soup::Rgb colour) final
			{
				if (colour != soup::Rgb::BLACK)
				{
					GridRenderer::DrawRect(float(x), float(y), float(w), float(h), Theme::kAccent);
				}
			}
		};
	}

	GridItemQrcode::GridItemQrcode(int16_t size, uint8_t priority, const std::string& text)
		: GridItem(GRIDITEM_INDIFFERENT, size, size, priority)
	{
		auto qr = soup::QrCode::encodeText(text);
		c = qr.toCanvas(2, soup::Rgb::WHITE, soup::Rgb::BLACK);
		c.resizeNearestNeighbour(width, height);
	}

	void GridItemQrcode::draw()
	{
		DirectX::XMFLOAT4 bg_colour{0.f, 0.f, 0.f, 1.f};
		if (!ColourUtil::isContrastSufficient(bg_colour, Theme::kAccent))
			bg_colour = DirectX::XMFLOAT4{1.f, 1.f, 1.f, 1.f};
		GridRenderer::DrawRect(x, y, width, height, bg_colour);

		RenderTargetQrcode rt{};
		rt.drawCanvas(x, y, c);
	}
}
