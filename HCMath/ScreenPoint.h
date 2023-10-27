#pragma once

#include "HCCore.h"
#include "Vector2.h"

namespace HC
{
	struct ScreenPoint
	{
		FORCEINLINE static Vector2 ScreenToCartesian(const Vector2& InPoint, const uint32 Width, const uint32 Height)
		{
			return Vector2(InPoint.X - Width * 0.5f + 0.5f, -(InPoint.Y + 0.5f) + Height * 0.5f);
		}

		FORCEINLINE static Vector2 CartesianToScreen(const Vector2& InPoint, const uint32 Width, const uint32 Height)
		{
			return Vector2(InPoint.X + Width * 0.5f, -InPoint.Y + Height * 0.5f);
		}
	};
}