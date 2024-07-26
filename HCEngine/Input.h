#pragma once

namespace HC
{
	enum class EKeyCode
	{
		NONE = 0,
		A = 0x41,
		B = 0x42,
		C = 0x43,
		D = 0x44,
		E = 0x45,
		F = 0x46,
		G = 0x47,
		H = 0x48,
		I = 0x49,
		J = 0x4A,
		K = 0x4B,
		L = 0x4C,
		M = 0x4D,
		N = 0x4E,
		O = 0x4F,
		P = 0x50,
		Q = 0x51,
		R = 0x52,
		S = 0x53,
		T = 0x54,
		U = 0x55,
		V = 0x56,
		W = 0x57,
		X = 0x58,
		Y = 0x59,
		Z = 0x5A,
		F1 = VK_F1,
		F2 = VK_F2,
		F3 = VK_F3,
		F4 = VK_F4,
		F5 = VK_F5,
		F6 = VK_F6,
		F7 = VK_F7,
		F8 = VK_F8,
		F9 = VK_F9,
		F10 = VK_F10,
		F11 = VK_F11,
		F12 = VK_F12,
		ESC = VK_ESCAPE,
		ALPHA_0 = 0x30,
		ALPHA_1 = 0x31,
		ALPHA_2 = 0x32,
		ALPHA_3 = 0x33,
		ALPHA_4 = 0x34,
		ALPHA_5 = 0x35,
		ALPHA_6 = 0x36,
		ALPHA_7 = 0x37,
		ALPHA_8 = 0x38,
		ALPHA_9 = 0x39,
		NUM_0 = VK_NUMPAD0,
		NUM_1 = VK_NUMPAD1,
		NUM_2 = VK_NUMPAD2,
		NUM_3 = VK_NUMPAD3,
		NUM_4 = VK_NUMPAD4,
		NUM_5 = VK_NUMPAD5,
		NUM_6 = VK_NUMPAD6,
		NUM_7 = VK_NUMPAD7,
		NUM_8 = VK_NUMPAD8,
		NUM_9 = VK_NUMPAD9,
		SPACE = VK_SPACE,
		BACKSPACE = VK_BACK,
		TAB = VK_TAB,
		ENTER = VK_RETURN,
		LEFT_ARROW = VK_LEFT,
		RIGHT_ARROW = VK_RIGHT,
		UP_ARROW = VK_UP,
		DOWN_ARROW = VK_DOWN,
	};

	enum class EAxis
	{
		NONE,
		VERTICAL,
		HORIZONTAL,
		AXIS_1,
		AXIS_2,
		AXIS_3,
		AXIS_4,
	};

	class Input
	{
	public:
		bool GetKeyDown(EKeyCode InKey);
		bool GetKey(EKeyCode InKey);
		bool GetKeyUp(EKeyCode InKey);
		float GetAxis(EAxis InAxis);
		void Update();

	private:
		std::unordered_set<EKeyCode> CurrentlyPressedKey;
		std::unordered_set<EKeyCode> PreviouslyPressedKey;

		bool GetKeyWasDowned(EKeyCode InKey);
	};
}
