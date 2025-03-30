#pragma once
#include "pch.h"

namespace HC
{
	struct Math
	{
		static constexpr float PI = 3.1415926535897932f;
		static constexpr float TwoPI = 2 * PI;
		static constexpr float HalfPI = 0.5 * PI;
		static constexpr float InvPI = 1 / PI;
		static constexpr float KindaSmallNumber = 1.e-4f;

		static constexpr float Rad2Deg = PI / 180.f;
		static constexpr float Deg2Rad = 180.f / PI;

		FORCEINLINE static constexpr float GetInvSqrt(const float inValue)
		{
			constexpr uint8 newtonRaphsonIteration = 3;

			float x2 = inValue * 0.5f;
			float y = inValue;
			int32 i = *(int32*)&y;
			i = 0x5f3759df - (i >> 1);
			y = *(float*)&i;

			for (uint8 iter = 0; iter < newtonRaphsonIteration; ++iter)
			{
				constexpr float threeHalfs = 1.5f;
				y = y * (threeHalfs - (x2 * y * y));
			}

			return y;
		}

		template <class T>
		FORCEINLINE static constexpr T Abs(const T& inValue)
		{
			return (inValue > (T)0) ? inValue : -inValue;
		}

		template <class T>
		FORCEINLINE static constexpr T Square(const T& inValue)
		{
			return inValue * inValue;
		}

		template <class T>
		FORCEINLINE static constexpr T Lerp(const T& inFrom, const T& inTo, float inAlpha)
		{
			inAlpha = Clamp(inAlpha, 0.f, 1.f);
			return inFrom * (1 - inAlpha) + inTo * inAlpha;
		}

		template <class T>
		FORCEINLINE static constexpr T SmoothStep(const T& inMin, const T& inMax, float inAlpha)
		{
			inAlpha = Clamp(inAlpha, 0.f, 1.f);
			return Square(inAlpha) * (-2 * inAlpha + 3) * (inMax - inMin) + inMin;
		}

		FORCEINLINE static constexpr float FMod(const float x, const float y)
		{
			// 0에 대한 나머지 연산은 정의되지 않음
			if (y <= KindaSmallNumber)
			{
				return 0.f;
			}

			float quotient = static_cast<float>(static_cast<int>(x / y));
			return x - y * quotient;
		}

		template <class T>
		FORCEINLINE static constexpr T Clamp(const T& inValue, float inMin, float inMax)
		{
			return (inValue < inMin) ? (inMin) : (inValue < inMax ? inValue : inMax);
		}

		template <class T>
		FORCEINLINE static constexpr T Max(const T& a, const T& b)
		{
			return a > b ? a : b;
		}

		template <class T>
		FORCEINLINE static constexpr T Min(const T& a, const T& b)
		{
			return a < b ? a : b;
		}

		FORCEINLINE static float Sin(const float inValue)
		{
			return sinf(inValue);
		}

		FORCEINLINE static double Sin(const double inValue)
		{
			return sin(inValue);
		}

		FORCEINLINE static float Asin(const float inValue)
		{
			return asinf(inValue);
		}

		FORCEINLINE static double Asin(const double inValue)
		{
			return asin(inValue);
		}

		FORCEINLINE static float Cos(const float inValue)
		{
			return cosf(inValue);
		}

		FORCEINLINE static double Cos(const double inValue)
		{
			return cos(inValue);
		}

		FORCEINLINE static float Acos(const float inValue)
		{
			return acosf(inValue);
		}

		FORCEINLINE static double Acos(const double inValue)
		{
			return acos(inValue);
		}

		FORCEINLINE static float Tan(const float inValue)
		{
			return tanf(inValue);
		}

		FORCEINLINE static double Tan(const double inValue)
		{
			return tan(inValue);
		}

		FORCEINLINE static float Atan(const float inValue)
		{
			return atanf(inValue);
		}

		FORCEINLINE static double Atan(const double inValue)
		{
			return atan(inValue);
		}

		FORCEINLINE static float Atan2(const float inY, const float inX)
		{
			return atan2f(inY, inX);
		}

		FORCEINLINE static double Atan2(const double inY, const double inX)
		{
			return atan2(inY, inX);
		}
	};
}

