#include "GameLogic.h"

#ifndef GRAPHICSWINDOW_H
#define GRAPHICSWINDOW_H


class GraphicsWindow{
    public:
        GraphicsWindow();
        ~GraphicsWindow();
        void gameLoop();

    private:
        void setBackground();
        void setFighter();
        void setBullet();
        void setBonus();
        void setEnemy(std::string &fileName, sf::Texture &texture, sf::Sprite &sprite, float &scale );
        void setGameInfo();
        void setFighterInitialPosition();
        void clearAndDisplay();
        bool isTheBulletShot();
        bool isEnemyOnBoard();
        void drawBasicBoard();
        void printEnemies(Enemies *enemies, sf::Sprite sprite);
        void displayGameOverScreen();
        void displayStartGameScreen();
        void displayPausedGameScreen();
        void displaySaveGameScreen();
        void drawBonus();

        void uploadTextures();
        void showGameInfo();
        void moveBackground();
        void eventLoop();


        GameLogic gameLogic;
        sf::RenderWindow window;
        sf::Texture textureBakground;
        sf::Texture textureFighter;
        sf::Texture textureBullet;
        sf::Texture textureEnemyYellow;
        sf::Texture textureEnemyGreen;
        sf::Texture textureEnemyRed;
        sf::Texture textureBonus;
        sf::Sprite background;
        sf::Sprite fighterSprite;
        sf::Sprite bulletSprite;
        sf::Sprite bonusSprite;
        sf::Sprite enemyYellowSprite;
        sf::Sprite enemyRedSprite;
        sf::Sprite enemyGreenSprite;
        sf::Font font;
        sf::Text score;
        sf::Text lives;
        sf::Text highScore;
        sf::Text gameOver;
        sf::Text startGame;
        sf::Text gameSaved;
        sf::Time time;
        sf::Text gamePaused;
        sf::Clock backgroundClock;

        float yellowEnemyScale;
        float redEnemyScale;
        float greenEnemyScale;
        float fighterScale;
        float bulletScale;
        float bonusScale;
        int frameLimit;
        int windowSizeX;
        int windowSizeY;
        int backgroundTextureUp;
        int backgroundTextureDown;
        std::string fileNameYellowEnemy;
        std::string fileNameGreenEnemy;
        std::string fileNameRedEnemy;


        template < typename T > std::string to_string( const T& n )
        {
            std::ostringstream stm ;
            stm << n ;
            return stm.str() ;
        }

    private:



};

#endif // GRAPHICSWINDOW_H
