#include "../include/BezierCurve.h"
BezierCurve::BezierCurve(){

}

BezierCurve::~BezierCurve(){

}

Point BezierCurve::getCurve(curveType type, int x0, int y0, double t){
    if(type == curveType::curve1)
        return getCurve1(x0, y0, t);
    else
        return getCurve2(x0, y0, t);
}

Point BezierCurve::getCurve1(int x0, int y0, double t){
    Point pointArray[5];
    Point p0(x0,y0);
    pointArray[0] = p0;
    Point p1(x0 - 400,y0 + 100);
    pointArray[1] = p1;
    Point p2(x0 - 400,y0 + 800);
    pointArray[2] = p2;
    Point p3(x0 - 10,y0 + 800);
    pointArray[3] = p3;
    pointArray[4] = p0;

    return getGeneralCurve(4, pointArray, t);

}

Point BezierCurve::getCurve2(int x0, int y0, double t){
    Point pointArray[6];
    Point p0(x0,y0);
    pointArray[0] = p0;
    Point p1(x0 - 10,y0 + 800);
    pointArray[1] = p1;
    Point p2(x0 - 500,y0 + 10);
    pointArray[2] = p2;
    Point p3(x0 - 200,y0 + 800);
    pointArray[3] = p3;
    Point p4(x0 - 500,y0 + 800);
    pointArray[4] = p4;
    pointArray[5] = p0;

    return getGeneralCurve(5, pointArray, t);
}

Point BezierCurve::getGeneralCurve(int order, Point pointArray[], double t){
    Point point(0.,0.);
    for(int i = 0; i <= order; i++){
        point.x = point.x + (pointArray[i].x * binomialCoeff(order, i) * pow((1-t),(order-i)) * pow(t, i));
        point.y = point.y + (pointArray[i].y * binomialCoeff(order, i) * pow((1-t),(order-i)) * pow(t, i));
    }
    return point;
}

int BezierCurve::binomialCoeff(int n, int k)
{
    if (k==0 || k==n)
        return 1;

    return  binomialCoeff(n-1, k-1) + binomialCoeff(n-1, k);
}
