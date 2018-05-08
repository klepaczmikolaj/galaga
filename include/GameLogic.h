#include "Enemies.h"
#include "EnemiesGreen.h"
#include "EnemiesRed.h"
#include "Bullets.h"
#include "Fighter.h"
#include "Logger.h"
#include "BezierCurve.h"
#include "Bonus.h"

#ifndef GAMELOGIC_H
#define GAMELOGIC_H


class GameLogic{
    public:
        GameLogic();
        ~GameLogic();
        Logger *logger;
        int lives;
        int highScore;
        int score;

        Enemies *enemies;
        EnemiesGreen *enemiesGreen;
        EnemiesRed *enemiesRed;
        Bullets bullets;
        Fighter fighter;
        Bonus bonus;

        std::fstream highScoreFile;

        bool isGameStarted;
        bool windowClose;
        bool isGamePaused;
        bool isGameSaved;
        bool isGameLoaded;
        bool doubleBullets;

        bool isGameOver();
        bool isTheBulletShot();

        void setFighterInitialPosition();
        void updateGame();
        void appendHighScore();
        void shootTheBullet();
        void shootDoubleBullets();

    private:
        float baseFighterSpeed;
        float fighterSpeed;
        int rightBorder;
        int leftBorder;
        int upBorder;
        int downBorder;
        int hitScoreAppend;
        float bulletPositionX;
        float bulletPositionX2;
        float bulletPositionY;

        bool isPlayerImmortal;

        std::fstream saveGameFile;
        std::string yellowFilename;
        std::string greenFilename;
        std::string redFilename;
        std::string scoreLivesMapFilename;

        sf::Time time;
        sf::Clock backgroundClock;
        sf::Clock bulletClock;
        sf::Vector2f deadEnemyPosition;

        std::vector <int> attackingEnemyIndexVector;

        void saveGame(Enemies*, std::string);
        void pauseGame();
        void eventLoop();
        void uploadTextures();
        void moveBackground();
        void handleKeyboardInput();
        void moveFighterLeft();
        void moveFighterRight();
        void updateBullets();
        void removeBullets();
        void saveScoreLivesMap();
        void loadScore(std::string, int&, int&);
        void loadScore(std::string, int&);
        void appendLogger(Enemies::Enemy);
        void appendLoggerFighter();
        void addToScore();
        void updateLives();
        void restartGame();
        void setHorisontalDirection();
        void applyBonus();
        void resetBonus();
        void initializeGameJson(std::string, Enemies*);
        void setMap(int);

        bool isPlayerHit();
        bool isEnemyOnBoard();
        bool isBonusDrawn(sf::Vector2f);

        sf::Vector2f removeDeadEnemyWithBullet(Enemies *enem);
        sf::Vector2f removeDeadEnemyWithBullet();

        template<typename T>
        bool areTexturesCovered(T t, Enemies::Enemy enemy){
            int enemyLeft = enemy.position.x;
            int enemyRight = enemyLeft + enemies->sizeX;
            int enemyUp = enemy.position.y;
            int enemyDown = enemyUp + enemies->sizeY;

            int TLeft = t.position.x;
            int TRight = TLeft + t.sizeX;
            int TUp = t.position.y;
            int TDown = TUp + t.sizeY;

            return enemyUp < TDown && enemyDown > TUp && enemyLeft < TRight && enemyRight > TLeft;
        }
};

#endif // GAMELOGIC_H

