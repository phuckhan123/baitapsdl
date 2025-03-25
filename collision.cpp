
    #include "collision.h"
    #include <algorithm>

    bool checkCircleRectCollision(int cx, int cy, int r, SDL_Rect rect) {
        int closestX = std::max(rect.x, std::min(cx, rect.x + rect.w));
        int closestY = std::max(rect.y, std::min(cy, rect.y + rect.h));

        int distanceX = cx - closestX;
        int distanceY = cy - closestY;
        int distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);

        return distanceSquared < (r * r);
    }

