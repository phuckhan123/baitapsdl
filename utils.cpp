#include "utils.h"
#include "constants.h"
#include <fstream> // Cần include thư viện này

// Hàm load điểm cao nhất từ file
int loadHighScore() {
    std::ifstream file(HIGH_SCORE_FILE);
    int highScore = 0;
    if (file.is_open()) {
        file >> highScore;
        file.close();
    }
    return highScore;
}

// Hàm lưu điểm cao nhất vào file
void saveHighScore(int score) {
    std::ofstream file(HIGH_SCORE_FILE);
    if (file.is_open()) {
        file << score;
        file.close();
    }
}

// Hàm load số coin đã tích lũy từ file
int loadGlobalCoins() {
    std::ifstream file("globalcoins.txt");
    int globalCoins = 0;
    if (file.is_open()) {
        file >> globalCoins;
        file.close();
    }
    return globalCoins;
}

// Hàm lưu số coin đã tích lũy vào file
void saveGlobalCoins(int coins) {
    std::ofstream file("globalcoins.txt");
    if (file.is_open()) {
        file << coins;
        file.close();
    }
}

// Hàm log lỗi và thoát chương trình
void logErrorAndExit(const char* msg, const char* error) {
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
    SDL_Quit();
}
