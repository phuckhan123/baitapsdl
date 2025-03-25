#include "utils.h"
#include "constants.h" // Để sử dụng HIGH_SCORE_FILE
using namespace std;
int loadHighScore() {
    ifstream file(HIGH_SCORE_FILE);
    int highScore = 0;
    if (file.is_open()) {
        file >> highScore;
        file.close();
    }
    return highScore;
}

void saveHighScore(int score) {
    ofstream file(HIGH_SCORE_FILE);
    if (file.is_open()) {
        file << score;
        file.close();
    }
}

void logErrorAndExit(const char* msg, const char* error) {
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
    SDL_Quit();
}
