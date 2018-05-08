#include "EnemiesRed.h"
#include "BezierCurve.h"

#ifndef ENEMIESGREEN_H
#define ENEMIESGREEN_H


class EnemiesGreen : public EnemiesRed
{
    public:
        EnemiesGreen(int, int, int, int);
        virtual ~EnemiesGreen();
        virtual std::string getName();

};

#endif // ENEMIESGREEN_H
