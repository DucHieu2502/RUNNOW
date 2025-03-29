#include "Spaceship.h"
#include "map.h"
#include "Bullet.h"
#include "Constants.h"
#include "enemy.h"

Spaceship::Spaceship(){
    x_val_ = 0;
    y_val_ = 0;
    x_pos_ = 0;
    y_pos_ = 0;
    precise_x_pos = x_pos_;
    precise_y_pos = y_pos_;
    status_ = NONE;
}

Spaceship::~Spaceship(){
    if (shoot_sound_ != nullptr) {
        Mix_FreeChunk(shoot_sound_);
        shoot_sound_ = nullptr;
    }
    for(auto* bullet : p_bullet_list_){
        delete bullet;
    }
    p_bullet_list_.clear();
    Free();
}

bool Spaceship::LoadIMG(SDL_Renderer* renderer){
    bool ret = Load_IMG(SPACESHIP, renderer);
    int vertical_center = (SCREEN_HEIGHT - rect_.h) / 2;
    SetRect(0, vertical_center);
    SetPosXY(0, vertical_center);
    precise_x_pos = 0;
    precise_y_pos = vertical_center;
    return ret;
}

void Spaceship::Show(SDL_Renderer* renderer){
    render(renderer);
}

void Spaceship::HandleInputAction(SDL_Event events, SDL_Renderer* renderer){
    if(events.type == SDL_KEYDOWN){
        switch(events.key.keysym.sym){
        case SDLK_UP:
            status_ = MOVE_UP;
            y_val_ = -SPEED_SPACESHIP; // toc do di chuyen len
            break;
        case SDLK_DOWN:
            status_ = MOVE_DOWN;
            y_val_ = SPEED_SPACESHIP; // toc do di chuyen xuong
            break;
        case SDLK_RIGHT:
            status_ = MOVE_RIGHT;
            x_val_ = SPEED_SPACESHIP; // toc do di chuyen sang phai
            break;
        case SDLK_LEFT:
            status_ = MOVE_LEFT;
            x_val_ = -SPEED_SPACESHIP;
            break;
        default:
            if(events.key.keysym.sym == SDLK_SPACE){
            Bullet* p_bullet = new Bullet();
            //   p_bullet -> Load_IMG(BULLET, renderer);
            bool result = p_bullet->Load_IMG("img//bullet.png", renderer);
            if (!result) {
            // In ra lỗi cụ thể của SDL
            printf("IMG Load Error: %s\n", IMG_GetError());
            }
            p_bullet -> Graphics::SetRect(this-> rect_.x + rect_.w - 50, rect_.y + rect_.h/2 - 20);
            p_bullet -> set_x_val(15.0f);
            p_bullet -> set_is_move(true);

            p_bullet_list_.push_back(p_bullet);

            PlayShootSound(); //chay am thanh
            }
            break;
        }
    }
    else if(events.type == SDL_KEYUP){
        switch(events.key.keysym.sym){
        case SDLK_UP:
            y_val_ = 0;
            break;
        case SDLK_DOWN:
            y_val_ = 0;
            break;
        case SDLK_RIGHT:
            x_val_ = 0;
            break;
        case SDLK_LEFT:
            x_val_ = 0;
            break;
        }
    }
}


void Spaceship::Update(int map_start_x, GameMap& game_map){
    // Cập nhật vị trí của tàu vũ trụ
    precise_x_pos += x_val_;
    precise_y_pos += y_val_;

    rect_.x = static_cast<int>(precise_x_pos);
    rect_.y = static_cast<int>(precise_y_pos);
    // Giới hạn tàu vũ trụ trong 1/3 đầu tiên của màn hình
    int screen_third = SCREEN_WIDTH ;

    if (rect_.x < 0) {
        rect_.x = 0;
        precise_x_pos = 0;
    }
    else if (rect_.x + rect_.w > screen_third) {
        rect_.x = screen_third - rect_.w;
        precise_x_pos = rect_.x;
    }

    if (rect_.y < 0) {
        rect_.y = 0;
        precise_y_pos = 0;
    }
    else if (rect_.y + rect_.h > SCREEN_HEIGHT) {
        rect_.y = SCREEN_HEIGHT - rect_.h;
        precise_y_pos = rect_.y;
    }
    x_pos_ = rect_.x;
    y_pos_ = rect_.y;

    if (CheckCollisionWithEnemyTiles(game_map)) {
        game_over_ = true;
    }
}

