#ifndef SDL_HELPER_H
#define SDL_HELPER_H

#include <SDL.h>
#include <SDL_image.h>

// Khai báo các hàm SDL
SDL_Window* initSDL(int SCREEN_WIDTH, int SCREEN_HEIGHT, const char* WINDOW_TITLE);
SDL_Renderer* createRenderer(SDL_Window* window);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
SDL_Texture* loadTexture(const char* filename, SDL_Renderer* renderer);
void renderTexture(SDL_Texture* texture, int x, int y, SDL_Renderer* renderer);

#endif
