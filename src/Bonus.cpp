#include "Bonus.h"

Bonus::Bonus(){
    toApply = false;
    isVisible = false;
    isActive = false;
    activeClockInterval = 5;   //seconds
    movementClockInterval = 10; //milliseconds
    movementDistance = 1;
}

Bonus::~Bonus(){

}

void Bonus::handleBonus(sf::Vector2f deadEnemyPosition, Fighter fighter){
    if(isBonusDrawn(deadEnemyPosition) && !isVisible && !isActive)
        initializeBonus(deadEnemyPosition);

    if(isVisible)
        moveBonus();

    if(areTexturesCovered(*this, fighter)){
        isVisible = false;
        position.x = -1;
        position.y = -1;
        toApply = true;
    }
}

bool Bonus::isBonusDrawn(sf::Vector2f position){
    int bonusProbability = 50;
    if(position.x >= 0 && position.y >= 0){
        int ran = rand() % bonusProbability;
        return ran == 0;
    }

    return false;
}

void Bonus::initializeBonus(sf::Vector2f pos){
    setPosition(pos.x, pos.y);
    setBonusType();
    isVisible = true;
}

void Bonus::moveBonus(){
    if(movementClock.getElapsedTime().asMilliseconds() > movementClockInterval)
        move(0, movementDistance);
}

void Bonus::setBonusType(){
    type = static_cast<BonusType>(rand() % 3);
}
