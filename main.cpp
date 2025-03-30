#include "Constants.h"
#include "Graphics.h"
#include "map.h"
#include "Spaceship.h"
#include "Text.h"
#include "menu.h"

using namespace std;

Graphics background;
Spaceship spaceship;
TTF_Font* font_time = NULL;
TTF_Font* game_over_font = NULL;
Text score_text;
Text game_over_text;
Mix_Music* sound_track_ = NULL;
Menu game_menu;
bool game_started = false;

bool initSDL(){
    bool success = true;
    int rec = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    if(rec < 0) return false;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(window == NULL){
        success = false;
    }else{
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if(renderer == NULL){
            success = false;
        }else{
            SDL_SetRenderDrawColor(renderer, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            int imgFlags = IMG_INIT_PNG;
            int imgFlag = IMG_INIT_JPG;
            if(!(IMG_Init(imgFlags | imgFlag) && (imgFlags|imgFlag))){
                success = false;
            }
        }

        if(TTF_Init() == -1){
            success = false;
        }

        if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
            success = false;
        }

        font_time = TTF_OpenFont("font/Courier.ttf", 24);
        if(font_time == NULL){
            success = false;
        }
    }
    return success;
}

bool loadBackground(){
    bool ret = background.Load_IMG(BACKGROUND_.c_str(), renderer);
    if(ret == false) return false;
    return true;
}

bool loadSpaceship(){
    bool ret = spaceship.LoadIMG(renderer);
    if(ret == false) return false;
    return true;
}

bool loadShootSound(){
    return spaceship.LoadShootSound("sound/bulletsound.wav");
}

bool loadSoundTrack(){
    sound_track_ = Mix_LoadMUS("sound/soundtrack.mp3");
    if(sound_track_ == NULL){
        return false;
    }
    return true;
}

bool loadGameOverFont() {
    game_over_font = TTF_OpenFont("font/BebasNeue-Regular.ttf", 100);
    if(game_over_font == NULL){
        return false;
    }
    return true;
}

