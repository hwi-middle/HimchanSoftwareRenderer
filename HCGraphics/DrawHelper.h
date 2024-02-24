#pragma once
#include <iostream>
#include "pch.h"

namespace HC
{
	static class DrawHelper
	{
	private:
		typedef int ViewportRegion;
		static constexpr ViewportRegion INSIDE_VIEWPORT = 0b0000;
		static constexpr ViewportRegion LEFT = 0b0001;
		static constexpr ViewportRegion RIGHT = 0b0010;
		static constexpr ViewportRegion BOTTOM = 0b0100;
		static constexpr ViewportRegion TOP = 0b1000;

		static bool ClipLine(Vector2& InOutStartPos, Vector2& InOutEndPos, const Vector2& InMinPos, const Vector2& InMaxPos)
		{
			std::cout << "line to draw: " << InOutStartPos.ToString() << " to " << InOutEndPos.ToString() << "\n";

			ViewportRegion StartRegion = ComputeViewportRegion(InOutStartPos, InMinPos, InMaxPos);
			ViewportRegion EndRegion = ComputeViewportRegion(InOutEndPos, InMinPos, InMaxPos);

			while (true)
			{
				if (!(StartRegion | EndRegion))
				{
					std::cout << "no need to clip now: " << InOutStartPos.ToString() << " to " << InOutEndPos.ToString() << "\n";
					return true;
				}
				else if (StartRegion & EndRegion)
				{
					std::cout << "whole line is not inside viewport: " << InOutStartPos.ToString() << " to " << InOutEndPos.ToString() << "\n";
					return false;
				}
				else
				{
					std::cout << "clipped!\n";

					ViewportRegion RegionToClip = max(StartRegion, EndRegion);
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

		static ViewportRegion ComputeViewportRegion(const Vector2& InPos, const Vector2& InMinPos, const Vector2& InMaxPos)
		{
			ViewportRegion Result = INSIDE_VIEWPORT;
			if (InPos.X < InMinPos.X)
			{
				Result |= LEFT;
			}
			else if (InPos.X > InMaxPos.X)
			{
				Result |= RIGHT;
			}

			if (InPos.Y < InMinPos.Y)
			{
				Result |= BOTTOM;
			}
			else if (InPos.Y > InMaxPos.Y)
			{
				Result |= TOP;
			}

			return Result;
		}

	public:
		static void DrawLine(const HDC InHdc, const Vector2& InScreenSize, const Vector2& InStartPos, const Vector2& InEndPos, const COLORREF InColor)
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
					SetPixel(InHdc, X, Y, InColor);

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
					SetPixel(InHdc, X, Y, InColor);

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
	};
}