#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H

#include "Point.h"
#include <cmath>

class BezierCurve{
    public:
        enum class curveType{curve1, curve2};
        BezierCurve();
        ~BezierCurve();
        Point getCurve(curveType, int, int, double);

    private:
        int binomialCoeff(int, int);
        Point getGeneralCurve(int order, Point pointArray[], double t);
        Point getCurve1(int, int, double);
        Point getCurve2(int, int, double);
};


#endif // BEZIERCURVE_H
