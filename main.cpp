#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cmath>
#include <SDL_ttf.h>
#include <random>
#include "constants.h"
#include "utils.h"
#include "sdl_helper.h"
#include "structs.h"
#include "game.h"
#include "collision.h"
using namespace std;

int main(int argc, char* argv[]) {
    int score = 0;
    int highScore = loadHighScore();
    Graphics graphics;
    graphics.init();
    Box Bird;
    SDL_Texture* pipeNorth = graphics.loadTexture("pipeNorth.png");
    SDL_Texture* pipeSouth = graphics.loadTexture("pipeSouth.png");
    TTF_Init();
    TTF_Font* font = TTF_OpenFont("Game Bubble.ttf", 28);
    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Color red = { 255, 0, 0, 255 };
    SDL_Color yellow = { 255, 255, 0, 255 };
    srand(time(0)); // Khởi tạo seed cho random
    vector<Obstacle> obstacles;
    const int numObstacles = 5; // Số lượng cột (cặp cột) ban đầu
    const int minHeight = 50;
    const int maxHeight = 300;
    const int gapSize = 200;
    const int distanceBetween = 300;
    vector<int> obstacleXPositions;
    int lastX = SCREEN_WIDTH;

    // Khởi tạo random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> heightVariation(-15, 15); // Điều chỉnh chiều cao từ -15 đến 15 (giảm phạm vi)
    int minBottomHeight = 50; // Chiều cao tối thiểu cho ống dưới
    std::uniform_int_distribution<> coinPosition(50, SCREEN_HEIGHT - 50);

     // Load texture cho coin
    SDL_Texture* coinTexture = graphics.loadTexture("coin.png");
    // Số coin hiện tại và total
    int currentRunCoins = 0;
    int globalCoins = loadGlobalCoins();
    int lives = 1;
    std::uniform_int_distribution<> lifeUpsPos(50, SCREEN_HEIGHT - 50);
    // Load texture cho life
    SDL_Texture* lifeTexture = graphics.loadTexture("life.png");
    // Tạo các life
    vector<LifeUp> lifeUps;
    for(int i =0; i < 5; ++i)
    {
     LifeUp life;
     life.rect.x = 1500 + i*3000;
     life.rect.y = lifeUpsPos(gen);
     life.rect.w = 30;
     life.rect.h = 30;
     lifeUps.push_back(life);
}

    // Khởi tạo coin
    vector<Coin> coinsVector;
    for (int i = 0; i < 10; i++)
    {
        Coin coin;
        coin.rect.x = 1500 + i * 300;
        coin.rect.y = coinPosition(gen);
        coin.rect.w = 30;
        coin.rect.h = 30;
        coinsVector.push_back(coin);
    }
    // Khởi tạo chướng ngại vật ban đầu
    for (int i = 0; i < numObstacles; i++) {
        int height = minHeight + rand() % (maxHeight - minHeight);
        int xPosition = lastX + distanceBetween;
        obstacles.push_back({ {xPosition, 0, 60, height} });
        obstacles.push_back({ {xPosition, height + gapSize + (rand() % 100), 60, SCREEN_HEIGHT - (height + gapSize)} });
        obstacleXPositions.push_back(xPosition);
        lastX = xPosition;
    }
    Sprite bird;
    SDL_Texture* birdTexture = graphics.loadTexture(BIRD_SPRITE_FILE);
    bird.init(birdTexture, BIRD_FRAMES, BIRD_CLIPS);
    ScrollingBackground background;
    background.setTexture(graphics.loadTexture("4622710.png"));
    bool quit = false;
    SDL_Event e;
    GameState gameState = GameState::PLAYING;

    // Tốc độ ban đầu và hiện tại của chướng ngại vật
    int obstacleBaseSpeed = 8;
    int currentObstacleSpeed = 0;

    //Tốc độ cuộn của background
    int backgroundBaseScrollSpeed = 8;
    int currentBackgroundScrollSpeed = 8;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;

            // Xử lý sự kiện nhấn phím
            if (e.type == SDL_KEYDOWN) {
                // Nhảy khi đang chơi
                if ((e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_SPACE) && gameState == GameState::PLAYING) {
                    Bird.velocity = Bird.jumpPower;
                }
                // Reset game khi ở trạng thái GAME_OVER và nhấn phím space
                     if (e.key.keysym.sym == SDLK_SPACE && gameState == GameState::GAME_OVER) {
                          gameState = GameState::PLAYING;
                          resetGame(Bird, obstacles, score,coinsVector,lifeUps, coinPosition,lifeUpsPos,gen,currentRunCoins,SCREEN_WIDTH,SCREEN_HEIGHT);
                 }
            }
        }

        // Cập nhật game chỉ khi ở trạng thái PLAYING
        if (gameState == GameState::PLAYING) {
            bird.tick();
            currentObstacleSpeed = obstacleBaseSpeed + 1 * (score / 10);
            currentBackgroundScrollSpeed = backgroundBaseScrollSpeed + 1 * (score / 10);
            background.scroll(currentBackgroundScrollSpeed); // Cập nhật tốc độ cuộn

            Bird.updateBird();

            //Kiểm tra và tạo coin
            for (size_t i = 0; i < coinsVector.size(); i++) {
                coinsVector[i].speed = currentObstacleSpeed;
                coinsVector[i].move();
            }
            //Tái tạo coin khi coin đi hết màn hình
             for (size_t i = 0; i < coinsVector.size(); i++)
            {
                 if (coinsVector[i].rect.x + coinsVector[i].rect.w < 0) {
                    coinsVector[i].rect.x = 4500 + i*300;
                    coinsVector[i].rect.y = coinPosition(gen);
                    coinsVector[i].collected = false;
                }
            }

           //Kiểm tra va chạm coin
           for (size_t i = 0; i < coinsVector.size(); i++)
            {
                SDL_Rect coinRect = coinsVector[i].rect;
                SDL_Rect birdRect = { Bird.x, Bird.y, 92, 84 };
                if (!coinsVector[i].collected && SDL_HasIntersection(&birdRect, &coinRect)) {
                    currentRunCoins++;
                    globalCoins++;
                    coinsVector[i].collected = true;
                }
            }
            //Kiểm tra và tạo life

        for (size_t i = 0; i < lifeUps.size(); i++) {
            lifeUps[i].speed = currentObstacleSpeed;
            lifeUps[i].move();
        }
        //Tái tạo life khi life đi hết màn hình
         for (size_t i = 0; i < lifeUps.size(); i++)
        {
             if (lifeUps[i].rect.x + lifeUps[i].rect.w < 0) {
                lifeUps[i].rect.x = 16500 + i*3000;
                lifeUps[i].rect.y = lifeUpsPos(gen);
                lifeUps[i].collected = false;
            }
        }
         //Kiểm tra chạm life
         for (size_t i = 0; i < lifeUps.size(); i++)
            {

               SDL_Rect birdRect = { Bird.x, Bird.y, 92, 84 };
            if (!lifeUps[i].collected && SDL_HasIntersection(&birdRect, &lifeUps[i].rect)) {
                 lives++;
                 if (lives >= 5 ) lives = 5;
                  lifeUps[i].collected = true;
            }
        }



            // Di chuyển và tái tạo chướng ngại vật
            for (size_t i = 0; i < obstacles.size(); i += 2) {
                obstacles[i].speed = currentObstacleSpeed;
                obstacles[i + 1].speed = currentObstacleSpeed;
                obstacles[i].move();
                obstacles[i + 1].move();
                //Thay đổi chiều cao của chướng ngại vật
                if ( (score>=30 and score<= 40) or ( score >=  70 and score <= 80 )) {
                   int variation = heightVariation(gen); // Lấy giá trị ngẫu nhiên

                        // Thay đổi chiều cao của cả hai ống sao cho tổng chiều cao không đổi
                         obstacles[i].rect.h += variation;
                        obstacles[i + 1].rect.h -= variation;

                        //Giữ cố định phần dưới
                        obstacles[i+1].rect.y = obstacles[i].rect.h + gapSize;
                        // Giới hạn chiều cao của ống trên và ống dưới
                        if (obstacles[i].rect.h < minHeight) {
                            obstacles[i].rect.h = minHeight;
                           obstacles[i+1].rect.h =SCREEN_HEIGHT-obstacles[i].rect.h - gapSize;
                        }
                        if (obstacles[i].rect.h > maxHeight) {
                            obstacles[i].rect.h = maxHeight;
                             obstacles[i+1].rect.h =SCREEN_HEIGHT-obstacles[i].rect.h - gapSize;

                        }
                        if (obstacles[i+1].rect.h < minBottomHeight)
                        {
                            obstacles[i+1].rect.h = minBottomHeight;
                            obstacles[i].rect.h = SCREEN_HEIGHT - gapSize - obstacles[i+1].rect.h;
                        }
                }

                if (Bird.x > obstacles[i].rect.x + obstacles[i].rect.w && !obstacles[i].passed) {
                    score++;
                    obstacles[i].passed = true;
                }
                if (obstacles[i].rect.x + obstacles[i].rect.w < 0) {
                    int newX;
                    newX = 1500;
                    int newHeight = minHeight + rand() % (maxHeight - minHeight);
                    obstacles[i].rect = { newX, 0, 60, newHeight };
                    obstacles[i + 1].rect = { newX, newHeight + gapSize + (rand() % 100), 60, SCREEN_HEIGHT - (newHeight + gapSize) };
                    obstacleXPositions.push_back(newX);
                    if (obstacleXPositions.size() > numObstacles) {
                        obstacleXPositions.erase(obstacleXPositions.begin());
                    }
                    lastX = newX;
                    obstacles[i].passed = false;
                }
            }

           //Kiểm tra va chạm

            int birdCenterX = Bird.x + 45; // Tâm hình tròn
            int birdCenterY = Bird.y + 40;
            int birdRadius = 30;
        for (auto& obs : obstacles) {
            if (checkCircleRectCollision(birdCenterX, birdCenterY, birdRadius, obs.rect)) {
                 if(lives>1)
                  {
                     lives-=1;
                        Bird.y = 50;
                        Bird.velocity = 0;
                        int lastX = SCREEN_WIDTH;
         for (size_t i = 0; i < obstacles.size(); i += 2) {
        int newHeight = minHeight + rand() % (maxHeight - minHeight); // Tạo chiều cao ngẫu nhiên
         obstacles[i].rect.x = lastX + distanceBetween;
          obstacles[i+1].rect.x = obstacles[i].rect.x;
        obstacles[i].rect.h = newHeight; // Đặt chiều cao ngẫu nhiên
         obstacles[i+1].rect.h = SCREEN_HEIGHT -newHeight - gapSize; // Đặt chiều cao ngẫu nhiên
        obstacles[i+1].rect.y = obstacles[i].rect.h + gapSize; // Đặt vị trí Y để tạo khoảng cách
         lastX = obstacles[i+1].rect.x;
    }
                  }
                else{
                // Chuyển sang trạng thái GAME_OVER khi va chạm
                gameState = GameState::GAME_OVER;
                if (score > highScore) {
                    highScore = score;
                    saveHighScore(highScore);
                }
                saveGlobalCoins(globalCoins);


                if (score >= WINNING_SCORE) { // Thay 100 bằng WINNING_SCORE nếu dùng hằng số
                 gameState = GameState::WIN;
                  saveGlobalCoins(globalCoins);
        }
                }

    }
        }
        }

        // Vẽ
        graphics.prepareScene();
        graphics.renderTexture(background.texture, background.scrollingOffset, 0);
        graphics.renderTexture(background.texture, background.scrollingOffset - background.width, 0);

           //Vẽ coin
            for (size_t i = 0; i < coinsVector.size(); i++)
            {
                if (!coinsVector[i].collected) {
                     graphics.renderTexture(coinTexture, coinsVector[i].rect.x, coinsVector[i].rect.y);
                }
            }
                     //Vẽ life up
         for (size_t i = 0; i < lifeUps.size(); i++)
        {
            if (!lifeUps[i].collected) {
                 graphics.renderTexture(lifeTexture, lifeUps[i].rect.x, lifeUps[i].rect.y);
            }
        }
        // Vẽ chướng ngại vật
        SDL_SetRenderDrawColor(graphics.renderer, 0, 255, 0, 255);
        for (size_t i = 0; i < obstacles.size(); i += 2) {
            renderObstacle(graphics.renderer, pipeNorth, pipeSouth, obstacles[i].rect, obstacles[i + 1].rect);
        }
        graphics.render(Bird.x, Bird.y, bird, Bird.angle);

        // Vẽ UI
    std::string scoreText = "Score: " + std::to_string(score);
    graphics.renderText(scoreText.c_str(), 20, 20, white, font);
    string highScoreText = "High Score: " + to_string(highScore);
    graphics.renderText(highScoreText.c_str(), 20, 50, white, font);

    // Draw current run coin
    string currentRunCoin = ":" + to_string(currentRunCoins);
     graphics.renderText(currentRunCoin.c_str(),SCREEN_WIDTH - 50,20,yellow,font);
    graphics.renderTexture( coinTexture,SCREEN_WIDTH - 80,20);
    //ve total coin
    std::string globalCoinText = "Total Coin:"+ to_string(globalCoins);
    graphics.renderText(globalCoinText.c_str(),SCREEN_WIDTH - 200,50,yellow,font);
     std::string livesText = ":" + to_string(lives);
     graphics.renderText(livesText.c_str(),50,80,red,font);
      graphics.renderLife(lifeTexture,20,80,30,30);
    // Vẽ thông báo GAME OVER
    if (gameState == GameState::GAME_OVER) {
        graphics.renderText("GAME OVER! PRESS SPACE TO RESTART", SCREEN_WIDTH / 2 - 250, SCREEN_HEIGHT / 2 - 20, white, font);
    }
     if (gameState == GameState::WIN) {
         graphics.renderText("YOU WIN! CONGRATULATIONS!", SCREEN_WIDTH / 2 - 250, SCREEN_HEIGHT / 2 - 20, white, font);
     }
        graphics.presentScene();
        SDL_Delay(50);
    }

    SDL_DestroyTexture(pipeNorth);
    SDL_DestroyTexture(pipeSouth);
    SDL_DestroyTexture(birdTexture);
    SDL_DestroyTexture(background.texture);
    SDL_DestroyTexture(coinTexture);
    graphics.quit();
    return 0;
}
