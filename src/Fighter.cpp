#include "Fighter.h"

Fighter::Fighter()
{
    //ctor
}

Fighter::~Fighter()
{
    //dtor
}

void Fighter::setPosition(float x, float y){
    position.x = x;
    position.y = y;
}

void Fighter::move(float x, float y){
    position.x += x;
    position.y += y;
}

template < typename T > std::string to_string( const T& n ){
    std::ostringstream stm ;
    stm << n ;
    return stm.str() ;
}

std::ostream& operator<<(std::ostream& os, const Fighter& fighter)  {
    os << "Fighter was killed on coordinates: X = " << to_string(fighter.position.x) + "  Y = " << to_string(fighter.position.y);
    return os;
}
