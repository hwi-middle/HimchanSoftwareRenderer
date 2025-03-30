#pragma once

#include "HCCore.h"
#include "Vector2.h"

namespace HC
{
	struct ScreenPoint
	{
		FORCEINLINE static Vector2 ScreenToCartesian(const Vector2& inPoint, const uint32 width, const uint32 height)
		{
			return Vector2(inPoint.x - width * 0.5f + 0.5f, -(inPoint.y + 0.5f) + height * 0.5f);
		}

		FORCEINLINE static Vector2 CartesianToScreen(const Vector2& inPoint, const uint32 width, const uint32 height)
		{
			return Vector2(static_cast<int>(inPoint.x + width * 0.5f), static_cast<int>(-inPoint.y + height * 0.5f));
		}
	};
}