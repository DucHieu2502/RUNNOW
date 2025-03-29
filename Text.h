#ifndef TEXT_H
#define TEXT_H

#include "Constants.h"

class Text{
public:
    Text();
    ~Text();

    enum TextColor{
        RED_TEXT = 0,
        WHITE_TEXT = 1,
        BLACK_TEXT = 2,
    };

    bool LoadFromRenderText(TTF_Font* font, SDL_Renderer* renderer);
    void Free();
    void SetColor(Uint8 red,Uint8 green, Uint8 blue);
    void SetColor(int type);

    void RenderText(SDL_Renderer* renderer,
                    int xp, int yp,
                    SDL_Rect* clip = NULL,
                    double angle = 0.0,
                    SDL_Point* center = NULL,
                    SDL_RendererFlip flip = SDL_FLIP_NONE);
    int GetWidth() const {return width_;}
    int GetHeight() const {return height_;}

    void SetText(const std::string& text) {str_val_ = text;}
    std::string GetText() const {return str_val_;}
private:
    std::string str_val_;
    SDL_Color text_color_;
    SDL_Texture* texture;
    int width_;
    int height_;
};
#endif // TEXT_H
