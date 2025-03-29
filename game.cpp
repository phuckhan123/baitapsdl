#include "game.h"
#include "constants.h"
#include <cstdlib>
#include<random>

void resetGame(Box& Bird, std::vector<Obstacle>& obstacles, int& score, std::vector<Coin>& coinsVector, std::vector<LifeUp>& lifeUps, std::uniform_int_distribution<>& coinPosition, std::uniform_int_distribution<>& lifePosition, std::mt19937& gen, int currentRunCoins, int SCREEN_WIDTH, int SCREEN_HEIGHT)
{
Bird.y = 50;
Bird.velocity = 0;
obstacles.clear();
lifeUps.clear();
coinsVector.clear();
score = 0;
currentRunCoins = 0;
for (Coin &coin : coinsVector) {
coin.collected = false;
coin.rect.x = 1500+ (rand() % 10)*300;
coin.rect.y = coinPosition(gen);
}
for (LifeUp &life : lifeUps) {
life.collected = false;
life.rect.x = 1500+ (rand() % 10)*300 + 400;
life.rect.y = lifePosition(gen);
}
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
