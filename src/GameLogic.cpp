#include "../include/GameLogic.h"

GameLogic::GameLogic(){
    logger = new Logger(Logger::outputType::file);
    leftBorder = 10;
    rightBorder = 590;
    upBorder = 10;
    downBorder = 590;
    highScore = 0;
    score = 0;
    hitScoreAppend = 100;
    baseFighterSpeed = 2.5f;
    fighterSpeed = baseFighterSpeed;
    lives = 3;
    enemies = new Enemies(leftBorder, rightBorder, upBorder, downBorder);
    enemyVector.push_back(enemies);
    enemiesGreen = new EnemiesGreen(leftBorder, rightBorder, upBorder, downBorder);
    enemyVector.push_back(enemiesGreen);
    enemiesRed = new EnemiesRed(leftBorder, rightBorder, upBorder, downBorder);
    enemyVector.push_back(enemiesRed);
    windowClose = false;
    isPlayerImmortal = false;
    isGameStarted = false;
    isGamePaused = false;
    isGameSaved = false;
    isGameLoaded = false;
    doubleBullets = false;
    yellowFilename = "txt/saveGameFileYellow.txt";
    greenFilename = "txt/saveGameFileGreen.txt";
    redFilename = "txt/saveGameFileRed.txt";
    scoreLivesMapFilename = "txt/save.txt";
    loadScore("txt/highscore.txt", highScore);
    setMap(1);
}

GameLogic::~GameLogic(){
    delete logger;
    delete enemies;
    delete enemiesGreen;
    delete enemiesRed;
}

void GameLogic::updateGame(){
    if(!isGameStarted){
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return)){
            isGameStarted = true;
            if(isGameLoaded){
                for(auto iter : enemyVector)
                    iter->initializeGroupFile();
                loadScore(scoreLivesMapFilename, score, lives);
                isGameLoaded = false;
            }
            else{
                for(auto iter : enemyVector)
                    iter->initializeGameJson();
            }
        }

        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            windowClose = true;
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::L))
            isGameLoaded = true;
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1) && !isGameLoaded)
            setMap(1);
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2) && !isGameLoaded)
            setMap(2);
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num3) && !isGameLoaded)
            setMap(3);
    }
    else if(!isGameOver() && !isGamePaused){
        if(isTheBulletShot())
            updateBullets();

        for(auto iter : enemyVector)
            iter->handleEnemies();

        if(isEnemyOnBoard()){
            deadEnemyPosition = removeDeadEnemyWithBullet();
        }

        bonus.handleBonus(deadEnemyPosition, fighter);
        if(bonus.toApply)
            applyBonus();
        if(bonus.isActive && bonus.activeClock.getElapsedTime().asSeconds() > bonus.activeClockInterval)
            resetBonus();

        if(isPlayerHit() && !isPlayerImmortal){
            appendLoggerFighter();
            for(auto iter : enemyVector)
                iter->initializeGameJson();
            setFighterInitialPosition();
            updateLives();
            resetBonus();
        }
        handleKeyboardInput();
    }
    else if(isGamePaused){
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::R)){
            restartGame();
            isGameSaved = false;
            isGamePaused = false;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Y)){
            for(auto iter : enemyVector)
                iter->attackClock.restart();
            isGamePaused = false;
            isGameSaved = false;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
            for(auto iter : enemyVector)
                saveGame(iter);
            saveScoreLivesMap();
            isGameSaved = true;
        }
    }
    else
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::R))
            restartGame();
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            windowClose = true;
}

void GameLogic::saveGame(Enemies *enem){
    saveGameFile.open(enem->enemySaveFilename,std::ios::out);
    if(enem->enemyVector.empty())
        saveGameFile << std::endl;
    for(auto enemy : enem->enemyVector)
        saveGameFile << (int)enemy.initialPosition.x << " " << (int)enemy.initialPosition.y << " ";

    saveGameFile.close();
}

void GameLogic::saveScoreLivesMap(){
    std::fstream saveGameFile;
    saveGameFile.open(scoreLivesMapFilename, std::ios::out);
    saveGameFile << score << " " << lives << " " << enemies->mapFilename;
    saveGameFile.close();
}

