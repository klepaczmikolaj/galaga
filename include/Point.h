#ifndef POINT_H
#define POINT_H


class Point{
    public:
        Point();
        ~Point();
        Point(double, double);
        Point(const Point& point);
        Point& operator=(const Point& point);
        Point& operator*(const double& mult);
        Point& operator+(const Point& point);
        double x;
        double y;

    private:

};

#endif // POINT_H
