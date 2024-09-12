#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

int initSDL(SDL_Window*& window, SDL_Renderer*& renderer) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    // Create window
    window = SDL_CreateWindow("SDL2 Sprite Sheet", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480,
                              SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    return 0;
}

void unloadTexture(SDL_Texture*& texture) { SDL_DestroyTexture(texture); }

SDL_Texture* loadTexture(SDL_Renderer*& renderer, std::string filePath) {
    // Load image
    SDL_Surface* tempSurface = IMG_Load(filePath.c_str());
    if (tempSurface == NULL) {
        printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    return texture;
}

void deInitSDL(SDL_Window*& window, SDL_Renderer*& renderer) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* args[]) {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    initSDL(window, renderer);

    bool quit = false;
    SDL_Event event;

    SDL_Texture* spriteSheetTexture = loadTexture(renderer, "enter_the_gungeon_spritesheet.png");

    // Variables for animation timing and sprite movement
    Uint32 lastTime = 0;            // Time of the last frame change
    int frame = 0;                  // Current frame (sprite in the sprite sheet)
    const int spriteWidth = 16;     // Width of each sprite
    const int spriteHeight = 25;    // Height of each sprite
    const int frameCount = 6;       // Total number of frames in the sprite sheet
    const int animationSpeed = 200; // Time between frames in milliseconds (200 ms)

    while (!quit) {
        // Event handling
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Get the current time in milliseconds
        Uint32 currentTime = SDL_GetTicks();

        // Check if 200 ms have passed
        if (currentTime > lastTime + animationSpeed) {
            // Update to the next frame
            frame = (frame + 1) % frameCount; // Loops back to 0 after the last frame

            // Update the last frame change time
            lastTime = currentTime;
        }

        // Clear screen
        SDL_RenderClear(renderer);

        // Define the sprite sheet source rect
        SDL_Rect srcRect;
        srcRect.x = (frame * spriteWidth) + 22; // Move horizontally in the sprite sheet
        srcRect.y = 187;          // Keep the vertical position constant (you can change this for vertical movement)
        srcRect.w = spriteWidth;  // The width of the sprite
        srcRect.h = spriteHeight; // The height of the sprite

        // Define the destination rect where the image will be drawn
        SDL_Rect destRect;
        destRect.x = 100;    // The x position on the screen
        destRect.y = 100;    // The y position on the screen
        destRect.w = 18 * 4; // The width of the drawn image (scaling)
        destRect.h = 26 * 4; // The height of the drawn image (scaling)

        // Copy part of the sprite sheet to the renderer
        SDL_RenderCopy(renderer, spriteSheetTexture, &srcRect, &destRect);

        // Update the screen
        SDL_RenderPresent(renderer);
    }

    // Clean up
    unloadTexture(spriteSheetTexture);
    deInitSDL(window, renderer);
    return 0;
}
