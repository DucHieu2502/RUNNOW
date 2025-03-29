#include "Graphics.h"

Graphics::Graphics(){
    texture_ = NULL;
    rect_.x = 0;
    rect_.y = 0;
    rect_.w = 0;
    rect_.h = 0;
}

Graphics::~Graphics(){
    Free();
}

bool Graphics::Load_IMG(std::string path, SDL_Renderer *renderer){
    SDL_Texture* new_texture = NULL;

    SDL_Surface* load_surface = IMG_Load(path.c_str());
    if(load_surface != NULL){
        SDL_SetColorKey(load_surface, SDL_TRUE, SDL_MapRGB(load_surface -> format,COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B));
        new_texture = SDL_CreateTextureFromSurface(renderer, load_surface);
        if(new_texture != NULL){
            rect_.w = load_surface -> w;
            rect_.h = load_surface -> h;
        }
        SDL_FreeSurface(load_surface);
    }
    texture_ = new_texture;
    return texture_ != NULL;
}

void Graphics::render(SDL_Renderer* renderer, const SDL_Rect* clip){
    SDL_Rect renderquad = {rect_.x, rect_.y, rect_.w, rect_.h};
    SDL_RenderCopy(renderer, texture_, clip, &renderquad);
}

void Graphics::Free(){
    if(texture_ != NULL){
        SDL_DestroyTexture(texture_);
        texture_ = NULL;
        rect_.x = 0;
        rect_.y = 0;
        rect_.w = 0;
        rect_.h = 0;
    }
}
