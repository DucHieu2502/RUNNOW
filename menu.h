#ifndef MENU_H
#define MENU_H

#include "Constants.h"
#include "Graphics.h"
#include "Text.h"
#include <vector>

enum MenuScreens {
    MAIN_MENU,
    GUIDE_SCREEN,
    GAMEPLAY
};

class MenuItem {
public:
    MenuItem(const std::string& text, int x, int y, TTF_Font* font, SDL_Renderer* renderer);
    ~MenuItem();

    void Render(SDL_Renderer* renderer);
    void SetSelected(bool selected);
    SDL_Rect GetRect() const { return rect_; }

private:
    Text text_;
    SDL_Rect rect_;
    bool selected_;
};

class Menu {
public:
    Menu();
    ~Menu();

    bool Init(SDL_Renderer* renderer, TTF_Font* font);
    void HandleEvents(SDL_Event& event, bool& quit, bool& start_game);
    void Render(SDL_Renderer* renderer);

    MenuScreens GetCurrentScreen() const { return current_screen_; }
    void SetCurrentScreen(MenuScreens screen) { current_screen_ = screen; }

private:
    Graphics background_main_;
    Graphics background_guide_;

    std::vector<MenuItem*> main_menu_items_;
    int selected_item_;

    MenuScreens current_screen_;

    bool LoadBackgrounds(SDL_Renderer* renderer);
    void HandleMainMenuEvents(SDL_Event& event, bool& quit, bool& start_game);
    void HandleGuideScreenEvents(SDL_Event& event);

    Mix_Chunk* select_sound_;
    Mix_Chunk* navigate_sound_;
};

#endif // MENU_H
