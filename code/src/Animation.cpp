#include "Animation.h"

// Animation::Animation() {}
//
// Animation::Animation(Texture*& aSourceTexture, Rectangle aStartingFrame, int aAmountOfFrames)
//     : mSourceTexture(aSourceTexture) {
//     for (int i = 0; i < aAmountOfFrames; i++) {
//         Rectangle frame;
//
//         frame.x = aStartingFrame.x + (i * aStartingFrame.w);
//         frame.y = aStartingFrame.y;
//         frame.h = aStartingFrame.h;
//         frame.w = aStartingFrame.w;
//
//         sprites.push_back(frame);
//     }
// }
//
// Animation::~Animation() {
//     std::cout << "~Animation()" << std::endl;
//     return;
// }
//
// Texture*& Animation::getTexture() { return this->mSourceTexture; }
//
// Rectangle& Animation::getCurrentFrame() {
//     // Get the current time in milliseconds
//     Uint32 currentTime = SDL_GetTicks();
//
//     int currentFrameNr = (currentTime % (sprites.size() * animationSpeed)) / animationSpeed;
//
//     currentFrame = sprites[currentFrameNr];
//
//     return currentFrame;
// }
