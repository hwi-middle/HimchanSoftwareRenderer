#pragma once

#include <sstream>

#include "pch.h"
#include "MathUtil.h"
#include "Vector4.h"

namespace HC
{
	struct Matrix4x4
	{
		static constexpr uint8 Rank = 4;

		std::array<Vector4, Rank> Cols = { Vector4::UnitX, Vector4::UnitY, Vector4::UnitZ };

		FORCEINLINE constexpr Matrix4x4() = default;
		FORCEINLINE constexpr Matrix4x4(const Vector4& InCol0, const Vector4& InCol1, const Vector4& InCol2, const Vector4& InCol3) { Cols = { InCol0, InCol1, InCol2, InCol3 }; }

		FORCEINLINE constexpr const Vector4& operator[](const uint8 InIndex) const;
		FORCEINLINE constexpr Vector4& operator[](const uint8 InIndex);
		FORCEINLINE constexpr Matrix4x4 operator*(float InScalar) const;
		FORCEINLINE Matrix4x4 operator*(const Matrix4x4& InMatrix) const;
		FORCEINLINE Vector4 operator*(const Vector4& InVector) const;
		FORCEINLINE friend Vector4 operator*=(Vector4& InVector, const Matrix4x4& InMatrix)
		{
			InVector = InMatrix * InVector;
			return InVector;
		}

		Matrix4x4 Transpose() const;
	};
}

FORCEINLINE constexpr const Vector4& Matrix4x4::operator[](const uint8 InIndex) const
{
	assert(InIndex < Rank);
	return Cols[InIndex];
}

FORCEINLINE constexpr Vector4& Matrix4x4::operator[](const uint8 InIndex)
{
	assert(InIndex < Rank);
	return Cols[InIndex];
}
FORCEINLINE constexpr Matrix4x4 Matrix4x4::operator*(float InScalar) const
{
	return Matrix4x4(
		Cols[0] * InScalar,
		Cols[1] * InScalar,
		Cols[2] * InScalar,
		Cols[3] * InScalar
	);
}


FORCEINLINE Matrix4x4 Matrix4x4::operator*(const Matrix4x4& InMatrix) const
{
	Matrix4x4 transposedMatrix = Transpose();
	return Matrix4x4(
		Vector4(Vector4::Dot(transposedMatrix[0], InMatrix[0]), Vector4::Dot(transposedMatrix[1], InMatrix[0]), Vector4::Dot(transposedMatrix[2], InMatrix[0]), Vector4::Dot(transposedMatrix[3], InMatrix[0])),
		Vector4(Vector4::Dot(transposedMatrix[0], InMatrix[1]), Vector4::Dot(transposedMatrix[1], InMatrix[1]), Vector4::Dot(transposedMatrix[2], InMatrix[1]), Vector4::Dot(transposedMatrix[3], InMatrix[1])),
		Vector4(Vector4::Dot(transposedMatrix[0], InMatrix[2]), Vector4::Dot(transposedMatrix[1], InMatrix[2]), Vector4::Dot(transposedMatrix[2], InMatrix[2]), Vector4::Dot(transposedMatrix[3], InMatrix[2])),
		Vector4(Vector4::Dot(transposedMatrix[0], InMatrix[3]), Vector4::Dot(transposedMatrix[1], InMatrix[3]), Vector4::Dot(transposedMatrix[2], InMatrix[3]), Vector4::Dot(transposedMatrix[3], InMatrix[3]))
	);
}

FORCEINLINE Vector4 Matrix4x4::operator*(const Vector4& InVector) const
{
	Matrix4x4 transposedMatrix = Transpose();
	return Vector4(
		Vector4::Dot(transposedMatrix[0], InVector),
		Vector4::Dot(transposedMatrix[1], InVector),
		Vector4::Dot(transposedMatrix[2], InVector),
		Vector4::Dot(transposedMatrix[3], InVector)
	);
}

FORCEINLINE Matrix4x4 Matrix4x4::Transpose() const
{
	return Matrix4x4(
		Vector4(Cols[0].X, Cols[1].X, Cols[2].X, Cols[3].X),
		Vector4(Cols[0].Y, Cols[1].Y, Cols[2].Y, Cols[3].Y),
		Vector4(Cols[0].Z, Cols[1].Z, Cols[2].Z, Cols[3].Z),
		Vector4(Cols[0].W, Cols[1].W, Cols[2].W, Cols[3].W)
	);
}