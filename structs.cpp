#include "structs.h"
#include <algorithm>
#include "sdl_helper.h"
#include<string>

// Định nghĩa các hàm của struct Box
void Box::updateBird() {
    velocity += gravity;
    if (velocity > 13) velocity = 13;  // Giới hạn tốc độ rơi
    y += velocity;

    double bounceFactor = 1.5; // Hệ số giảm độ nảy (có thể điều chỉnh)

    // Nếu chim chạm vào đỉnh màn hình
    if (y < 0) {
        y = 0;
        velocity = -velocity * bounceFactor; // Đảo ngược vận tốc và giảm dần
    }

    // Nếu chim chạm vào đáy màn hình
    if (y + 84 >= SCREEN_HEIGHT) {
        y = SCREEN_HEIGHT - 84;
        velocity = -velocity * bounceFactor; // Đảo ngược vận tốc và giảm dần

        // Nếu vận tốc quá nhỏ, dừng nảy (tránh nảy vô hạn)
        if (abs(velocity) < 1) velocity = 0;
    }
    if (velocity < 0) {
        // Bay lên => xoay lên (tối đa -30 độ)
        angle = std::max(-60.00, velocity * 3);
    }
    else if (velocity >= 11) {
        // Rơi nhanh => xoay xuống (tối đa 90 độ)
        angle = std::min(90.00, angle + 5);
    }
    else {
        // Khi rơi chậm => từ từ về 0
        angle *= 0.9;
    }
}

// Định nghĩa các hàm của struct ScrollingBackground
void ScrollingBackground::setTexture(SDL_Texture* _texture) {
    texture = _texture;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);
}

void ScrollingBackground::scroll(int distance) {
    scrollingOffset -= distance;
    if (scrollingOffset < 0) { scrollingOffset = width; }
}

// Định nghĩa các hàm của struct Sprite
void Sprite::init(SDL_Texture* _texture, int frames, const int _clips[][4]) {
    texture = _texture;

    SDL_Rect clip;
    for (int i = 0; i < frames; i++) {
        clip.x = _clips[i][0];
        clip.y = _clips[i][1];
        clip.w = _clips[i][2];
        clip.h = _clips[i][3];
        clips.push_back(clip);
    }
}

void Sprite::tick() {
    currentFrame = (currentFrame + 1) % clips.size();
}

const SDL_Rect* Sprite::getCurrentClip() const {
    return &(clips[currentFrame]);
}

// Định nghĩa các hàm của struct Graphics
void Graphics::logErrorAndExit(const char* msg, const char* error) {
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
    SDL_Quit();
}

void Graphics::init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logErrorAndExit("SDL_Init", SDL_GetError());

    window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    //full screen
    //window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (window == nullptr) logErrorAndExit("CreateWindow", SDL_GetError());

    if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
        logErrorAndExit("SDL_image error:", IMG_GetError());
     SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
        SDL_RENDERER_PRESENTVSYNC);
    //Khi chạy trong máy ảo (ví dụ phòng máy ở trường)
    //renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));

    if (renderer == nullptr) logErrorAndExit("CreateRenderer", SDL_GetError());

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void Graphics::prepareScene(SDL_Texture* background) {
    SDL_RenderClear(renderer);
    if (background != nullptr) SDL_RenderCopy(renderer, background, NULL, NULL);
}

void Graphics::presentScene() {
    SDL_RenderPresent(renderer);
}

SDL_Texture* Graphics::loadTexture(const char* filename) {
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);

    SDL_Texture* texture = IMG_LoadTexture(renderer, filename);
    if (texture == NULL)
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Load texture %s", IMG_GetError());

    return texture;
}

void Graphics::renderTexture(SDL_Texture* texture, int x, int y) {
    SDL_Rect dest;

    dest.x = x;
    dest.y = y;
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

    SDL_RenderCopy(renderer, texture, NULL, &dest);
}

void Graphics::blitRect(SDL_Texture* texture, SDL_Rect* src, int x, int y) {
    SDL_Rect dest;

    dest.x = x;
    dest.y = y;
    dest.w = src->w;
    dest.h = src->h;

    SDL_RenderCopy(renderer, texture, src, &dest);
}

void Graphics::quit() {
    IMG_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Graphics::render(int x, int y, const Sprite& sprite, double angle) {
    const SDL_Rect* clip = sprite.getCurrentClip();
    SDL_Rect renderQuad = { x, y, clip->w, clip->h };

    SDL_RenderCopyEx(renderer, sprite.texture, clip, &renderQuad, angle, NULL, SDL_FLIP_NONE);
}

void Graphics::renderText(const char* text, int x, int y, SDL_Color color, TTF_Font* font) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dest = { x, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

// Định nghĩa các hàm của struct Obstacle
void Obstacle::move() {
    rect.x -= speed;  // Di chuyển sang trái

}
void renderCoinWithCount(Graphics& graphics, SDL_Texture* coinIconTexture, int x, int y, int count, SDL_Color color, TTF_Font* font) {
    int coinIconWidth = 30;
    int coinIconHeight = 30;
    SDL_Rect coinIconRect = {x, y, coinIconWidth, coinIconHeight};
    SDL_RenderCopy(graphics.renderer, coinIconTexture, NULL, &coinIconRect);

    std::string coinText = std::to_string(count);
    int coinTextX = x + coinIconWidth + 5;
    int coinTextY = y;
    graphics.renderText(coinText.c_str(), coinTextX, coinTextY, color, font);
}