bool GameLogic::isTheBulletShot(){
    return !bullets.bulletVector.empty();
}

bool GameLogic::isEnemyOnBoard(){
    for(auto iter : enemyVector){
        if(!iter->enemyVector.empty())
            return true;
        else
            continue;
    }
    return false;
}

void GameLogic::handleKeyboardInput(){
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        moveFighterRight();
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        moveFighterLeft();
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::R))
        restartGame();
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        windowClose = true;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::P)){
        pauseGame();
    }
}

void GameLogic::pauseGame(){
    isGamePaused = true;
}

void GameLogic::moveFighterRight(){
    if(fighter.position.x < rightBorder-fighter.sizeX)
        fighter.move(fighterSpeed,0);
}

void GameLogic::moveFighterLeft(){
    if(fighter.position.x > leftBorder)
        fighter.move(-fighterSpeed,0);
}

void GameLogic::shootTheBullet(){
    bulletPositionX = fighter.position.x + (fighter.sizeX-bullets.sizeX)/2;
    bulletPositionY = fighter.position.y - fighter.sizeY/2;
    Bullets::Bullet bullet;
    bullet.setPosition(bulletPositionX,bulletPositionY);
    bullet.sizeX = bullets.sizeX;
    bullet.sizeY = bullets.sizeY;
    bullets.bulletVector.push_back(bullet);
    bulletClock.restart();
}

void GameLogic::shootDoubleBullets(){
    bulletPositionX = fighter.position.x + (fighter.sizeX-bullets.sizeX) * 0.1;
    bulletPositionX2 = fighter.position.x + (fighter.sizeX-bullets.sizeX) * 0.9;
    bulletPositionY = fighter.position.y - fighter.sizeY/2;

    Bullets::Bullet bullet;
    Bullets::Bullet bullet2;

    bullet.setPosition(bulletPositionX,bulletPositionY);
    bullet2.setPosition(bulletPositionX2, bulletPositionY);

    bullet.sizeX = bullets.sizeX;
    bullet.sizeY = bullets.sizeY;
    bullet2.sizeX = bullets.sizeX;
    bullet2.sizeY = bullets.sizeY;

    bullets.bulletVector.push_back(bullet);
    bullets.bulletVector.push_back(bullet2);
    bulletClock.restart();
}

void GameLogic::updateBullets(){
    time = bulletClock.getElapsedTime();
    if(time.asMilliseconds() > 20){
        for(std::vector<Bullets::Bullet>::iterator it = bullets.bulletVector.begin(); it != bullets.bulletVector.end(); it++)
            it->move(0, -20);
        removeBullets();
        bulletClock.restart();
    }
}

void GameLogic::removeBullets(){
    for(std::vector<Bullets::Bullet>::iterator it = bullets.bulletVector.begin(); it != bullets.bulletVector.end(); it++){
        bulletPositionY = it->position.y;
        if(bulletPositionY <= upBorder){
            bullets.bulletVector.erase(it);
            return;
        }
    }
}

sf::Vector2f GameLogic::removeDeadEnemyWithBullet(){
    sf::Vector2f outputPosition;

    for(auto iter : enemyVector){
        outputPosition = removeDeadEnemyWithBullet(iter);
        if(outputPosition.x >= 0 && outputPosition.y >= 0)
            return outputPosition;
    }

    return outputPosition;
}

