#include "Application.h"

Application::Application(AppConfig config)
{
	this->config = config;
	init();
}

Application::Application()
{
	this->config = AppConfig();
	init();
}

void Application::init()
{
	initCL();
	initSDL();
}


void Application::run()
{
	running = true;
	bool leftMouseButtonDown = false;

	uint32_t* pixels = new uint32_t[(size_t)config.width * config.height];
	memset(pixels, 0, (size_t)config.width * config.height * sizeof(uint32_t));
	int pitch = config.width * sizeof(uint32_t);


	SDL_Event event;	 // used to store any events from the OS
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	while (running)
	{
		// poll for events from SDL
		while (SDL_PollEvent(&event))
		{
			running = event.type != SDL_QUIT;
		}
		api->render(pixels, config.width, config.height);
		SDL_UpdateTexture(buffer, NULL, pixels, pitch);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, buffer, NULL, NULL);
		SDL_RenderPresent(renderer);
	}
}

void Application::initSDL()
{
	// Initialize SDL with video
	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow(config.title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, config.width, config.height, SDL_WINDOW_OPENGL);

	// if failed to create a window
	if (!window)
	{
		// we'll print an error message and exit
		std::cerr << "Error failed to create window!\n";
	}

	// Create an OpenGL context (so we can use OpenGL functions)
	context = SDL_GL_CreateContext(window);

	// if we failed to create a context
	if (!context)
	{
		// we'll print out an error message and exit
		std::cerr << "Error failed to create a context\n!";
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	buffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STATIC, config.width, config.height);
}
void Application::initCL()
{
	api = new ComputeAPI();
}

void Application::destroy()
{
	SDL_DestroyTexture(buffer);
	SDL_DestroyRenderer(renderer);

	// Destroy the context
	SDL_GL_DeleteContext(context);

	// Destroy the window
	SDL_DestroyWindow(window);

	// And quit SDL
	SDL_Quit();
}
