
#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <fstream>
#include <SDL.h>

// Hàm load điểm cao nhất từ file
int loadHighScore();

// Hàm lưu điểm cao nhất vào file
void saveHighScore(int score);

// Hàm log lỗi và thoát chương trình
void logErrorAndExit(const char* msg, const char* error);

#endif
