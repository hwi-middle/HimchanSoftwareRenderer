#pragma once

namespace HC
{
	struct Vector3
	{
		static constexpr uint8 Dimension = 3;

		union
		{
			struct
			{
				float X;
				float Y;
				float Z;
			};

			std::array<float, Dimension> Components = { 0.f, 0.f, 0.f };
		};

		static const Vector3 UnitX;
		static const Vector3 UnitY;
		static const Vector3 UnitZ;
		static const Vector3 Zero;
		static const Vector3 One;

		FORCEINLINE constexpr Vector3() = default;
		FORCEINLINE explicit constexpr Vector3(const float InX, const float InY, const float InZ) : X(InX), Y(InY), Z(InZ) {}

		FORCEINLINE constexpr float operator[](const uint8 InIndex) const;
		FORCEINLINE constexpr Vector3 operator-() const;
		FORCEINLINE constexpr Vector3 operator+(const Vector3& InVector) const;
		FORCEINLINE constexpr Vector3 operator-(const Vector3& InVector) const;
		FORCEINLINE constexpr Vector3 operator*(const Vector3& InVector) const;
		FORCEINLINE constexpr Vector3 operator*(const float InScalar) const;
		FORCEINLINE constexpr Vector3 operator/(const Vector3& InVector) const;
		FORCEINLINE constexpr Vector3 operator/(const float InScalar) const;
		FORCEINLINE constexpr Vector3& operator+=(const Vector3& InVector);
		FORCEINLINE constexpr Vector3& operator-=(const Vector3& InVector);
		FORCEINLINE constexpr Vector3& operator*=(const Vector3& InVector);
		FORCEINLINE constexpr Vector3& operator/=(const Vector3& InVector);

		[[nodiscard]] FORCEINLINE float GetMagnitude() const;
		[[nodiscard]] FORCEINLINE constexpr float GetSquaredMagnitude() const;
		FORCEINLINE void Normalize();
		[[nodiscard]] FORCEINLINE Vector3 GetNormalized() const;

		FORCEINLINE static constexpr float Dot(const Vector3& InVector1, const Vector3& InVector2);
		FORCEINLINE static constexpr Vector3 Cross(const Vector3& InVector1, const Vector3& InVector2);

		[[nodiscard]] std::string ToString() const;
	};

	FORCEINLINE constexpr float Vector3::operator[](const uint8 InIndex) const
	{
		assert(InIndex < Dimension);
		return Components[InIndex];
	}

	FORCEINLINE constexpr Vector3 Vector3::operator-() const
	{
		return Vector3(-X, -Y, -Z);
	}

	FORCEINLINE constexpr Vector3 Vector3::operator+(const Vector3& InVector) const
	{
		return Vector3(X + InVector.X, Y + InVector.Y, Z + InVector.Z);
	}

	FORCEINLINE constexpr Vector3 Vector3::operator-(const Vector3& InVector) const
	{
		return Vector3(X - InVector.X, Y - InVector.Y, Z - InVector.Z);
	}

	FORCEINLINE constexpr Vector3 Vector3::operator*(const Vector3& InVector) const
	{
		return Vector3(X * InVector.X, Y * InVector.Y, Z * InVector.Z);
	}

	FORCEINLINE constexpr Vector3 Vector3::operator*(const float InScalar) const
	{
		return Vector3(X * InScalar, Y * InScalar, Z * InScalar);
	}

	FORCEINLINE constexpr Vector3 Vector3::operator/(const Vector3& InVector) const
	{
		return Vector3(X / InVector.X, Y / InVector.Y, Z / InVector.Z);
	}

	FORCEINLINE constexpr Vector3 Vector3::operator/(const float InScalar) const
	{
		return Vector3(X / InScalar, Y / InScalar, Z / InScalar);
	}

	FORCEINLINE constexpr Vector3& Vector3::operator+=(const Vector3& InVector)
	{
		X += InVector.X;
		Y += InVector.Y;
		Z += InVector.Z;
		return *this;
	}

	FORCEINLINE constexpr Vector3& Vector3::operator-=(const Vector3& InVector)
	{
		X -= InVector.X;
		Y -= InVector.Y;
		Z -= InVector.Z;
		return *this;
	}

	FORCEINLINE constexpr Vector3& Vector3::operator*=(const Vector3& InVector)
	{
		X *= InVector.X;
		Y *= InVector.Y;
		Z *= InVector.Z;
		return *this;
	}

	FORCEINLINE constexpr Vector3& Vector3::operator/=(const Vector3& InVector)
	{
		X /= InVector.X;
		Y /= InVector.Y;
		Z /= InVector.Z;
		return *this;
	}

	FORCEINLINE float Vector3::GetMagnitude() const
	{
		return sqrtf(GetSquaredMagnitude());
	}

	FORCEINLINE constexpr float Vector3::GetSquaredMagnitude() const
	{
		return X * X + Y * Y + Z * Z;
	}

	FORCEINLINE void Vector3::Normalize()
	{
		*this = GetNormalized();
	}

	FORCEINLINE Vector3 Vector3::GetNormalized() const
	{
		return Vector3(X, Y, Z) * Math::GetInvSqrt(GetSquaredMagnitude());
	}

	FORCEINLINE std::string Vector3::ToString() const
	{
		std::ostringstream ss;
		ss << "(" << X << ", " << Y << ", " << Z << ")";
		return ss.str();
	}

	FORCEINLINE constexpr float Vector3::Dot(const Vector3& InVector1, const Vector3& InVector2)
	{
		return InVector1.X * InVector2.X + InVector1.Y * InVector2.Y + InVector1.Z * InVector2.Z;
	}

	FORCEINLINE constexpr Vector3 Vector3::Cross(const Vector3& InVector1, const Vector3& InVector2)
	{
		return Vector3(
			InVector1.Y * InVector2.Z - InVector1.Z * InVector2.Y,
			InVector1.Z * InVector2.X - InVector1.X * InVector2.Z,
			InVector1.X * InVector2.Y - InVector1.Y * InVector2.X);
	}
}