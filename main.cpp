#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cmath>
#include <SDL_ttf.h>

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
    srand(time(0)); // Khởi tạo seed cho random
    vector<Obstacle> obstacles;
    const int numObstacles = 5; // Số lượng cột (cặp cột) ban đầu
    const int minHeight = 50;
    const int maxHeight = 300;
    const int gapSize = 200;
    const int distanceBetween = 300;
    vector<int> obstacleXPositions;
    int lastX = SCREEN_WIDTH;
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
                if (e.key.keysym.sym == SDLK_UP && gameState == GameState::PLAYING) {
                    Bird.velocity = Bird.jumpPower;
                }
                // Reset game khi ở trạng thái GAME_OVER và nhấn phím space
                if (e.key.keysym.sym == SDLK_SPACE && gameState == GameState::GAME_OVER) {
                    gameState = GameState::PLAYING;
                    resetGame(Bird, obstacles, score);
                }
            }
        }

        // Cập nhật game chỉ khi ở trạng thái PLAYING
        if (gameState == GameState::PLAYING) {
            bird.tick();
            currentObstacleSpeed = obstacleBaseSpeed + 1*(score / 10);
            currentBackgroundScrollSpeed = backgroundBaseScrollSpeed + 1*(score / 10);
            background.scroll(currentBackgroundScrollSpeed); // Cập nhật tốc độ cuộn

            Bird.updateBird();

            // Di chuyển và tái tạo chướng ngại vật
            bool moveObstaclesVertically = (score >= 10);

            // Di chuyển và tái tạo chướng ngại vật
           for (size_t i = 0; i < obstacles.size(); i += 2) {
                obstacles[i].speed = currentObstacleSpeed;
                obstacles[i + 1].speed = currentObstacleSpeed;
                obstacles[i].move();
                obstacles[i + 1].move();

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

            // Kiểm tra va chạm
            int birdCenterX = Bird.x + 40; // Tâm hình tròn
            int birdCenterY = Bird.y + 40;
            int birdRadius = 40;
            for (auto& obs : obstacles) {
                if (checkCircleRectCollision(birdCenterX, birdCenterY, birdRadius, obs.rect)) {
                    // Chuyển sang trạng thái GAME_OVER khi va chạm
                    gameState = GameState::GAME_OVER;
                    if (score > highScore) {
                        highScore = score;
                        saveHighScore(highScore);
                    }
                }
            }
            if (score >= WINNING_SCORE) { // Thay 100 bằng WINNING_SCORE nếu dùng hằng số
                gameState = GameState::WIN;
            }
        }

        // Vẽ
        graphics.prepareScene();
        graphics.renderTexture(background.texture, background.scrollingOffset, 0);
        graphics.renderTexture(background.texture, background.scrollingOffset - background.width, 0);

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
    graphics.quit();
    return 0;
}
