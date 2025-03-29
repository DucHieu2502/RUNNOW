#ifndef BULLET_H
#define BULLET_H

#include "Graphics.h"
#include "Constants.h"

class Bullet : public Graphics{
public:
    Bullet();
    ~Bullet();

    void set_x_val (const int& xVal) {x_val_ = xVal;}
    void set_y_val (const int& yVal) {y_val_ = yVal;}
    int get_x_val() const {return x_val_;}
    int get_y_val() const {return y_val_;}

    void set_is_move (const bool& isMove) {is_move_ = isMove;}
    bool get_is_move() const {return is_move_;}

    void HandleMove (const int& x_border, const int& y_border);
private:
    int x_val_;
    int y_val_;
    bool is_move_;
};

#endif // BULLET_H
