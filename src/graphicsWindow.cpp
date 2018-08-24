#include "../include/graphicsWindow.h"

GraphicsWindow::GraphicsWindow(){
    yellowEnemyScale = 0.125f;
    greenEnemyScale = 0.14f;
    redEnemyScale = 0.12f;
    fighterScale = 0.25f;
    bulletScale = 0.2f;
    bonusScale = 0.25f;
    frameLimit = 100;
    windowSizeX = 800;
    windowSizeY = 600;
    window.create(sf::VideoMode(windowSizeX, windowSizeY), "GALAGA");
    window.setFramerateLimit(frameLimit);
    fileNameYellowEnemy = "png/enemyYellow.png";
    fileNameGreenEnemy = "png/enemyGreen.png";
    fileNameRedEnemy = "png/enemyRed.png";
}

GraphicsWindow::~GraphicsWindow(){

}

void GraphicsWindow::setBackground(){
    if(!textureBakground.loadFromFile("png/background.png")){
        gameLogic.logger->appendLogger(Logger::severity::error,"Unable to open png/background.png");
        window.close();
    }
    backgroundTextureUp = 0;
    backgroundTextureDown = backgroundTextureUp + 600;
    background.setTextureRect(sf::IntRect(0,backgroundTextureUp,windowSizeY,backgroundTextureDown));
    background.setTexture(textureBakground);
}

void GraphicsWindow::setFighter(){
    if(!textureFighter.loadFromFile("png/fighter.png")){
        gameLogic.logger->appendLogger(Logger::severity::error,"Unable to open png/fighter.png");
        window.close();
    }
    fighterSprite.setTexture(textureFighter);
    fighterSprite.setTextureRect(sf::IntRect(0,0,200,200));
    fighterSprite.setScale(fighterScale,fighterScale);
    gameLogic.fighter.sizeX = fighterSprite.getLocalBounds().width*fighterScale;
    gameLogic.fighter.sizeY = fighterSprite.getLocalBounds().height*fighterScale;
    gameLogic.setFighterInitialPosition();
}

void GraphicsWindow::setBullet(){
    if(!textureBullet.loadFromFile("png/bullet.png")){
        gameLogic.logger->appendLogger(Logger::severity::error,"Unable to open png/bullet.png");
        window.close();
    }
    bulletSprite.setTexture(textureBullet);
    bulletSprite.setScale(bulletScale,bulletScale);
    gameLogic.bullets.sizeX = bulletSprite.getLocalBounds().width*bulletScale;
    gameLogic.bullets.sizeY = bulletSprite.getLocalBounds().height*bulletScale;
}

void GraphicsWindow::setBonus(){
    if(!textureBonus.loadFromFile("png/bonus.png")){
        gameLogic.logger->appendLogger(Logger::severity::error,"Unable to open png/bonus.png");
        window.close();
    }
    bonusSprite.setTexture(textureBonus);
    bonusSprite.setScale(bonusScale,bonusScale);
    gameLogic.bonus.sizeX = bonusSprite.getLocalBounds().width*bonusScale;
    gameLogic.bonus.sizeY = bonusSprite.getLocalBounds().height*bonusScale;
}

void GraphicsWindow::setEnemy(std::string &fileName, sf::Texture &texture, sf::Sprite &sprite, float &scale){
    if(!texture.loadFromFile(fileName)){
        std::string out = "Unable to open " + fileName;
        gameLogic.logger->appendLogger(Logger::severity::error,out);
        window.close();
    }
    sprite.setTexture(texture);
    sprite.setScale(scale, scale);
    gameLogic.enemies->sizeX = sprite.getLocalBounds().width*scale;
    gameLogic.enemies->sizeY = sprite.getLocalBounds().height*scale;
}

void GraphicsWindow::setGameInfo(){
    if(!font.loadFromFile("txt/joystix monospace.ttf")){
        gameLogic.logger->appendLogger(Logger::severity::error,"unable to read font from txt/joystix monospace.ttf");
        std::cout << "Unable to read font";
        window.close();
    }
    score.setFont(font);
    score.setString("SCORE \n  0");
    score.setCharacterSize(20);
    score.setFillColor(sf::Color::Red);
    score.setPosition(600,20);

    lives.setFont(font);
    lives.setString("LIVES 3");
    lives.setCharacterSize(20);
    lives.setFillColor(sf::Color::Red);
    lives.setPosition(600,180);

    highScore.setFont(font);
    highScore.setString("HIGH SCORE \n  0");
    highScore.setCharacterSize(20);
    highScore.setFillColor(sf::Color::Red);
    highScore.setPosition(600,100);

    gameOver.setFont(font);
    gameOver.setString("GAME OVER");
    gameOver.setCharacterSize(50);
    gameOver.setFillColor(sf::Color::White);
    gameOver.setPosition(100,265);

    startGame.setFont(font);
    startGame.setString("HIT ENTER TO START");
    startGame.setCharacterSize(30);
    startGame.setFillColor(sf::Color::White);
    startGame.setPosition(70,235);

    gamePaused.setFont(font);
    gamePaused.setString("GAME PAUSED");
    gamePaused.setCharacterSize(40);
    gamePaused.setFillColor(sf::Color::White);
    gamePaused.setPosition(100,265);

    gameSaved.setFont(font);
    gameSaved.setString("GAME SAVED");
    gameSaved.setCharacterSize(40);
    gameSaved.setFillColor(sf::Color::White);
    gameSaved.setPosition(100,265);
}

