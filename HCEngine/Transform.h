#pragma once

namespace HC
{
	class Transform
	{
	public:
		Transform() = default;

		FORCEINLINE Matrix4x4 GetModelingMatrix() const;

		FORCEINLINE const Vector3& GetPosition() const;
		FORCEINLINE const Vector3& GetLocalX() const;
		FORCEINLINE const Vector3& GetLocalY() const;
		FORCEINLINE const Vector3& GetLocalZ() const;

	private:
		Vector3 Position;
		Vector3 Scale;

		Vector3 Right = Vector3::UnitX;
		Vector3 Up = Vector3::UnitY;
		Vector3 Forward = Vector3::UnitZ;

		float Yaw;
		float Pitch;
		float Roll;
	};

	FORCEINLINE Matrix4x4 Transform::GetModelingMatrix() const
	{
		return Matrix4x4(
			Vector4(Right * Scale.X, 0.f),
			Vector4(Up * Scale.Y, 0.f),
			Vector4(Forward * Scale.Z, 0.f),
			Vector4(Position, 1)
		);
	}

	FORCEINLINE const Vector3& Transform::GetPosition() const
	{
		return Position;
	}

	FORCEINLINE const Vector3& Transform::GetLocalX() const
	{
		return Right;
	}

	FORCEINLINE const Vector3& Transform::GetLocalY() const
	{
		return Up;
	}

	FORCEINLINE const Vector3& Transform::GetLocalZ() const
	{
		return Forward;
	}
}