void Spaceship::HandleBullet(SDL_Renderer* des, GameMap& game_map){
    // Sử dụng iterator để tránh lỗi khi xóa phần tử
    for(auto it = p_bullet_list_.begin(); it != p_bullet_list_.end();){
        Bullet* p_bullet = *it;

        if(p_bullet && p_bullet->get_is_move()){
            SDL_Rect bullet_rect = p_bullet->GetRect();
            bool bullet_removed = false;

            // Kiểm tra va chạm với đạn địch
            auto& enemy_bullets = game_map.get_enemy_bullets();
            for(auto enemy_it = enemy_bullets.begin(); enemy_it != enemy_bullets.end();){
                EnemyBullet* enemy_bullet = *enemy_it;
                SDL_Rect enemy_bullet_rect = enemy_bullet->GetRect();

                if(SDL_HasIntersection(&bullet_rect, &enemy_bullet_rect)){
                    // Xóa cả hai viên đạn an toàn
                    delete p_bullet;
                    delete enemy_bullet;

                    it = p_bullet_list_.erase(it);
                    enemy_it = enemy_bullets.erase(enemy_it);

                    bullet_removed = true;
                    break;
                }
                else {
                    ++enemy_it;
                }
            }

            if(bullet_removed){
                continue;
            }

            // Kiểm tra phá hủy ô
            if(game_map.DestroyTile(
                bullet_rect.x,
                bullet_rect.y,
                bullet_rect.w,
                bullet_rect.h
            )) {
                AddScore(5);
                delete p_bullet;
                it = p_bullet_list_.erase(it);
                continue;
            }

            p_bullet->HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT);
            p_bullet->render(des);
            ++it;
        }
        else {
            // Xóa các viên đạn không di chuyển
            delete p_bullet;
            it = p_bullet_list_.erase(it);
        }
    }
    if(CheckCollisionWithEnemyBullets(game_map)) {
        game_over_ = true;
    }
}

bool Spaceship::LoadShootSound(const std::string& soundPath) {
    // Load sound effect
    shoot_sound_ = Mix_LoadWAV(soundPath.c_str());
    if (shoot_sound_ == nullptr) {
        printf("Failed to load shoot sound! SDL_mixer Error: %s\n", Mix_GetError());
        return false;
    }
    return true;
}

void Spaceship::PlayShootSound() {
    if (shoot_sound_ != nullptr) {
        Mix_PlayChannel(-1, shoot_sound_, 0);
    }
}

bool Spaceship::CheckCollisionWithEnemyBullets(GameMap& game_map) {
    int margin_left = rect_.w * 0.2;    // 20% from left
    int margin_right = rect_.w * 0.2;   // 20% from right
    int margin_top = rect_.h * 0.3;     // 30% from top
    int margin_bottom = rect_.h * 0.3;  // 30% from bottom

    SDL_Rect hit_box = {
        rect_.x + margin_left,
        rect_.y + margin_top,
        rect_.w - (margin_left + margin_right),
        rect_.h - (margin_top + margin_bottom)
    };

    auto& enemy_bullets = game_map.get_enemy_bullets();
    for(auto enemy_bullet : enemy_bullets) {
        SDL_Rect enemy_bullet_rect = enemy_bullet->GetRect();

        if(SDL_HasIntersection(&hit_box, &enemy_bullet_rect)) {
            return true;
        }
    }

    return false;
}

bool Spaceship::CheckCollisionWithEnemyTiles(GameMap& game_map) {
    Map current_map = game_map.GetMap();

    int margin_left = rect_.w * 0.2;    // 20% from left
    int margin_right = rect_.w * 0.2;   // 20% from right
    int margin_top = rect_.h * 0.3;     // 30% from top
    int margin_bottom = rect_.h * 0.3;  // 30% from bottom

    SDL_Rect hit_box = {
        rect_.x + margin_left,
        rect_.y + margin_top,
        rect_.w - (margin_left + margin_right),
        rect_.h - (margin_top + margin_bottom)
    };

    // tinh toan vi tri
    int tile_x1 = (hit_box.x + current_map.start_x_) / TILE_SIZE;
    int tile_x2 = (hit_box.x + hit_box.w + current_map.start_x_) / TILE_SIZE;
    int tile_y1 = hit_box.y / TILE_SIZE;
    int tile_y2 = (hit_box.y + hit_box.h) / TILE_SIZE;

    if (tile_x1 < 0 || tile_x2 >= MAX_MAP_X || tile_y1 < 0 || tile_y2 >= MAX_MAP_Y)
        return false;

    for (int y = tile_y1; y <= tile_y2; y++) {
        for (int x = tile_x1; x <= tile_x2; x++) {
            // Kiểm tra va chạm với tàu địch loại 1 va 2
            if (current_map.tile[y][x] == 1||current_map.tile[y][x] == 2) {
                return true;
            }
        }
    }

    return false;
}
