 #include "constants.h"

 // Định nghĩa các biến hằng (chỉ một lần)
 const char* HIGH_SCORE_FILE = "highscore.txt";
 const char* BIRD_SPRITE_FILE = "83127-sprite-area-line-animated-bird-film.png";
 const int BIRD_CLIPS[][4] = {
      {1, 1, 90, 82}, {93, 1, 90, 82}, {185, 1, 90, 82}, {275, 1, 90, 82}, {369, 1, 90, 82},
     {1, 85, 90, 82}, {93, 85, 90, 82}, {185, 85, 90, 82}, {277, 85, 90, 82}, {369, 85, 90, 82},
     {1, 169, 90, 82}, {93, 169, 90, 82}, {183, 169, 90, 82}, {277, 169, 90, 82}
 };
 const int BIRD_FRAMES = sizeof(BIRD_CLIPS) / sizeof(int) / 4;
 const int SCREEN_WIDTH = 800;
 const int SCREEN_HEIGHT = 600;
 const int WINNING_SCORE = 100;
 const char* WINDOW_TITLE = "Flappy Bird";
