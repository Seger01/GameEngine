#include "Animation.h"

Animation::Animation() {}

Animation::Animation(SDL_Texture*& aSourceTexture, SDL_Rect aStartingFrame, int aAmountOfFrames)
    : sourceTexture(aSourceTexture) {
    for (int i = 0; i < aAmountOfFrames; i++) {
        SDL_Rect frame;

        frame.x = aStartingFrame.x + (i * aStartingFrame.w);
        frame.y = aStartingFrame.y;
        frame.h = aStartingFrame.h;
        frame.w = aStartingFrame.w;

        sprites.push_back(frame);
    }
}

Animation::~Animation() {
    std::cout << "~Animation()" << std::endl;
    return;
}

SDL_Texture*& Animation::getTexture() { return this->sourceTexture; }

SDL_Rect& Animation::getCurrentFrame() {
    // Get the current time in milliseconds
    Uint32 currentTime = SDL_GetTicks();

    int currentFrameNr = (currentTime % (sprites.size() * animationSpeed)) / animationSpeed;

    currentFrame = sprites[currentFrameNr];

    return currentFrame;
}
