
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>

#ifndef BULLETS_H
#define BULLETS_H


class Bullets
{
    public:
        Bullets();
        ~Bullets();
        class Bullet{
            public:
                Bullet();
                sf::Vector2f position;
                void setPosition(float, float);
                void move(float, float);
                friend std::ostream& operator<<(std::ostream& os, const Bullets::Bullet& bullet) ;
                float sizeX;
                float sizeY;
        };
        std::vector <Bullet> bulletVector;
        float sizeX;
        float sizeY;

};

#endif // BULLETS_H

