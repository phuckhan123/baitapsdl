#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <vector>
#include "structs.h"

// Hàm reset game
 void resetGame(Box& Bird, std::vector<Obstacle>& obstacles, int& score, int &coins);

// Hàm render chướng ngại vật
void renderObstacle(SDL_Renderer* renderer, SDL_Texture* pipeNorth, SDL_Texture* pipeSouth, SDL_Rect topRect, SDL_Rect bottomRect);

#endif
