#include "pch.h"
#include "Transform.h"

void Transform::Rotate(float InYaw, float InPitch, float InRoll)
{
	// 회전 값 적용
	Yaw += InYaw;
	Pitch += InPitch;
	Roll += InRoll;

	// 범위 제한
	Yaw = Math::FMod(Yaw, 360.f);
	Pitch = Math::FMod(Pitch, 360.f);
	Roll = Math::FMod(Roll, 360.f);

	if (Yaw < 0.f)
	{
		Yaw += 360.f;
	}

	if (Pitch < 0.f)
	{
		Pitch += 360.f;
	}

	if (Roll < 0.f)
	{
		Roll += 360.f;
	}

	// 로컬 축 업데이트
	float cy = Math::Cos(Yaw);
	float sy = Math::Sin(Yaw);
	float cp = Math::Cos(Pitch);
	float sp = Math::Sin(Pitch);
	float cr = Math::Cos(Roll);
	float sr = Math::Sin(Roll);

	Right = Vector3(cy * cr + sy * sp * sr, cp * sr, -sy * cr + cy * sp * sr);
	Up = Vector3(-cy * sr + sy * sp * cr, cp * cr, sy * sr + cy * sp * cr);
	Forward = Vector3(sy * cp, -sp, cy * cp);
}
