#pragma once
#include "pch.h"

namespace HC
{
	struct Vector2
	{
		static constexpr uint8 dimension = 2;

		union
		{
			struct
			{
				float x;
				float y;
			};

			std::array<float, dimension> components = { 0.f, 0.f };
		};

		static const Vector2 unit_x;
		static const Vector2 unit_y;
		static const Vector2 zero;
		static const Vector2 one;

		FORCEINLINE constexpr Vector2() = default;
		FORCEINLINE explicit constexpr Vector2(const float inX, const float inY) : x(inX), y(inY) {}

		FORCEINLINE constexpr float operator[](const uint8 inIndex) const;
		FORCEINLINE constexpr Vector2 operator-() const;
		FORCEINLINE constexpr Vector2 operator+(const Vector2& inVector) const;
		FORCEINLINE constexpr Vector2 operator-(const Vector2& inVector) const;
		FORCEINLINE constexpr Vector2 operator*(const Vector2& inVector) const;
		FORCEINLINE constexpr Vector2 operator*(const float inScalar) const;
		FORCEINLINE constexpr Vector2 operator/(const Vector2& inVector) const;
		FORCEINLINE constexpr Vector2 operator/(const float inScalar) const;
		FORCEINLINE constexpr Vector2& operator+=(const Vector2& inVector);
		FORCEINLINE constexpr Vector2& operator-=(const Vector2& inVector);
		FORCEINLINE constexpr Vector2& operator*=(const Vector2& inVector);
		FORCEINLINE constexpr Vector2& operator/=(const Vector2& inVector);

		[[nodiscard]] FORCEINLINE float GetMagnitude() const;
		[[nodiscard]] FORCEINLINE constexpr float GetSquaredMagnitude() const;
		FORCEINLINE void Normalize();
		[[nodiscard]] FORCEINLINE Vector2 GetNormalized() const;

		FORCEINLINE static constexpr float Dot(const Vector2& inVector1, const Vector2& inVector2);

		[[nodiscard]] std::string ToString() const;
	};

	FORCEINLINE constexpr float Vector2::operator[](const uint8 inIndex) const
	{
		assert(inIndex < dimension);
		return components[inIndex];
	}

	FORCEINLINE constexpr Vector2 Vector2::operator-() const
	{
		return Vector2(-x, -y);
	}

	FORCEINLINE constexpr Vector2 Vector2::operator+(const Vector2& inVector) const
	{
		return Vector2(x + inVector.x, y + inVector.y);
	}

	FORCEINLINE constexpr Vector2 Vector2::operator-(const Vector2& inVector) const
	{
		return Vector2(x - inVector.x, y - inVector.y);
	}

	FORCEINLINE constexpr Vector2 Vector2::operator*(const Vector2& inVector) const
	{
		return Vector2(x * inVector.x, y * inVector.y);
	}

	FORCEINLINE constexpr Vector2 Vector2::operator*(const float inScalar) const
	{
		return Vector2(x * inScalar, y * inScalar);
	}

	FORCEINLINE constexpr Vector2 Vector2::operator/(const Vector2& inVector) const
	{
		return Vector2(x / inVector.x, y / inVector.y);
	}

	FORCEINLINE constexpr Vector2 Vector2::operator/(const float inScalar) const
	{
		return Vector2(x / inScalar, y / inScalar);
	}

	FORCEINLINE constexpr Vector2& Vector2::operator+=(const Vector2& inVector)
	{
		x += inVector.x;
		y += inVector.y;
		return *this;
	}

	FORCEINLINE constexpr Vector2& Vector2::operator-=(const Vector2& inVector)
	{
		x -= inVector.x;
		y -= inVector.y;
		return *this;
	}

	FORCEINLINE constexpr Vector2& Vector2::operator*=(const Vector2& inVector)
	{
		x *= inVector.x;
		y *= inVector.y;
		return *this;
	}

	FORCEINLINE constexpr Vector2& Vector2::operator/=(const Vector2& inVector)
	{
		x /= inVector.x;
		y /= inVector.y;
		return *this;
	}

	FORCEINLINE float Vector2::GetMagnitude() const
	{
		return sqrtf(GetSquaredMagnitude());
	}

	FORCEINLINE constexpr float Vector2::GetSquaredMagnitude() const
	{
		return x * x + y * y;
	}

	FORCEINLINE void Vector2::Normalize()
	{
		*this = GetNormalized();
	}

	FORCEINLINE Vector2 Vector2::GetNormalized() const
	{
		return Vector2(x, y) * Math::GetInvSqrt(GetSquaredMagnitude());
	}

	FORCEINLINE std::string Vector2::ToString() const
	{
		std::ostringstream ss;
		ss << "(" << x << ", " << y << ")";
		return ss.str();
	}

	FORCEINLINE constexpr float Vector2::Dot(const Vector2& inVector1, const Vector2& inVector2)
	{
		return inVector1.x * inVector2.x + inVector1.y * inVector2.y;
	}
}