#pragma once
#include "pch.h"

namespace HC
{
	struct Vector3
	{
		static constexpr uint8 dimension = 3;

		union
		{
			struct
			{
				float x;
				float y;
				float z;
			};

			std::array<float, dimension> components = { 0.f, 0.f, 0.f };
		};

		static const Vector3 unit_x;
		static const Vector3 unit_y;
		static const Vector3 unit_z;
		static const Vector3 zero;
		static const Vector3 one;

		FORCEINLINE constexpr Vector3() = default;
		FORCEINLINE explicit constexpr Vector3(const float inX, const float inY, const float inZ) : x(inX), y(inY), z(inZ) {}

		FORCEINLINE constexpr float operator[](const uint8 inIndex) const;
		FORCEINLINE constexpr Vector3 operator-() const;
		FORCEINLINE constexpr Vector3 operator+(const Vector3& inVector) const;
		FORCEINLINE constexpr Vector3 operator-(const Vector3& inVector) const;
		FORCEINLINE constexpr Vector3 operator*(const Vector3& inVector) const;
		FORCEINLINE constexpr Vector3 operator*(const float inScalar) const;
		FORCEINLINE constexpr Vector3 operator/(const Vector3& inVector) const;
		FORCEINLINE constexpr Vector3 operator/(const float inScalar) const;
		FORCEINLINE constexpr Vector3& operator+=(const Vector3& inVector);
		FORCEINLINE constexpr Vector3& operator-=(const Vector3& inVector);
		FORCEINLINE constexpr Vector3& operator*=(const Vector3& inVector);
		FORCEINLINE constexpr Vector3& operator/=(const Vector3& inVector);

		[[nodiscard]] FORCEINLINE float GetMagnitude() const;
		[[nodiscard]] FORCEINLINE constexpr float GetSquaredMagnitude() const;
		FORCEINLINE void Normalize();
		[[nodiscard]] FORCEINLINE Vector3 GetNormalized() const;

		FORCEINLINE static constexpr float Dot(const Vector3& inVector1, const Vector3& inVector2);
		FORCEINLINE static constexpr Vector3 Cross(const Vector3& inVector1, const Vector3& inVector2);

		[[nodiscard]] std::string ToString() const;
	};

	FORCEINLINE constexpr float Vector3::operator[](const uint8 inIndex) const
	{
		assert(inIndex < dimension);
		return components[inIndex];
	}

	FORCEINLINE constexpr Vector3 Vector3::operator-() const
	{
		return Vector3(-x, -y, -z);
	}

	FORCEINLINE constexpr Vector3 Vector3::operator+(const Vector3& inVector) const
	{
		return Vector3(x + inVector.x, y + inVector.y, z + inVector.z);
	}

	FORCEINLINE constexpr Vector3 Vector3::operator-(const Vector3& inVector) const
	{
		return Vector3(x - inVector.x, y - inVector.y, z - inVector.z);
	}

	FORCEINLINE constexpr Vector3 Vector3::operator*(const Vector3& inVector) const
	{
		return Vector3(x * inVector.x, y * inVector.y, z * inVector.z);
	}

	FORCEINLINE constexpr Vector3 Vector3::operator*(const float inScalar) const
	{
		return Vector3(x * inScalar, y * inScalar, z * inScalar);
	}

	FORCEINLINE constexpr Vector3 Vector3::operator/(const Vector3& inVector) const
	{
		return Vector3(x / inVector.x, y / inVector.y, z / inVector.z);
	}

	FORCEINLINE constexpr Vector3 Vector3::operator/(const float inScalar) const
	{
		return Vector3(x / inScalar, y / inScalar, z / inScalar);
	}

	FORCEINLINE constexpr Vector3& Vector3::operator+=(const Vector3& inVector)
	{
		x += inVector.x;
		y += inVector.y;
		z += inVector.z;
		return *this;
	}

	FORCEINLINE constexpr Vector3& Vector3::operator-=(const Vector3& inVector)
	{
		x -= inVector.x;
		y -= inVector.y;
		z -= inVector.z;
		return *this;
	}

	FORCEINLINE constexpr Vector3& Vector3::operator*=(const Vector3& inVector)
	{
		x *= inVector.x;
		y *= inVector.y;
		z *= inVector.z;
		return *this;
	}

	FORCEINLINE constexpr Vector3& Vector3::operator/=(const Vector3& inVector)
	{
		x /= inVector.x;
		y /= inVector.y;
		z /= inVector.z;
		return *this;
	}

	FORCEINLINE float Vector3::GetMagnitude() const
	{
		return sqrtf(GetSquaredMagnitude());
	}

	FORCEINLINE constexpr float Vector3::GetSquaredMagnitude() const
	{
		return x * x + y * y + z * z;
	}

	FORCEINLINE void Vector3::Normalize()
	{
		*this = GetNormalized();
	}

	FORCEINLINE Vector3 Vector3::GetNormalized() const
	{
		return Vector3(x, y, z) * Math::GetInvSqrt(GetSquaredMagnitude());
	}

	FORCEINLINE std::string Vector3::ToString() const
	{
		std::ostringstream ss;
		ss << "(" << x << ", " << y << ", " << z << ")";
		return ss.str();
	}

	FORCEINLINE constexpr float Vector3::Dot(const Vector3& inVector1, const Vector3& inVector2)
	{
		return inVector1.x * inVector2.x + inVector1.y * inVector2.y + inVector1.z * inVector2.z;
	}

	FORCEINLINE constexpr Vector3 Vector3::Cross(const Vector3& inVector1, const Vector3& inVector2)
	{
		return Vector3(
			inVector1.y * inVector2.z - inVector1.z * inVector2.y,
			inVector1.z * inVector2.x - inVector1.x * inVector2.z,
			inVector1.x * inVector2.y - inVector1.y * inVector2.x);
	}
}