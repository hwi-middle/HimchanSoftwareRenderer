#pragma once

namespace HC
{
	class WinRenderer
	{
	public:
		WinRenderer() = default;
		~WinRenderer();

		bool initialize(int32 inWidth, int32 inHeight);
		void Release();

		void Resize(int32 inWidth, int32 inHeight);

		void SwapBuffer();
		void FillBuffer();

		void DrawLine(const Vector2& inStartPos, const Vector2& inEndPos, const Color& inColor);
		void DrawPoint(const Vector2& inPos, const Color inColor);
		void DrawTriangle(const Vertex& inVertex1, const Vertex& inVertex2, const Vertex& inVertex3, const Color inColor);

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


		FORCEINLINE bool isinScreen(const int32 inX, const int32 inY) const;
		FORCEINLINE void setPixel(int32 inX, int32 inY, const Color& inColor);
		FORCEINLINE void setPixel(int32 inX, int32 inY, const Color32& inColor);

		enum eEViewportRegion : uint8
		{
			INSIDE_VIEWPORT = 0b0000,
			LEFT = 0b0001,
			RIGHT = 0b0010,
			BOTTOM = 0b0100,
			TOP = 0b1000
		};

		bool clipLine(Vector2& inOutStartPos, Vector2& inOutEndPos, const Vector2& inMinPos, const Vector2& inMaxPos);
		eEViewportRegion computeViewportRegion(const Vector2& inPos, const Vector2& inMinPos, const Vector2& inMaxPos);

		void initTextureBuffer();
		Color32 sampleTexture(const Vector2& inUv) const;
		Color32 sampleTexture(const Vertex& inVertex) const;
	};

	FORCEINLINE void WinRenderer::setPixel(int32 inX, int32 inY, const Color& inColor)
	{
		if (!isinScreen(inX, inY))
		{
			return;
		}

		int index = inY * mWidth + inX;
		mScreenBuffer[index] = inColor.ToColor32();
	}

	FORCEINLINE void WinRenderer::setPixel(int32 inX, int32 inY, const Color32& inColor)
	{
		if (!isinScreen(inX, inY))
		{
			return;
		}

		int index = inY * mWidth + inX;
		mScreenBuffer[index] = inColor;
	}

	FORCEINLINE bool WinRenderer::isinScreen(const int32 inX, const int32 inY) const
	{
		if ((inX < 0 || inX >= mWidth) || (inY < 0 || inY >= mHeight))
		{
			return false;
		}

		return true;
	}
}