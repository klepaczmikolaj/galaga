#include "EnemiesGreen.h"

EnemiesGreen::~EnemiesGreen(){

}

EnemiesGreen::EnemiesGreen(int left, int right, int up, int down) : EnemiesRed(left, right, up, down){
    numberOfRaws = 1;
    enemyMaxXNumber = 4;
    initialEnemyNumber = numberOfRaws * enemyMaxXNumber;
    attackingInterval = 7;  //seconds
    startLeft = leftBorder + 100;
    startUp = upBorder;
    attackCurveSpeed = 0.005;
    mommentOfAttack = 0;
    directionRightInit = true;
    curveType = BezierCurve::curveType::curve1;
}

std::string EnemiesGreen::getName(){
    return "green";
}
