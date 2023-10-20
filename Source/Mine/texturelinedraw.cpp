#include "texturelinedraw.h"
#include "Mine.h"
#include "Tasks/Task.h"


//a is current point, b is previous point, c is point before pointB
void textureLineDraw(uint8 mipMap[], FIntVector2 *pointA, FIntVector2 *pointB, FIntVector2 *pointC)
{
    
    if(pointB != nullptr)
    {
        if(pointC != nullptr)
        {

            if(*pointA.X == *pointB.X && *pointA.Y == *pointB.Y)
            {
                //the point is the same as the previous point, do nothing

            }
            if(*pointB.X == *pointC.X || *pointB.X == *pointA.X)
            {
                //slope is infinite or point has not moved

            }
            else{
                //probably want to check if the point is more than like 5 points away to check if we want to do a curve or paint a straight line

                //do the "normal operation here"

                //below derives cubic line based off the equations
                // by = m(bx^3) + d(bx^2) + v(bx) + z
                // ay = m(ax^3) + d(ax^2) + v(ax) + z
                // bslope = 3m(bx^2) + 2d(bx) + v
                // aslope = 3m(ax^2) + 2d(ax) + v

                float ax = *pointA->X;
                float bx = *pointB->X;
                float ay = *pointA->Y;
                float by = *pointB->Y;

                float slopeA = (float)(ay - by) / (float)(ax - bx);
                float slopeb = (float)(by - *pointC->Y) / (float)(bx - *pointC->X);

                if(slopeb > (10/3))slopeb = 10/3;

                int t1 = ax - bx;
                int q1 = (ax * ax) - (bx * bx) - (2 * bx * t1);
                float k1 = ((ax * ax * ax) - (bx * bx * bx) - (t1 * bx * bx)) / q1;
                float f1 = (ay - by - t1 * slopeb) / q1;
                int j1 = (2 * ax) - (2 * bx);
                float m1 = (slopeA - slopeb - (f1 * j1)) / ((3 * ((ax * ax) - (bx * bx))) - (k1 * j1));
                float d1 = f1 - (k1 * m1);
                float v1 = slopeb - (2 * bx * f1) - (((3 * bx * bx) - (2 * bx * k1)) * m1);
                float z1 = by - (bx * slopeb) + (f1 * bx * bx) + (((2 * bx * bx * bx) - (k1 * bx * bx)) * m1);

                //line equals
                // f(x) = m1(x^3) + d1(x^2) + v1(x) + z1

            }


        }
        else
        {//just straight line since we dont have a slope for the first point because it is the 2nd point on the draw

        }
    }
    else
    {//this is the first point, just draw a circle

    }
    *pointB = *pointA;
    *pointC = *pointB;
    FTask task;
    
}