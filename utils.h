#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <fstream>
#include <SDL.h>

// Hàm load điểm cao nhất từ file
int loadHighScore();

// Hàm lưu điểm cao nhất vào file
void saveHighScore(int score);

// Hàm load số coin đã tích lũy từ file
int loadGlobalCoins();

// Hàm lưu số coin đã tích lũy vào file
void saveGlobalCoins(int coins);

// Hàm log lỗi và thoát chương trình
void logErrorAndExit(const char* msg, const char* error);

#endif