sf::Vector2f GameLogic::removeDeadEnemyWithBullet(Enemies *enemies){
    sf::Vector2f outputPosition;
    outputPosition.x = -1;
    outputPosition.y = -1;

    for(std::vector<Enemies::Enemy>::iterator itEnemy = enemies->enemyVector.begin(); itEnemy != enemies->enemyVector.end(); itEnemy++){
        for(std::vector<Bullets::Bullet>::iterator itBullet = bullets.bulletVector.begin(); itBullet != bullets.bulletVector.end(); itBullet++){
            if(areTexturesCovered(*itBullet, *itEnemy)){
                addToScore();
                logger->appendLogger(Logger::severity::info,*itEnemy);
                logger->appendLogger(Logger::severity::info,*itBullet);

                int hitEnemyIndex = (itEnemy - enemies->enemyVector.begin());

                outputPosition = itEnemy->position;

                //erase bullet enemy and attackingEnemyIndex
                bullets.bulletVector.erase(itBullet);
                enemies->enemyVector.erase(itEnemy);

                if(enemies->attackingEnemyIndexVector.empty())
                    enemies->isAttackStarted = false;
                //if itEnemy is attacking erase proper element from attackingVector
                else if(enemies->isSpecifiedEnemyAlreadyAttacking(hitEnemyIndex))
                    enemies->attackingEnemyIndexVector.erase
                    (std::find(enemies->attackingEnemyIndexVector.begin(),enemies->attackingEnemyIndexVector.end(),hitEnemyIndex));

                //update attackingEnemyIndexVector because erase(itEnemy), changed vector element indexes
                for(unsigned int i = 0; i < enemies->attackingEnemyIndexVector.size(); i++){
                    if(enemies->attackingEnemyIndexVector[i] > hitEnemyIndex)
                        enemies->attackingEnemyIndexVector[i]--;
                }

                return outputPosition;
            }
            if(!isTheBulletShot())
                return outputPosition;
        }
    }
    return outputPosition;
}

void GameLogic::appendLoggerFighter(){
    logger->appendLogger(Logger::severity::info,fighter);
}

bool GameLogic::isPlayerHit(){
    for(auto iter : enemyVector)
        for(auto itEnemy = iter->enemyVector.begin(); itEnemy != iter->enemyVector.end(); itEnemy++)
            if(areTexturesCovered(fighter,*itEnemy))
                return true;

    return false;
}

void GameLogic::setFighterInitialPosition(){
    fighter.setPosition(275,540);
}

void GameLogic::loadScore(std::string filename, int &score, int &lives){
    highScoreFile.open(filename, std::ios::in);
    std::string mapFilename;
    if(!highScoreFile.is_open()){
        std::cout << "Unable to read " << filename << " file" << std::endl << "Press any key to exit" << std::endl;
        windowClose = true;
        std::cin.ignore();
    }
    else
        highScoreFile >> score >> lives >> mapFilename;

    for(auto iter : enemyVector)
        iter->mapFilename = mapFilename;

    highScoreFile.close();
}

void GameLogic::loadScore(std::string filename, int &score){
    highScoreFile.open(filename, std::ios::in);
    if(!highScoreFile.is_open()){
        std::cout << "Unable to read " << filename << " file" << std::endl << "Press any key to exit" << std::endl;
        windowClose = true;
        std::cin.ignore();
    }
    else
        highScoreFile >> score;

    highScoreFile.close();
}

void GameLogic::addToScore(){
    score += hitScoreAppend;
    if(score > highScore)
        highScore = score;
}

void GameLogic::updateLives(){
    if(lives != 0)
        lives--;
}

void GameLogic::appendHighScore(){
    highScoreFile.open("txt/highscore.txt", std::ios::out);
    highScoreFile << highScore;
    highScoreFile.close();
}

void GameLogic::restartGame(){
    lives = 4;
    score = -hitScoreAppend;
    updateLives();
    addToScore();
    isGameStarted = false;
    isGameLoaded = false;
    resetBonus();
    setFighterInitialPosition();
    bullets.bulletVector.clear();
}

bool GameLogic::isGameOver(){
    return lives <= 0;
}

void GameLogic::applyBonus(){
    bonus.isActive = true;
    bonus.activeClock.restart();
    if(bonus.type == bonus.BonusType::fasterFighter)
        fighterSpeed = 5.f;
    else if(bonus.type == bonus.BonusType::slowerFighter)
        fighterSpeed = 1.f;
    else
        doubleBullets = true;

    bonus.toApply = false;
}

void GameLogic::resetBonus(){
    bonus.isVisible = false;
    bonus.isActive = false;
    fighterSpeed = baseFighterSpeed;
    doubleBullets = false;
}

void GameLogic::setMap(int number){
    std::string filename = "json/map";
    filename += to_string(number);
    filename += ".json";
    for(auto iter : enemyVector)
        iter->mapFilename = filename;
}
