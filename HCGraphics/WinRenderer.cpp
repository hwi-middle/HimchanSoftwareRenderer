#include "pch.h"
using namespace HC;

WinRenderer::~WinRenderer()
{
}

bool WinRenderer::Initialize(uint32 InWidth, uint32 InHeight)
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
	bmi.bmiHeader.biHeight = Height;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;

	MemBitmap = ::CreateDIBSection(MemDC, &bmi, DIB_RGB_COLORS, (void**)&ScreenBuffer, NULL, 0);
	OriginalBitmap = (HBITMAP)::SelectObject(MemDC, MemBitmap);

	return true;
}

void WinRenderer::Release()
{
	::DeleteObject(OriginalBitmap);
	::DeleteObject(MemBitmap);
	::ReleaseDC(Handle, MemDC);
	::ReleaseDC(Handle, ScreenDC);
}

void WinRenderer::SwapBuffer()
{
	::BitBlt(ScreenDC, 0, 0, Width, Height, MemDC, 0, 0, SRCCOPY);
}

void WinRenderer::FillBuffer()
{
	for (uint32 i = 0; i < Width * Height; i++)
	{
		ScreenBuffer[i] = Color::White.ToColor32();
	}
}

void WinRenderer::DrawLine(const Vector2& InScreenSize, const Vector2& InStartPos, const Vector2& InEndPos, const Color InColor)
{
	Vector2 HalfScreen = InScreenSize * 0.5f;
	Vector2 MinScreen = -HalfScreen;
	Vector2 MaxScreen = HalfScreen;
	Vector2 ClippedStartPos = InStartPos;
	Vector2 ClippedEndPos = InEndPos;

	if (!ClipLine(ClippedStartPos, ClippedEndPos, MinScreen, MaxScreen))
	{
		return;
	}

	Vector2 StartPosScreen = ScreenPoint::CartesianToScreen(ClippedStartPos, InScreenSize.X, InScreenSize.Y);
	Vector2 EndPosScreen = ScreenPoint::CartesianToScreen(ClippedEndPos, InScreenSize.X, InScreenSize.Y);

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

