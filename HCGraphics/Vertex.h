#include "pch.h"

namespace HC
{
	struct Vertex
	{
	public:
		Vector4 position;
		Color color;
		Vector2 uv;

		FORCEINLINE constexpr Vertex() = default;
		FORCEINLINE constexpr Vertex(const Vector4& inPos) : position(inPos) { }
		FORCEINLINE constexpr Vertex(const Vector4& inPos, const ::Color& inColor) : position(inPos), color(inColor) { }
		FORCEINLINE constexpr Vertex(const Vector4& inPos, const ::Color& inColor, const Vector2& inUv) : position(inPos), color(inColor), uv(inUv) { }
	};
}
