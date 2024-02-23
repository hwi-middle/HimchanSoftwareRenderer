#pragma once

#include <sstream>

#include "pch.h"
#include "Vector2.h"

namespace HC
{
	struct Matrix2x2
	{
		static constexpr uint8 Rank = 2;

		std::array<Vector2, Rank> Cols = { Vector2::UnitX, Vector2::UnitY };

		FORCEINLINE constexpr Matrix2x2() = default;
		FORCEINLINE constexpr Matrix2x2(const Vector2& InCol0, const Vector2& InCol1) { Cols = { InCol0, InCol1 }; }

		FORCEINLINE constexpr const Vector2& operator[](const uint8 InIndex) const;
		FORCEINLINE constexpr Vector2& operator[](const uint8 InIndex);
		FORCEINLINE constexpr Matrix2x2 operator*(float InScalar) const;
		FORCEINLINE Matrix2x2 operator*(const Matrix2x2& InMatrix) const;
		FORCEINLINE Vector2 operator*(const Vector2& InVector) const;
		FORCEINLINE friend Vector2 operator*=(Vector2& InVector, const Matrix2x2& InMatrix)
		{
			InVector = InMatrix * InVector;
			return InVector;
		}

		Matrix2x2 Transpose() const;
	};
}

FORCEINLINE constexpr const Vector2& Matrix2x2::operator[](const uint8 InIndex) const
{
	assert(InIndex < Rank);
	return Cols[InIndex];
}

FORCEINLINE constexpr Vector2& Matrix2x2::operator[](const uint8 InIndex)
{
	assert(InIndex < Rank);
	return Cols[InIndex];
}
FORCEINLINE constexpr Matrix2x2 Matrix2x2::operator*(float InScalar) const
{
	return Matrix2x2(
		Cols[0] * InScalar,
		Cols[1] * InScalar
	);
}


FORCEINLINE Matrix2x2 Matrix2x2::operator*(const Matrix2x2& InMatrix) const
{
	Matrix2x2 transposedMatrix = Transpose();
	return Matrix2x2(
		Vector2(Vector2::Dot(transposedMatrix[0], InMatrix[0]), Vector2::Dot(transposedMatrix[1], InMatrix[0])),
		Vector2(Vector2::Dot(transposedMatrix[0], InMatrix[1]), Vector2::Dot(transposedMatrix[1], InMatrix[1]))
	);
}

FORCEINLINE Vector2 Matrix2x2::operator*(const Vector2& InVector) const
{
	Matrix2x2 transposedMatrix = Transpose();
	return Vector2(
		Vector2::Dot(transposedMatrix[0], InVector),
		Vector2::Dot(transposedMatrix[1], InVector)
	);
}

FORCEINLINE Matrix2x2 Matrix2x2::Transpose() const
{
	return Matrix2x2(
		Vector2(Cols[0].X, Cols[1].X),
		Vector2(Cols[0].Y, Cols[1].Y)
	);
}