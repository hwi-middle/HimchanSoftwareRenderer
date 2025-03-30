#pragma once

namespace HC
{
	struct Matrix2x2
	{
		static constexpr uint8 rank = 2;

		std::array<Vector2, rank> cols = { Vector2::unit_x, Vector2::unit_y };

		FORCEINLINE constexpr Matrix2x2() = default;
		FORCEINLINE constexpr Matrix2x2(const Vector2& inCol0, const Vector2& inCol1) { cols = { inCol0, inCol1 }; }

		FORCEINLINE constexpr const Vector2& operator[](const uint8 inIndex) const;
		FORCEINLINE Vector2& operator[](const uint8 inIndex);
		FORCEINLINE constexpr Matrix2x2 operator*(float inScalar) const;
		FORCEINLINE Matrix2x2 operator*(const Matrix2x2& inMatrix) const;
		FORCEINLINE Vector2 operator*(const Vector2& inVector) const;
		FORCEINLINE friend Vector2 operator*=(Vector2& inVector, const Matrix2x2& inMatrix)
		{
			inVector = inMatrix * inVector;
			return inVector;
		}

		Matrix2x2 Transpose() const;
	};

	FORCEINLINE constexpr const Vector2& Matrix2x2::operator[](const uint8 inIndex) const
	{
		assert(inIndex < rank);
		return cols[inIndex];
	}

	FORCEINLINE Vector2& Matrix2x2::operator[](const uint8 inIndex)
	{
		assert(inIndex < rank);
		return cols[inIndex];
	}
	FORCEINLINE constexpr Matrix2x2 Matrix2x2::operator*(float inScalar) const
	{
		return Matrix2x2(
			cols[0] * inScalar,
			cols[1] * inScalar
		);
	}


	FORCEINLINE Matrix2x2 Matrix2x2::operator*(const Matrix2x2& inMatrix) const
	{
		Matrix2x2 transposedMatrix = Transpose();
		return Matrix2x2(
			Vector2(Vector2::Dot(transposedMatrix[0], inMatrix[0]), Vector2::Dot(transposedMatrix[1], inMatrix[0])),
			Vector2(Vector2::Dot(transposedMatrix[0], inMatrix[1]), Vector2::Dot(transposedMatrix[1], inMatrix[1]))
		);
	}

	FORCEINLINE Vector2 Matrix2x2::operator*(const Vector2& inVector) const
	{
		Matrix2x2 transposedMatrix = Transpose();
		return Vector2(
			Vector2::Dot(transposedMatrix[0], inVector),
			Vector2::Dot(transposedMatrix[1], inVector)
		);
	}

	FORCEINLINE Matrix2x2 Matrix2x2::Transpose() const
	{
		return Matrix2x2(
			Vector2(cols[0].x, cols[1].x),
			Vector2(cols[0].y, cols[1].y)
		);
	}
}