#pragma once

namespace HC
{
	class WinRenderer
	{
	public:
		WinRenderer() = default;
		~WinRenderer();

		bool Initialize(int32 inWidth, int32 inHeight);
		void Release();

		void Resize(int32 inWidth, int32 inHeight);

		void SwapBuffer();
		void FillBuffer();

		void DrawLine(const Vector2& InStartPos, const Vector2& InEndPos, const Color& InColor);
		void DrawPoint(const Vector2& InPos, const Color InColor);
		void DrawTriangle(const Vertex& InVertex1, const Vertex& InVertex2, const Vertex& InVertex3, const Color InColor);

	private:
		HWND mHandle;
		HDC mScreenDc;
		HDC mMemDc;
		HBITMAP mOriginalBitmap;
		HBITMAP mMemBitmap;
		Color32* mScreenBuffer;

		int32 mWidth;
		int32 mHeight;

		Color32* mTextureBuffer;
		int mTexWidth;
		int mTexHeight;
		int mTexChannels;


		FORCEINLINE bool isInScreen(const int32 InX, const int32 InY) const;
		FORCEINLINE void setPixel(int32 InX, int32 InY, const Color& InColor);
		FORCEINLINE void setPixel(int32 InX, int32 InY, const Color32& InColor);

		enum eEViewportRegion : uint8
		{
			INSIDE_VIEWPORT = 0b0000,
			LEFT = 0b0001,
			RIGHT = 0b0010,
			BOTTOM = 0b0100,
			TOP = 0b1000
		};

		bool clipLine(Vector2& InOutStartPos, Vector2& InOutEndPos, const Vector2& InMinPos, const Vector2& InMaxPos);
		eEViewportRegion ComputeViewportRegion(const Vector2& InPos, const Vector2& InMinPos, const Vector2& InMaxPos);

		void initTextureBuffer();
		Color32 sampleTexture(const Vector2& InUV) const;
		Color32 sampleTexture(const Vertex& InVertex) const;
	};

	FORCEINLINE void WinRenderer::setPixel(int32 InX, int32 InY, const Color& InColor)
	{
		if (!isInScreen(InX, InY))
		{
			return;
		}

		int Index = InY * mWidth + InX;
		mScreenBuffer[Index] = InColor.ToColor32();
	}

	FORCEINLINE void WinRenderer::setPixel(int32 InX, int32 InY, const Color32& InColor)
	{
		if (!isInScreen(InX, InY))
		{
			return;
		}

		int Index = InY * mWidth + InX;
		mScreenBuffer[Index] = InColor;
	}

	FORCEINLINE bool WinRenderer::isInScreen(const int32 InX, const int32 InY) const
	{
		if ((InX < 0 || InX >= mWidth) || (InY < 0 || InY >= mHeight))
		{
			return false;
		}

		return true;
	}
}