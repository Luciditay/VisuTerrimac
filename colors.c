#include <math.h>

#include "colors.h"

ColorRGB createColor(float r, float g, float b)
{
    ColorRGB c;
    c.r = r;
    c.g = g;
    c.b = b;
    return c;
}

ColorRGB addColors(ColorRGB c1, ColorRGB c2)
{
    c1.r += c2.r;
    c1.g += c2.g;
    c1.b += c2.b;
    return c1;
}

ColorRGB subColors(ColorRGB c1, ColorRGB c2)
{
    c1.r -= c2.r;
    c1.g -= c2.g;
    c1.b -= c2.b;
    return c1;
}

ColorRGB multColors(ColorRGB c1, ColorRGB c2)
{
    c1.r *= c2.r;
    c1.g *= c2.g;
    c1.b *= c2.b;
    return c1;
}

ColorRGB multColor(ColorRGB c, float a)
{
    if (a == 0.) {
        c.r = 0.;
        c.g = 0.;
        c.b = 0.;
        return c; 
    }

    c.r *= a;
    c.g *= a;
    c.b *= a;
    return c;
}

ColorRGB divColor(ColorRGB c, float a)
{
    if (a == 0.)
        return c;
    c.r /= a;
    c.g /= a;
    c.b /= a;
    return c;
}

ColorRGB clampColor(ColorRGB c) {
    c.r = min(1, max(0, c.r));
    c.g = min(1, max(0, c.g));
    c.b = min(1, max(0, c.b));
    return c;
}