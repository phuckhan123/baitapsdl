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
 const int numObstacles = 5; // Số lượng cột (cặp cột) ban đầu
    const int minHeight = 50;
    const int maxHeight = 300;
    const int gapSize = 200;
    const int distanceBetween = 300;


 // Enum cho trạng thái game
 enum class GameState {
     PLAYING,
     GAME_OVER,
     WIN
 };

 #endif
