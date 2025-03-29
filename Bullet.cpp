#include "Bullet.h"

Bullet::Bullet(){
    x_val_ = 0;
    y_val_ = 0;
    is_move_ = false;
}

Bullet::~Bullet(){

}

void Bullet::HandleMove(const int& x_border, const int& y_border){
    rect_.x += x_val_;

    if(rect_.x > x_border){
        is_move_ = false;
    }
}
