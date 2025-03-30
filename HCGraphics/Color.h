#pragma once

namespace HC
{
	struct Color
	{
	public:

		FORCEINLINE constexpr Color() = default;
		FORCEINLINE explicit constexpr Color(float inR, float inG, float inB, float inA = 1.f) : r(inR), g(inG), b(inB), a(inA) {}
		FORCEINLINE explicit constexpr Color(Color32& inColor)
		{
			constexpr float oneOver255 = 1.f / 255.f;
			r = float(inColor.r) * oneOver255;
			g = float(inColor.g) * oneOver255;
			b = float(inColor.b) * oneOver255;
			a = float(inColor.a) * oneOver255;
		}

		float r = 0.f;
		float g = 0.f;
		float b = 0.f;
		float a = 1.f;

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

		FORCEINLINE constexpr Color operator+(const Color& inColor) const;
		FORCEINLINE constexpr Color operator-(const Color& inColor) const;
		FORCEINLINE constexpr Color operator*(const Color& inColor) const;
		FORCEINLINE constexpr Color operator*(float inScalar) const;
		FORCEINLINE constexpr Color operator/(float inScalar) const;
		FORCEINLINE constexpr Color& operator*=(float inScalar);
		FORCEINLINE constexpr Color& operator/=(float inScalar);
		FORCEINLINE constexpr Color& operator+=(const Color& inColor);
		FORCEINLINE constexpr Color& operator-=(const Color& inColor);

		FORCEINLINE constexpr bool operator==(const Color& inColor) const;
		FORCEINLINE constexpr bool operator!=(const Color& inColor) const;

	};

	FORCEINLINE constexpr Color32 Color::ToColor32() const
	{
		return Color32(
			(uint8)(0.5f + Math::Clamp(r, 0.f, 1.f) * 255.f),
			(uint8)(0.5f + Math::Clamp(g, 0.f, 1.f) * 255.f),
			(uint8)(0.5f + Math::Clamp(b, 0.f, 1.f) * 255.f),
			(uint8)(0.5f + Math::Clamp(a, 0.f, 1.f) * 255.f)
		);
	}

	FORCEINLINE constexpr Color Color::operator+(const Color& inColor) const
	{
		return Color(
			r + inColor.r,
			g + inColor.g,
			b + inColor.b,
			a + inColor.a
		);
	}

	FORCEINLINE constexpr Color Color::operator-(const Color& inColor) const
	{
		return Color(
			r - inColor.r,
			g - inColor.g,
			b - inColor.b,
			a - inColor.a
		);
	}

	FORCEINLINE constexpr Color Color::operator*(const Color& inColor) const
	{
		return Color(
			r * inColor.r,
			g * inColor.g,
			b * inColor.b,
			a * inColor.a
		);
	}

	FORCEINLINE constexpr Color Color::operator*(float inScalar) const
	{
		return Color(
			r * inScalar,
			g * inScalar,
			b * inScalar,
			a * inScalar
		);
	}

	FORCEINLINE constexpr Color Color::operator/(float inScalar) const
	{
		return Color(
			r / inScalar,
			g / inScalar,
			b / inScalar,
			a / inScalar
		);
	}

	FORCEINLINE constexpr Color& Color::operator*=(float inScale)
	{
		r *= inScale;
		g *= inScale;
		b *= inScale;
		a *= inScale;
		return *this;
	}

	FORCEINLINE constexpr Color& Color::operator/=(float inScale)
	{
		r /= inScale;
		g /= inScale;
		b /= inScale;
		a /= inScale;
		return *this;
	}

	FORCEINLINE constexpr Color& Color::operator+=(const Color& inColor)
	{
		r += inColor.r;
		g += inColor.g;
		b += inColor.b;
		a += inColor.a;
		return *this;
	}

	FORCEINLINE constexpr Color& Color::operator-=(const Color& inColor)
	{
		r -= inColor.r;
		g -= inColor.g;
		b -= inColor.b;
		a -= inColor.a;
		return *this;
	}

	FORCEINLINE constexpr bool Color::operator==(const Color& inColor) const
	{
		return r == inColor.r
			&& g == inColor.g
			&& b == inColor.b
			&& a == inColor.a;
	}

	FORCEINLINE constexpr bool Color::operator!=(const Color& inColor) const
	{
		return r != inColor.r
			|| g != inColor.g
			|| b != inColor.b
			|| a != inColor.a;
	}
}