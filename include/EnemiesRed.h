#include "Enemies.h"
#include "BezierCurve.h"

#ifndef ENEMIESRED_H
#define ENEMIESRED_H


class EnemiesRed : public Enemies
{
    public:
        EnemiesRed(int, int, int, int);
        virtual ~EnemiesRed();
        virtual void handleEnemies();
        virtual std::string getName();

    protected:
        int startLeft;
        int startUp;
        double attackCurveSpeed;
        BezierCurve bezier;
        BezierCurve::curveType curveType;
        virtual void moveVertical();

};

#endif // ENEMIESRED_H
