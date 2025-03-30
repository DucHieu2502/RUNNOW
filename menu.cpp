#include "menu.h"

MenuItem::MenuItem(const std::string& text, int x, int y, TTF_Font* font, SDL_Renderer* renderer) {
    text_.SetText(text);
    text_.SetColor(Text::WHITE_TEXT);
    text_.LoadFromRenderText(font, renderer);

    rect_.x = x;
    rect_.y = y;
    rect_.w = text_.GetWidth();
    rect_.h = text_.GetHeight();

    selected_ = false;
}

MenuItem::~MenuItem() {
    text_.Free();
}

void MenuItem::Render(SDL_Renderer* renderer) {
    if (selected_) {
        text_.SetColor(255, 255, 0);
    } else {
        text_.SetColor(Text::WHITE_TEXT);
    }

    text_.LoadFromRenderText(TTF_OpenFont("font/Gagalin-Regular.otf", 45), renderer);
    text_.RenderText(renderer, rect_.x, rect_.y);
}

void MenuItem::SetSelected(bool selected) {
    selected_ = selected;
}

Menu::Menu() {
    selected_item_ = 0;
    current_screen_ = MAIN_MENU;
    select_sound_ = nullptr;
    navigate_sound_ = nullptr;
}

Menu::~Menu() {
    for (auto item : main_menu_items_) {
        delete item;
    }
    main_menu_items_.clear();

    background_main_.Free();
    background_guide_.Free();

    if (select_sound_ != nullptr) {
        Mix_FreeChunk(select_sound_);
        select_sound_ = nullptr;
    }

    if (navigate_sound_ != nullptr) {
        Mix_FreeChunk(navigate_sound_);
        navigate_sound_ = nullptr;
    }
}

bool Menu::Init(SDL_Renderer* renderer, TTF_Font* font) {
    if (!LoadBackgrounds(renderer)) {
        return false;
    }

    TTF_Font* menu_font = TTF_OpenFont("font/BebasNeue-Regular.ttf", 36);
    if (menu_font == nullptr) {
        printf("Failed to load font: %s\n", TTF_GetError());
        return false;
    }

    int center_x = SCREEN_WIDTH / 2;
    int play_y = SCREEN_HEIGHT / 2 + 30;
    int guide_y = play_y + 70;
    int exit_y = guide_y + 70;

    MenuItem* play_item = new MenuItem("PLAY", center_x - 50, play_y, menu_font, renderer);
    MenuItem* guide_item = new MenuItem("GUIDE", center_x - 55, guide_y, menu_font, renderer);
    MenuItem* exit_item = new MenuItem("EXIT", center_x - 45, exit_y, menu_font, renderer);

    main_menu_items_.push_back(play_item);
    main_menu_items_.push_back(guide_item);
    main_menu_items_.push_back(exit_item);

    main_menu_items_[selected_item_]->SetSelected(true);

    select_sound_ = Mix_LoadWAV("sound/select.mp3");
    navigate_sound_ = Mix_LoadWAV("sound/navigate.mp3");
    if (select_sound_ == nullptr || navigate_sound_ == nullptr) {
        printf("Failed to load sound effects: %s\n", Mix_GetError());
    }

    TTF_CloseFont(menu_font);
    return true;
}

bool Menu::LoadBackgrounds(SDL_Renderer* renderer) {

    if (!background_main_.Load_IMG("img/main_menu.jpg", renderer)) {
        printf("Failed to load main menu background: %s\n", IMG_GetError());
        return false;
    }

    if (!background_guide_.Load_IMG("img/guide_screen.jpg", renderer)) {
        printf("Failed to load guide screen background: %s\n", IMG_GetError());
        return false;
    }

    return true;
}

void Menu::HandleEvents(SDL_Event& event, bool& quit, bool& start_game) {
    switch (current_screen_) {
        case MAIN_MENU:
            HandleMainMenuEvents(event, quit, start_game);
            break;
        case GUIDE_SCREEN:
            HandleGuideScreenEvents(event);
            break;
        default:
            break;
    }
}

void Menu::HandleMainMenuEvents(SDL_Event& event, bool& quit, bool& start_game) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_UP:

                main_menu_items_[selected_item_]->SetSelected(false);
                selected_item_ = (selected_item_ - 1 + main_menu_items_.size()) % main_menu_items_.size();
                main_menu_items_[selected_item_]->SetSelected(true);

                if (navigate_sound_ != nullptr) {
                    Mix_PlayChannel(-1, navigate_sound_, 0);
                }
                break;

            case SDLK_DOWN:

                main_menu_items_[selected_item_]->SetSelected(false);
                selected_item_ = (selected_item_ + 1) % main_menu_items_.size();
                main_menu_items_[selected_item_]->SetSelected(true);

                if (navigate_sound_ != nullptr) {
                    Mix_PlayChannel(-1, navigate_sound_, 0);
                }
                break;

            case SDLK_RETURN:
            case SDLK_SPACE:

                if (select_sound_ != nullptr) {
                    Mix_PlayChannel(-1, select_sound_, 0);
                }

                switch (selected_item_) {
                    case 0: // Play
                        start_game = true;
                        current_screen_ = GAMEPLAY;
                        break;
                    case 1: // Guide
                        current_screen_ = GUIDE_SCREEN;
                        break;
                    case 2: // Exit
                        quit = true;
                        break;
                }
                break;

            case SDLK_ESCAPE:
                quit = true;
                break;
        }
    }
}

void Menu::HandleGuideScreenEvents(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE ||
            event.key.keysym.sym == SDLK_BACKSPACE ||
            event.key.keysym.sym == SDLK_RETURN ||
            event.key.keysym.sym == SDLK_SPACE) {
            current_screen_ = MAIN_MENU;

            if (select_sound_ != nullptr) {
                Mix_PlayChannel(-1, select_sound_, 0);
            }
        }
    }
}

void Menu::Render(SDL_Renderer* renderer) {
    switch (current_screen_) {
        case MAIN_MENU:

            background_main_.render(renderer);

            for (auto item : main_menu_items_) {
                item->Render(renderer);
            }
            break;

        case GUIDE_SCREEN:

            background_guide_.render(renderer);
            break;

        default:
            break;
    }
}
