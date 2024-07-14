#pragma once

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

		static const Vector2 UnitX;
		static const Vector2 UnitY;
		static const Vector2 Zero;
		static const Vector2 One;

		FORCEINLINE constexpr Vector2() = default;
		FORCEINLINE explicit constexpr Vector2(const float InX, const float InY) : X(InX), Y(InY) {}

		FORCEINLINE constexpr float operator[](const uint8 InIndex) const;
		FORCEINLINE constexpr Vector2 operator-() const;
		FORCEINLINE constexpr Vector2 operator+(const Vector2& InVector) const;
		FORCEINLINE constexpr Vector2 operator-(const Vector2& InVector) const;
		FORCEINLINE constexpr Vector2 operator*(const Vector2& InVector) const;
		FORCEINLINE constexpr Vector2 operator*(const float InScalar) const;
		FORCEINLINE constexpr Vector2 operator/(const Vector2& InVector) const;
		FORCEINLINE constexpr Vector2 operator/(const float InScalar) const;
		FORCEINLINE constexpr Vector2& operator+=(const Vector2& InVector);
		FORCEINLINE constexpr Vector2& operator-=(const Vector2& InVector);
		FORCEINLINE constexpr Vector2& operator*=(const Vector2& InVector);
		FORCEINLINE constexpr Vector2& operator/=(const Vector2& InVector);

		[[nodiscard]] FORCEINLINE float GetMagnitude() const;
		[[nodiscard]] FORCEINLINE constexpr float GetSquaredMagnitude() const;
		FORCEINLINE void Normalize();
		[[nodiscard]] FORCEINLINE Vector2 GetNormalized() const;

		FORCEINLINE static constexpr float Dot(const Vector2& InVector1, const Vector2& InVector2);

		[[nodiscard]] std::string ToString() const;
	};

	FORCEINLINE constexpr float Vector2::operator[](const uint8 InIndex) const
	{
		assert(InIndex < Dimension);
		return Components[InIndex];
	}

	FORCEINLINE constexpr Vector2 Vector2::operator-() const
	{
		return Vector2(-X, -Y);
	}

	FORCEINLINE constexpr Vector2 Vector2::operator+(const Vector2& InVector) const
	{
		return Vector2(X + InVector.X, Y + InVector.Y);
	}

	FORCEINLINE constexpr Vector2 Vector2::operator-(const Vector2& InVector) const
	{
		return Vector2(X - InVector.X, Y - InVector.Y);
	}

	FORCEINLINE constexpr Vector2 Vector2::operator*(const Vector2& InVector) const
	{
		return Vector2(X * InVector.X, Y * InVector.Y);
	}

	FORCEINLINE constexpr Vector2 Vector2::operator*(const float InScalar) const
	{
		return Vector2(X * InScalar, Y * InScalar);
	}

	FORCEINLINE constexpr Vector2 Vector2::operator/(const Vector2& InVector) const
	{
		return Vector2(X / InVector.X, Y / InVector.Y);
	}

	FORCEINLINE constexpr Vector2 Vector2::operator/(const float InScalar) const
	{
		return Vector2(X / InScalar, Y / InScalar);
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

	FORCEINLINE constexpr Vector2& Vector2::operator*=(const Vector2& InVector)
	{
		X *= InVector.X;
		Y *= InVector.Y;
		return *this;
	}

	FORCEINLINE constexpr Vector2& Vector2::operator/=(const Vector2& InVector)
	{
		X /= InVector.X;
		Y /= InVector.Y;
		return *this;
	}

	FORCEINLINE float Vector2::GetMagnitude() const
	{
		return sqrtf(GetSquaredMagnitude());
	}

	FORCEINLINE constexpr float Vector2::GetSquaredMagnitude() const
	{
		return X * X + Y * Y;
	}

	FORCEINLINE void Vector2::Normalize()
	{
		*this = GetNormalized();
	}

	FORCEINLINE Vector2 Vector2::GetNormalized() const
	{
		return Vector2(X, Y) * Math::GetInvSqrt(GetSquaredMagnitude());
	}

	FORCEINLINE std::string Vector2::ToString() const
	{
		std::ostringstream ss;
		ss << "(" << X << ", " << Y << ")";
		return ss.str();
	}

	FORCEINLINE constexpr float Vector2::Dot(const Vector2& InVector1, const Vector2& InVector2)
	{
		return InVector1.X * InVector2.X + InVector1.Y * InVector2.Y;
	}
}