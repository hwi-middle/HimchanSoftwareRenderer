﻿#include "pch.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace HC;

WinRenderer::~WinRenderer()
{
}

bool WinRenderer::Initialize(int32 InWidth, int32 InHeight)
{
	Width = InWidth;
	Height = InHeight;

	Handle = ::GetActiveWindow();
	ScreenDC = ::GetDC(Handle);
	MemDC = ::CreateCompatibleDC(ScreenDC);

	BITMAPINFO bmi;
	memset(&bmi, 0, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = Width;
	bmi.bmiHeader.biHeight = -Height;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;

	MemBitmap = ::CreateDIBSection(MemDC, &bmi, DIB_RGB_COLORS, (void**)&ScreenBuffer, NULL, 0);
	if (MemBitmap == NULL)
	{
		Release();
		return false;
	}

	OriginalBitmap = (HBITMAP)::SelectObject(MemDC, MemBitmap);
	if (OriginalBitmap == NULL)
	{
		Release();
		return false;
	}

	InitTextureBuffer();

	return true;
}

void WinRenderer::Release()
{
	::DeleteObject(OriginalBitmap);
	::DeleteObject(MemBitmap);
	::DeleteDC(MemDC);
	::DeleteDC(ScreenDC);
}

void WinRenderer::Resize(int32 InWidth, int32 InHeight)
{
	Release();
	Initialize(InWidth, InHeight);
}

void WinRenderer::SwapBuffer()
{
	::BitBlt(ScreenDC, 0, 0, Width, Height, MemDC, 0, 0, SRCCOPY);
}

void WinRenderer::FillBuffer()
{
	memset(ScreenBuffer, 180, Width * Height * sizeof(Color32));
}

void WinRenderer::DrawLine(const Vector2& InStartPos, const Vector2& InEndPos, const Color& InColor)
{
	Vector2 ScreenSize = Vector2(Width, Height);
	Vector2 HalfScreen = ScreenSize * 0.5f;
	Vector2 MinScreen = -HalfScreen;
	Vector2 MaxScreen = HalfScreen;
	Vector2 ClippedStartPos = InStartPos;
	Vector2 ClippedEndPos = InEndPos;

	if (!ClipLine(ClippedStartPos, ClippedEndPos, MinScreen, MaxScreen))
	{
		return;
	}

	Vector2 StartPosScreen = ScreenPoint::CartesianToScreen(ClippedStartPos, ScreenSize.X, ScreenSize.Y);
	Vector2 EndPosScreen = ScreenPoint::CartesianToScreen(ClippedEndPos, ScreenSize.X, ScreenSize.Y);

	int Width = EndPosScreen.X - StartPosScreen.X;
	int Height = EndPosScreen.Y - StartPosScreen.Y;

	bool bIsGradualScope = Math::Abs(Width) >= Math::Abs(Height);
	int DeltaX = (Width >= 0) ? 1 : -1;
	int DeltaY = (Height >= 0) ? 1 : -1;
	int Fw = DeltaX * Width;
	int Fh = DeltaY * Height;

	int Discriminant = bIsGradualScope ? Fh * 2 - Fw : 2 * Fw - Fh;
	int DeltaWhenDiscriminantIsNegative = bIsGradualScope ? 2 * Fh : 2 * Fw;
	int DeltaWhenDiscriminantIsPositive = bIsGradualScope ? 2 * (Fh - Fw) : 2 * (Fw - Fh);

	int X = StartPosScreen.X;
	int Y = StartPosScreen.Y;

	if (bIsGradualScope)
	{
		while (X != EndPosScreen.X)
		{
			SetPixel(X, Y, InColor);

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
		while (Y != EndPosScreen.Y)
		{
			SetPixel(X, Y, InColor);

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
	Vector2 res = ScreenPoint::CartesianToScreen(Vector2(InPos.X, InPos.Y), Width, Height);
	SetPixel(res.X, res.Y, InColor);
}

void WinRenderer::DrawTriangle(const Vertex& InVertex1, const Vertex& InVertex2, const Vertex& InVertex3, const Color InColor)
{
	return;
}

bool WinRenderer::ClipLine(Vector2& InOutStartPos, Vector2& InOutEndPos, const Vector2& InMinPos, const Vector2& InMaxPos)
{
	//std::cout << "line to draw: " << InOutStartPos.ToString() << " to " << InOutEndPos.ToString() << "\n";

	EViewportRegion StartRegion = ComputeViewportRegion(InOutStartPos, InMinPos, InMaxPos);
	EViewportRegion EndRegion = ComputeViewportRegion(InOutEndPos, InMinPos, InMaxPos);

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

			EViewportRegion RegionToClip = Math::Max(StartRegion, EndRegion);
			Vector2 ClippedPos;

			float Width = (InOutEndPos.X - InOutStartPos.X);
			float Height = (InOutEndPos.Y - InOutStartPos.Y);

			if (RegionToClip & LEFT)
			{
				ClippedPos.X = InMinPos.X;
				ClippedPos.Y = Height / Width * (ClippedPos.X - InOutStartPos.X) + InOutStartPos.Y;
			}
			else if (RegionToClip & RIGHT)
			{
				ClippedPos.X = InMaxPos.X;
				ClippedPos.Y = Height / Width * (ClippedPos.X - InOutStartPos.X) + InOutStartPos.Y;
			}
			else if (RegionToClip & TOP)
			{
				ClippedPos.X = Width / Height * (ClippedPos.Y - InOutStartPos.Y) + InOutStartPos.X;
				ClippedPos.Y = InMaxPos.Y;
			}
			else if (RegionToClip & BOTTOM)
			{
				ClippedPos.X = Width / Height * (ClippedPos.Y - InOutStartPos.Y) + InOutStartPos.X;
				ClippedPos.Y = InMinPos.Y;
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

WinRenderer::EViewportRegion WinRenderer::ComputeViewportRegion(const Vector2& InPos, const Vector2& InMinPos, const Vector2& InMaxPos)
{
	EViewportRegion Result = EViewportRegion::INSIDE_VIEWPORT;
	if (InPos.X < InMinPos.X)
	{
		Result = static_cast<EViewportRegion>(Result | EViewportRegion::LEFT);
	}
	else if (InPos.X > InMaxPos.X)
	{
		Result = static_cast<EViewportRegion>(Result | EViewportRegion::RIGHT);
	}

	if (InPos.Y < InMinPos.Y)
	{
		Result = static_cast<EViewportRegion>(Result | EViewportRegion::BOTTOM);
	}
	else if (InPos.Y > InMaxPos.Y)
	{
		Result = static_cast<EViewportRegion>(Result | EViewportRegion::TOP);
	}

	return Result;
}

void WinRenderer::InitTextureBuffer()
{
	FILE* File = nullptr;
	const std::string FileName = "checkerboard.png";
	unsigned char* LoadBuffer = stbi_load(FileName.c_str(), &TexWidth, &TexHeight, &TexChannels, 0);

	if (LoadBuffer == nullptr)
	{
		std::cout << "Failed to load texture\n";
		if (stbi_failure_reason())
		{
			std::cout << stbi_failure_reason();
		}

		return;
	}

	std::cout << "Texture loaded: " << TexWidth << "x" << TexHeight << " " << TexChannels << " channels\n";
	TextureBuffer = new Color32[TexWidth * TexHeight];
	for (int i = 0; i < TexWidth * TexHeight; ++i)
	{
		TextureBuffer[i].R = LoadBuffer[i * TexChannels];
		TextureBuffer[i].G = LoadBuffer[i * TexChannels + 1];
		TextureBuffer[i].B = LoadBuffer[i * TexChannels + 2];
		TextureBuffer[i].A = 255;
	}

	stbi_image_free(LoadBuffer);
}

Color32 WinRenderer::SampleTexture(const Vector2& InUV) const
{
	int X = Math::Clamp(static_cast<int>(InUV.X * TexWidth + 0.5f), 0, TexWidth - 1);
	int Y = Math::Clamp(static_cast<int>(InUV.Y * TexHeight + 0.5f), 0, TexHeight - 1);
	return TextureBuffer[Y * TexWidth + X];
}

Color32 WinRenderer::SampleTexture(const Vertex& InVertex) const
{
	return SampleTexture(InVertex.UV);
}

