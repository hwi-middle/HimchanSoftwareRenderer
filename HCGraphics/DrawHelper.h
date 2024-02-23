#pragma once
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
			ViewportRegion StartRegion = ComputeViewportRegion(InOutStartPos, InMinPos, InMaxPos);
			ViewportRegion EndRegion = ComputeViewportRegion(InOutEndPos, InMinPos, InMaxPos);

			while (true)
			{
				if (!(StartRegion | EndRegion))
				{
					return true;
				}
				else if (StartRegion & EndRegion)
				{
					return false;
				}
				else
				{
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
						InOutStartPos = ClippedPos;
						EndRegion = ComputeViewportRegion(InOutStartPos, InMinPos, InMaxPos);
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
		static float DrawLine(const Vector2& InStartPos, const Vector2& InEndPos, const COLORREF InColor)
		{
			// TODO
		}
	};
}