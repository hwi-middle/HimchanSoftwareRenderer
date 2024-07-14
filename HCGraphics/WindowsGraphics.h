#pragma once

namespace HC
{
	class WindowsGraphics
	{
	public:
		WindowsGraphics() = default;
		~WindowsGraphics();

		bool Initialize(uint32 InWidth, uint32 InHeight);
		void Release();

		FORCEINLINE void SetPixel(int32 InX, int32 InY, const Color& InColor);
		void SwapBuffers();

	private:
		HWND Handle;
		HDC ScreenDC, MemDC;
		HBITMAP OriginalBitmap, MemBitmap;
		Color32* ScreenBuffer;

		uint32 Width, Height;

		FORCEINLINE bool IsInScreen(const int32 InX, const int32 InY) const;
	};

	void WindowsGraphics::SetPixel(int32 InX, int32 InY, const Color& InColor)
	{
		if (!IsInScreen(InX, InY))
		{
			return;
		}

		int Index = InY * Width + InX;
		ScreenBuffer[Index] = InColor.ToColor32();
	}

	bool WindowsGraphics::IsInScreen(const int32 InX, const int32 InY) const
	{
		if ((InX < 0 || InX >= Width) || (InY < 0 || InY >= Height))
		{
			return false;
		}

		return true;
	}
}