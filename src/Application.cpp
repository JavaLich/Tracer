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
	initSDL();
}

void Application::run()
{
	running = true;

	SDL_Event event;	 // used to store any events from the OS
	glClearColor(1, 1, 0, 1);
	while (running)
	{
		// poll for events from SDL
		while (SDL_PollEvent(&event))
		{
			// determine if the user still wants to have the window open
			// (this basically checks if the user has pressed 'X')
			running = event.type != SDL_QUIT;
		}

		glClear(GL_COLOR_BUFFER_BIT);

		// Swap OpenGL buffers
		SDL_GL_SwapWindow(window);
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
}

void Application::destroy()
{
	// Destroy the context
	SDL_GL_DeleteContext(context);

	// Destroy the window
	SDL_DestroyWindow(window);

	// And quit SDL
	SDL_Quit();
}
