#include "SDL3/SDL_surface.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>

#define WINDOW_WIDTH 1250
#define WINDOW_HEIGHT 800
#define BLOCK_WIDTH 25
#define NUM_BLOCKS_X (WINDOW_WIDTH / BLOCK_WIDTH)
#define NUM_BLOCKS_Y (WINDOW_HEIGHT / BLOCK_WIDTH)
#define MAX_SNAKE_LENGTH (NUM_BLOCKS_X * NUM_BLOCKS_Y) 
#define INIT_SNAKE_LENGTH 10
#define SURFACE_COLOR 0x007aa976
#define SNAKE_COLOR 0x002d1e1b
#define FOOD_COLOR 0x00d92f20

static bool isSnakeBody[MAX_SNAKE_LENGTH];

int getCellIndex(SDL_Rect* rect) {
    int row = rect->x / 25, col = rect->y / 25;
    return row + NUM_BLOCKS_X * col;
}

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT,
} SnakeDir;

typedef struct {
    SDL_Rect rects[MAX_SNAKE_LENGTH];
    int length;
    SnakeDir dir;
} Snake;

void initSnakeArray(SDL_Surface* surface, Snake* snake) {
    for (int i = 0; i < INIT_SNAKE_LENGTH; i++) {
        int x = WINDOW_WIDTH / 2 - i * BLOCK_WIDTH, y = WINDOW_HEIGHT / 2;
        snake->rects[i] = (SDL_Rect) {x, y, BLOCK_WIDTH, BLOCK_WIDTH};
        isSnakeBody[getCellIndex(&snake->rects[i])] = true;
    }
    SDL_FillSurfaceRects(surface, snake->rects, INIT_SNAKE_LENGTH, SNAKE_COLOR);
}

void moveSnake(SDL_Surface* surface, Snake* snake, bool ateFood) {
    if (!ateFood) {
        SDL_FillSurfaceRect(surface, &snake->rects[snake->length - 1], SURFACE_COLOR);
        isSnakeBody[getCellIndex(&snake->rects[snake->length - 1])] = false;
    } else
        snake->length++;
    
    for (int i = snake->length - 1; i > 0; i--) {
        snake->rects[i] = snake->rects[i - 1];
    }
    if (snake->dir == UP)
        snake->rects[0].y = snake->rects[0].y <= 0 ? WINDOW_HEIGHT - BLOCK_WIDTH : (snake->rects[0].y - BLOCK_WIDTH);
    else if (snake->dir == DOWN)
        snake->rects[0].y = (snake->rects[0].y + BLOCK_WIDTH) % WINDOW_HEIGHT;
    else if (snake->dir == LEFT) 
        snake->rects[0].x = snake->rects[0].x <= 0 ? WINDOW_WIDTH - BLOCK_WIDTH : (snake->rects[0].x - BLOCK_WIDTH);
    else if (snake->dir == RIGHT)
        snake->rects[0].x = (snake->rects[0].x + BLOCK_WIDTH) % WINDOW_WIDTH;
    isSnakeBody[getCellIndex(&snake->rects[0])] = true;
    SDL_FillSurfaceRects(surface, snake->rects, snake->length, SNAKE_COLOR);
}

SDL_Rect generateRandomFood(SDL_Surface* surface) {
    int randx = SDL_rand(NUM_BLOCKS_X);
    int randy = SDL_rand(NUM_BLOCKS_Y);
    while (isSnakeBody[randx + NUM_BLOCKS_X * randy]) {
        randx = SDL_rand(NUM_BLOCKS_X);
        randy = SDL_rand(NUM_BLOCKS_Y);
    }
    SDL_Rect food = {randx * BLOCK_WIDTH, randy * BLOCK_WIDTH, BLOCK_WIDTH, BLOCK_WIDTH};
    SDL_FillSurfaceRect(surface, &food, FOOD_COLOR);
    return food;
}

bool ateSelf(Snake* snake) {
    SDL_Rect nextRect;
    if (snake->dir == UP) {
        int x = snake->rects[0].x, y = snake->rects[0].y - BLOCK_WIDTH;
        nextRect = (SDL_Rect) {x, y, BLOCK_WIDTH, BLOCK_WIDTH};   
    } else if (snake->dir == DOWN) {
        int x = snake->rects[0].x, y = snake->rects[0].y + BLOCK_WIDTH;
        nextRect = (SDL_Rect) {x, y, BLOCK_WIDTH, BLOCK_WIDTH};
    } else if (snake->dir == LEFT) {
        int x = snake->rects[0].x - BLOCK_WIDTH, y = snake->rects[0].y;
        nextRect = (SDL_Rect) {x, y, BLOCK_WIDTH, BLOCK_WIDTH};
    } else if (snake->dir == RIGHT) {
        int x = snake->rects[0].x + BLOCK_WIDTH, y = snake->rects[0].y;
        nextRect = (SDL_Rect) {x, y, BLOCK_WIDTH, BLOCK_WIDTH};
    }
    if (isSnakeBody[getCellIndex(&nextRect)])
        return true;
    return false;
}

void resetGame(SDL_Surface* surface, Snake* snake, SDL_Rect* food) {
    for (int i = 0; i < MAX_SNAKE_LENGTH; i++)
        isSnakeBody[i] = false;
    SDL_FillSurfaceRect(surface, NULL, SURFACE_COLOR);
    snake->dir = RIGHT;
    snake->length = INIT_SNAKE_LENGTH;
    initSnakeArray(surface, snake);
    *food = generateRandomFood(surface);
}

int main(void) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Snake", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Surface* surface = SDL_GetWindowSurface(window);

    Snake snake;
    SDL_Rect food;
    
    resetGame(surface, &snake, &food);

    SDL_UpdateWindowSurface(window);

    SDL_Event event;
    int running = 1;
    bool ateFood = false;
    
    while (running) {
        SDL_PollEvent(&event);
        if (event.type == SDL_EVENT_QUIT)
            running = 0;

        if (ateSelf(&snake)) {
            resetGame(surface, &snake, &food); 
        }

        if (snake.rects[0].x == food.x && snake.rects[0].y == food.y) {
           food = generateRandomFood(surface);
           ateFood = true;
        } 
        
        if (event.key.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_UP && snake.dir != DOWN)
            snake.dir = UP;
        else if (event.key.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_DOWN && snake.dir != UP)
            snake.dir = DOWN;
        else if (event.key.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_LEFT && snake.dir != RIGHT)
            snake.dir = LEFT;
        else if (event.key.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_RIGHT && snake.dir != LEFT)
            snake.dir = RIGHT;

        moveSnake(surface, &snake, ateFood);
        ateFood = false;

        SDL_UpdateWindowSurface(window);
        SDL_Delay(60);
    }
    SDL_DestroyWindowSurface(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
