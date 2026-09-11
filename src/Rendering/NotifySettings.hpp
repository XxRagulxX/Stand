#pragma once
#include <DirectXMath.h>

#include <cstdint>

namespace Stand::Rendering::NotifySettings
{
	enum class Type
	{
		Game,
		StandNextToMap,
		StandCustomPosition,
	};

	inline Type kType = Type::StandNextToMap;

	inline bool kInvertFlow = false;
	inline float kWidth = 400.f;
	inline float kPadding = 10.f;

	inline int16_t kCustomPositionX = 32;
	inline int16_t kCustomPositionY = 532;

	inline DirectX::XMFLOAT4 kBorderColour{1.f, 0.f, 1.f, 1.f};
	inline DirectX::XMFLOAT4 kFlashColour{0.6196f, 0.f, 0.6196f, 1.f};
	inline DirectX::XMFLOAT4 kBackgroundColour{0.f, 0.f, 0.f, 77.f / 255.f};

	inline std::uint16_t kReadingSpeedWpm = 250;
	inline int kReadingStartDelayMs = 500;
	inline int kMinDurationMs = 2000;
	inline int kMaxDurationMs = 10000;

	constexpr int kFlashMs = 500;
}
