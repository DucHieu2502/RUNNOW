#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "Constants.h"

class Graphics{
public:
    Graphics();
    ~Graphics();
    void SetRect(const float x, const float y) {rect_.x = x; rect_.y = y;}
    SDL_Rect GetRect() const {return rect_;}
    SDL_Texture *GetTexture() const {return texture_;}
    bool Load_IMG(std::string path, SDL_Renderer *renderer);
    void render(SDL_Renderer *renderer, const SDL_Rect * clip = NULL);
    void Free();
protected:
    SDL_Texture *texture_;
    SDL_Rect rect_;
};
#endif // GRAPHICS_H
