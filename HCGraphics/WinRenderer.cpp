#include "pch.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace HC;

WinRenderer::~WinRenderer()
{
}

bool WinRenderer::initialize(int32 inWidth, int32 inHeight)
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
	initialize(inWidth, inHeight);
}

void WinRenderer::SwapBuffer()
{
	::BitBlt(mScreenDc, 0, 0, mWidth, mHeight, mMemDc, 0, 0, SRCCOPY);
}

void WinRenderer::FillBuffer()
{
	memset(mScreenBuffer, 180, mWidth * mHeight * sizeof(Color32));
}

void WinRenderer::DrawLine(const Vector2& inStartPos, const Vector2& inEndPos, const Color& inColor)
{
	Vector2 screenSize = Vector2(mWidth, mHeight);
	Vector2 halfScreen = screenSize * 0.5f;
	Vector2 minScreen = -halfScreen;
	Vector2 maxScreen = halfScreen;
	Vector2 clippedStartPos = inStartPos;
	Vector2 clippedEndPos = inEndPos;

	if (!clipLine(clippedStartPos, clippedEndPos, minScreen, maxScreen))
	{
		return;
	}

	Vector2 startPosScreen = ScreenPoint::CartesianToScreen(clippedStartPos, screenSize.x, screenSize.y);
	Vector2 endPosScreen = ScreenPoint::CartesianToScreen(clippedEndPos, screenSize.x, screenSize.y);

	int width = endPosScreen.x - startPosScreen.x;
	int height = endPosScreen.y - startPosScreen.y;

	bool bIsGradualScope = Math::Abs(width) >= Math::Abs(height);
	int deltaX = (width >= 0) ? 1 : -1;
	int deltaY = (height >= 0) ? 1 : -1;
	int fw = deltaX * width;
	int fh = deltaY * height;

	int discriminant = bIsGradualScope ? fh * 2 - fw : 2 * fw - fh;
	int deltaWhenDiscriminantIsNegative = bIsGradualScope ? 2 * fh : 2 * fw;
	int deltaWhenDiscriminantIsPositive = bIsGradualScope ? 2 * (fh - fw) : 2 * (fw - fh);

	int x = startPosScreen.x;
	int y = startPosScreen.y;

	if (bIsGradualScope)
	{
		while (x != endPosScreen.x)
		{
			setPixel(x, y, inColor);

			if (discriminant < 0)
			{
				discriminant += deltaWhenDiscriminantIsNegative;
			}
			else
			{
				discriminant += deltaWhenDiscriminantIsPositive;
				y += deltaY;
			}

			x += deltaX;
		}
	}
	else
	{
		while (y != endPosScreen.y)
		{
			setPixel(x, y, inColor);

			if (discriminant < 0)
			{
				discriminant += deltaWhenDiscriminantIsNegative;
			}
			else
			{
				discriminant += deltaWhenDiscriminantIsPositive;
				x += deltaX;
			}

			y += deltaY;
		}
	}
}

void WinRenderer::DrawPoint(const Vector2& inPos, const Color inColor)
{
	Vector2 res = ScreenPoint::CartesianToScreen(Vector2(inPos.x, inPos.y), mWidth, mHeight);
	setPixel(res.x, res.y, inColor);
}

