#include "graphics.h"
#include "game.h"
#include "config.h"

void update(float ms)
{
    Game* game = reinterpret_cast<Game*> (graphics::getUserData());
    game->update();
}
 
// The window content drawing function.
void draw()
{
    Game* game = reinterpret_cast<Game*> (graphics::getUserData());
    game->draw();
}
 
int main()
{
    Game pong;

    graphics::createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Pong");

    graphics::setUserData(&pong);
 
    graphics::setDrawFunction(draw);
    graphics::setUpdateFunction(update);
 
    graphics::setCanvasSize(CANVAS_WIDTH, CANVAS_HEIGHT);
    graphics::setCanvasScaleMode(graphics::CANVAS_SCALE_FIT);
    
    //pong.init();
    graphics::startMessageLoop();
 
    return 0;
}
