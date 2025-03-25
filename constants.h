#ifndef CONSTANTS_H
 #define CONSTANTS_H

 #include <SDL.h>

 // Khai báo (chứ không định nghĩa) các biến hằng
 extern const char* HIGH_SCORE_FILE;
 extern const char* BIRD_SPRITE_FILE;
 extern const int BIRD_CLIPS[][4];
 extern const int BIRD_FRAMES;
 extern const int SCREEN_WIDTH;
 extern const int SCREEN_HEIGHT;
 extern const char* WINDOW_TITLE;
 extern const int WINNING_SCORE;

 // Enum cho trạng thái game
 enum class GameState {
     PLAYING,
     GAME_OVER,
     WIN
 };

 #endif
