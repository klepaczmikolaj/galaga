#include "Enemies.h"

Enemies::~Enemies(){

}

Enemies::Enemies(int left, int right, int up, int down): leftBorder(left), rightBorder(right), upBorder(up), downBorder(down){
    numberOfRaws = 2;
    enemyMaxXNumber = 7;
    initialEnemyNumber = numberOfRaws * enemyMaxXNumber;
    attackSpeed = 8;
    attackingInterval = 5;  //seconds
    startLeft = leftBorder;
    startUp = upBorder + 140;
    mommentOfAttack = 0;
    directionRightInit = true;
    mapFilename = "json/map0.json";
}

Enemies::Enemy::Enemy(){

}
void Enemies::Enemy::setPosition(float x, float y){
    position.x = x;
    position.y = y;
}

void Enemies::Enemy::setInitialPosition(float x, float y){
    initialPosition.x = x;
    initialPosition.y = y;
}

void Enemies::Enemy::move(float x, float y){
    position.x += x;
    position.y += y;
}

std::string Enemies::getName(){
    return "yellow";
}

std::ostream& operator<<(std::ostream& os, const Enemies::Enemy& enemy)  {
    os << "Enemy was killed on coordinates: X = " << to_string(enemy.position.x) + "  Y = " << to_string(enemy.position.y);
    return os;
}

void Enemies::handleEnemies(){
    if(enemyVector.empty()){
        isAttackStarted = false;
        initializeGameJson();
    }
    else {
        if(movementClock.getElapsedTime().asMilliseconds() > 10)
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

void Enemies::initializeGame(){
    enemyVector.clear();
    mommentOfAttack = 0;
    isAttackStarted = false;
    directionRight = true;
    horisontalEnemyMovementDistance = 1;
    movementClock.restart();
    attackClock.restart();
    directionRight = directionRightInit;
}

void Enemies::initializeGameJson(){
    initializeGame();

    std::ifstream file(mapFilename);
    if(!file.is_open()){
        std::cout << "Unable to read " << mapFilename << " file" << std::endl << "Press any key to exit" << std::endl;
        exit(0);
        std::cin.ignore();
    }

    json j;
    file >> j;
    if(j[getName()] == nullptr){
        return;
    }

    std::vector <int> positionX = j[getName()]["positionX"];
    std::vector <int> positionY = j[getName()]["positionY"];
    if(positionX.size() != positionY.size()){
        std::cout << "Incorrect data in " << mapFilename << " file" << std::endl;
        std::cin.ignore();
        exit(0);
    }

    Enemies::Enemy enemy;
    for(unsigned int i = 0; i < positionX.size(); i++){
        enemy.setPosition(positionX[i], positionY[i]);
        enemy.setInitialPosition(positionX[i], positionY[i]);
        enemyVector.push_back(enemy);
    }

    file.close();
}

void Enemies::initializeGroupFile(std::string filename){
    enemyVector.clear();
    mommentOfAttack = 0;
    isAttackStarted = false;
    horisontalEnemyMovementDistance = 1;

    saveGameFile.open(filename, std::ios::in);
    if(!saveGameFile.is_open()){
        std::cout << "Unable to read " << filename << " file" << std::endl << "Press any key to exit" << std::endl;
        std::cin.ignore();
    }
    else{
        std::string str;
        std::getline(saveGameFile, str);
        std::istringstream ss(str);
        int num;
        while(ss >> num){
            Enemies::Enemy enemy;
            enemy.position.x = num;
            enemy.initialPosition.x = num;
            ss >> num;
            enemy.position.y = num;
            enemy.initialPosition.y = num;
            enemyVector.push_back(enemy);
        }
    }
    saveGameFile.close();

    movementClock.restart();
    attackClock.restart();
}

void Enemies::appendSpeed(){
    int enemyCount = enemyVector.size();
    if(enemyCount == initialEnemyNumber/2)
        horisontalEnemyMovementDistance = 2;
    else if(enemyCount == initialEnemyNumber/3)
        horisontalEnemyMovementDistance = 3;
    else if(enemyCount == initialEnemyNumber/4)
        horisontalEnemyMovementDistance = 4;
}

void Enemies::moveHorisontal(){
    setHorisontalDirection();

    for(std::vector<Enemy>::iterator it = enemyVector.begin(); it != enemyVector.end(); it++){
        if(directionRight){
            it->move(horisontalEnemyMovementDistance, 0);
            it->initialPosition.x += horisontalEnemyMovementDistance;
        }

        else{
            it->move(-horisontalEnemyMovementDistance, 0);
            it->initialPosition.x -= horisontalEnemyMovementDistance;
        }
    }
    movementClock.restart();
}

void Enemies::setHorisontalDirection(){
    for(auto it = enemyVector.begin(); it != enemyVector.end(); it++){
        if(it->position.x <= leftBorder)
            directionRight = true;
        else if(it->position.x >= rightBorder-sizeX-35)
            directionRight = false;
    }
}

void Enemies::drawAttackingEnemies(){
    attackingEnemyIndexVector.clear();
    unsigned int numberOfAttackers = 1 + rand() % enemyVector.size();

    while(attackingEnemyIndexVector.size() != numberOfAttackers){
        int randomAttackingEnemyIndex = rand() % enemyVector.size();

        if(!isSpecifiedEnemyAlreadyAttacking(randomAttackingEnemyIndex)){
            attackingEnemyIndexVector.push_back(randomAttackingEnemyIndex);
            enemyVector.at(randomAttackingEnemyIndex).initialPosition.x = enemyVector.at(randomAttackingEnemyIndex).position.x;
            enemyVector.at(randomAttackingEnemyIndex).initialPosition.y = enemyVector.at(randomAttackingEnemyIndex).position.y;
        }
    }
}

bool Enemies::isSpecifiedEnemyAlreadyAttacking(int attackingEnemyIndex){
    for(unsigned int i = 0; i < attackingEnemyIndexVector.size(); i++)
        if(attackingEnemyIndexVector[i] == attackingEnemyIndex)
            return true;

    return false;
}

void Enemies::moveVertical(){
    if(isAnyEnemyTouchingDownBorder())
        directionDown = false;

    if(!directionDown && isAnyEnemyTouchingUpBorder()){
        isAttackStarted = false;
        attackingEnemyIndexVector.clear();
    }

    for(unsigned int iter = 0; iter < attackingEnemyIndexVector.size(); iter++){
        if(directionDown)
            enemyVector[attackingEnemyIndexVector[iter]].move(0, attackSpeed);
        else
            enemyVector[attackingEnemyIndexVector[iter]].move(0, -attackSpeed);
    }
}

bool Enemies::isAnyEnemyTouchingDownBorder(){
    for(unsigned int i = 0; i < attackingEnemyIndexVector.size(); i++){
        int itEnemyIndex = attackingEnemyIndexVector[i];
        int enemyPositionY = enemyVector[itEnemyIndex].position.y;
        if(enemyPositionY >= downBorder-sizeY + 150)
            return true;
    }
    return false;
}

bool Enemies::isAnyEnemyTouchingUpBorder(){
    for(unsigned int i = 0; i < attackingEnemyIndexVector.size(); i++){
        int itEnemyIndex = attackingEnemyIndexVector[i];
        int enemyPositionY = enemyVector[itEnemyIndex].position.y;
        int initialPositionY = enemyVector[itEnemyIndex].initialPosition.y;
        if(enemyPositionY <= initialPositionY)
            return true;
    }
    return false;
}
