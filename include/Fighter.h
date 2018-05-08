#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>

#ifndef FIGHTER_H
#define FIGHTER_H


class Fighter
{
    public:
        Fighter();
        ~Fighter();
        sf::Vector2f position;
        float sizeX;
        float sizeY;
        void setPosition(float, float);
        void move(float, float);
        friend std::ostream& operator<<(std::ostream& os, const Fighter& fighter) ;

};

#endif // FIGHTER_H