void WinRenderer::DrawTriangle(const Vertex& inVertex1, const Vertex& inVertex2, const Vertex& inVertex3, const Color inColor)
{
	std::array<Vertex, 3> vertices =
	{
		inVertex1,
		inVertex2,
		inVertex3
	};
	std::for_each(vertices.begin(), vertices.end(), [&](Vertex& inVertex) {
		inVertex.position = Vector4(ScreenPoint::CartesianToScreen(inVertex.position.ToVector2(), mWidth, mHeight), 0, 0);
		});
	std::sort(vertices.begin(), vertices.end(), [](const Vertex& inLhs, const Vertex& inRhs) { return inLhs.position.y < inRhs.position.y; });

	const int32 x1 = vertices[0].position.x;
	const int32 y1 = vertices[0].position.y;
	const int32 x2 = vertices[1].position.x;
	const int32 y2 = vertices[1].position.y;
	const int32 x3 = vertices[2].position.x;
	const int32 y3 = vertices[2].position.y;
	const Vector2 uv1 = vertices[0].uv;
	const Vector2 uv2 = vertices[1].uv;
	const Vector2 uv3 = vertices[2].uv;

	// Degenerate triangle
	if ((x2 - x1) * (y3 - y1) == (x3 - x1) * (y2 - y1))
	{
		return;
	}


	if (y1 >= mHeight || y3 < 0)
	{
		return;
	}

	if ((x1 < 0 && x2 < 0 && x3 < 0) ||
		(x1 >= mWidth && x2 >= mWidth && x3 >= mWidth))
	{
		return;
	}


	const float a12 = y1 != y2 ? (x2 - x1) / static_cast<float>(y2 - y1) : 0.f;
	const float a13 = (x3 - x1) / static_cast<float>(y3 - y1);
	const float a23 = y2 != y3 ? (x3 - x2) / static_cast<float>(y3 - y2) : 0.f;

	Vector2 deltaUvStart = (y2 != y1) ? (uv2 - uv1) / static_cast<float>(y2 - y1) : Vector2(0.f, 0.f);
	Vector2 deltaUvEnd = (y3 != y1) ? (uv3 - uv1) / static_cast<float>(y3 - y1) : Vector2(0.f, 0.f);
	Vector2 uvStart = uv1;
	Vector2 uvEnd = uv1;

	for (int y = y1; y < y2; ++y)
	{
		int32 xStart = x1 + a12 * (y - y1);
		int32 xEnd = x1 + a13 * (y - y1);

		bool bIsSwapped = false;
		if (xStart > xEnd)
		{
			std::swap(xStart, xEnd);
			std::swap(uvStart, uvEnd);
			bIsSwapped = true;
		}

		const int32 clipXStart = Math::Clamp(xStart, 0, mWidth - 1);
		const int32 clipXEnd = Math::Clamp(xEnd, 0, mWidth - 1);

		for (int x = clipXStart; x <= clipXEnd; ++x)
		{
			Vector2 uv = Math::Lerp(uvStart, uvEnd, (x - xStart) / static_cast<float>(xEnd - xStart));
			setPixel(x, y, sampleTexture(uv));
		}

		if (bIsSwapped)
		{
			std::swap(uvStart, uvEnd);
		}

		uvStart += deltaUvStart;
		uvEnd += deltaUvEnd;
	}

	deltaUvStart = (y3 != y2) ? (uv3 - uv2) / static_cast<float>(y3 - y2) : Vector2(0, 0);

	// 첫 번째 루프를 건너뛰는 경우가 있으므로 다시 계산
	uvStart = uv2;
	uvEnd = uv1 + deltaUvEnd * (y2 - y1);

	for (int y = y2; y <= y3; ++y)
	{
		int32 xStart = x2 + a23 * static_cast<float>(y - y2);
		int32 xEnd = x1 + a13 * static_cast<float>(y - y1);

		bool bIsSwapped = false;
		if (xStart > xEnd)
		{
			std::swap(xStart, xEnd);
			std::swap(uvStart, uvEnd);
			bIsSwapped = true;
		}

		const int32 clipXStart = Math::Clamp(xStart, 0, mWidth - 1);
		const int32 clipXEnd = Math::Clamp(xEnd, 0, mWidth - 1);

		for (int x = clipXStart; x <= clipXEnd; ++x)
		{
			Vector2 uv = Math::Lerp(uvStart, uvEnd, (x - xStart) / static_cast<float>(xEnd - xStart));
			setPixel(x, y, sampleTexture(uv));
		}

		if (bIsSwapped)
		{
			std::swap(uvStart, uvEnd);
		}

		uvStart += deltaUvStart;
		uvEnd += deltaUvEnd;
	}
}

