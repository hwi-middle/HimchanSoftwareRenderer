#pragma once
#include "pch.h"

struct Color
{
public:

	FORCEINLINE constexpr Color() = default;
	FORCEINLINE explicit constexpr Color(float InR, float InG, float InB, float InA = 1.f) : R(InR), G(InG), B(InB), A(InA) {}
	// TODO: Color32도 받아서 생성자 만들기


	float R = 0.f;
	float G = 0.f;
	float B = 0.f;
	float A = 1.f;

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