#pragma once

namespace HC
{
	class Camera
	{
	public:
		Camera() = default;

		FORCEINLINE Matrix4x4 GetViewMatrix() const;
		FORCEINLINE Transform& GetTransform() { return TransformComponent; }

	private:
		Transform TransformComponent;
	};

	FORCEINLINE Matrix4x4 Camera::GetViewMatrix() const
	{
		const Vector3 viewX = TransformComponent.GetLocalX();
		const Vector3 viewY = TransformComponent.GetLocalY();
		const Vector3 viewZ = TransformComponent.GetLocalZ();
		const Vector3 position = TransformComponent.GetPosition();

		return Matrix4x4(
			Vector4(Vector3(viewX.X, viewY.X, viewZ.X), 0.f),
			Vector4(Vector3(viewX.Y, viewY.Y, viewZ.Y), 0.f),
			Vector4(Vector3(viewX.Z, viewY.Z, viewZ.Z), 0.f),
			Vector4(Vector3::Dot(-viewX, position), Vector3::Dot(-viewY, position), Vector3::Dot(-viewZ,position), 1.f)
		);
	}
}