bool WinRenderer::clipLine(Vector2& inOutStartPos, Vector2& inOutEndPos, const Vector2& inMinPos, const Vector2& inMaxPos)
{
	//std::cout << "line to draw: " << inOutStartPos.ToString() << " to " << inOutEndPos.ToString() << "\n";

	eEViewportRegion startRegion = computeViewportRegion(inOutStartPos, inMinPos, inMaxPos);
	eEViewportRegion endRegion = computeViewportRegion(inOutEndPos, inMinPos, inMaxPos);

	while (true)
	{
		if (!(startRegion | endRegion))
		{
			//std::cout << "no need to clip now: " << inOutStartPos.ToString() << " to " << inOutEndPos.ToString() << "\n";
			return true;
		}
		else if (startRegion & endRegion)
		{
			//std::cout << "whole line is not inside viewport: " << inOutStartPos.ToString() << " to " << inOutEndPos.ToString() << "\n";
			return false;
		}
		else
		{
			//std::cout << "clipped!\n";

			eEViewportRegion regionToClip = Math::Max(startRegion, endRegion);
			Vector2 clippedPos;

			float width = (inOutEndPos.x - inOutStartPos.x);
			float height = (inOutEndPos.y - inOutStartPos.y);

			if (regionToClip & LEFT)
			{
				clippedPos.x = inMinPos.x;
				clippedPos.y = height / width * (clippedPos.x - inOutStartPos.x) + inOutStartPos.y;
			}
			else if (regionToClip & RIGHT)
			{
				clippedPos.x = inMaxPos.x;
				clippedPos.y = height / width * (clippedPos.x - inOutStartPos.x) + inOutStartPos.y;
			}
			else if (regionToClip & TOP)
			{
				clippedPos.x = width / height * (clippedPos.y - inOutStartPos.y) + inOutStartPos.x;
				clippedPos.y = inMaxPos.y;
			}
			else if (regionToClip & BOTTOM)
			{
				clippedPos.x = width / height * (clippedPos.y - inOutStartPos.y) + inOutStartPos.x;
				clippedPos.y = inMinPos.y;
			}

			if (regionToClip == startRegion)
			{
				inOutStartPos = clippedPos;
				startRegion = computeViewportRegion(inOutStartPos, inMinPos, inMaxPos);
			}
			else
			{
				inOutEndPos = clippedPos;
				endRegion = computeViewportRegion(inOutEndPos, inMinPos, inMaxPos);
			}
		}
	}
}

WinRenderer::eEViewportRegion WinRenderer::computeViewportRegion(const Vector2& inPos, const Vector2& inMinPos, const Vector2& inMaxPos)
{
	eEViewportRegion result = eEViewportRegion::INSIDE_VIEWPORT;
	if (inPos.x < inMinPos.x)
	{
		result = static_cast<eEViewportRegion>(result | eEViewportRegion::LEFT);
	}
	else if (inPos.x > inMaxPos.x)
	{
		result = static_cast<eEViewportRegion>(result | eEViewportRegion::RIGHT);
	}

	if (inPos.y < inMinPos.y)
	{
		result = static_cast<eEViewportRegion>(result | eEViewportRegion::BOTTOM);
	}
	else if (inPos.y > inMaxPos.y)
	{
		result = static_cast<eEViewportRegion>(result | eEViewportRegion::TOP);
	}

	return result;
}

void WinRenderer::initTextureBuffer()
{
	FILE* file = nullptr;
	const std::string fileName = "checkerboard.png";
	unsigned char* loadBuffer = stbi_load(fileName.c_str(), &mTexWidth, &mTexHeight, &mTexChannels, 0);

	if (loadBuffer == nullptr)
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
		mTextureBuffer[i].r = loadBuffer[i * mTexChannels];
		mTextureBuffer[i].g = loadBuffer[i * mTexChannels + 1];
		mTextureBuffer[i].b = loadBuffer[i * mTexChannels + 2];
		mTextureBuffer[i].a = 255;
	}

	stbi_image_free(loadBuffer);
}

Color32 WinRenderer::sampleTexture(const Vector2& inUv) const
{
	int X = Math::Clamp(static_cast<int>(inUv.x * mTexWidth + 0.5f), 0, mTexWidth - 1);
	int Y = Math::Clamp(static_cast<int>(inUv.y * mTexHeight + 0.5f), 0, mTexHeight - 1);
	return mTextureBuffer[Y * mTexWidth + X];
}

Color32 WinRenderer::sampleTexture(const Vertex& inVertex) const
{
	return sampleTexture(inVertex.uv);
}

