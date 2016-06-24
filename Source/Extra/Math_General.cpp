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

    // Logic explained in 'Interpolation - Smooth or Cubic.png'.
    case IT_smooth :
    {
        double dx = finalX-initialX;
        double dy = finalY-initialY;
        double f  = dy / (0.25*pow(dx, 3));

        double Ax = (initialX + finalX)/2;

        if (currentX < Ax)
            result = f*pow(currentX-initialX, 3) + initialY;
        else
            result = f*pow(currentX-finalX, 3) + finalY;

    }
    break;

    case IT_exponential :
    {
        double root = (0.05*finalX*finalX*initialY + 4) / (0.05*initialY);
        double xI   = 0.5 * (finalX + pow(root, 0.5));
        double yI   = 1/(0.05 * (xI-finalX));
        result = 1/(0.05 * (currentX - xI)) + yI;
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
