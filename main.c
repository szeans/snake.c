#include <stdio.h>
#include <SDL2/SDL.h>

#define WIDTH (680)
#define HEIGHT (400)
#define WALL_THICKNESS (20)
#define CELL_WIDTH (20)
#define CELL_HEIGHT (20)
#define CELL_COUNT ((WIDTH - WALL_THICKNESS * 2) * (HEIGHT - WALL_THICKNESS * 2) / (CELL_WIDTH * CELL_HEIGHT))
#define SNAKE_START_X (200)
#define SNAKE_START_Y (200)

void initialize(void);
void terminate(int exit_code);
void handle_input(void);
void draw_walls(void);
void draw_snake(void);
void spawn_snake(void);
void move_snake(void);

struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
    int running;
    SDL_Rect snake[CELL_COUNT]; // array for snake
    int dx;
    int dy;
} typedef Game;

// global structure to store game state
// and SDL renderer can use it in all functions
Game game = {
    .running = 1,
    .snake = {0},
    .dx = CELL_WIDTH,
    .dy = 0
};

int main() {
    // initializes SDL and relevant functions
    initialize();

    spawn_snake();

    while (game.running) {
        // clear game screen to black before drawing
        SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
        SDL_RenderClear(game.renderer);

        handle_input();

        // TODO: add update and draw functions
        move_snake();
        // draw_food();
        draw_snake();
        draw_walls();

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
        SDL_DestroyWindow(game.window);
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

void draw_walls() {
    // make wall purple
    SDL_SetRenderDrawColor(game.renderer, 210, 210, 210, 255);

    SDL_Rect block = {
        .x = 0,
        .y = 0,
        .w = WALL_THICKNESS,
        .h = HEIGHT
    };

    // left wall
    SDL_RenderFillRect(game.renderer, &block);

    // right wall
    block.x = WIDTH - WALL_THICKNESS;
    SDL_RenderFillRect(game.renderer, &block);

    // top wall
    block.x = 0;
    block.w = WIDTH;
    block.h = WALL_THICKNESS;
    SDL_RenderFillRect(game.renderer, &block);

    // bottom wall;
    block.y = HEIGHT - WALL_THICKNESS;
    SDL_RenderFillRect(game.renderer, &block);
}

void draw_snake(void) {
    // draw snake head
    SDL_SetRenderDrawColor(game.renderer, 0, 128, 0, 255);
    SDL_RenderFillRect(game.renderer, &game.snake[0]);

    // draw snake
    for (int i = 1; i < sizeof(game.snake) / sizeof(game.snake[0]); i++) {
        // draw active elements only
        if (game.snake[i].w == 0) {
            break;
        }

        // TODO: make snake red when dead

        // draw green cell
        SDL_SetRenderDrawColor(game.renderer, 0, 128, 0 , 255);
        SDL_RenderFillRect(game.renderer, &game.snake[i]);

        // black border around each snake cell
        SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(game.renderer, &game.snake[i]);
    }
}

void spawn_snake() {
    // make snake empty
    for (int i = 0; i < sizeof(game.snake) / sizeof(game.snake[0]); i++) {
        game.snake[i].x = 0;
        game.snake[i].y = 0;
        game.snake[i].w = 0;
        game.snake[i].h = 0;
    }

    // first element is head
    game.snake[0].x = SNAKE_START_X;
    game.snake[0].y = SNAKE_START_Y;
    game.snake[0].w = CELL_WIDTH;
    game.snake[0].h = CELL_HEIGHT;

    // 4 more snake elements for body
    for (int i = 1; i <= 4; i++) {
        game.snake[i] = game.snake[0];
        game.snake[i].x -= CELL_WIDTH * i;
    }
}

void move_snake() {
    // shift elements right to make room for new head
    for (int i = sizeof(game.snake) / sizeof(game.snake[0]) - 1; i >= 0; i--) {
        game.snake[i] = game.snake[i-1];
    }

    // insert head's new position
    game.snake[0].x = game.snake[1].x + game.dx;
    game.snake[0].y = game.snake[1].y + game.dy;
    game.snake[0].w = CELL_WIDTH;
    game.snake[0].h = CELL_HEIGHT;

    //remove tail by finding last inactive element then zero one before it
    for (int i = 5; i < sizeof(game.snake) / sizeof(game.snake[0]); i++) {
        if (game.snake[i].w == 0) {
            game.snake[i-1].x = 0;
            game.snake[i-1].y = 0;
            game.snake[i-1].w = 0;
            game.snake[i-1].h = 0;
            break;
        }
    }

    // TODO:
    // if snake ate food, dont remove tail and increase score
    // check if snake is dead
}