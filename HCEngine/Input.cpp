#include "pch.h"

constexpr int PRESS = 0x8000;

const std::unordered_map<EAxis, AxisData> Input::AxisMap =
{
	{ 
		EAxis::HORIZONTAL,
		{ 
			EKeyCode::RIGHT_ARROW, 
			EKeyCode::LEFT_ARROW, 
			EKeyCode::D, 
			EKeyCode::A
		} 
	},
	{
		EAxis::VERTICAL,
		{
			EKeyCode::UP_ARROW,
			EKeyCode::DOWN_ARROW,
			EKeyCode::W,
			EKeyCode::S
		}
	}
};

bool Input::GetKeyDown(EKeyCode InKey)
{
	int CurrentState = GetAsyncKeyState(static_cast<int>(InKey));
	bool bIsDown = (CurrentState & PRESS) != 0;
	bool bWasDown = GetKeyWasDowned(InKey);
	if (bIsDown)
	{
		CurrentlyPressedKey.insert(InKey);
	}

	return bIsDown && !bWasDown;
}

bool Input::GetKey(EKeyCode InKey)
{
	int CurrentState = GetAsyncKeyState(static_cast<int>(InKey));
	bool bIsDown = (CurrentState & PRESS) != 0;
	if (bIsDown)
	{
		CurrentlyPressedKey.insert(InKey);
	}

	return bIsDown;
}

bool Input::GetKeyUp(EKeyCode InKey)
{
	int CurrentState = GetAsyncKeyState(static_cast<int>(InKey));
	bool bIsUp = (CurrentState & PRESS) == 0;
	bool bWasDown = GetKeyWasDowned(InKey);

	return bIsUp && bWasDown;
}

float Input::GetAxis(EAxis InAxis)
{
	const AxisData& AxisData = AxisMap.at(InAxis);
	bool bPositive = GetKey(AxisData.Positive) || GetKey(AxisData.AltPositive);
	bool bNegative = GetKey(AxisData.Negative) || GetKey(AxisData.AltNegative);

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
	PreviouslyPressedKey = CurrentlyPressedKey;
	CurrentlyPressedKey.clear();
}

bool Input::GetKeyWasDowned(EKeyCode InKey)
{
	return PreviouslyPressedKey.find(InKey) != PreviouslyPressedKey.end();
}
