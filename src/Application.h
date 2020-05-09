#include <SDL.h>

// if we're compiling for iOS (iPhone/iPad)
#ifdef __IPHONEOS__
#	include <SDL2/SDL_opengles.h> // we want to use OpenGL ES
#else
#	include <SDL_opengl.h> // otherwise we want to use OpenGL
#endif

#include <iostream>

#include "ComputeAPI.h"

#define FRAME_SAMPLE 100

struct AppConfig {
	unsigned int width = 800;
	unsigned int height = 600;
	const char* title = "Raytracer";
	SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
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
	void benchmark();

private:
	SDL_Window* window;
	SDL_GLContext context;
	SDL_Texture* buffer;
	SDL_Renderer* renderer;

	AppConfig config;

	ComputeAPI* api;

	uint32_t frametimes[FRAME_SAMPLE];
	uint32_t currentIndex = 0;
	uint32_t lastFrame;
	double averageFrameTime;
	double averageFPS;
	uint32_t benchmark_count = 0;

	bool running = false;
};