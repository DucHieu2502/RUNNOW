#include "Text.h"

Text::Text(){
    text_color_.r = 255;
    text_color_.g = 255;
    text_color_.b = 255;
    texture = NULL;
}

Text::~Text(){

}

bool Text::LoadFromRenderText(TTF_Font* font,SDL_Renderer* renderer){
    SDL_Surface* text_surface = TTF_RenderText_Solid(font, str_val_.c_str(), text_color_);
    if(text_surface){
        texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        width_ = text_surface -> w;
        height_ = text_surface -> h;

        SDL_FreeSurface(text_surface);
    }
    return texture != NULL;
}

void Text::Free(){
    if(texture != NULL){
        SDL_DestroyTexture(texture);
        texture = NULL;
    }
}

void Text::SetColor(Uint8 red, Uint8 green, Uint8 blue){
    text_color_.r = red;
    text_color_.g = green;
    text_color_.b = blue;
}

void Text::SetColor(int type){
    if(type == RED_TEXT){
        SDL_Color color = {255, 0, 0};
        text_color_ = color;
    }
    else if(type == WHITE_TEXT){
        SDL_Color color = {255, 255, 255};
        text_color_ = color;
    }
    else if(type == BLACK_TEXT){
        SDL_Color color = {0, 0, 0};
        text_color_ = color;
    }
}

void Text::RenderText(SDL_Renderer* renderer,
                      int xp, int yp, SDL_Rect* clip /* = NULL */,
                      double angle /* = 0.0 */,
                      SDL_Point* center /*= NULL*/,
                      SDL_RendererFlip flip /*= SDL_FLIP_NONE*/){
    SDL_Rect renderQuad = {xp, yp, width_, height_};
    if(clip != NULL){
        renderQuad.w = clip -> w;
        renderQuad.h = clip -> h;
    }
    SDL_RenderCopyEx(renderer, texture, clip, &renderQuad, angle, center, flip);
}
