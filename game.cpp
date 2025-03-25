#include "game.h"
#include "constants.h"
#include <cstdlib>

void resetGame(Box& Bird, std::vector<Obstacle>& obstacles, int& score) {
    Bird.y = 50;
    Bird.velocity = 0;
    obstacles.clear();
    score = 0;
    int lastX = SCREEN_WIDTH;
    for (int i = 0; i < 5; i++) {
        int height = 50 + rand() % 250;
        int x = lastX + 300;
        obstacles.push_back({ {x, 0, 60, height} });
        obstacles.push_back({ {x, height + 200, 60, SCREEN_HEIGHT - (height + 200)} });
        lastX = x;
    }
}

void renderObstacle(SDL_Renderer* renderer, SDL_Texture* pipeNorth, SDL_Texture* pipeSouth, SDL_Rect topRect, SDL_Rect bottomRect) {
    SDL_RenderCopy(renderer, pipeNorth, NULL, &topRect);
    SDL_RenderCopy(renderer, pipeSouth, NULL, &bottomRect);
}
