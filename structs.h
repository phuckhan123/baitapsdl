#ifndef STRUCTS_H
#define STRUCTS_H

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <cmath>
#include <SDL_ttf.h>
#include "constants.h"

struct Box {
    int x = 50;
    int y = 0;
    double velocity = 0;   // Vận tốc
    double gravity = 1.5;  // Gia tốc trọng lực
    double jumpPower = -15; // Lực nhảy
    double angle = 0;

    void updateBird(); // Khai báo, định nghĩa ở file .cpp
};

struct ScrollingBackground {
    SDL_Texture* texture;
    int scrollingOffset = 0;
    int width, height;

    void setTexture(SDL_Texture* _texture); // Khai báo
    void scroll(int distance);               // Khai báo
};

struct Sprite {
    SDL_Texture* texture;
    std::vector<SDL_Rect> clips;
    int currentFrame = 0;

    void init(SDL_Texture* _texture, int frames, const int _clips[][4]); // Khai báo
    void tick();                                                            // Khai báo
    const SDL_Rect* getCurrentClip() const;                                 // Khai báo
};

struct Graphics {
    SDL_Renderer* renderer;
    SDL_Window* window;

    void logErrorAndExit(const char* msg, const char* error); // Khai báo
    void init();                                                // Khai báo
    void prepareScene(SDL_Texture* background = nullptr);     // Khai báo
    void presentScene();                                       // Khai báo
    SDL_Texture* loadTexture(const char* filename);           // Khai báo
    void renderTexture(SDL_Texture* texture, int x, int y);   // Khai báo
    void blitRect(SDL_Texture* texture, SDL_Rect* src, int x, int y); // Khai báo
    void quit();                                               // Khai báo
    void render(int x, int y, const Sprite& sprite, double angle); // Khai báo
    void renderText(const char* text, int x, int y, SDL_Color color, TTF_Font* font); // Khai báo
};

struct Obstacle {
    SDL_Rect rect;
    int speed = 0;
    bool passed = false;

    void move(); // Khai báo
};
 struct Coin {
    SDL_Rect rect;
    bool collected = false;
    int speed = 8;

    void move() {
        rect.x -= speed;
    }
};
// Khai báo hàm renderCoinWithCount là hàm toàn cục
void renderCoinWithCount(Graphics& graphics, SDL_Texture* coinIconTexture, int x, int y, int count, SDL_Color color, TTF_Font* font);


#endif