void GraphicsWindow::gameLoop(){
    uploadTextures();
    backgroundClock.restart();
    while (window.isOpen()){
        showGameInfo();
        moveBackground();
        eventLoop();

        gameLogic.updateGame();

        if(!gameLogic.isGameStarted){
            displayStartGameScreen();
        }

        else if(!gameLogic.isGameOver() && !gameLogic.isGamePaused)
            clearAndDisplay();
        else if(gameLogic.isGamePaused){
            if(gameLogic.isGameSaved)
                displaySaveGameScreen();
            else
                displayPausedGameScreen();
        }
        else
            displayGameOverScreen();


        if(gameLogic.windowClose == true)
            window.close();
    }
    gameLogic.appendHighScore();
    gameLogic.highScoreFile.close();
}

void GraphicsWindow::uploadTextures(){
    setBackground();
    setFighter();
    setBullet();
    setBonus();
    setEnemy(fileNameYellowEnemy, textureEnemyYellow, enemyYellowSprite, yellowEnemyScale);
    setEnemy(fileNameGreenEnemy, textureEnemyGreen, enemyGreenSprite, greenEnemyScale);
    setEnemy(fileNameRedEnemy, textureEnemyRed, enemyRedSprite, redEnemyScale);
    setGameInfo();
}

void GraphicsWindow::showGameInfo(){
    std::stringstream newHighScoreString;
    newHighScoreString << "HIGH SCORE \n  " << gameLogic.highScore;
    highScore.setString(newHighScoreString.str());

    std::stringstream newScoreString;
    newScoreString << "SCORE \n  " << gameLogic.score;
    score.setString(newScoreString.str());

    std::stringstream newLivesString;
    newLivesString << "LIVES " << gameLogic.lives;
    lives.setString(newLivesString.str());
}

void GraphicsWindow::moveBackground(){
    time = backgroundClock.getElapsedTime();
    if(time.asMilliseconds() > 20){
        backgroundClock.restart();
        backgroundTextureUp = (backgroundTextureUp + 2) % 1200;
        backgroundTextureDown = backgroundTextureUp + 600;
        background.setTextureRect(sf::IntRect(0,1800 - backgroundTextureDown,
                    windowSizeY,1800 - backgroundTextureUp));
    }
}

void GraphicsWindow::drawBasicBoard(){
    window.clear();
    window.draw(background);
    window.draw(score);
    window.draw(lives);
    window.draw(highScore);
}

void GraphicsWindow::clearAndDisplay(){
    drawBasicBoard();

    fighterSprite.setPosition(gameLogic.fighter.position);
    window.draw(fighterSprite);

    if(gameLogic.isTheBulletShot())
        for(unsigned int i = 0; i < gameLogic.bullets.bulletVector.size(); i++){
            bulletSprite.setPosition(gameLogic.bullets.bulletVector[i].position);
            window.draw(bulletSprite);
        }

    if(gameLogic.bonus.isVisible)
        drawBonus();

    printEnemies(gameLogic.enemies, enemyYellowSprite);
    printEnemies(gameLogic.enemiesGreen, enemyGreenSprite);
    printEnemies(gameLogic.enemiesRed, enemyRedSprite);
    window.display();
}

void GraphicsWindow::drawBonus(){
    bonusSprite.setPosition(gameLogic.bonus.position);
    window.draw(bonusSprite);
}

void GraphicsWindow::displayGameOverScreen(){
    drawBasicBoard();
    window.draw(gameOver);
    window.display();
}

void GraphicsWindow::printEnemies(Enemies *enemies, sf::Sprite sprite){
    for(unsigned int i = 0; i < enemies->enemyVector.size(); i++){
        sprite.setPosition(enemies->enemyVector[i].position);
        window.draw(sprite);
    }
}

void GraphicsWindow::displayStartGameScreen(){
    drawBasicBoard();
    if(gameLogic.isGameLoaded)
        startGame.setString("   GAME LOADED\n\nHIT ENTER TO START ");
    else
        startGame.setString("SELECT MAP (1-3) AND\n HIT ENTER TO START");

    window.draw(startGame);
    window.display();
}

void GraphicsWindow::displayPausedGameScreen(){
    drawBasicBoard();
    window.draw(gamePaused);
    window.display();
}

void GraphicsWindow::displaySaveGameScreen(){
    drawBasicBoard();
    window.draw(gameSaved);
    window.display();
}

void GraphicsWindow::eventLoop(){
    sf::Event event;
    while (window.pollEvent(event)){
        if (event.type == sf::Event::Closed)
            window.close();
        else if(event.type == sf::Event::KeyReleased){
            if(event.key.code == sf::Keyboard::Up && gameLogic.isGameStarted){
                if(gameLogic.doubleBullets)
                    gameLogic.shootDoubleBullets();
                else
                    gameLogic.shootTheBullet();
            }
        }
    }
}
