#include <iostream>
#include <cmath>
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_ttf.h>

constexpr int WIDTH = 800;
constexpr int HEIGHT = 640;
constexpr float FPS = 60;


int main() {
    std::cout << "Hello, World!" << std::endl;
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "Failed to init video!" << std::endl;
        exit(1);
    }
    if (TTF_Init() != 0) {
        std::cout << "Failed to init ttf!" << std::endl;
        exit(1);
    }
    std::cout << "Initialized SDL." << std::endl;

    auto window = SDL_CreateWindow("Platformer",
                                   SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED,
                                   WIDTH,
                                   HEIGHT,
                                   SDL_WINDOW_OPENGL);

    auto ctx = SDL_GL_CreateContext(window);
    if (!ctx) {
        std::cout << "Failed to create gl context!" << std::endl;
        exit(1);
    }

    if (0 > SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4)
        || 0 > SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1)) {
        std::cout << "Failed to set gl attributes!" << std::endl;
        exit(1);
    };

    if(glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize glew!" << std::endl;
        exit(1);
    }

    if (0 > SDL_GL_MakeCurrent(window, ctx)) {
        std::cout << "Failed set the gl context as current!" << std::endl;
        exit(1);
    }

    auto renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_Event event;
    const auto render_timer = roundf(1000.0f / (float) FPS);

    // exes are output in the cmake-build-debug directory
    auto font = TTF_OpenFont("..\\assets\\Arial.ttf", 24);
    if (!font) {
        std::cout << "Failed to find font!" << std::endl;
        exit(1);
    }

    bool running = true;

    while (running) {
        const Uint32 start_frame_time = SDL_GetTicks();
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        int w, h;
        const char *text = "SDL Time!";
        TTF_SizeText(font, text, &w, &h);
        SDL_Color white = {255, 255, 255, 255};
        SDL_Color black = {0, 0, 0, 255};
        SDL_Surface *surface;
        surface = TTF_RenderText_Shaded(font, text, white, black);

        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

        SDL_Rect rect = {.x = 10, .y = 10, .w = w, .h = h};
        SDL_RenderCopy(renderer, texture, nullptr, &rect);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);

        const Uint32 end_frame_time = SDL_GetTicks();
        SDL_Delay(std::max(10, int(render_timer) - int(end_frame_time - start_frame_time)));
        SDL_RenderPresent(renderer);
    }

    return 0;
}
