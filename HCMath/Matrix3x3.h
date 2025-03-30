#pragma once

namespace HC
{
	struct Matrix3x3
	{
		static constexpr uint8 rank = 3;

		std::array<Vector3, rank> cols = { Vector3::unit_x, Vector3::unit_y, Vector3::unit_z };

		FORCEINLINE constexpr Matrix3x3() = default;
		FORCEINLINE constexpr Matrix3x3(const Vector3& inCol0, const Vector3& inCol1, const Vector3& inCol2) { cols = { inCol0, inCol1, inCol2 }; }

		FORCEINLINE constexpr const Vector3& operator[](const uint8 inIndex) const;
		FORCEINLINE Vector3& operator[](const uint8 inIndex);
		FORCEINLINE constexpr Matrix3x3 operator*(float inScalar) const;
		FORCEINLINE Matrix3x3 operator*(const Matrix3x3& inMatrix) const;
		FORCEINLINE Vector3 operator*(const Vector3& inVector) const;
		FORCEINLINE friend Vector3 operator*=(Vector3& inVector, const Matrix3x3& inMatrix)
		{
			inVector = inMatrix * inVector;
			return inVector;
		}

		Matrix3x3 Transpose() const;
	};


	FORCEINLINE constexpr const Vector3& Matrix3x3::operator[](const uint8 inIndex) const
	{
		assert(inIndex < rank);
		return cols[inIndex];
	}

	FORCEINLINE Vector3& Matrix3x3::operator[](const uint8 inIndex)
	{
		assert(inIndex < rank);
		return cols[inIndex];
	}
	FORCEINLINE constexpr Matrix3x3 Matrix3x3::operator*(float inScalar) const
	{
		return Matrix3x3(
			cols[0] * inScalar,
			cols[1] * inScalar,
			cols[2] * inScalar
		);
	}


	FORCEINLINE Matrix3x3 Matrix3x3::operator*(const Matrix3x3& inMatrix) const
	{
		Matrix3x3 transposedMatrix = Transpose();
		return Matrix3x3(
			Vector3(Vector3::Dot(transposedMatrix[0], inMatrix[0]), Vector3::Dot(transposedMatrix[1], inMatrix[0]), Vector3::Dot(transposedMatrix[2], inMatrix[0])),
			Vector3(Vector3::Dot(transposedMatrix[0], inMatrix[1]), Vector3::Dot(transposedMatrix[1], inMatrix[1]), Vector3::Dot(transposedMatrix[2], inMatrix[1])),
			Vector3(Vector3::Dot(transposedMatrix[0], inMatrix[2]), Vector3::Dot(transposedMatrix[1], inMatrix[2]), Vector3::Dot(transposedMatrix[2], inMatrix[2]))
		);
	}

	FORCEINLINE Vector3 Matrix3x3::operator*(const Vector3& inVector) const
	{
		Matrix3x3 transposedMatrix = Transpose();
		return Vector3(
			Vector3::Dot(transposedMatrix[0], inVector),
			Vector3::Dot(transposedMatrix[1], inVector),
			Vector3::Dot(transposedMatrix[2], inVector)
		);
	}

	FORCEINLINE Matrix3x3 Matrix3x3::Transpose() const
	{
		return Matrix3x3(
			Vector3(cols[0].x, cols[1].x, cols[2].x),
			Vector3(cols[0].y, cols[1].y, cols[2].y),
			Vector3(cols[0].z, cols[1].z, cols[2].z)
		);
	}
}