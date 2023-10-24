#pragma once

#include "pch.h"

namespace HC
{
	struct Vector2
	{
		static constexpr uint8 Dimension = 2;

		union
		{
			struct
			{
				float X;
				float Y;
			};

			std::array<float, Dimension> Components = { 0.f, 0.f };
		};

		FORCEINLINE constexpr Vector2() = default;
		FORCEINLINE constexpr Vector2(const float InX, const float InY) : X(InX), Y(InY) {}

		FORCEINLINE constexpr float operator[](const uint8 InIndex) const;
		FORCEINLINE constexpr Vector2 operator+(const Vector2& InVector) const;
		FORCEINLINE constexpr Vector2 operator-(const Vector2& InVector) const;
		FORCEINLINE constexpr Vector2& operator+=(const Vector2& InVector);
		FORCEINLINE constexpr Vector2& operator-=(const Vector2& InVector);

		FORCEINLINE static constexpr Vector2 Dot(const Vector2& InVector1, const Vector2& InVector2);
	};
}

FORCEINLINE constexpr float Vector2::operator[](const uint8 InIndex) const
{
	assert(InIndex < Dimension);
	return Components[InIndex];
}

FORCEINLINE constexpr Vector2 Vector2::operator+(const Vector2& InVector) const
{
	return Vector2(X + InVector.X, Y + InVector.Y);
}

FORCEINLINE constexpr Vector2 Vector2::operator-(const Vector2& InVector) const
{
	return Vector2(X - InVector.X, Y - InVector.Y);
}

FORCEINLINE constexpr Vector2& Vector2::operator+=(const Vector2& InVector)
{
	X += InVector.X;
	Y += InVector.Y;
	return *this;
}

FORCEINLINE constexpr Vector2& Vector2::operator-=(const Vector2& InVector)
{
	X -= InVector.X;
	Y -= InVector.Y;
	return *this;
}

FORCEINLINE constexpr Vector2 Vector2::Dot(const Vector2& InVector1, const Vector2& InVector2)
{
	return Vector2(InVector1.X * InVector2.X, InVector1.Y * InVector2.Y);
}
