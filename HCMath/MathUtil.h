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

		FORCEINLINE static constexpr float GetInvSqrt(const float InValue)
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

		template <class T>
		FORCEINLINE static constexpr T Square(const T& InValue)
		{
			return InValue * InValue;
		}

		template <class T>
		FORCEINLINE static constexpr T Lerp(const T& InFrom, const T& InTo, float InAlpha)
		{
			InAlpha = Clamp(InAlpha, 0.f, 1.f);
			return (1 - InAlpha) * InFrom + InAlpha * InTo;
		}

		template <class T>
		FORCEINLINE static constexpr T SmoothStep(const T& InMin, const T& InMax, float InAlpha)
		{
			InAlpha = Clamp(InAlpha, 0.f, 1.f);
			return Square(InAlpha) * (-2 * InAlpha + 3) * (InMax - InMin) + InMin;
		}

		template <class T>
		FORCEINLINE static constexpr T Clamp(const T& InValue, float InMin, float InMax)
		{
			return (InValue < InMin) ? (InMin) : (InValue < InMax ? InValue : InMax);
		}

		template <class T>
		FORCEINLINE static constexpr T Max(const T& A, const T& B)
		{
			return A > B ? A : B;
		}

		template <class T>
		FORCEINLINE static constexpr T Min(const T& A, const T& B)
		{
			return A < B ? A : B;
		}

		FORCEINLINE static float Sin(const float InValue)
		{
			return sinf(InValue);
		}

		FORCEINLINE static double Sin(const double InValue)
		{
			return sin(InValue);
		}

		FORCEINLINE static float Asin(const float InValue)
		{
			return asinf(InValue);
		}

		FORCEINLINE static double Asin(const double InValue)
		{
			return asin(InValue);
		}

		FORCEINLINE static float Cos(const float InValue)
		{
			return cosf(InValue);
		}

		FORCEINLINE static double Cos(const double InValue)
		{
			return cos(InValue);
		}

		FORCEINLINE static float Acos(const float InValue)
		{
			return acosf(InValue);
		}

		FORCEINLINE static double Acos(const double InValue)
		{
			return acos(InValue);
		}

		FORCEINLINE static float Tan(const float InValue)
		{
			return tanf(InValue);
		}

		FORCEINLINE static double Tan(const double InValue)
		{
			return tan(InValue);
		}

		FORCEINLINE static float Atan(const float InValue)
		{
			return atanf(InValue);
		}

		FORCEINLINE static double Atan(const double InValue)
		{
			return atan(InValue);
		}

		FORCEINLINE static float Atan2(const float InY, const float InX)
		{
			return atan2f(InY, InX);
		}

		FORCEINLINE static double Atan2(const double InY, const double InX)
		{
			return atan2(InY, InX);
		}
	};
}

