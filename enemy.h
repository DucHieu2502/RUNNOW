#ifndef ENEMY_H
#define ENEMY_H

#include "Bullet.h"

class EnemyBullet : public Bullet{
public:
    EnemyBullet();
    ~EnemyBullet();

    void set_x_direction(const int& direction) {x_direction = direction;}
    int get_x_direction() {return x_direction;}

private:
    int x_direction;
};

#endif // ENEMY_H
