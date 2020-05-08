#include <SDL.h>

// if we're compiling for iOS (iPhone/iPad)
#ifdef __IPHONEOS__
#	include <SDL2/SDL_opengles.h> // we want to use OpenGL ES
#else
#	include <SDL_opengl.h> // otherwise we want to use OpenGL
#endif

#include <iostream>

#include "ComputeAPI.h"

struct AppConfig {
	int width = 800;
	int height = 600;
	const char* title = "Raytracer";
	SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888);
};

class Application {
public:
	Application(AppConfig config);
	Application();

	void init();
	void run();
	
private:
	void initSDL();
	void initCL();
	void destroy();

private:
	SDL_Window* window;
	SDL_GLContext context;
	SDL_Texture* buffer;
	SDL_Renderer* renderer;

	AppConfig config;

	ComputeAPI* api;

	bool running = false;
};