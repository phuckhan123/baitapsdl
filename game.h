#ifndef GAME_H
#define GAME_H
#include <SDL.h>
#include <vector>
#include "structs.h"
#include<random>

// Hàm reset game
 void resetGame(Box& Bird, std::vector<Obstacle>& obstacles, int& score, std::vector<Coin>& coinsVector, std::vector<LifeUp>& lifeUps, std::uniform_int_distribution<>& coinPosition, std::uniform_int_distribution<>& lifePosition, std::mt19937& gen, int currentRunCoins, int SCREEN_WIDTH, int SCREEN_HEIGHT);

// Hàm render chướng ngại vật
void renderObstacle(SDL_Renderer* renderer, SDL_Texture* pipeNorth, SDL_Texture* pipeSouth, SDL_Rect topRect, SDL_Rect bottomRect);

#endif
