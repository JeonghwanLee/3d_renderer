#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <memory.h>
#include <SDL.h>


size_t  WINDOW_WIDTH;
size_t  WINDOW_HEIGHT;
size_t COLOR_BUFFER_BYTE_SIZE;

SDL_Window* g_sdl_window = NULL;
SDL_Renderer* g_sdl_renderer = NULL;
SDL_Texture* g_sdl_texture = NULL;

uint32_t* pa_color_buffer = NULL;
static bool g_is_running = false;


bool init_window(void)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error initializing SDL.\n");
		return false; 
	}

	// Get full screen max width and height
	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);

	WINDOW_WIDTH = display_mode.w;
	WINDOW_HEIGHT = display_mode.h;
	COLOR_BUFFER_BYTE_SIZE = sizeof(uint32_t) * WINDOW_WIDTH * WINDOW_HEIGHT;

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

	SDL_SetWindowFullscreen(g_sdl_window, SDL_WINDOW_FULLSCREEN_DESKTOP);

	return true;
}

bool setup(void)
{
	pa_color_buffer = (uint32_t*) malloc(COLOR_BUFFER_BYTE_SIZE);

	if (!pa_color_buffer) {
		fprintf(
			stderr, 
			"Error allocating memory of %lu bytes for color buffer\n", 
			WINDOW_WIDTH * WINDOW_HEIGHT
		);
		return false;
	}

	// used to display the above color buffer
	g_sdl_texture = SDL_CreateTexture(
		g_sdl_renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		WINDOW_WIDTH,
		WINDOW_HEIGHT
	);

	if (!g_sdl_texture) {
		fprintf(stderr, "Error creating SDL texture.\n");
		return false;
	}

	return true;
}

void clear_color_buffer(void)
{
	for (size_t row = 0; row < WINDOW_HEIGHT; row++) {
		for (size_t col = 0; col < WINDOW_WIDTH; col++) {
			pa_color_buffer[row * WINDOW_WIDTH + col] = 0xFFFFFFFF;
		}
	}
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

void render_color_buffer(void)
{
	SDL_UpdateTexture(
		g_sdl_texture,
		NULL,
		pa_color_buffer,
		(int)(WINDOW_WIDTH * sizeof(uint32_t))
	);
	
	SDL_RenderCopy(
		g_sdl_renderer, 
		g_sdl_texture, 
		NULL, 
		NULL
	);
}

void draw_grid(void)
{
	for (size_t row = 0; row < WINDOW_HEIGHT; row++) {
		for (size_t col = 0; col < WINDOW_WIDTH; col++) {
			if (row % 10 == 0 || col % 10 == 0) {
				pa_color_buffer[row * WINDOW_WIDTH + col] = 0xFF000000;
			}
		}
	}
}

void render(void)
{
	// RGBA
	SDL_SetRenderDrawColor(g_sdl_renderer, 0, 0, 0, 255);
	SDL_RenderClear(g_sdl_renderer);

	draw_grid();

	render_color_buffer();
	clear_color_buffer();

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
