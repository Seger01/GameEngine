#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int MOVE_SPEED = 10; // Speed of movement in pixels per keypress

bool init(SDL_Window **window, SDL_Renderer **renderer);
bool loadMedia(SDL_Renderer *renderer, SDL_Texture **texture);
void close(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *texture);

int main(int argc, char *args[]) {
  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;
  SDL_Texture *texture = nullptr;

  if (!init(&window, &renderer)) {
    std::cout << "Failed to initialize!\n";
    return -1;
  }

  if (!loadMedia(renderer, &texture)) {
    std::cout << "Failed to load media!\n";
    return -1;
  }

  // Main loop flag
  bool quit = false;
  SDL_Event e;

  // Define the rectangle for rendering the texture
  SDL_Rect renderQuad;
  renderQuad.x = 100; // X position
  renderQuad.y = 50;  // Y position
  renderQuad.w = 200; // Width of the image
  renderQuad.h = 150; // Height of the image

  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) {
        quit = true;
      }

      // Handle key press events
      if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
        case SDLK_w: // Move up
          renderQuad.y -= MOVE_SPEED;
          break;
        case SDLK_s: // Move down
          renderQuad.y += MOVE_SPEED;
          break;
        case SDLK_a: // Move left
          renderQuad.x -= MOVE_SPEED;
          break;
        case SDLK_d: // Move right
          renderQuad.x += MOVE_SPEED;
          break;
        }
      }
    }

    // Make sure the image stays within screen bounds
    if (renderQuad.x < 0)
      renderQuad.x = 0;
    if (renderQuad.x + renderQuad.w > SCREEN_WIDTH)
      renderQuad.x = SCREEN_WIDTH - renderQuad.w;
    if (renderQuad.y < 0)
      renderQuad.y = 0;
    if (renderQuad.y + renderQuad.h > SCREEN_HEIGHT)
      renderQuad.y = SCREEN_HEIGHT - renderQuad.h;

    // Clear screen
    SDL_RenderClear(renderer);

    // Render the texture to the specified rectangle
    SDL_RenderCopy(renderer, texture, NULL, &renderQuad);

    // Update screen
    SDL_RenderPresent(renderer);
  }

  close(window, renderer, texture);
  return 0;
}

bool init(SDL_Window **window, SDL_Renderer **renderer) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError()
              << "\n";
    return false;
  }

  *window = SDL_CreateWindow("SDL2 PNG Example", SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                             SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (*window == nullptr) {
    std::cout << "Window could not be created! SDL_Error: " << SDL_GetError()
              << "\n";
    return false;
  }

  *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
  if (*renderer == nullptr) {
    std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError()
              << "\n";
    return false;
  }

  int imgFlags = IMG_INIT_PNG;
  if (!(IMG_Init(imgFlags) & imgFlags)) {
    std::cout << "SDL_image could not initialize! SDL_image Error: "
              << IMG_GetError() << "\n";
    return false;
  }

  return true;
}

bool loadMedia(SDL_Renderer *renderer, SDL_Texture **texture) {
  // Load PNG texture
  std::string path =
      "player.png"; // Make sure this path points to a valid PNG file
  SDL_Surface *loadedSurface = IMG_Load(path.c_str());
  if (loadedSurface == nullptr) {
    std::cout << "Unable to load image " << path
              << "! SDL_image Error: " << IMG_GetError() << "\n";
    return false;
  }

  *texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
  if (*texture == nullptr) {
    std::cout << "Unable to create texture from " << path
              << "! SDL_Error: " << SDL_GetError() << "\n";
    return false;
  }

  SDL_FreeSurface(loadedSurface);
  return true;
}

void close(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *texture) {
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  texture = nullptr;
  renderer = nullptr;
  window = nullptr;

  IMG_Quit();
  SDL_Quit();
}
