#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>
#include <SDL.h>
#include <windows.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 640;
const int SCREEN_BPP = 32;

static SDL_Renderer *renderer = NULL;
static SDL_Event event;
static SDL_Window *window = NULL;

const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int COLOR_KEY_B = 180;
const int RENDER_DRAW_COLOR = 0xff;

const float AUTO_SCROLL_SPEED = 3.5f;
const float SPEED_SPACESHIP = 7.0f;

const std::string WINDOW_TITLE = "Run Now";
const std::string BACKGROUND_ = "img//background_galaxy.png";
const std::string SPACESHIP = "img//spaceship.png";
const std::string BULLET = "img//bullet.png";
#define TILE_SIZE 64

#define MAX_MAP_X 400
#define MAX_MAP_Y 10

typedef struct Map{
    int start_x_;  // vi tri bat dau
    int start_y_;

    int max_x_;   // vi tri o
    int max_y_;

    int tile[MAX_MAP_Y][MAX_MAP_X];
    char* file_name_;
};
#endif // CONSTANTS_H


