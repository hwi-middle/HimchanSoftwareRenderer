#pragma once

namespace HC
{
	class Transform
	{
	public:
		Transform() :mYaw(0), mPitch(0), mRoll(0) {};

		FORCEINLINE Matrix4x4 GetModelingMatrix() const;

		FORCEINLINE void SetPosition(const Vector3& inPos);
		FORCEINLINE const Vector3& GetPosition() const;

		void SetRotation(float inYaw, float inPitch, float inRoll);
		void AddYawRotation(float inYaw);
		void AddPitchRotation(float inPitch);
		void AddRollRotation(float inRoll);
		FORCEINLINE const Vector3& GetLocalX() const;
		FORCEINLINE const Vector3& GetLocalY() const;
		FORCEINLINE const Vector3& GetLocalZ() const;

		FORCEINLINE void SetScale(const Vector3& inV);
		FORCEINLINE const Vector3& GetScale() const;

	private:
		void updateRotation();

		Vector3 mPosition;
		Vector3 mScale;

		Vector3 mRight = Vector3::unit_x;
		Vector3 mUp = Vector3::unit_y;
		Vector3 mForward = Vector3::unit_z;

		float mYaw;
		float mPitch;
		float mRoll;
	};

	FORCEINLINE Matrix4x4 Transform::GetModelingMatrix() const
	{
		return Matrix4x4(
			Vector4(mRight * mScale.x, 0.f),
			Vector4(mUp * mScale.y, 0.f),
			Vector4(mForward * mScale.z, 0.f),
			Vector4(mPosition, 1.f)
		);
	}

	FORCEINLINE void Transform::SetPosition(const Vector3& inPos)
	{
		mPosition = inPos;
	}

	FORCEINLINE const Vector3& Transform::GetPosition() const
	{
		return mPosition;
	}

	FORCEINLINE const Vector3& Transform::GetLocalX() const
	{
		return mRight;
	}

	FORCEINLINE const Vector3& Transform::GetLocalY() const
	{
		return mUp;
	}

	FORCEINLINE const Vector3& Transform::GetLocalZ() const
	{
		return mForward;
	}

	FORCEINLINE void Transform::SetScale(const Vector3& inV)
	{
		mScale = inV;
	}

	FORCEINLINE const Vector3& Transform::GetScale() const
	{
		return mScale;
	}
}

