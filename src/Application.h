#include <SDL.h>

// if we're compiling for iOS (iPhone/iPad)
#ifdef __IPHONEOS__
#	include <SDL2/SDL_opengles.h> // we want to use OpenGL ES
#else
#	include <SDL_opengl.h> // otherwise we want to use OpenGL
#endif

#include <iostream>

struct AppConfig {
	int width = 800;
	int height = 600;
	const char* title = "Raytracer";
};

class Application {
public:
	Application(AppConfig config);
	Application();

	void init();
	void run();
	
private:
	void initSDL();
	void destroy();

private:
	SDL_Window* window;
	SDL_GLContext context;

	AppConfig config;

	bool running = false;
};