void close(){
    if(game_over_font != NULL){
        TTF_CloseFont(game_over_font);
        game_over_font = NULL;
    }
    if(sound_track_ != NULL){
        Mix_FreeMusic(sound_track_);
        sound_track_ = NULL;
    }

    background.Free();
    spaceship.Free();
    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    if(font_time != NULL){
        TTF_CloseFont(font_time);
        font_time = NULL;
    }

    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char *argv[]){
    if(initSDL() == false)
        return -1;
    if(loadBackground() == false)
        return -1;
    if(loadSpaceship() == false)
        return -1;
    if(loadSoundTrack() == false)
        return -1;
    if(loadShootSound() == false)
        return -1;
    if(loadGameOverFont() == false)
        return -1;

    if (!game_menu.Init(renderer, font_time)) {
        printf("Failed to initialize menu\n");
        return -1;
    }

    Mix_PlayMusic(sound_track_, -1); //phat nhac vong lap vo han

    GameMap game_map;
    game_map.loadMap("tilesmap/map01.dat");
    game_map.loadTiles(renderer);

    Text pause;

    bool quit = false;
    bool is_pause = false;
    bool win = false;
    const int SCREEN_FPS = 60;
    const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;

    while(!quit){
        Uint32 frame_start = SDL_GetTicks();

        while(SDL_PollEvent(&event) != 0){
            if(event.type == SDL_QUIT){
                quit = true;
            }

            if (game_menu.GetCurrentScreen() != GAMEPLAY) {
                game_menu.HandleEvents(event, quit, game_started);
            } else {

                if(is_pause){
                    if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r){
                        spaceship = Spaceship();
                        spaceship.LoadIMG(renderer);
                        spaceship.LoadShootSound("sound//bulletsound.wav");
                        game_map = GameMap();
                        game_map.loadMap("tilesmap/map01.dat");
                        game_map.loadTiles(renderer);
                        is_pause = false;
                    }
                    if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE){

                        game_menu.SetCurrentScreen(MAIN_MENU);
                        game_started = false;

                        spaceship = Spaceship();
                        spaceship.LoadIMG(renderer);
                        spaceship.LoadShootSound("sound//bulletsound.wav");
                        game_map = GameMap();
                        game_map.loadMap("tilesmap/map01.dat");
                        game_map.loadTiles(renderer);
                        is_pause = false;
                    }
                }
                else if(win){
                    if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r){
                        spaceship = Spaceship();
                        spaceship.LoadIMG(renderer);
                        spaceship.LoadShootSound("sound//bulletsound.wav");
                        game_map = GameMap();
                        game_map.loadMap("tilesmap/map01.dat");
                        game_map.loadTiles(renderer);
                        win = false;
                    }
                    if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE){

                        game_menu.SetCurrentScreen(MAIN_MENU);
                        game_started = false;

                        spaceship = Spaceship();
                        spaceship.LoadIMG(renderer);
                        spaceship.LoadShootSound("sound//bulletsound.wav");
                        game_map = GameMap();
                        game_map.loadMap("tilesmap/map01.dat");
                        game_map.loadTiles(renderer);
                        win = false;
                    }
                }
                if (!spaceship.IsGameOver()) {
                    if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_p && !win){
                        is_pause = !is_pause;
                    }

                    if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE){

                        game_menu.SetCurrentScreen(MAIN_MENU);
                        game_started = false;
                    }

                    if (!is_pause) {
                        game_map.UpdateMapPos(spaceship.GetRect().x);            //cuon man
                        spaceship.Update(game_map.GetMap().start_x_, game_map);  //cap nhat tau va va cham
                        spaceship.HandleInputAction(event, renderer);
                    }
                } else {      //neu game over
                    if(event.type == SDL_KEYDOWN){
                        switch(event.key.keysym.sym){
                        case SDLK_r:  //reset game
                            spaceship = Spaceship();
                            spaceship.LoadIMG(renderer);
                            spaceship.LoadShootSound("sound//bulletsound.wav");
                            game_map = GameMap();
                            game_map.loadMap("tilesmap/map01.dat");
                            game_map.loadTiles(renderer);
                            is_pause = false;
                            break;
                        case SDLK_ESCAPE:

                            game_menu.SetCurrentScreen(MAIN_MENU);
                            game_started = false;

                            spaceship = Spaceship();
                            spaceship.LoadIMG(renderer);
                            spaceship.LoadShootSound("sound//bulletsound.wav");
                            game_map = GameMap();
                            game_map.loadMap("tilesmap/map01.dat");
                            game_map.loadTiles(renderer);
                            break;
                        }
                    }
                }
            }
        }


        SDL_SetRenderDrawColor(renderer, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
        SDL_RenderClear(renderer);

        if (game_menu.GetCurrentScreen() != GAMEPLAY) {
            // Render menu
            game_menu.Render(renderer);
        } else {
            if (!spaceship.IsGameOver()) {
                if (!is_pause) {
                    game_map.UpdateMapPos(spaceship.GetRect().x);
                    spaceship.Update(game_map.GetMap().start_x_, game_map);

                    background.render(renderer);
                    game_map.DrawMap(renderer);

                    game_map.GenerateEnemyBullets(renderer);
                    game_map.HandleEnemyBullets(renderer);

                    spaceship.HandleBullet(renderer, game_map);
                    spaceship.Show(renderer);

                    score_text.SetText("Score: " + std::to_string(spaceship.GetScore()));
                    score_text.SetColor(Text::WHITE_TEXT);
                    score_text.LoadFromRenderText(font_time, renderer);
                    score_text.RenderText(renderer, (SCREEN_WIDTH - score_text.GetWidth()) / 2, 10);

                    pause.SetText("Pause: [P]");
                    pause.SetColor(Text::WHITE_TEXT);
                    pause.LoadFromRenderText(font_time, renderer);
                    pause.RenderText(renderer, 1100, 10);

                    if(spaceship.GetWin()){
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
                        SDL_Rect overlay = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
                        SDL_RenderFillRect(renderer, &overlay);

                        background.render(renderer);

                        Text pause_title;
                        pause_title.SetText("YOU WIN!");
                        pause_title.SetColor(Text::WHITE_TEXT);
                        pause_title.LoadFromRenderText(game_over_font, renderer);
                        pause_title.RenderText(renderer,
                                            (SCREEN_WIDTH - pause_title.GetWidth()) / 2,
                                             SCREEN_HEIGHT / 2 - 200);

                        Text reset_text;
                        reset_text.SetText("Reset Game [R]");
                        reset_text.SetColor(Text::WHITE_TEXT);
                        reset_text.LoadFromRenderText(font_time, renderer);
                        reset_text.RenderText(renderer,
                                            (SCREEN_WIDTH - reset_text.GetWidth()) / 2,
                                             SCREEN_HEIGHT / 2 + 50);

                        Text exit_text;
                        exit_text.SetText("Exit to Menu [ESC]");
                        exit_text.SetColor(Text::WHITE_TEXT);
                        exit_text.LoadFromRenderText(font_time, renderer);
                        exit_text.RenderText(renderer,
                                            (SCREEN_WIDTH - exit_text.GetWidth()) / 2,
                                            SCREEN_HEIGHT / 2 + 150);
                        win = true;
                    }
                } else {
                    // neu game pause
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
                    SDL_Rect overlay = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
                    SDL_RenderFillRect(renderer, &overlay);

                    background.render(renderer);

                    Text pause_title;
                    pause_title.SetText("GAME PAUSED!");
                    pause_title.SetColor(Text::WHITE_TEXT);
                    pause_title.LoadFromRenderText(game_over_font, renderer);
                    pause_title.RenderText(renderer,
                                          (SCREEN_WIDTH - pause_title.GetWidth()) / 2,
                                           SCREEN_HEIGHT / 2 - 200);

                    Text continue_text;
                    continue_text.SetText("Continue [P]");
                    continue_text.SetColor(Text::WHITE_TEXT);
                    continue_text.LoadFromRenderText(font_time, renderer);
                    continue_text.RenderText(renderer,
                                            (SCREEN_WIDTH - continue_text.GetWidth()) / 2,
                                             SCREEN_HEIGHT / 2 - 50);
                    Text reset_text;
                    reset_text.SetText("Reset Game [R]");
                    reset_text.SetColor(Text::WHITE_TEXT);
                    reset_text.LoadFromRenderText(font_time, renderer);
                    reset_text.RenderText(renderer,
                                         (SCREEN_WIDTH - reset_text.GetWidth()) / 2,
                                         SCREEN_HEIGHT / 2 + 50);

                    Text exit_text;
                    exit_text.SetText("Exit to Menu [ESC]");
                    exit_text.SetColor(Text::WHITE_TEXT);
                    exit_text.LoadFromRenderText(font_time, renderer);
                    exit_text.RenderText(renderer,
                                        (SCREEN_WIDTH - exit_text.GetWidth()) / 2,
                                        SCREEN_HEIGHT / 2 + 150);
                }
            } else { //neu game over
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);

                background.render(renderer);

                game_over_text.SetText("GAME OVER!");
                game_over_text.SetColor(Text::WHITE_TEXT);
                game_over_text.LoadFromRenderText(game_over_font, renderer);
                game_over_text.RenderText(renderer,
                    (SCREEN_WIDTH - game_over_text.GetWidth()) / 2,
                    SCREEN_HEIGHT / 2 - 150
                );

                score_text.SetText("Your Score: " + std::to_string(spaceship.GetScore()));
                score_text.SetColor(Text::WHITE_TEXT);
                score_text.LoadFromRenderText(font_time, renderer);
                score_text.RenderText(renderer,
                    (SCREEN_WIDTH - score_text.GetWidth()) / 2,
                    SCREEN_HEIGHT / 2
                );

                Text restart_text;
                restart_text.SetText("Press [R] to Restart or [ESC] for Menu");
                restart_text.SetColor(Text::WHITE_TEXT);
                restart_text.LoadFromRenderText(font_time, renderer);
                restart_text.RenderText(renderer,
                    (SCREEN_WIDTH - restart_text.GetWidth()) / 2,
                    SCREEN_HEIGHT / 2 + 100);
            }
        }

        SDL_RenderPresent(renderer);

        // Cap framerate
        Uint32 frame_time = SDL_GetTicks() - frame_start;
        if(frame_time < SCREEN_TICK_PER_FRAME){
            SDL_Delay(SCREEN_TICK_PER_FRAME - frame_time);
        }
    }

    close();
    return 0;
}
