#include <stdio.h>
#include <SDL.h>

#define WIDTH 680
#define HEIGHT 400

void initialize(void);
void terminate(int exit_code);
void handle_input(void);

struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
    int running;
} typedef Game;

// global structure to store game state
// and SDL renderer can use it in all functions
Game game = {
    .running = 1
};

int main() {
    // initializes SDL and relevant functions
    initialize();

    // TODO: initialize spawn snake
    // spawn_snake();

    while (game.running) {
        // clear game screen to black before drawing
        SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
        SDL_RenderClear(game.renderer);

        handle_input();

        // TODO: add update and draw functions
        // move_snake();
        // draw_food();
        // draw_snake();
        // draw_wall;

        SDL_RenderPresent(game.renderer);

        // delay before next iteration;
        SDL_Delay(100);
    }

    // program should clean upon exit
    terminate(EXIT_SUCCESS);
}

void initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("ERROR: failed to init SDL: %s\n", SDL_GetError());
        terminate(EXIT_FAILURE);
    }

    // game window
    game.window = SDL_CreateWindow("Score: 0",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WIDTH,
        HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!game.window) {
        printf("ERROR: failed to open %d x %d window: %s\n", WIDTH, HEIGHT, SDL_GetError());
        terminate(EXIT_FAILURE);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    game.renderer = SDL_CreateRenderer(game.window, -1, SDL_RENDERER_ACCELERATED);

    if (!game.renderer) {
        printf("ERROR: failed to create renderer: %s\n", SDL_GetError());
        terminate(EXIT_FAILURE);
    }
}

void terminate(int exit_code) {
    if (game.renderer) {
        SDL_DestroyRenderer(game.renderer);
    }

    if (game.window) {
        SDL_DestoryWindow(game.window);
    }

    SDL_Quit();
    exit(exit_code);
}

void handle_input() {
    SDL_Event e;
    
    while (SDL_PollEvent(&e)) {
        // game state should be "not running" when close or esc key is pressed
        if ((e.type == SDL_QUIT) || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
            game.running = 0;
        }
    }
}