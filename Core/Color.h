#ifndef IMAGE_COLOR_H
#define IMAGE_COLOR_H

namespace Color
{
	struct RGB
	{
		RGB() = default;
		RGB(double r, double g, double b);

		double R;
		double G;
		double B;
	};
}

#endif