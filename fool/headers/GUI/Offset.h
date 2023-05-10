#pragma once

namespace GUI
{
	class Offset
	{
	public:
		float left;
		float top;
		float right;
		float bottom;

	public:
		Offset();
		Offset(float left, float top, float right, float bottom);
		Offset(float leftRight, float topBottom);
	};
}