#pragma once
#include "pch.h"

namespace HC
{
	struct Math
	{
		static constexpr float PI = 3.1415926535897932f;
		static constexpr float TWO_PI = 2 * PI;
		static constexpr float HALF_PI = 0.5 * PI;
		static constexpr float INV_PI = 1 / PI;
		static constexpr float KINDA_SMALL_NUMBER = 1.e-4f;

		static constexpr float RAD_2DEG = PI / 180.f;
		static constexpr float DEG2_RAD = 180.f / PI;

		static float GetInvSqrt(const float inValue)
		{
			if (MachineInfo::GetInstance()->IsSseSupport())
			{
				const __m128 one = _mm_set_ss(1.0f);
				const __m128 val = _mm_set_ss(inValue);
				const __m128 sqrtVal = _mm_sqrt_ss(val);
				const __m128 res = _mm_div_ss(one, sqrtVal);
				float ret;
				_mm_store_ss(&ret, res);
				return ret;
			}

			constexpr uint8 newtonRaphsonIteration = 3;

			const float x2 = inValue * 0.5f;
			float y = inValue;
			int i = *std::bit_cast<int*>(&y);
			i = 0x5f3759df - (i >> 1);
			y = *std::bit_cast<float*>(&i);

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
			if (y <= KINDA_SMALL_NUMBER)
			{
				return 0.f;
			}

			float quotient = static_cast<float>(static_cast<int>(x / y));
			return x - y * quotient;
		}

		template <class T>
		FORCEINLINE static constexpr T Clamp(const T& inValue, T inMin, T inMax)
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

