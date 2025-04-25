#pragma once

#include "HCCore.h"
#include "Vector2.h"

namespace HC
{
	struct ScreenPoint
	{
		ScreenPoint(uint32 inWidth, uint32 inHeight) : x(inWidth), y(inHeight) {}

		uint32 x;
		uint32 y;

		FORCEINLINE static Vector2 ScreenToCartesian(const ScreenPoint& inPoint, const uint32 inWidth, const uint32 inHeight)
		{
			return Vector2(inPoint.x - inWidth * 0.5f + 0.5f, -(inPoint.y + 0.5f) + inHeight * 0.5f);
		}

		FORCEINLINE static ScreenPoint CartesianToScreen(const Vector2& inPoint, const uint32 inWidth, const uint32 inHeight)
		{
			return ScreenPoint(static_cast<int>(inPoint.x + inWidth * 0.5f), static_cast<int>(-inPoint.y + inHeight * 0.5f));
		}

		FORCEINLINE static Vector2 ToVector2(const ScreenPoint& inPoint)
		{
			return Vector2(static_cast<float>(inPoint.x), static_cast<float>(inPoint.y));
		}
	};
}