#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <vector>
#include "Constants.h"
#include "Graphics.h"
#include "Bullet.h"
#include "map.h"
#include <SDL_mixer.h>

class Spaceship : public Graphics{
public:
    Spaceship();
    ~Spaceship();

    enum MoveType{
        MOVE_LEFT = 1,
        MOVE_RIGHT = 0,
        MOVE_UP = 2,
        MOVE_DOWN = 3,
        NONE = 4
    };

    bool LoadIMG(SDL_Renderer* renderer);
    void Show(SDL_Renderer* renderer);
    void HandleInputAction(SDL_Event events, SDL_Renderer* renderer); //xu ly ban phim
    void Update(int map_start_x, GameMap& game_map); //cap nhat tau

    void set_x_val(const float& val) { x_val_ = val; }
    void set_y_val(const float& val) { y_val_ = val; }
    float get_x_val() const { return x_val_; } // lay gia tri cua x_val_
    float get_y_val() const { return y_val_; }

    void SetMapXY(const int map_x, const int map_y)  {map_x_ = map_x; map_y_ = map_y;}
    void SetPosXY(const int x, const int y)  {x_pos_ = x; y_pos_ = y;}
    float GetPosX() const {return x_pos_;}
    float GetPosY() const {return y_pos_;}

    void set_bullet_list (const std::vector<Bullet*> bulletList)  {p_bullet_list_ = bulletList;}
    std::vector<Bullet*> get_bullet_list() const {return p_bullet_list_;}

    void HandleBullet(SDL_Renderer* des, GameMap& game_map);
    bool CheckCollisionWithEnemyBullets(GameMap& game_map); //kiem tra va cham voi dan
    bool CheckCollisionWithEnemyTiles(GameMap& game_map); // kiem tra va cham voi tau

    friend class GameMap;

    void AddScore(int points) { score_ = std::max(0, score_ + points); }
    int GetScore() const { return score_; }

    bool LoadShootSound(const std::string& soundPath);
    void PlayShootSound();

    bool IsGameOver() const { return game_over_; }
    void SetGameOver(bool status) { game_over_ = status; }

    bool GetWin() const {return is_win;}
    void SetWin(bool win) {is_win = win;}
private:
    std::vector <Bullet*> p_bullet_list_;

    float x_val_;
    float y_val_;

    float x_pos_;
    float y_pos_;

    float precise_x_pos;
    float precise_y_pos;

    int map_x_;
    int map_y_;
    MoveType status_;

    Mix_Chunk* shoot_sound_;

    int score_ = 0;
    bool game_over_ = false;
    bool is_win = false;
};

#endif // SPACESHIP_H
