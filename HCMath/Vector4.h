#pragma once

#include <sstream>

#include "pch.h"
#include "MathUtil.h"

namespace HC
{
	struct Vector4
	{
		static constexpr uint8 dimension = 4;

		union
		{
			struct
			{
				float x;
				float y;
				float z;
				float w;
			};

			std::array<float, dimension> components = { 0.f, 0.f, 0.f, 0.f };
		};

		static const Vector4 unit_x;
		static const Vector4 unit_y;
		static const Vector4 unit_z;
		static const Vector4 unit_w;
		static const Vector4 zero;
		static const Vector4 one;

		FORCEINLINE constexpr Vector4() = default;
		FORCEINLINE explicit constexpr Vector4(const float inX, const float inY, const float inZ, const float inW) : x(inX), y(inY), z(inZ), w(inW) {}
		FORCEINLINE explicit constexpr Vector4(const Vector2& inV, const float inZ, const float inW) : x(inV.x), y(inV.y), z(inZ), w(inW) { }
		FORCEINLINE constexpr Vector4(const Vector3& inV, const float inW) : x(inV.x), y(inV.y), z(inV.z), w(inW) {}

		FORCEINLINE constexpr Vector2 ToVector2() const;
		FORCEINLINE constexpr Vector3 ToVector3() const;

		FORCEINLINE constexpr float operator[](const uint8 inIndex) const;
		FORCEINLINE constexpr Vector4 operator-() const;
		FORCEINLINE constexpr Vector4 operator+(const Vector4& inVector) const;
		FORCEINLINE constexpr Vector4 operator-(const Vector4& inVector) const;
		FORCEINLINE constexpr Vector4 operator*(const Vector4& inVector) const;
		FORCEINLINE constexpr Vector4 operator*(const float inScalar) const;
		FORCEINLINE constexpr Vector4 operator/(const Vector4& inVector) const;
		FORCEINLINE constexpr Vector4 operator/(const float inScalar) const;
		FORCEINLINE constexpr Vector4& operator+=(const Vector4& inVector);
		FORCEINLINE constexpr Vector4& operator-=(const Vector4& inVector);
		FORCEINLINE constexpr Vector4& operator*=(const Vector4& inVector);
		FORCEINLINE constexpr Vector4& operator/=(const Vector4& inVector);

		[[nodiscard]] FORCEINLINE float GetMagnitude() const;
		[[nodiscard]] FORCEINLINE constexpr float GetSquaredMagnitude() const;
		FORCEINLINE void Normalize();
		[[nodiscard]] FORCEINLINE Vector4 GetNormalized() const;

		FORCEINLINE static constexpr float Dot(const Vector4& inVector1, const Vector4& inVector2);

		[[nodiscard]] std::string ToString() const;
	};

	FORCEINLINE constexpr Vector2 Vector4::ToVector2() const
	{
		return Vector2(x, y);
	}

	FORCEINLINE constexpr Vector3 Vector4::ToVector3() const
	{
		return Vector3(x, y, z);
	}

	FORCEINLINE constexpr float Vector4::operator[](const uint8 inIndex) const
	{
		assert(inIndex < dimension);
		return components[inIndex];
	}

	FORCEINLINE constexpr Vector4 Vector4::operator-() const
	{
		return Vector4(-x, -y, -z, -w);
	}

	FORCEINLINE constexpr Vector4 Vector4::operator+(const Vector4& inVector) const
	{
		return Vector4(x + inVector.x, y + inVector.y, z + inVector.z, w + inVector.w);
	}

	FORCEINLINE constexpr Vector4 Vector4::operator-(const Vector4& inVector) const
	{
		return Vector4(x - inVector.x, y - inVector.y, z - inVector.z, w - inVector.w);
	}

	FORCEINLINE constexpr Vector4 Vector4::operator*(const Vector4& inVector) const
	{
		return Vector4(x * inVector.x, y * inVector.y, z * inVector.z, w * inVector.w);
	}

	FORCEINLINE constexpr Vector4 Vector4::operator*(const float inScalar) const
	{
		return Vector4(x * inScalar, y * inScalar, z * inScalar, w * inScalar);
	}

	FORCEINLINE constexpr Vector4 Vector4::operator/(const Vector4& inVector) const
	{
		return Vector4(x / inVector.x, y / inVector.y, z / inVector.z, w / inVector.w);
	}

	FORCEINLINE constexpr Vector4 Vector4::operator/(const float inScalar) const
	{
		return Vector4(x / inScalar, y / inScalar, z / inScalar, w / inScalar);
	}

	FORCEINLINE constexpr Vector4& Vector4::operator+=(const Vector4& inVector)
	{
		x += inVector.x;
		y += inVector.y;
		z += inVector.z;
		w += inVector.w;
		return *this;
	}

	FORCEINLINE constexpr Vector4& Vector4::operator-=(const Vector4& inVector)
	{
		x -= inVector.x;
		y -= inVector.y;
		z -= inVector.z;
		w -= inVector.w;
		return *this;
	}

	FORCEINLINE constexpr Vector4& Vector4::operator*=(const Vector4& inVector)
	{
		x *= inVector.x;
		y *= inVector.y;
		z *= inVector.z;
		w *= inVector.w;
		return *this;
	}

	FORCEINLINE constexpr Vector4& Vector4::operator/=(const Vector4& inVector)
	{
		x /= inVector.x;
		y /= inVector.y;
		z /= inVector.z;
		w /= inVector.w;
		return *this;
	}

	FORCEINLINE float Vector4::GetMagnitude() const
	{
		return sqrtf(GetSquaredMagnitude());
	}

	FORCEINLINE constexpr float Vector4::GetSquaredMagnitude() const
	{
		return x * x + y * y + z * z + w * w;
	}

	FORCEINLINE void Vector4::Normalize()
	{
		*this = GetNormalized();
	}

	FORCEINLINE Vector4 Vector4::GetNormalized() const
	{
		return Vector4(x, y, z, w) * Math::GetInvSqrt(GetSquaredMagnitude());
	}

	FORCEINLINE std::string Vector4::ToString() const
	{
		std::ostringstream ss;
		ss << "(" << x << ", " << y << ", " << z << ", " << w << ")";
		return ss.str();
	}

	FORCEINLINE constexpr float Vector4::Dot(const Vector4& inVector1, const Vector4& inVector2)
	{
		return inVector1.x * inVector2.x + inVector1.y * inVector2.y + inVector1.z * inVector2.z + inVector1.w * inVector2.w;
	}
}