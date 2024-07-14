#pragma once

namespace HC
{
	struct Color
	{
	public:

		FORCEINLINE constexpr Color() = default;
		FORCEINLINE explicit constexpr Color(float InR, float InG, float InB, float InA = 1.f) : R(InR), G(InG), B(InB), A(InA) {}
		FORCEINLINE explicit constexpr Color(Color32& InColor)
		{
			constexpr float OneOver255 = 1.f / 255.f;
			R = float(InColor.R) * OneOver255;
			G = float(InColor.G) * OneOver255;
			B = float(InColor.B) * OneOver255;
			A = float(InColor.A) * OneOver255;
		}

		float R = 0.f;
		float G = 0.f;
		float B = 0.f;
		float A = 1.f;

		static const Color Black;
		static const Color Gray;
		static const Color White;
		static const Color Red;
		static const Color Green;
		static const Color Blue;
		static const Color Yellow;
		static const Color Cyan;
		static const Color Magenta;
		static const Color Clear;

		FORCEINLINE constexpr Color32 ToColor32() const;

		FORCEINLINE constexpr Color operator+(const Color& InColor) const;
		FORCEINLINE constexpr Color operator-(const Color& InColor) const;
		FORCEINLINE constexpr Color operator*(const Color& InColor) const;
		FORCEINLINE constexpr Color operator*(float InScalar) const;
		FORCEINLINE constexpr Color& operator*=(float InScalar);
		FORCEINLINE constexpr Color& operator/=(float InScalar);
		FORCEINLINE constexpr Color& operator+=(const Color& InColor);
		FORCEINLINE constexpr Color& operator-=(const Color& InColor);

		FORCEINLINE constexpr bool operator==(const Color& InColor) const;
		FORCEINLINE constexpr bool operator!=(const Color& InColor) const;

	};

	FORCEINLINE constexpr Color32 Color::ToColor32() const
	{
		return Color32(
			(uint8)(0.5f + Math::Clamp(R, 0.f, 1.f) * 255.f),
			(uint8)(0.5f + Math::Clamp(G, 0.f, 1.f) * 255.f),
			(uint8)(0.5f + Math::Clamp(B, 0.f, 1.f) * 255.f),
			(uint8)(0.5f + Math::Clamp(A, 0.f, 1.f) * 255.f)
		);
	}

	FORCEINLINE constexpr Color Color::operator+(const Color& InColor) const
	{
		return Color(
			R + InColor.R,
			G + InColor.G,
			B + InColor.B,
			A + InColor.A
		);
	}

	FORCEINLINE constexpr Color Color::operator-(const Color& InColor) const
	{
		return Color(
			R - InColor.R,
			G - InColor.G,
			B - InColor.B,
			A - InColor.A
		);
	}

	FORCEINLINE constexpr Color Color::operator*(const Color& InColor) const
	{
		return Color(
			R * InColor.R,
			G * InColor.G,
			B * InColor.B,
			A * InColor.A
		);
	}

	FORCEINLINE constexpr Color Color::operator*(float InScalar) const
	{
		return Color(
			R * InScalar,
			G * InScalar,
			B * InScalar,
			A * InScalar
		);
	}

	FORCEINLINE constexpr Color& Color::operator*=(float InScale)
	{
		R *= InScale;
		G *= InScale;
		B *= InScale;
		A *= InScale;
		return *this;
	}

	FORCEINLINE constexpr Color& Color::operator/=(float InScale)
	{
		R /= InScale;
		G /= InScale;
		B /= InScale;
		A /= InScale;
		return *this;
	}

	FORCEINLINE constexpr Color& Color::operator+=(const Color& InColor)
	{
		R += InColor.R;
		G += InColor.G;
		B += InColor.B;
		A += InColor.A;
		return *this;
	}

	FORCEINLINE constexpr Color& Color::operator-=(const Color& InColor)
	{
		R -= InColor.R;
		G -= InColor.G;
		B -= InColor.B;
		A -= InColor.A;
		return *this;
	}

	FORCEINLINE constexpr bool Color::operator==(const Color& InColor) const
	{
		return R == InColor.R
			&& G == InColor.G
			&& B == InColor.B
			&& A == InColor.A;
	}

	FORCEINLINE constexpr bool Color::operator!=(const Color& InColor) const
	{
		return R != InColor.R
			|| G != InColor.G
			|| B != InColor.B
			|| A != InColor.A;
	}
}