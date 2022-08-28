#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <SDL.h>

#define WINDOW_WIDTH (800)
#define WINDOW_HEIGHT (600)

SDL_Window* g_sdl_window = NULL;
SDL_Renderer* g_sdl_renderer = NULL;

static bool g_is_running = false;

uint32_t* pa_color_buffer = NULL;

bool init_window(void)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error initializing SDL.\n");
		return false; 
	}

	g_sdl_window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_BORDERLESS				
	);

	if (!g_sdl_window) {
		fprintf(stderr, "Error creating SDL window.\n");
		return false;
	}

	g_sdl_renderer = SDL_CreateRenderer(g_sdl_window, -1, 0);

	if (!g_sdl_renderer) {
		fprintf(stderr, "Error creating SDL renderer.\n");
		return false;
	}

	return true;
}

bool setup(void)
{
	pa_color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * WINDOW_WIDTH * WINDOW_HEIGHT);

	if (!pa_color_buffer) {
		fprintf(stderr, 
		"Error allocating memory of %d bytes for color buffer\n", 
		WINDOW_WIDTH * WINDOW_HEIGHT
		);
		return false;
	}

	return true;
}

void destroy_window(void)
{
	if (pa_color_buffer)
	{
		free(pa_color_buffer);
		pa_color_buffer = NULL;
	}

	SDL_DestroyRenderer(g_sdl_renderer);
	SDL_DestroyWindow(g_sdl_window);
	SDL_Quit();
}

void process_input(void)
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
		case SDL_QUIT: 
			// Click event on close, 'x', button of window
			g_is_running = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				g_is_running = false;
			}
			break;
		default:
			break;
	}
}

void update(void)
{
	// TODO: Update
}

void render(void)
{
	// RGBA
	SDL_SetRenderDrawColor(g_sdl_renderer, 0, 255, 0, 255);

	SDL_RenderClear(g_sdl_renderer);

	SDL_RenderPresent(g_sdl_renderer);
}

int main(void)
{
	g_is_running = init_window();

	if (!setup()) {
		return -1;
	}

	while (g_is_running) {
		process_input();
		update();
		render();
	}

	destroy_window();

	return 0;
}
