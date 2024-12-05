#pragma once
#include "Component.hpp"
#include <SDL.h>

class RenderComponent : public Component {
public:
    RenderComponent(int r = 255, int g = 0, int b = 0, int a = 255) 
        : color{r, g, b, a} {}

    void Render(SDL_Renderer* renderer) override {
        SDL_Rect rect = { 
            static_cast<int>(owner->GetX() - owner->GetWidth() / 2), 
            static_cast<int>(owner->GetY() - owner->GetHeight() / 2), 
            owner->GetWidth(), 
            owner->GetHeight() 
        };
        SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], color[3]);
        SDL_RenderFillRect(renderer, &rect);
    }

    void SetColor(int r, int g, int b, int a = 255) {
        color[0] = r;
        color[1] = g;
        color[2] = b;
        color[3] = a;
    }

    std::string GetName() const override { return "RenderComponent"; }

private:
    int color[4];
};