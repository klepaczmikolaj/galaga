#include "json.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <fstream>

#ifndef ENEMIES_H
#define ENEMIES_H

using json = nlohmann::json;

class Enemies{
    public:
        Enemies(int, int, int, int);
        virtual ~Enemies();
        float sizeX;
        float sizeY;

        class Enemy{
            public:
                Enemy();
                sf::Vector2f position;
                //for attacker
                sf::Vector2f initialPosition;

                void setPosition(float, float);
                void setInitialPosition(float, float);
                void move(float, float);
                friend std::ostream& operator<<(std::ostream& os, const Enemy& enemy);

        };

        std::vector <Enemy> enemyVector;
        std::vector <int> attackingEnemyIndexVector;
        std::string mapFilename;
        std::string enemySaveFilename;
        sf::Clock attackClock;
        bool isAttackStarted;

        virtual std::string getName();
        virtual void handleEnemies();
        void initializeGameJson();
        void initializeGroupFile();
        bool isSpecifiedEnemyAlreadyAttacking(int);

    protected:
        sf::Clock movementClock;
        sf::Clock attackMovementClock;
        std::fstream saveGameFile;

        double mommentOfAttack;
        int horisontalEnemyMovementDistance;
        int leftBorder;
        int rightBorder;
        int upBorder;
        int downBorder;
        int enemyMaxXNumber;
        int numberOfRaws;
        int initialEnemyNumber;
        int attackSpeed;
        int attackingInterval;
        int startLeft;
        int startUp;

        bool directionRight;
        bool directionDown;
        bool directionRightInit;


        bool isAnyEnemyTouchingDownBorder();
        bool isAnyEnemyTouchingUpBorder();

        virtual void moveVertical();
        void setHorisontalDirection();
        void initializeGame();
        void drawAttackingEnemies();
        void appendSpeed();
        void moveHorisontal();
};

#endif // ENEMIES_H

