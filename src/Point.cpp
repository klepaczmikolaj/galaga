#include "Point.h"

Point::Point()
{
    //ctor
}

Point::~Point()
{
    //dtor
}

Point::Point(double xx, double yy): x(xx), y(yy){

}

Point::Point(const Point& point){
    x = point.x;
    y = point.y;
}

Point& Point::operator=(const Point& point){
    if(&point == this)
        return *this;
    x = point.x;
    y = point.y;

    return *this;
}

Point& Point::operator*(const double& mult){
    this->x *= mult;
    this->y *= mult;

    return *this;
}

Point& Point::operator+(const Point& point){
    this->x += point.x;
    this->y += point.y;

    return *this;
}
