#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>

SDL_Window* g_sdl_window = NULL;
SDL_Renderer* g_sdl_renderer = NULL;

static bool g_is_running = false;

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
		800,
		600,
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

void setup(void)
{
	// TODO: Update
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

	setup();

	while (g_is_running) {
		process_input();
		update();
		render();
	}

	return 0;
}
