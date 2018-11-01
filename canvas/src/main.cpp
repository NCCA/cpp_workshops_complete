// Some crap we need to compile on Windows (not tested)
#ifdef _WIN32
#include <windows.h>
#endif

// Includes needed for SDL and GL
#include <SDL2/SDL.h>
#include <iostream>

// Include the header file for our current scene
#include "canvas.h"

// Change this if you want something different.
#define WINDOW_TITLE "Canvas Application"

// These defines are for the initial window size
#define WIDTH 640
#define HEIGHT 480

// Our scene, which will store all the drawing stuff
Canvas canvas;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

// The renderer
SDL_Renderer *gRenderer = NULL;

/**
 * @brief initSDL fires up the SDL window and readies it for OpenGL
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int initSDL()
{
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        return EXIT_FAILURE;
    }
    else
    {
        //Create window
        gWindow = SDL_CreateWindow( "SDL Tutorial",
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    WIDTH,
                                    HEIGHT,
                                    SDL_WINDOW_SHOWN );
        if( gWindow == NULL ) {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            return EXIT_FAILURE;
        } else {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
        }
    }

    return EXIT_SUCCESS;
}

/**
 * @brief main The main opengl loop is managed here
 * @param argc Not used
 * @param args Not used
 * @return EXIT_SUCCESS if it went well!
 */
int main( int argc, char* args[] ) {
    //Start up SDL and create window
    if( initSDL() == EXIT_FAILURE ) return EXIT_FAILURE;

    //Main loop flag
    bool quit = false;

    //Event handler
    SDL_Event e;

    //Enable text input
    SDL_StartTextInput();

    // Mouse position variables
    int x = 0, y = 0;

    //While application is running
    while( !quit )
    {
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 )
        {
            // The window has been resized
            if ((e.type == SDL_WINDOWEVENT) &&
                    (e.window.event == SDL_WINDOWEVENT_RESIZED)) {
                SDL_SetWindowSize(gWindow, e.window.data1, e.window.data2);
            }
            //User requests quit
            else if( e.type == SDL_QUIT ) {
                quit = true;
            }
            //Handle keypress with current mouse position
            else if( e.type == SDL_TEXTINPUT ) {
                // Only handle the key if it is printable
                char key = e.text.text[0];

                // Handle the user input by the canvas
                SDL_GetMouseState( &x, &y );
                canvas.handleKey(key, x, y );
            }
            // Handle mouse down
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                SDL_GetMouseState( &x, &y );
                canvas.mouseDown(x, y);
            } // Handle mouse up
            else if (e.type == SDL_MOUSEBUTTONUP) {
                SDL_GetMouseState( &x, &y );
                canvas.mouseUp(x, y);
            } // Handle mouse moving
            else if (e.type == SDL_MOUSEMOTION) {
                SDL_GetMouseState( &x, &y );
                canvas.mouseMove(x, y);
            }
        }

        //Clear screen
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( gRenderer );

        //Render the scene
        canvas.draw(gRenderer);

        //Update screen
        SDL_RenderPresent( gRenderer );
    }

    //Disable text input
    SDL_StopTextInput();

    //Destroy window
    SDL_DestroyWindow( gWindow );

    //Quit SDL subsystems
    SDL_Quit();

    return EXIT_SUCCESS;
}
