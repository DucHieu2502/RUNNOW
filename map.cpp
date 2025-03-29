#include "map.h"
#include <random>
#include <algorithm>

GameMap::GameMap() : last_bullet_time_(0){

}

GameMap::~GameMap(){
    for(auto* bullet : enemy_bullets_){
        delete bullet;
    }
    enemy_bullets_.clear();
}

void GameMap::loadMap(char* name){
    FILE* fp = NULL;
    fopen_s(&fp, name, "rb");
    if(fp == NULL){
        return;
    }
    game_map_.max_x_ = 0;
    game_map_.max_y_ = 0;

    for(int i = 0; i < MAX_MAP_Y; i++){
        for(int j = 0; j < MAX_MAP_X; j++){
            fscanf(fp, "%d", &game_map_.tile[i][j]);
            int val = game_map_.tile[i][j];
            if(val > 0){
                if(j > game_map_.max_x_){
                    game_map_.max_x_ = j;
                }
                if(i > game_map_.max_y_){
                    game_map_.max_y_ = i;
                }
            }
        }
    }
    game_map_.max_x_ = (game_map_.max_x_ + 1)*TILE_SIZE;
    game_map_.max_y_ = (game_map_.max_y_ + 1)*TILE_SIZE;

    game_map_.start_x_ = 0;
    game_map_.start_y_ = 0;

    game_map_.file_name_ = name;
    fclose(fp);
}

void GameMap::loadTiles(SDL_Renderer* renderer){
    char file_img[30];
    FILE* fp = NULL;
    for(int i = 0; i < MAX_TILES; i++){
        sprintf_s(file_img, "img//%d.png", i);
        fopen_s(&fp, file_img, "rb");
        if(fp == NULL){
            continue;
        }

        fclose(fp);

        tile_map[i].Load_IMG(file_img, renderer);
    }
}

void GameMap::DrawMap(SDL_Renderer* renderer){
    int x1 = 0;
    int x2 = 0;

    int y1 = 0;
    int y2 = 0;

    int map_x = game_map_.start_x_/TILE_SIZE;

    x1 = (game_map_.max_x_%TILE_SIZE)*-1;
    x2 = x1 + SCREEN_WIDTH + (x1 == 0 ? 0 : TILE_SIZE);

    int map_y = game_map_.start_y_/TILE_SIZE;

    y1 = (game_map_.max_y_%TILE_SIZE)*-1;
    y2 = x1 + SCREEN_HEIGHT + (y1 == 0 ? 0 : TILE_SIZE);

    for(int i = y1; i < y2; i += TILE_SIZE){
        map_x = game_map_.start_x_/TILE_SIZE;
        for(int j = x1; j < x2; j += TILE_SIZE){
            int val = game_map_.tile[map_y][map_x];
            if(val > 0){
                tile_map[val].SetRect(j , i);
                tile_map[val].render(renderer);
            }
            map_x++;
        }
        map_y++;
    }
}

void GameMap::UpdateMapPos(const int& player_x){
    // Sử dụng thời gian thực để điều chỉnh việc cuộn
    static Uint32 last_time = 0;
    Uint32 current_time = SDL_GetTicks();

    // Tính toán thời gian giữa các khung hình
    float delta_time = (current_time - last_time) / 1000.0f;

    // Giảm tốc độ cuộn xuống và làm mượt hơn
    float smooth_scroll_speed = 2.4f * 60.0f * delta_time;

    // Thêm một chút đệm để cuộn mượt hơn
    static float scroll_accumulator = 0.0f;
    scroll_accumulator += smooth_scroll_speed;

    // Chỉ cuộn khi accumulator đủ lớn
    int scroll_amount = static_cast<int>(scroll_accumulator);
    if (scroll_amount > 0) {
        game_map_.start_x_ += scroll_amount;
        scroll_accumulator -= scroll_amount;
    }

    // Giới hạn vị trí map
    if (game_map_.start_x_ < 0) {
        game_map_.start_x_ = 0;
    }

    if (game_map_.start_x_ > game_map_.max_x_ - SCREEN_WIDTH) {
        game_map_.start_x_ = game_map_.max_x_ - SCREEN_WIDTH;
    }

    // Cập nhật thời gian cuối cùng
    last_time = current_time;
}

bool GameMap::DestroyTile(int bullet_x, int bullet_y, int bullet_width, int bullet_height) {
    // dieu chinh cuon ban do
    int adjusted_x = bullet_x + game_map_.start_x_;

    // toa do cac o tile
    int tile_x1 = adjusted_x / TILE_SIZE;
    int tile_x2 = (adjusted_x + bullet_width) / TILE_SIZE;
    int tile_y1 = bullet_y / TILE_SIZE;
    int tile_y2 = (bullet_y + bullet_height) / TILE_SIZE;

    if (tile_x1 < 0 || tile_x2 >= MAX_MAP_X || tile_y1 < 0 || tile_y2 >= MAX_MAP_Y)
        return false;

    // tim kiem dich dau tien cua day
    for (int y = tile_y1; y <= tile_y2; y++) {
        for (int x = tile_x1; x <= tile_x2; x++) {
            // kiem tra xem co dich khong
            if (game_map_.tile[y][x] == 1||game_map_.tile[y][x] == 2) {
                // pha huy anh dich
                game_map_.tile[y][x] = 0;
                return true;
            }
            if(game_map_.tile[y][x] == 3){
                is_win = true;
                game_map_.tile[y][x] = 0;
            }
        }
    }

    return false;
}

void GameMap::GenerateEnemyBullets(SDL_Renderer* renderer){
    // Giảm tần suất sinh đạn
    static Uint32 last_bullet_time = 0;
    Uint32 current_time = SDL_GetTicks();

    // Tăng khoảng thời gian giữa các lần bắn (ví dụ: 1000ms)
    if(current_time - last_bullet_time < 1000) return;
    last_bullet_time = current_time;

    // Hạn chế số lượng địch được bắn
    int max_shooters = 2;
    int shooter_count = 0;

    for(int y = 0; y < MAX_MAP_Y && shooter_count < max_shooters; y++){
        for(int x = 0; x < MAX_MAP_X && shooter_count < max_shooters; x++){
            // Chỉ bắn từ các địch trong phạm vi màn hình
            if((game_map_.tile[y][x] == 1 || game_map_.tile[y][x] == 2) &&
               x >= game_map_.start_x_/TILE_SIZE &&
               x < (game_map_.start_x_ + SCREEN_WIDTH)/TILE_SIZE){

                EnemyBullet* enemy_bullet = new EnemyBullet();
                if(enemy_bullet->Load_IMG("img//enemy_bullet.png", renderer)){
                    int bullet_x = x * TILE_SIZE - game_map_.start_x_;
                    int bullet_y = y * TILE_SIZE;

                    enemy_bullet->SetRect(bullet_x, bullet_y);
                    enemy_bullet->set_x_val(-12.0f);
                    enemy_bullet->set_is_move(true);
                    enemy_bullet->set_x_direction(-1);

                    enemy_bullets_.push_back(enemy_bullet);
                    shooter_count++;
                }
            }
        }
    }
}


void GameMap::HandleEnemyBullets(SDL_Renderer* renderer) {
    for (auto it = enemy_bullets_.begin(); it != enemy_bullets_.end();) {
        EnemyBullet* bullet = *it;

        if (bullet->get_is_move()) {
            bullet->HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT);
            bullet->render(renderer);

            if (bullet->GetRect().x < 0) {
                delete bullet;
                it = enemy_bullets_.erase(it);
                continue;
            }
        }
        ++it;
    }
}
