#pragma once

namespace HC
{
	struct Color32
	{
	public:
		FORCEINLINE constexpr Color32() : r(0), g(0), b(0), a(0) {}
		FORCEINLINE explicit constexpr Color32(byte inR, byte inG, byte inB, byte inA = 255) : r(inR), g(inG), b(inB), a(inA) {}

		FORCEINLINE constexpr Color32 operator+(const Color32& inColor) const;
		FORCEINLINE constexpr Color32 operator-(const Color32& inColor) const;
		FORCEINLINE constexpr Color32 operator*(const Color32& inColor) const;
		FORCEINLINE constexpr Color32 operator*(float inScalar) const;
		FORCEINLINE constexpr Color32 operator/(float inScalar) const;
		FORCEINLINE constexpr Color32& operator*=(float inScalar);
		FORCEINLINE constexpr Color32& operator/=(float inScalar);
		FORCEINLINE constexpr Color32& operator+=(const Color32& inColor);
		FORCEINLINE constexpr Color32& operator-=(const Color32& inColor);

		FORCEINLINE constexpr bool operator==(const Color32& inColor) const;
		FORCEINLINE constexpr bool operator!=(const Color32& inColor) const;

		union
		{
			struct
			{
				byte b, g, r, a;
			};

			uint32 color_value;
		};
	};

	FORCEINLINE constexpr Color32 Color32::operator+(const Color32& inColor) const
	{
		return Color32(
			r + inColor.r,
			g + inColor.g,
			b + inColor.b,
			a + inColor.a
		);
	}

	FORCEINLINE constexpr Color32 Color32::operator-(const Color32& inColor) const
	{
		return Color32(
			r - inColor.r,
			g - inColor.g,
			b - inColor.b,
			a - inColor.a
		);
	}

	FORCEINLINE constexpr Color32 Color32::operator*(const Color32& inColor) const
	{
		return Color32(
			r * inColor.r,
			g * inColor.g,
			b * inColor.b,
			a * inColor.a
		);
	}

	FORCEINLINE constexpr Color32 Color32::operator*(float inScalar) const
	{
		return Color32(
			static_cast<HC::byte>(static_cast<float>(r) * inScalar),
			static_cast<HC::byte>(static_cast<float>(g) * inScalar),
			static_cast<HC::byte>(static_cast<float>(b) * inScalar),
			static_cast<HC::byte>(static_cast<float>(a) * inScalar)
		);
	}

	FORCEINLINE constexpr Color32 Color32::operator/(float inScalar) const
	{
		return Color32(
			static_cast<HC::byte>(static_cast<float>(r) / inScalar),
			static_cast<HC::byte>(static_cast<float>(g) / inScalar),
			static_cast<HC::byte>(static_cast<float>(b) / inScalar),
			static_cast<HC::byte>(static_cast<float>(a) / inScalar)
		);
	}

	FORCEINLINE constexpr Color32& Color32::operator*=(float inScale)
	{
		r = static_cast<HC::byte>(static_cast<float>(r) * inScale);
		g = static_cast<HC::byte>(static_cast<float>(g) * inScale);
		b = static_cast<HC::byte>(static_cast<float>(b) * inScale);
		a = static_cast<HC::byte>(static_cast<float>(a) * inScale);
		return *this;
	}

	FORCEINLINE constexpr Color32& Color32::operator/=(float inScale)
	{
		r = static_cast<HC::byte>(static_cast<float>(r) / inScale);
		g = static_cast<HC::byte>(static_cast<float>(g) / inScale);
		b = static_cast<HC::byte>(static_cast<float>(b) / inScale);
		a = static_cast<HC::byte>(static_cast<float>(a) / inScale);
		return *this;
	}

	FORCEINLINE constexpr Color32& Color32::operator+=(const Color32& inColor)
	{
		r += inColor.r;
		g += inColor.g;
		b += inColor.b;
		a += inColor.a;
		return *this;
	}

	FORCEINLINE constexpr Color32& Color32::operator-=(const Color32& inColor)
	{
		r -= inColor.r;
		g -= inColor.g;
		b -= inColor.b;
		a -= inColor.a;
		return *this;
	}

	FORCEINLINE constexpr bool Color32::operator==(const Color32& inColor) const
	{
		return r == inColor.r
			&& g == inColor.g
			&& b == inColor.b
			&& a == inColor.a;
	}

	FORCEINLINE constexpr bool Color32::operator!=(const Color32& inColor) const
	{
		return r != inColor.r
			|| g != inColor.g
			|| b != inColor.b
			|| a != inColor.a;
	}
}