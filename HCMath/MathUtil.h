#pragma once
#include "HCCore.h"

namespace HC
{
	struct Math
	{
		static constexpr float PI = 3.1415926535897932f;
		static constexpr float TwoPI = 2 * PI;
		static constexpr float HalfPI = 0.5 * PI;
		static constexpr float InvPI = 1 / PI;

		static constexpr float Rad2Deg = PI / 180.f;
		static constexpr float Deg2Rad = 180.f / PI;

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

