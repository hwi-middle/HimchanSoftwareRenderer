#include "pch.h"
#include "Transform.h"

void Transform::SetRotation(float inYaw, float inPitch, float inRoll)
{
	// 회전 값 적용
	mYaw += inYaw;
	mPitch += inPitch;
	mRoll += inRoll;

	updateRotation();
}

void Transform::AddYawRotation(float inYaw)
{
	mYaw += inYaw;
	updateRotation();
}

void Transform::AddPitchRotation(float inPitch)
{
	mPitch += inPitch;
	updateRotation();
}

void Transform::AddRollRotation(float inRoll)
{
	mRoll += inRoll;
	updateRotation();
}

void Transform::updateRotation()
{
	// 범위 제한
	mYaw = Math::FMod(mYaw, 360.f);
	mPitch = Math::FMod(mPitch, 360.f);
	mRoll = Math::FMod(mRoll, 360.f);

	if (mYaw < 0.f)
	{
		mYaw += 360.f;
	}

	if (mPitch < 0.f)
	{
		mPitch += 360.f;
	}

	if (mRoll < 0.f)
	{
		mRoll += 360.f;
	}

	// 로컬 축 업데이트
	float cy = Math::Cos(mYaw);
	float sy = Math::Sin(mYaw);
	float cp = Math::Cos(mPitch);
	float sp = Math::Sin(mPitch);
	float cr = Math::Cos(mRoll);
	float sr = Math::Sin(mRoll);

	mRight = Vector3(cy * cr - sy * sp * sr, cp * sr, -sy * cr + cy * sp * sr);
	mUp = Vector3(-cy * sr - sy * sp * cr, cp * cr, sy * sr + cy * sp * cr);
	mForward = Vector3(sy * cp, sp, cy * cp);
}