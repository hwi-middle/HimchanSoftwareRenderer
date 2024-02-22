#pragma once

#include <sstream>

#include "pch.h"
#include "MathUtil.h"
#include "Vector3.h"

namespace HC
{
	struct Matrix3x3
	{
		static constexpr uint8 Rank = 3;

		std::array<Vector3, Rank> Cols = { Vector3::UnitX, Vector3::UnitY, Vector3::UnitZ };

		FORCEINLINE constexpr Matrix3x3() = default;
		FORCEINLINE constexpr Matrix3x3(const Vector3& InCol0, const Vector3& InCol1, const Vector3& InCol2) { Cols = { InCol0, InCol1, InCol2 }; }

		FORCEINLINE constexpr const Vector3& operator[](const uint8 InIndex) const;
		FORCEINLINE constexpr Vector3& operator[](const uint8 InIndex);
		FORCEINLINE constexpr Matrix3x3 operator*(float InScalar) const;
		FORCEINLINE Matrix3x3 operator*(const Matrix3x3& InMatrix) const;
		FORCEINLINE Vector3 operator*(const Vector3& InVector) const;
		FORCEINLINE friend Vector3 operator*=(Vector3& InVector, const Matrix3x3& InMatrix)
		{
			InVector = InMatrix * InVector;
			return InVector;
		}

		Matrix3x3 Transpose() const;
	};
}

FORCEINLINE constexpr const Vector3& Matrix3x3::operator[](const uint8 InIndex) const
{
	assert(InIndex < Rank);
	return Cols[InIndex];
}

FORCEINLINE constexpr Vector3& Matrix3x3::operator[](const uint8 InIndex)
{
	assert(InIndex < Rank);
	return Cols[InIndex];
}
FORCEINLINE constexpr Matrix3x3 Matrix3x3::operator*(float InScalar) const
{
	return Matrix3x3(
		Cols[0] * InScalar,
		Cols[1] * InScalar,
		Cols[2] * InScalar
	);
}


FORCEINLINE Matrix3x3 Matrix3x3::operator*(const Matrix3x3& InMatrix) const
{
	Matrix3x3 transposedMatrix = Transpose();
	return Matrix3x3(
		Vector3(Vector3::Dot(transposedMatrix[0], InMatrix[0]), Vector3::Dot(transposedMatrix[1], InMatrix[0]), Vector3::Dot(transposedMatrix[2], InMatrix[0])),
		Vector3(Vector3::Dot(transposedMatrix[0], InMatrix[1]), Vector3::Dot(transposedMatrix[1], InMatrix[1]), Vector3::Dot(transposedMatrix[2], InMatrix[1])),
		Vector3(Vector3::Dot(transposedMatrix[0], InMatrix[2]), Vector3::Dot(transposedMatrix[1], InMatrix[2]), Vector3::Dot(transposedMatrix[2], InMatrix[2]))
	);
}

FORCEINLINE Vector3 Matrix3x3::operator*(const Vector3& InVector) const
{
	Matrix3x3 transposedMatrix = Transpose();
	return Vector3(
		Vector3::Dot(transposedMatrix[0], InVector),
		Vector3::Dot(transposedMatrix[1], InVector),
		Vector3::Dot(transposedMatrix[2], InVector)
	);
}

FORCEINLINE Matrix3x3 Matrix3x3::Transpose() const
{
	return Matrix3x3(
		Vector3(Cols[0].X, Cols[1].X, Cols[2].X),
		Vector3(Cols[0].Y, Cols[1].Y, Cols[2].Y),
		Vector3(Cols[0].Z, Cols[1].Z, Cols[2].Z)
	);
}