 #include "constants.h"

 // Định nghĩa các biến hằng (chỉ một lần)
 const char* HIGH_SCORE_FILE = "highscore.txt";
 const char* BIRD_SPRITE_FILE = "83127-sprite-area-line-animated-bird-film.png";
 const int BIRD_CLIPS[][4] = {
     {0, 0, 92, 84}, {92, 0, 92, 84}, {184, 0, 92, 84}, {276, 0, 92, 84}, {368, 0, 92, 84},
     {0, 84, 92, 84}, {92, 84, 92, 84}, {184, 84, 92, 84}, {276, 84, 92, 84}, {368, 84, 92, 84},
     {0, 168, 92, 84}, {92, 168, 92, 84}, {184, 168, 92, 84}, {276, 168, 92, 84}
 };
 const int BIRD_FRAMES = sizeof(BIRD_CLIPS) / sizeof(int) / 4;
 const int SCREEN_WIDTH = 800;
 const int SCREEN_HEIGHT = 600;
 const int WINNING_SCORE = 100;
 const char* WINDOW_TITLE = "Flappy Bird";
