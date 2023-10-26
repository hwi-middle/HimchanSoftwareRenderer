#pragma once
#include "HCCore.h"

namespace HC
{
	struct MathUtil
	{
		FORCEINLINE static float GetInvSqrt(float InValue)
		{
			constexpr uint8 NewtonRaphsonIteration = 3;
			const float ThreeHalfs = 1.5f;

			float X2 = InValue * 0.5f;
			float Y = InValue;
			int32 I = *(int32*)&Y;
			I = 0x5f3759df - (I >> 1);
			Y = *(float*)&I;

			for (uint8 Iter = 0; Iter < NewtonRaphsonIteration; ++Iter)
			{
				Y = Y * (ThreeHalfs - (X2 * Y * Y));
			}

			return Y;
		}
	};
}

