#include "pch.h"

constexpr int PRESS = 0x8000;

const std::unordered_map<eEAxis, AxisData> Input::axis_map =
{
	{ 
		eEAxis::HORIZONTAL,
		{ 
			eEKeyCode::RIGHT_ARROW, 
			eEKeyCode::LEFT_ARROW, 
			eEKeyCode::D, 
			eEKeyCode::A
		} 
	},
	{
		eEAxis::VERTICAL,
		{
			eEKeyCode::UP_ARROW,
			eEKeyCode::DOWN_ARROW,
			eEKeyCode::W,
			eEKeyCode::S
		}
	}
};

bool Input::GetKeyDown(eEKeyCode inKey)
{
	int currentState = GetAsyncKeyState(static_cast<int>(inKey));
	bool bIsDown = (currentState & PRESS) != 0;
	bool bWasDown = getKeyWasDowned(inKey);
	if (bIsDown)
	{
		mCurrentlyPressedKey.insert(inKey);
	}

	return bIsDown && !bWasDown;
}

bool Input::GetKey(eEKeyCode inKey)
{
	int currentState = GetAsyncKeyState(static_cast<int>(inKey));
	bool bIsDown = (currentState & PRESS) != 0;
	if (bIsDown)
	{
		mCurrentlyPressedKey.insert(inKey);
	}

	return bIsDown;
}

bool Input::GetKeyUp(eEKeyCode inKey)
{
	int currentState = GetAsyncKeyState(static_cast<int>(inKey));
	bool bIsUp = (currentState & PRESS) == 0;
	bool bWasDown = getKeyWasDowned(inKey);

	return bIsUp && bWasDown;
}

float Input::GetAxis(const eEAxis inAxis)
{
	const AxisData& axisData = axis_map.at(inAxis);
	bool bPositive = GetKey(axisData.positive) || GetKey(axisData.altPositive);
	bool bNegative = GetKey(axisData.negative) || GetKey(axisData.altNegative);

	if (bPositive && bNegative)
	{
		return 0.f;
	}
	else if (bPositive)
	{
		return 1.f;
	}
	else if (bNegative)
	{
		return -1.f;
	}

	return 0.f;
}

void Input::Update()
{
	mPreviouslyPressedKey = mCurrentlyPressedKey;
	mCurrentlyPressedKey.clear();
}

bool Input::getKeyWasDowned(eEKeyCode inKey)
{
	return mPreviouslyPressedKey.find(inKey) != mPreviouslyPressedKey.end();
}
