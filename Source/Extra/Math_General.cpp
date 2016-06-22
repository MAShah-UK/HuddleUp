#include "Math_General.h"

#include <algorithm>

// Interpolate

Math::Interpolate::Interpolate()
{}

double Math::Interpolate::interpolatedValue(bool applyIncrement, interpType iT)
{
    double result = 0;

    switch (iT)
    {
    case IT_linear :
    {
        // y - y1 = m(x - x1)
        double gradient = (finalY-initialY) / (finalX-initialX);
        result          = gradient*(currentX-initialX) + initialY;
    }
    break;

    case IT_sinusoidal :
    {
        // Maybe not optimal, but works within range.

        double pi    = 3.14159265358;
        double xCalc = (currentX-initialX) / (finalX-initialX);
        double yMax  = (finalY-initialY)/2;
        result       = yMax * sin(pi*xCalc) + initialY;

        if (currentX > (finalX+initialX)/2)
           result = finalY + initialY - result;
    }
    break;
    }

    if (applyIncrement) currentX += incrementX;

    return result;
}

bool Math::Interpolate::outOfRange()
{
    return (currentX > finalX) ||
           (currentX < initialX)
           ? true : false;
}
