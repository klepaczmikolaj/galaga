#include "Fighter.h"

#ifndef BONUS_H
#define BONUS_H


class Bonus : public Fighter
{
    public:
        Bonus();
        ~Bonus();

        enum class BonusType{
            fasterFighter = 0,
            slowerFighter = 1,
            doubleBullets = 2
        }type;

        bool isVisible;
        bool isActive;
        bool toApply;
        int activeClockInterval;
        sf::Clock activeClock;

        void handleBonus(sf::Vector2f, Fighter);

    private:
        sf::Clock movementClock;
        int movementClockInterval;
        int movementDistance;

        void moveBonus();
        void initializeBonus(sf::Vector2f);
        void setBonusType();
        void resetBonus();
        bool isBonusDrawn(sf::Vector2f);

        template<typename T, typename K>
        bool areTexturesCovered(T t, K k){
            int KLeft = k.position.x;
            int KRight = KLeft + k.sizeX;
            int KUp = k.position.y;
            int KDown = KUp + k.sizeY;

            int TLeft = t.position.x;
            int TRight = TLeft + t.sizeX;
            int TUp = t.position.y;
            int TDown = TUp + t.sizeY;

            return KUp < TDown && KDown > TUp && KLeft < TRight && KRight > TLeft;
        }
};

#endif // BONUS_H
