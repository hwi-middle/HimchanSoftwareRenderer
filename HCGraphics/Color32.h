#pragma once

namespace HC
{
	struct Color32
	{
	public:
		FORCEINLINE constexpr Color32() : R(0), G(0), B(0), A(0) {}
		FORCEINLINE explicit constexpr Color32(byte InR, byte InG, byte InB, byte InA = 255) : R(InR), G(InG), B(InB), A(InA) {}

		FORCEINLINE constexpr Color32 operator+(const Color32& InColor) const;
		FORCEINLINE constexpr Color32 operator-(const Color32& InColor) const;
		FORCEINLINE constexpr Color32 operator*(const Color32& InColor) const;
		FORCEINLINE constexpr Color32 operator*(float InScalar) const;
		FORCEINLINE constexpr Color32 operator/(float InScalar) const;
		FORCEINLINE constexpr Color32& operator*=(float InScalar);
		FORCEINLINE constexpr Color32& operator/=(float InScalar);
		FORCEINLINE constexpr Color32& operator+=(const Color32& InColor);
		FORCEINLINE constexpr Color32& operator-=(const Color32& InColor);

		FORCEINLINE constexpr bool operator==(const Color32& InColor) const;
		FORCEINLINE constexpr bool operator!=(const Color32& InColor) const;

		union
		{
			struct
			{
				byte B, G, R, A;
			};

			uint32 ColorValue;
		};
	};

	FORCEINLINE constexpr Color32 Color32::operator+(const Color32& InColor) const
	{
		return Color32(
			R + InColor.R,
			G + InColor.G,
			B + InColor.B,
			A + InColor.A
		);
	}

	FORCEINLINE constexpr Color32 Color32::operator-(const Color32& InColor) const
	{
		return Color32(
			R - InColor.R,
			G - InColor.G,
			B - InColor.B,
			A - InColor.A
		);
	}

	FORCEINLINE constexpr Color32 Color32::operator*(const Color32& InColor) const
	{
		return Color32(
			R * InColor.R,
			G * InColor.G,
			B * InColor.B,
			A * InColor.A
		);
	}

	FORCEINLINE constexpr Color32 Color32::operator*(float InScalar) const
	{
		return Color32(
			R * InScalar,
			G * InScalar,
			B * InScalar,
			A * InScalar
		);
	}

	FORCEINLINE constexpr Color32 Color32::operator/(float InScalar) const
	{
		return Color32(
			R / InScalar,
			G / InScalar,
			B / InScalar,
			A / InScalar
		);
	}

	FORCEINLINE constexpr Color32& Color32::operator*=(float InScale)
	{
		R *= InScale;
		G *= InScale;
		B *= InScale;
		A *= InScale;
		return *this;
	}

	FORCEINLINE constexpr Color32& Color32::operator/=(float InScale)
	{
		R /= InScale;
		G /= InScale;
		B /= InScale;
		A /= InScale;
		return *this;
	}

	FORCEINLINE constexpr Color32& Color32::operator+=(const Color32& InColor)
	{
		R += InColor.R;
		G += InColor.G;
		B += InColor.B;
		A += InColor.A;
		return *this;
	}

	FORCEINLINE constexpr Color32& Color32::operator-=(const Color32& InColor)
	{
		R -= InColor.R;
		G -= InColor.G;
		B -= InColor.B;
		A -= InColor.A;
		return *this;
	}

	FORCEINLINE constexpr bool Color32::operator==(const Color32& InColor) const
	{
		return R == InColor.R
			&& G == InColor.G
			&& B == InColor.B
			&& A == InColor.A;
	}

	FORCEINLINE constexpr bool Color32::operator!=(const Color32& InColor) const
	{
		return R != InColor.R
			|| G != InColor.G
			|| B != InColor.B
			|| A != InColor.A;
	}
}