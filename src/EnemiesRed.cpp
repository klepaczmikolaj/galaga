#include "../include/EnemiesRed.h"

EnemiesRed::~EnemiesRed(){

}

EnemiesRed::EnemiesRed(int left, int right, int up, int down) : Enemies(left, right, up, down){
    numberOfRaws = 2;
    enemyMaxXNumber = 4;
    initialEnemyNumber = numberOfRaws * enemyMaxXNumber;
    attackingInterval = 6;  //seconds
    startLeft = leftBorder + 100;
    startUp = upBorder + 60;
    attackCurveSpeed = 1.0/240.0;
    mommentOfAttack = 0;
    directionRightInit = false;
    curveType = BezierCurve::curveType::curve2;
    enemySaveFilename = "txt/saveGameFileRed.txt";
}

void EnemiesRed::handleEnemies(){
    if(enemyVector.empty()){
        isAttackStarted = false;
        initializeGameJson();
    }
    else {
        if(!isAttackStarted && movementClock.getElapsedTime().asMilliseconds() > 10)
            moveHorisontal();
        if(attackClock.getElapsedTime().asSeconds() > attackingInterval){
            isAttackStarted = true;
            attackClock.restart();
            attackMovementClock.restart();

            drawAttackingEnemies();
            directionDown = true;
        }
        if(isAttackStarted && attackMovementClock.getElapsedTime().asMilliseconds() > 10){
            moveVertical();
            attackMovementClock.restart();
        }
        appendSpeed();

    }
}

std::string EnemiesRed::getName(){
    return "red";
}

void EnemiesRed::moveVertical(){
    mommentOfAttack += attackCurveSpeed;
    for(unsigned int iter = 0; iter < attackingEnemyIndexVector.size(); iter++){
        Point curveStartPoint, newPosition;
        curveStartPoint.x = enemyVector[attackingEnemyIndexVector[iter]].initialPosition.x;
        curveStartPoint.y = enemyVector[attackingEnemyIndexVector[iter]].initialPosition.y;

        newPosition = bezier.getCurve(curveType, curveStartPoint.x, curveStartPoint.y, mommentOfAttack);

        if((enemyVector[attackingEnemyIndexVector[iter]].position.x >= rightBorder-sizeX-35)){
            directionRight = false;
        }
        else
            enemyVector[attackingEnemyIndexVector[iter]].setPosition(newPosition.x, newPosition.y);
    }

    if(isAnyEnemyTouchingUpBorder() || attackingEnemyIndexVector.empty()){
        mommentOfAttack = 0;
        isAttackStarted = false;
        attackingEnemyIndexVector.clear();
    }

}
