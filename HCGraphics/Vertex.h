#include "pch.h"

namespace HC
{
	struct Vertex
	{
	public:
		Vector4 Position;
		::Color Color;
		Vector2 UV;

		FORCEINLINE constexpr Vertex() = default;
		FORCEINLINE constexpr Vertex(const Vector4& InPos) : Position(InPos) { }
		FORCEINLINE constexpr Vertex(const Vector4& InPos, const ::Color& InColor) : Position(InPos), Color(InColor) { }
		FORCEINLINE constexpr Vertex(const Vector4& InPos, const ::Color& InColor, const Vector2& InUV) : Position(InPos), Color(InColor), UV(InUV) { }
	};
}
