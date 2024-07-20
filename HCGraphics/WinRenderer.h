#pragma once

namespace HC
{
	class WinRenderer
	{
	public:
		WinRenderer() = default;
		~WinRenderer();

		bool Initialize(int32 InWidth, int32 InHeight);
		void Release();

		void Resize(int32 InWidth, int32 InHeight);

		FORCEINLINE void SetPixel(int32 InX, int32 InY, const Color& InColor);
		void SwapBuffer();
		void FillBuffer();

		void DrawLine(const Vector2& InStartPos, const Vector2& InEndPos, const Color& InColor);
		void DrawLine(const Vector2& InStartPos, const Vector2& InEndPos, const std::function<Color(float X, float Y)> InColorFunc);
		void DrawPoint(const Vector2& InPos, const Color InColor);
		void DrawTriangle(const Vertex& InVertex1, const Vertex& InVertex2, const Vertex& InVertex3, const Color InColor);

	private:
		HWND Handle;
		HDC ScreenDC, MemDC;
		HBITMAP OriginalBitmap, MemBitmap;
		Color32* ScreenBuffer;

		int32 Width, Height;

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

	FORCEINLINE void WinRenderer::SetPixel(int32 InX, int32 InY, const Color& InColor)
	{
		if (!IsInScreen(InX, InY))
		{
			return;
		}

		int Index = InY * Width + InX;
		ScreenBuffer[Index] = InColor.ToColor32();
	}

	FORCEINLINE bool WinRenderer::IsInScreen(const int32 InX, const int32 InY) const
	{
		if ((InX < 0 || InX >= Width) || (InY < 0 || InY >= Height))
		{
			return false;
		}

		return true;
	}
}