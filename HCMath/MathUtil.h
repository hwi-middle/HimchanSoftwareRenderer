#pragma once
#include "HCCore.h"

namespace HC
{
	struct MathUtil
	{
		FORCEINLINE constexpr static float GetInvSqrt(float InValue);
	};
}

FORCEINLINE constexpr static float GetInvSqrt(float InValue)
{
	const float ThreeHalfs = 1.5f;

	float X2 = InValue * 0.5f;
	float Y = InValue;
	int32 i = *(int32*)&Y;                    
	i = 0x5f3759df - (i >> 1);         
	Y = *(float*)&i;
	Y = Y * (ThreeHalfs - (X2 * Y * Y));
	//	y = y * ( threehalfs - ( x2 * y * y ) );

	return Y;
}
