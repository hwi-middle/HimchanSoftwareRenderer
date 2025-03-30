#pragma once

namespace HC
{
	class Transform
	{
	public:
		Transform() :Yaw(0), Pitch(0), Roll(0) {};

		FORCEINLINE Matrix4x4 GetModelingMatrix() const;

		FORCEINLINE const void SetPosition(const Vector3& InPos);
		FORCEINLINE const Vector3& GetPosition() const;

		void SetRotation(float InYaw, float InPitch, float InRoll);
		void AddYawRoation(float InYaw);
		void AddPitchRoation(float InPitch);
		void AddRollRoation(float InRoll);
		FORCEINLINE const Vector3& GetLocalX() const;
		FORCEINLINE const Vector3& GetLocalY() const;
		FORCEINLINE const Vector3& GetLocalZ() const;

		FORCEINLINE const void SetScale(const Vector3& InV);
		FORCEINLINE const Vector3& GetScale() const;

	private:
		void UpdateRotation();

		Vector3 Position;
		Vector3 Scale;

		Vector3 Right = Vector3::unit_x;
		Vector3 Up = Vector3::unit_y;
		Vector3 Forward = Vector3::unit_z;

		float Yaw;
		float Pitch;
		float Roll;
	};

	FORCEINLINE Matrix4x4 Transform::GetModelingMatrix() const
	{
		return Matrix4x4(
			Vector4(Right * Scale.x, 0.f),
			Vector4(Up * Scale.y, 0.f),
			Vector4(Forward * Scale.z, 0.f),
			Vector4(Position, 1.f)
		);
	}

	FORCEINLINE const void Transform::SetPosition(const Vector3& InPos)
	{
		Position = InPos;
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

	FORCEINLINE const void Transform::SetScale(const Vector3& InV)
	{
		Scale = InV;
	}

	FORCEINLINE const Vector3& Transform::GetScale() const
	{
		return Scale;
	}
}

