#ifndef MAP_H
#define MAP_H

#include <vector>
#include "Constants.h"
#include "Graphics.h"
#include "enemy.h"

#define MAX_TILES 20
#define MAX_ENEMY_BULLET 50

class TileMap : public Graphics{
public:
    TileMap() {;}
    ~TileMap() {;}
};

class GameMap{
public:
    GameMap();
    ~GameMap();

    void loadMap(char* name);
    void loadTiles(SDL_Renderer* renderer);
    void DrawMap(SDL_Renderer* renderer);

    void SetMap(const int& x) {game_map_.start_x_ = x;}
    Map GetMap() const {return game_map_;}
    void UpdateMapPos(const int& player_x);

    bool DestroyTile(int bullet_x, int bullet_y, int bullet_width, int bullet_height);
    void GenerateEnemyBullets (SDL_Renderer* renderer);
    void HandleEnemyBullets (SDL_Renderer* renderer);

    std::vector<EnemyBullet*>& get_enemy_bullets() { return enemy_bullets_; }

private:
    Map game_map_;
    TileMap tile_map[MAX_TILES];
    std::vector<EnemyBullet*> enemy_bullets_;
    Uint32 last_bullet_time_;

    bool destroyed_tiles[MAX_MAP_Y][MAX_MAP_X] = {false};

};
#endif // MAP_H
