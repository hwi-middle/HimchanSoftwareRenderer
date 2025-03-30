#include "pch.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace HC;

WinRenderer::~WinRenderer()
{
}

bool WinRenderer::Initialize(int32 inWidth, int32 inHeight)
{
	mWidth = inWidth;
	mHeight = inHeight;

	mHandle = ::GetActiveWindow();
	mScreenDc = ::GetDC(mHandle);
	mMemDc = ::CreateCompatibleDC(mScreenDc);

	BITMAPINFO bmi;
	memset(&bmi, 0, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = mWidth;
	bmi.bmiHeader.biHeight = -mHeight;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;

	mMemBitmap = ::CreateDIBSection(mMemDc, &bmi, DIB_RGB_COLORS, (void**)&mScreenBuffer, NULL, 0);
	if (mMemBitmap == NULL)
	{
		Release();
		return false;
	}

	mOriginalBitmap = static_cast<HBITMAP>(::SelectObject(mMemDc, mMemBitmap));
	if (mOriginalBitmap == NULL)
	{
		Release();
		return false;
	}

	initTextureBuffer();

	return true;
}

void WinRenderer::Release()
{
	::DeleteObject(mOriginalBitmap);
	::DeleteObject(mMemBitmap);
	::DeleteDC(mMemDc);
	::DeleteDC(mScreenDc);
}

void WinRenderer::Resize(int32 inWidth, int32 inHeight)
{
	Release();
	Initialize(inWidth, inHeight);
}

void WinRenderer::SwapBuffer()
{
	::BitBlt(mScreenDc, 0, 0, mWidth, mHeight, mMemDc, 0, 0, SRCCOPY);
}

void WinRenderer::FillBuffer()
{
	memset(mScreenBuffer, 180, mWidth * mHeight * sizeof(Color32));
}

void WinRenderer::DrawLine(const Vector2& InStartPos, const Vector2& InEndPos, const Color& InColor)
{
	Vector2 ScreenSize = Vector2(mWidth, mHeight);
	Vector2 HalfScreen = ScreenSize * 0.5f;
	Vector2 MinScreen = -HalfScreen;
	Vector2 MaxScreen = HalfScreen;
	Vector2 ClippedStartPos = InStartPos;
	Vector2 ClippedEndPos = InEndPos;

	if (!clipLine(ClippedStartPos, ClippedEndPos, MinScreen, MaxScreen))
	{
		return;
	}

	Vector2 StartPosScreen = ScreenPoint::CartesianToScreen(ClippedStartPos, ScreenSize.x, ScreenSize.y);
	Vector2 EndPosScreen = ScreenPoint::CartesianToScreen(ClippedEndPos, ScreenSize.x, ScreenSize.y);

	int Width = EndPosScreen.x - StartPosScreen.x;
	int Height = EndPosScreen.y - StartPosScreen.y;

	bool bIsGradualScope = Math::Abs(Width) >= Math::Abs(Height);
	int DeltaX = (Width >= 0) ? 1 : -1;
	int DeltaY = (Height >= 0) ? 1 : -1;
	int Fw = DeltaX * Width;
	int Fh = DeltaY * Height;

	int Discriminant = bIsGradualScope ? Fh * 2 - Fw : 2 * Fw - Fh;
	int DeltaWhenDiscriminantIsNegative = bIsGradualScope ? 2 * Fh : 2 * Fw;
	int DeltaWhenDiscriminantIsPositive = bIsGradualScope ? 2 * (Fh - Fw) : 2 * (Fw - Fh);

	int X = StartPosScreen.x;
	int Y = StartPosScreen.y;

	if (bIsGradualScope)
	{
		while (X != EndPosScreen.x)
		{
			setPixel(X, Y, InColor);

			if (Discriminant < 0)
			{
				Discriminant += DeltaWhenDiscriminantIsNegative;
			}
			else
			{
				Discriminant += DeltaWhenDiscriminantIsPositive;
				Y += DeltaY;
			}

			X += DeltaX;
		}
	}
	else
	{
		while (Y != EndPosScreen.y)
		{
			setPixel(X, Y, InColor);

			if (Discriminant < 0)
			{
				Discriminant += DeltaWhenDiscriminantIsNegative;
			}
			else
			{
				Discriminant += DeltaWhenDiscriminantIsPositive;
				X += DeltaX;
			}

			Y += DeltaY;
		}
	}
}

void WinRenderer::DrawPoint(const Vector2& InPos, const Color InColor)
{
	Vector2 res = ScreenPoint::CartesianToScreen(Vector2(InPos.x, InPos.y), mWidth, mHeight);
	setPixel(res.x, res.y, InColor);
}

void WinRenderer::DrawTriangle(const Vertex& InVertex1, const Vertex& InVertex2, const Vertex& InVertex3, const Color InColor)
{
	std::array<Vertex, 3> Vertices =
	{
		InVertex1,
		InVertex2,
		InVertex3
	};
	std::for_each(Vertices.begin(), Vertices.end(), [&](Vertex& InVertex) {
		InVertex.Position = Vector4(ScreenPoint::CartesianToScreen(InVertex.Position.ToVector2(), mWidth, mHeight), 0, 0);
		});
	std::sort(Vertices.begin(), Vertices.end(), [](const Vertex& InLhs, const Vertex& InRhs) { return InLhs.Position.y < InRhs.Position.y; });

	const int32 X1 = Vertices[0].Position.x;
	const int32 Y1 = Vertices[0].Position.y;
	const int32 X2 = Vertices[1].Position.x;
	const int32 Y2 = Vertices[1].Position.y;
	const int32 X3 = Vertices[2].Position.x;
	const int32 Y3 = Vertices[2].Position.y;
	const Vector2 UV1 = Vertices[0].UV;
	const Vector2 UV2 = Vertices[1].UV;
	const Vector2 UV3 = Vertices[2].UV;

	// Degenerate triangle
	if ((X2 - X1) * (Y3 - Y1) == (X3 - X1) * (Y2 - Y1))
	{
		return;
	}


	if (Y1 >= mHeight || Y3 < 0)
	{
		return;
	}

	if ((X1 < 0 && X2 < 0 && X3 < 0) ||
		(X1 >= mWidth && X2 >= mWidth && X3 >= mWidth))
	{
		return;
	}


	const float A12 = Y1 != Y2 ? (X2 - X1) / static_cast<float>(Y2 - Y1) : 0.f;
	const float A13 = (X3 - X1) / static_cast<float>(Y3 - Y1);
	const float A23 = Y2 != Y3 ? (X3 - X2) / static_cast<float>(Y3 - Y2) : 0.f;

	Vector2 DeltaUvStart = (Y2 != Y1) ? (UV2 - UV1) / static_cast<float>(Y2 - Y1) : Vector2(0.f, 0.f);
	Vector2 DeltaUvEnd = (Y3 != Y1) ? (UV3 - UV1) / static_cast<float>(Y3 - Y1) : Vector2(0.f, 0.f);
	Vector2 UvStart = UV1;
	Vector2 UvEnd = UV1;

	for (int Y = Y1; Y < Y2; ++Y)
	{
		int32 XStart = X1 + A12 * (Y - Y1);
		int32 XEnd = X1 + A13 * (Y - Y1);

		bool bIsSwapped = false;
		if (XStart > XEnd)
		{
			std::swap(XStart, XEnd);
			std::swap(UvStart, UvEnd);
			bIsSwapped = true;
		}

		const int32 ClipXStart = Math::Clamp(XStart, 0, mWidth - 1);
		const int32 ClipXEnd = Math::Clamp(XEnd, 0, mWidth - 1);

		for (int X = ClipXStart; X <= ClipXEnd; ++X)
		{
			Vector2 UV = Math::Lerp(UvStart, UvEnd, (X - XStart) / static_cast<float>(XEnd - XStart));
			setPixel(X, Y, sampleTexture(UV));
		}

		if (bIsSwapped)
		{
			std::swap(UvStart, UvEnd);
		}

		UvStart += DeltaUvStart;
		UvEnd += DeltaUvEnd;
	}

	DeltaUvStart = (Y3 != Y2) ? (UV3 - UV2) / static_cast<float>(Y3 - Y2) : Vector2(0, 0);

	// 첫 번째 루프를 건너뛰는 경우가 있으므로 다시 계산
	UvStart = UV2;
	UvEnd = UV1 + DeltaUvEnd * (Y2 - Y1);

	for (int Y = Y2; Y <= Y3; ++Y)
	{
		int32 XStart = X2 + A23 * static_cast<float>(Y - Y2);
		int32 XEnd = X1 + A13 * static_cast<float>(Y - Y1);

		bool bIsSwapped = false;
		if (XStart > XEnd)
		{
			std::swap(XStart, XEnd);
			std::swap(UvStart, UvEnd);
			bIsSwapped = true;
		}

		const int32 ClipXStart = Math::Clamp(XStart, 0, mWidth - 1);
		const int32 ClipXEnd = Math::Clamp(XEnd, 0, mWidth - 1);

		for (int X = ClipXStart; X <= ClipXEnd; ++X)
		{
			Vector2 UV = Math::Lerp(UvStart, UvEnd, (X - XStart) / static_cast<float>(XEnd - XStart));
			setPixel(X, Y, sampleTexture(UV));
		}

		if (bIsSwapped)
		{
			std::swap(UvStart, UvEnd);
		}

		UvStart += DeltaUvStart;
		UvEnd += DeltaUvEnd;
	}
}

bool WinRenderer::clipLine(Vector2& InOutStartPos, Vector2& InOutEndPos, const Vector2& InMinPos, const Vector2& InMaxPos)
{
	//std::cout << "line to draw: " << InOutStartPos.ToString() << " to " << InOutEndPos.ToString() << "\n";

	eEViewportRegion StartRegion = ComputeViewportRegion(InOutStartPos, InMinPos, InMaxPos);
	eEViewportRegion EndRegion = ComputeViewportRegion(InOutEndPos, InMinPos, InMaxPos);

	while (true)
	{
		if (!(StartRegion | EndRegion))
		{
			//std::cout << "no need to clip now: " << InOutStartPos.ToString() << " to " << InOutEndPos.ToString() << "\n";
			return true;
		}
		else if (StartRegion & EndRegion)
		{
			//std::cout << "whole line is not inside viewport: " << InOutStartPos.ToString() << " to " << InOutEndPos.ToString() << "\n";
			return false;
		}
		else
		{
			//std::cout << "clipped!\n";

			eEViewportRegion RegionToClip = Math::Max(StartRegion, EndRegion);
			Vector2 ClippedPos;

			float Width = (InOutEndPos.x - InOutStartPos.x);
			float Height = (InOutEndPos.y - InOutStartPos.y);

			if (RegionToClip & LEFT)
			{
				ClippedPos.x = InMinPos.x;
				ClippedPos.y = Height / Width * (ClippedPos.x - InOutStartPos.x) + InOutStartPos.y;
			}
			else if (RegionToClip & RIGHT)
			{
				ClippedPos.x = InMaxPos.x;
				ClippedPos.y = Height / Width * (ClippedPos.x - InOutStartPos.x) + InOutStartPos.y;
			}
			else if (RegionToClip & TOP)
			{
				ClippedPos.x = Width / Height * (ClippedPos.y - InOutStartPos.y) + InOutStartPos.x;
				ClippedPos.y = InMaxPos.y;
			}
			else if (RegionToClip & BOTTOM)
			{
				ClippedPos.x = Width / Height * (ClippedPos.y - InOutStartPos.y) + InOutStartPos.x;
				ClippedPos.y = InMinPos.y;
			}

			if (RegionToClip == StartRegion)
			{
				InOutStartPos = ClippedPos;
				StartRegion = ComputeViewportRegion(InOutStartPos, InMinPos, InMaxPos);
			}
			else
			{
				InOutEndPos = ClippedPos;
				EndRegion = ComputeViewportRegion(InOutEndPos, InMinPos, InMaxPos);
			}
		}
	}
}

WinRenderer::eEViewportRegion WinRenderer::ComputeViewportRegion(const Vector2& InPos, const Vector2& InMinPos, const Vector2& InMaxPos)
{
	eEViewportRegion Result = eEViewportRegion::INSIDE_VIEWPORT;
	if (InPos.x < InMinPos.x)
	{
		Result = static_cast<eEViewportRegion>(Result | eEViewportRegion::LEFT);
	}
	else if (InPos.x > InMaxPos.x)
	{
		Result = static_cast<eEViewportRegion>(Result | eEViewportRegion::RIGHT);
	}

	if (InPos.y < InMinPos.y)
	{
		Result = static_cast<eEViewportRegion>(Result | eEViewportRegion::BOTTOM);
	}
	else if (InPos.y > InMaxPos.y)
	{
		Result = static_cast<eEViewportRegion>(Result | eEViewportRegion::TOP);
	}

	return Result;
}

void WinRenderer::initTextureBuffer()
{
	FILE* File = nullptr;
	const std::string FileName = "checkerboard.png";
	unsigned char* LoadBuffer = stbi_load(FileName.c_str(), &mTexWidth, &mTexHeight, &mTexChannels, 0);

	if (LoadBuffer == nullptr)
	{
		std::cout << "Failed to load texture\n";
		if (stbi_failure_reason())
		{
			std::cout << stbi_failure_reason();
		}

		return;
	}

	std::cout << "Texture loaded: " << mTexWidth << "x" << mTexHeight << " " << mTexChannels << " channels\n";
	mTextureBuffer = new Color32[mTexWidth * mTexHeight];
	for (int i = 0; i < mTexWidth * mTexHeight; ++i)
	{
		mTextureBuffer[i].R = LoadBuffer[i * mTexChannels];
		mTextureBuffer[i].G = LoadBuffer[i * mTexChannels + 1];
		mTextureBuffer[i].B = LoadBuffer[i * mTexChannels + 2];
		mTextureBuffer[i].A = 255;
	}

	stbi_image_free(LoadBuffer);
}

Color32 WinRenderer::sampleTexture(const Vector2& InUV) const
{
	int X = Math::Clamp(static_cast<int>(InUV.x * mTexWidth + 0.5f), 0, mTexWidth - 1);
	int Y = Math::Clamp(static_cast<int>(InUV.y * mTexHeight + 0.5f), 0, mTexHeight - 1);
	return mTextureBuffer[Y * mTexWidth + X];
}

Color32 WinRenderer::sampleTexture(const Vertex& InVertex) const
{
	return sampleTexture(InVertex.UV);
}

