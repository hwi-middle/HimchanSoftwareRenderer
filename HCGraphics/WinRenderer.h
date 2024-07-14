#pragma once

namespace HC
{
	class WinRenderer
	{
	public:
		WinRenderer() = default;
		~WinRenderer();

		bool Initialize(uint32 InWidth, uint32 InHeight);
		void Release();

		FORCEINLINE void SetPixel(int32 InX, int32 InY, const Color& InColor);
		void SwapBuffer();
		void FillBuffer();

		void DrawLine(const Vector2& InScreenSize, const Vector2& InStartPos, const Vector2& InEndPos, const Color InColor);

	private:
		HWND Handle;
		HDC ScreenDC, MemDC;
		HBITMAP OriginalBitmap, MemBitmap;
		Color32* ScreenBuffer;

		uint32 Width, Height;

		FORCEINLINE bool IsInScreen(const int32 InX, const int32 InY) const;

		enum EViewportRegion : uint8
		{
			INSIDE_VIEWPORT = 0b0000,
			LEFT = 0b0001,
			RIGHT = 0b0010,
			BOTTOM = 0b0100,
			TOP = 0b1000
		};

		bool ClipLine(Vector2& InOutStartPos, Vector2& InOutEndPos, const Vector2& InMinPos, const Vector2& InMaxPos);
		EViewportRegion ComputeViewportRegion(const Vector2& InPos, const Vector2& InMinPos, const Vector2& InMaxPos);
	};

	void WinRenderer::SetPixel(int32 InX, int32 InY, const Color& InColor)
	{
		if (!IsInScreen(InX, InY))
		{
			return;
		}

		int Index = InY * Width + InX;
		ScreenBuffer[Index] = InColor.ToColor32();
	}

	bool WinRenderer::IsInScreen(const int32 InX, const int32 InY) const
	{
		if ((InX < 0 || InX >= Width) || (InY < 0 || InY >= Height))
		{
			return false;
		}

		return true;
	}
}