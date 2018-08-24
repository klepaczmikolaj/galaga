#include "../include/Bullets.h"

Bullets::Bullets()
{

}

Bullets::~Bullets()
{

}

Bullets::Bullet::Bullet(){

}

void Bullets::Bullet::setPosition(float x, float y){
    position.x = x;
    position.y = y;
}

void Bullets::Bullet::move(float x, float y){
    position.x += x;
    position.y += y;
}

template < typename T > std::string to_string( const T& n ){
    std::ostringstream stm ;
    stm << n ;
    return stm.str() ;
}

std::ostream& operator<<(std::ostream& os, const Bullets::Bullet& bullet)  {
    os << "Bullet was removed on coordinates: X = " << to_string(bullet.position.x) + "  Y = " << to_string(bullet.position.y);
    return os;
}
