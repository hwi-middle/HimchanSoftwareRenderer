#include "pch.h"
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
	memset(ScreenBuffer, 255, Width * Height * sizeof(Color32));
}

void WinRenderer::DrawLine(const Vector2& InStartPos, const Vector2& InEndPos, const Color& InColor)
{
	DrawLine(InStartPos, InEndPos, [&](float X, float Y) -> Color { return InColor; });
}

void WinRenderer::DrawLine(const Vector2& InStartPos, const Vector2& InEndPos, const std::function<Color(float X, float Y)> InColorFunc)
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
			SetPixel(X, Y, InColorFunc(X, Y));

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
			SetPixel(X, Y, InColorFunc(X, Y));

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
	std::array<Vector2, 3> Vertices =
	{
		ScreenPoint::CartesianToScreen(InVertex1.Position, Width, Height),
		ScreenPoint::CartesianToScreen(InVertex2.Position, Width, Height),
		ScreenPoint::CartesianToScreen(InVertex3.Position, Width, Height)
	};
	std::sort(Vertices.begin(), Vertices.end(), [](const Vector2& InLhs, const Vector2& InRhs) { return InLhs.Y < InRhs.Y; });

	int32 X1 = Vertices[0].X;
	int32 Y1 = Vertices[0].Y;
	int32 X2 = Vertices[1].X;
	int32 Y2 = Vertices[1].Y;
	int32 X3 = Vertices[2].X;
	int32 Y3 = Vertices[2].Y;

	float A12 = Y1 != Y2 ? (X2 - X1) / static_cast<float>(Y2 - Y1) : 0.f;
	float A13 = (X3 - X1) / static_cast<float>(Y3 - Y1);
	float A23 = Y2 != Y3 ? (X3 - X2) / static_cast<float>(Y3 - Y2) : 0.f;

	for (int Y = Y1; Y <= Y2; ++Y)
	{
		int32 XLeft = X1 + A12 * (Y - Y1);
		int32 XRight = X1 + A13 * (Y - Y1);

		if (XLeft > XRight)
		{
			std::swap(XLeft, XRight);
		}

		float AlphaLeft = Y2 != Y1 ? (Y - Y1) / static_cast<float>(Y2 - Y1) : 0.f;
		Color ColorLeft = Math::Lerp(InVertex1.Color, InVertex2.Color, AlphaLeft);
		float AlphaRight = Y3 != Y1 ? (Y - Y1) / static_cast<float>(Y3 - Y1) : 1.f;
		Color ColorRight = Math::Lerp(InVertex1.Color, InVertex3.Color, AlphaRight);

		DrawLine(
			ScreenPoint::ScreenToCartesian(Vector2(XLeft, Y), Width, Height),
			ScreenPoint::ScreenToCartesian(Vector2(XRight, Y), Width, Height),
			[&](float InX, float InY) -> Color {
				return Math::Lerp(ColorLeft, ColorRight, (InX - XLeft) / static_cast<float>(XRight - XLeft));
			});
	}

	for (int Y = Y2; Y <= Y3; ++Y)
	{
		int32 XLeft = X2 + A23 * static_cast<float>(Y - Y2);
		int32 XRight = X1 + A13 * static_cast<float>(Y - Y1);

		if (XLeft > XRight)
		{
			std::swap(XLeft, XRight);
		}

		float AlphaLeft = Y3 != Y2 ? (Y - Y2) / static_cast<float>(Y3 - Y2) : 0.f;
		Color ColorLeft = Math::Lerp(InVertex2.Color, InVertex3.Color, AlphaLeft);
		float AlphaRight = Y3 != Y1 ? (Y - Y1) / static_cast<float>(Y3 - Y1) : 1.f;
		Color ColorRight = Math::Lerp(InVertex1.Color, InVertex3.Color, (Y - Y1) / static_cast<float>(Y3 - Y1));

		DrawLine(
			ScreenPoint::ScreenToCartesian(Vector2(XLeft, Y), Width, Height),
			ScreenPoint::ScreenToCartesian(Vector2(XRight, Y), Width, Height),
			[&](float InX, float InY) -> Color {
				return Math::Lerp(ColorLeft, ColorRight, (InX - XLeft) / static_cast<float>(XRight - XLeft));
			});
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

