#include "GUI/Offset.h"

namespace GUI
{
	Offset::Offset()
	{
		left = right = top = bottom = 0;
	}

	Offset::Offset(float left, float top, float right, float bottom)
		: left(left), top(top), right(right), bottom(bottom)
	{
	}

	Offset::Offset(float leftRight, float topBottom)
		: left(leftRight), top(topBottom), right(leftRight), bottom(topBottom)
	{
	}
}
