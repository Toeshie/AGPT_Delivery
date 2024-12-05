#pragma once
#include "Component.hpp"
#include "AnimationClass.hpp"
#include "Sprite.hpp"
#include <unordered_map>
#include <memory>

class AnimationComponent : public Component {
public:
    AnimationComponent(SDL_Renderer* renderer, const std::string& spritePath) {
        sprite = std::make_unique<Sprite>(renderer, spritePath);
    }

    void AddAnimation(const std::string& name, std::shared_ptr<Animation> animation) {
        animations[name] = animation;
    }

    void AddAnimation(const std::string& name, const std::vector<SDL_Rect>& frames, float frameTime) {
        auto anim = std::make_shared<Animation>(
            sprite.get(),
            frames[0].w,    
            frames[0].h,    
            frames.size(),  
            frameTime,      
            frames.size(),  
            1              
        );
        animations[name] = anim;
    }

    void PlayAnimation(const std::string& name, bool loop = true) {
        auto it = animations.find(name);
        if (it != animations.end()) {
            currentAnimation = it->second;
            currentAnimation->SetLooping(loop);
            currentAnimation->Reset();
        } else {
            currentAnimation = nullptr;
        }
    }

    void Update(float deltaTime) override {
        if (currentAnimation) {
            currentAnimation->Update();
        }
    }

    void Render(SDL_Renderer* renderer) override {
        if (!isVisible || !currentAnimation) return;
        
        if (currentAnimation) {
            int frameX = currentAnimation->GetCurrentFrame() * currentAnimation->GetFrameWidth();
            int frameY = 0;  
            SDL_Rect srcRect = { frameX, frameY, currentAnimation->GetFrameWidth(), currentAnimation->GetFrameHeight() };
            SDL_Rect destRect = { static_cast<int>(owner->GetX() - owner->GetWidth() / 2),
                                  static_cast<int>(owner->GetY() - owner->GetHeight() / 2),
                                  currentAnimation->GetFrameWidth(), currentAnimation->GetFrameHeight() };
            SDL_RenderCopy(renderer, sprite->GetTexture(), &srcRect, &destRect);
        }
    }

    Sprite* GetSprite() const { return sprite.get(); }
    std::string GetName() const override { return "AnimationComponent"; }

    void SetVisible(bool visible) { isVisible = visible; }
    bool IsVisible() const { return isVisible; }
    
    std::string GetCurrentAnimation() const {
        if (!currentAnimation) return "";
        for (const auto& pair : animations) {
            if (pair.second == currentAnimation) {
                return pair.first;
            }
        }
        return "";
    }

private:
    std::unique_ptr<Sprite> sprite;
    std::unordered_map<std::string, std::shared_ptr<Animation>> animations;
    std::shared_ptr<Animation> currentAnimation = nullptr;
    bool isVisible = true;
};