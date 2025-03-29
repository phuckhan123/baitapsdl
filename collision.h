#ifndef COLLISION_H
#define COLLISION_H

#include <SDL.h>

// Hàm kiểm tra va chạm hình tròn và hình chữ nhật
bool checkCircleRectCollision(int cx, int cy, int r, SDL_Rect rect);

#endif
