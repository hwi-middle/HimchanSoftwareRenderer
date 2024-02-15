#pragma once

#include <sstream>

#include "pch.h"
#include "MathUtil.h"

namespace HC
{
	struct Vector4
	{
		static constexpr uint8 Dimension = 4;

		union
		{
			struct
			{
				float X;
				float Y;
				float Z;
				float W;
			};

			std::array<float, Dimension> Components = { 0.f, 0.f, 0.f, 0.f };
		};

		static const Vector4 UnitX;
		static const Vector4 UnitY;
		static const Vector4 UnitZ;
		static const Vector4 UnitW;
		static const Vector4 Zero;
		static const Vector4 One;

		FORCEINLINE constexpr Vector4() = default;
		FORCEINLINE explicit constexpr Vector4(const float InX, const float InY, const float InZ, const float InW) : X(InX), Y(InY), Z(InZ), W(InW) {}

		FORCEINLINE constexpr float operator[](const uint8 InIndex) const;
		FORCEINLINE constexpr Vector4 operator+(const Vector4& InVector) const;
		FORCEINLINE constexpr Vector4 operator-(const Vector4& InVector) const;
		FORCEINLINE constexpr Vector4 operator*(const Vector4& InVector) const;
		FORCEINLINE constexpr Vector4 operator*(const float InScalar) const;
		FORCEINLINE constexpr Vector4 operator/(const Vector4& InVector) const;
		FORCEINLINE constexpr Vector4 operator/(const float InScalar) const;
		FORCEINLINE constexpr Vector4& operator+=(const Vector4& InVector);
		FORCEINLINE constexpr Vector4& operator-=(const Vector4& InVector);
		FORCEINLINE constexpr Vector4& operator*=(const Vector4& InVector);
		FORCEINLINE constexpr Vector4& operator/=(const Vector4& InVector);

		[[nodiscard]] FORCEINLINE float GetMagnitude() const;
		[[nodiscard]] FORCEINLINE constexpr float GetSquaredMagnitude() const;
		FORCEINLINE void Normalize();
		[[nodiscard]] FORCEINLINE Vector4 GetNormalized() const;

		FORCEINLINE static constexpr float Dot(const Vector4& InVector1, const Vector4& InVector2);

		[[nodiscard]] std::string ToString() const;
	};
}

FORCEINLINE constexpr float Vector4::operator[](const uint8 InIndex) const
{
	assert(InIndex < Dimension);
	return Components[InIndex];
}

FORCEINLINE constexpr Vector4 Vector4::operator+(const Vector4& InVector) const
{
	return Vector4(X + InVector.X, Y + InVector.Y, Z + InVector.Z, W + InVector.W);
}

FORCEINLINE constexpr Vector4 Vector4::operator-(const Vector4& InVector) const
{
	return Vector4(X - InVector.X, Y - InVector.Y, Z - InVector.Z, W - InVector.W);
}

FORCEINLINE constexpr Vector4 Vector4::operator*(const Vector4& InVector) const
{
	return Vector4(X * InVector.X, Y * InVector.Y, Z * InVector.Z, W * InVector.W);
}

FORCEINLINE constexpr Vector4 Vector4::operator*(const float InScalar) const
{
	return Vector4(X * InScalar, Y * InScalar, Z * InScalar, W * InScalar);
}

FORCEINLINE constexpr Vector4 Vector4::operator/(const Vector4& InVector) const
{
	return Vector4(X / InVector.X, Y / InVector.Y, Z / InVector.Z, W / InVector.W);
}

FORCEINLINE constexpr Vector4 Vector4::operator/(const float InScalar) const
{
	return Vector4(X / InScalar, Y / InScalar, Z / InScalar, W / InScalar);
}

FORCEINLINE constexpr Vector4& Vector4::operator+=(const Vector4& InVector)
{
	X += InVector.X;
	Y += InVector.Y;
	Z += InVector.Z;
	W += InVector.W;
	return *this;
}

FORCEINLINE constexpr Vector4& Vector4::operator-=(const Vector4& InVector)
{
	X -= InVector.X;
	Y -= InVector.Y;
	Z -= InVector.Z;
	W -= InVector.W;
	return *this;
}

FORCEINLINE constexpr Vector4& Vector4::operator*=(const Vector4& InVector)
{
	X *= InVector.X;
	Y *= InVector.Y;
	Z *= InVector.Z;
	W *= InVector.W;
	return *this;
}

FORCEINLINE constexpr Vector4& Vector4::operator/=(const Vector4& InVector)
{
	X /= InVector.X;
	Y /= InVector.Y;
	Z /= InVector.Z;
	W /= InVector.W;
	return *this;
}

FORCEINLINE float Vector4::GetMagnitude() const
{
	return sqrtf(GetSquaredMagnitude());
}

FORCEINLINE constexpr float Vector4::GetSquaredMagnitude() const
{
	return X * X + Y * Y + Z * Z + W * W;
}

FORCEINLINE void Vector4::Normalize()
{
	*this = GetNormalized();
}

FORCEINLINE Vector4 Vector4::GetNormalized() const
{
	return Vector4(X, Y, Z, W) * Math::GetInvSqrt(GetSquaredMagnitude());
}

FORCEINLINE std::string Vector4::ToString() const
{
	std::ostringstream ss;
	ss << "(" << X << ", " << Y << ", " << Z << ", " << W << ")";
	return ss.str();
}

FORCEINLINE constexpr float Vector4::Dot(const Vector4& InVector1, const Vector4& InVector2)
{
	return InVector1.X * InVector2.X + InVector1.Y * InVector2.Y + InVector1.Z * InVector2.Z + InVector1.W * InVector2.W;
}