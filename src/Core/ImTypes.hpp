#pragma once

// Minimal drop-in replacement for the small slice of Dear ImGui's own
// math/color types this project had adopted as general-purpose types long
// before ImGui was removed from it - CommandColourCustom's stored value,
// Rendering/imgui_colors.h's ~150 named CSS-colour constants,
// Rendering/RainbowColor.hpp, Rendering/ChatDisplay.hpp's Show() colour
// parameter, and a couple of stray ImVec2 uses (e.g. AutoDriveHUD.cpp's
// own local panel-rect math) that never had anything to do with actually
// drawing through ImGui. Replacing every one of those call sites with
// DirectX::XMFLOAT2/4 was out of scope for removing ImGui - this keeps
// their existing field names (x/y/z/w) and arithmetic working completely
// unchanged instead.
//
// Force-included via Core/common.hpp's precompiled header, same as
// <imgui.h> was before it - every translation unit gets these for free,
// without needing its own include.
//
// This project always built with IMGUI_DEFINE_MATH_OPERATORS (see the old
// CMakeLists.txt compile definition), so ImVec2/ImVec4 arithmetic is used
// unconditionally in a few spots - the operators below are therefore
// unconditional too, rather than gated behind that macro the way real
// ImGui gates them.

struct ImVec2
{
	float x, y;
	constexpr ImVec2() :
	    x(0.0f),
	    y(0.0f)
	{
	}
	constexpr ImVec2(float _x, float _y) :
	    x(_x),
	    y(_y)
	{
	}
};

struct ImVec4
{
	float x, y, z, w;
	constexpr ImVec4() :
	    x(0.0f),
	    y(0.0f),
	    z(0.0f),
	    w(0.0f)
	{
	}
	constexpr ImVec4(float _x, float _y, float _z, float _w) :
	    x(_x),
	    y(_y),
	    z(_z),
	    w(_w)
	{
	}
};

using ImU32 = unsigned int;

// Same shape as real ImGui's ImColor: a float ImVec4 with an implicit
// ImVec4 conversion (RainbowColor.hpp's Color(ImVec4) constructor and
// imgui_colors.h's ~150 named constants both lean on this).
struct ImColor
{
	ImVec4 Value;
	constexpr ImColor() :
	    Value(0.0f, 0.0f, 0.0f, 0.0f)
	{
	}
	constexpr ImColor(float r, float g, float b, float a = 1.0f) :
	    Value(r, g, b, a)
	{
	}
	constexpr ImColor(const ImVec4& col) :
	    Value(col)
	{
	}
	constexpr operator ImVec4() const
	{
		return Value;
	}
};

constexpr ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs)
{
	return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y);
}
constexpr ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs)
{
	return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y);
}
constexpr ImVec2 operator*(const ImVec2& lhs, const ImVec2& rhs)
{
	return ImVec2(lhs.x * rhs.x, lhs.y * rhs.y);
}
constexpr ImVec2 operator/(const ImVec2& lhs, const ImVec2& rhs)
{
	return ImVec2(lhs.x / rhs.x, lhs.y / rhs.y);
}
constexpr ImVec2 operator*(const ImVec2& lhs, float rhs)
{
	return ImVec2(lhs.x * rhs, lhs.y * rhs);
}
constexpr ImVec2 operator/(const ImVec2& lhs, float rhs)
{
	return ImVec2(lhs.x / rhs, lhs.y / rhs);
}
constexpr ImVec2 operator-(const ImVec2& v)
{
	return ImVec2(-v.x, -v.y);
}
inline ImVec2& operator+=(ImVec2& lhs, const ImVec2& rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}
inline ImVec2& operator-=(ImVec2& lhs, const ImVec2& rhs)
{
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	return lhs;
}
inline ImVec2& operator*=(ImVec2& lhs, float rhs)
{
	lhs.x *= rhs;
	lhs.y *= rhs;
	return lhs;
}
inline ImVec2& operator/=(ImVec2& lhs, float rhs)
{
	lhs.x /= rhs;
	lhs.y /= rhs;
	return lhs;
}

constexpr ImVec4 operator+(const ImVec4& lhs, const ImVec4& rhs)
{
	return ImVec4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}
constexpr ImVec4 operator-(const ImVec4& lhs, const ImVec4& rhs)
{
	return ImVec4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}
constexpr ImVec4 operator*(const ImVec4& lhs, const ImVec4& rhs)
{
	return ImVec4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
}
