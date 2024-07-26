#include "pch.h"

constexpr int PRESS = 0x8000;

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
	return 0.0f;
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
