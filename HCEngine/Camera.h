#pragma once

namespace HC
{
	class Camera
	{
	public:
		Camera() = default;

		FORCEINLINE Matrix4x4 GetViewMatrix() const;
		FORCEINLINE Transform& GetTransform() { return mTransformComponent; }

	private:
		Transform mTransformComponent;
	};

	FORCEINLINE Matrix4x4 Camera::GetViewMatrix() const
	{
		const Vector3 viewX = mTransformComponent.GetLocalX();
		const Vector3 viewY = mTransformComponent.GetLocalY();
		const Vector3 viewZ = mTransformComponent.GetLocalZ();
		const Vector3 position = mTransformComponent.GetPosition();

		return Matrix4x4(
			Vector4(Vector3(viewX.x, viewY.x, viewZ.x), 0.f),
			Vector4(Vector3(viewX.y, viewY.y, viewZ.y), 0.f),
			Vector4(Vector3(viewX.z, viewY.z, viewZ.z), 0.f),
			Vector4(Vector3::Dot(-viewX, position), Vector3::Dot(-viewY, position), Vector3::Dot(-viewZ,position), 1.f)
		);
	}
}

