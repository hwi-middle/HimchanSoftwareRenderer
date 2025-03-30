#pragma once

namespace HC
{
	struct Matrix4x4
	{
		static constexpr uint8 rank = 4;

		std::array<Vector4, rank> cols = { Vector4::unit_x, Vector4::unit_y, Vector4::unit_z };

		FORCEINLINE constexpr Matrix4x4() = default;
		FORCEINLINE constexpr Matrix4x4(const Vector4& inCol0, const Vector4& inCol1, const Vector4& inCol2, const Vector4& inCol3) { cols = { inCol0, inCol1, inCol2, inCol3 }; }

		FORCEINLINE constexpr const Vector4& operator[](const uint8 inIndex) const;
		FORCEINLINE Vector4& operator[](const uint8 inIndex);
		FORCEINLINE constexpr Matrix4x4 operator*(float inScalar) const;
		FORCEINLINE Matrix4x4 operator*(const Matrix4x4& inMatrix) const;
		FORCEINLINE Vector4 operator*(const Vector4& inVector) const;
		FORCEINLINE friend Vector4 operator*=(Vector4& inVector, const Matrix4x4& inMatrix)
		{
			inVector = inMatrix * inVector;
			return inVector;
		}

		Matrix4x4 Transpose() const;
	};


	FORCEINLINE constexpr const Vector4& Matrix4x4::operator[](const uint8 inIndex) const
	{
		assert(inIndex < rank);
		return cols[inIndex];
	}

	FORCEINLINE Vector4& Matrix4x4::operator[](const uint8 inIndex)
	{
		assert(inIndex < rank);
		return cols[inIndex];
	}
	FORCEINLINE constexpr Matrix4x4 Matrix4x4::operator*(float inScalar) const
	{
		return Matrix4x4(
			cols[0] * inScalar,
			cols[1] * inScalar,
			cols[2] * inScalar,
			cols[3] * inScalar
		);
	}


	FORCEINLINE Matrix4x4 Matrix4x4::operator*(const Matrix4x4& inMatrix) const
	{
		Matrix4x4 transposedMatrix = Transpose();
		return Matrix4x4(
			Vector4(Vector4::Dot(transposedMatrix[0], inMatrix[0]), Vector4::Dot(transposedMatrix[1], inMatrix[0]), Vector4::Dot(transposedMatrix[2], inMatrix[0]), Vector4::Dot(transposedMatrix[3], inMatrix[0])),
			Vector4(Vector4::Dot(transposedMatrix[0], inMatrix[1]), Vector4::Dot(transposedMatrix[1], inMatrix[1]), Vector4::Dot(transposedMatrix[2], inMatrix[1]), Vector4::Dot(transposedMatrix[3], inMatrix[1])),
			Vector4(Vector4::Dot(transposedMatrix[0], inMatrix[2]), Vector4::Dot(transposedMatrix[1], inMatrix[2]), Vector4::Dot(transposedMatrix[2], inMatrix[2]), Vector4::Dot(transposedMatrix[3], inMatrix[2])),
			Vector4(Vector4::Dot(transposedMatrix[0], inMatrix[3]), Vector4::Dot(transposedMatrix[1], inMatrix[3]), Vector4::Dot(transposedMatrix[2], inMatrix[3]), Vector4::Dot(transposedMatrix[3], inMatrix[3]))
		);
	}

	FORCEINLINE Vector4 Matrix4x4::operator*(const Vector4& inVector) const
	{
		Matrix4x4 transposedMatrix = Transpose();
		return Vector4(
			Vector4::Dot(transposedMatrix[0], inVector),
			Vector4::Dot(transposedMatrix[1], inVector),
			Vector4::Dot(transposedMatrix[2], inVector),
			Vector4::Dot(transposedMatrix[3], inVector)
		);
	}

	FORCEINLINE Matrix4x4 Matrix4x4::Transpose() const
	{
		return Matrix4x4(
			Vector4(cols[0].x, cols[1].x, cols[2].x, cols[3].x),
			Vector4(cols[0].y, cols[1].y, cols[2].y, cols[3].y),
			Vector4(cols[0].z, cols[1].z, cols[2].z, cols[3].z),
			Vector4(cols[0].w, cols[1].w, cols[2].w, cols[3].w)
		);
	}
}