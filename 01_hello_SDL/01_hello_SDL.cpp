/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
  and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Key press surface constants
enum KeyPressSurfaces
{
  KEY_PRESS_SURFACE_DEFAULT,
  KEY_PRESS_SURFACE_UP,
  KEY_PRESS_SURFACE_DOWN,
  KEY_PRESS_SURFACE_LEFT,
  KEY_PRESS_SURFACE_RIGHT,
  KEY_PRESS_SURFACE_TOTAL
};

// Function declarations

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image as texture
SDL_Texture* loadTexture( std::string path );

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Current displayed texture
SDL_Texture* gTexture = NULL;

//Loads individual image
SDL_Surface* loadSurface( std::string path );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//Current displayed PNG image
SDL_Surface* gPNGSurface = NULL;

//The images that corresspond to a keypress
SDL_Surface* gKeyPressSurfaces[ KEY_PRESS_SURFACE_TOTAL ];

//Current displayed image
SDL_Surface* gStretchedSurface = NULL;

bool init()
{
  //Initialization flag
  bool success = true;

  //Initialize SDL
  if( SDL_Init( SDL_INIT_VIDEO ) == -1 )
  {
    printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    success = false;
  }
  else
  {
    //Create window
    gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    if(gWindow == NULL)
    {
      printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
      success = false;
    }
    else
    {
      //Create renderer for window
      gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
      if( gRenderer == NULL )
      {
        printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
        success = false;
      }
      else
      {
        //Initialize renderer colour
        SDL_SetRenderDrawColor( gRenderer, 0xFF,0xAF,0xAF,0xAF );

        //Initialize PNG loading
        int imgFlags = IMG_INIT_PNG;
        if( !( IMG_Init( imgFlags ) & imgFlags ) )
        {
          printf( "SDL_image could not initialize! SDL_Image Error: %s\n", IMG_GetError() );
          success = false;
        }
      }
    }
  }

  return success;
}

bool loadMedia( std::string path )
{
  //Loading flag
  bool success = true;

  //Load texture
  gTexture = loadTexture( path );
  if( gTexture == NULL )
  {
    printf( "Failed to load texture image!\n" );
    success = false;
  }

  return success;
}

void close()
{
  //Free loaded image
  SDL_DestroyTexture( gTexture );
  gTexture = NULL;

  //Destroy window and renderer
  SDL_DestroyWindow( gWindow );
  SDL_DestroyRenderer( gRenderer );
  gWindow = NULL;
  gRenderer = NULL;

  //Quit SDL subsystems
  IMG_Quit();
  SDL_Quit();
}

SDL_Texture* loadTexture( std::string path )
{
  //The final texture
  SDL_Texture* newTexture = NULL;

  //Load image at specified path
  SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
  if( loadedSurface == NULL )
  {
    printf( "Unable to load image %s! SDL Error %s!\n", path.c_str(), SDL_GetError() );
  }
  else
  {
    //Create texture from surface pixals
    newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
    if( newTexture == NULL )
    {
      printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
    }

    //Get rid of the old loaded surface
    SDL_FreeSurface( loadedSurface );
  }

  return newTexture;
}

SDL_Surface* loadSurface( std::string path )
{
  //The final optimized image
  SDL_Surface* optimizedSurface = NULL;

  //Load image at specified path
  SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
  if( loadedSurface == NULL )
  {
    printf( "Unable to load image %s! SDL_Error: %s.\n", path.c_str(), SDL_GetError() );
  }
  else
  {
    //Convert surface to screen format
    optimizedSurface = SDL_ConvertSurface( loadedSurface, gScreenSurface->format, NULL );
    if( optimizedSurface == NULL )
    {
      printf( "Unable to optimize image %s! SDL_Error: %s\n", path.c_str(), SDL_GetError() );
    }
    //Remove old loaded surface from memory
    SDL_FreeSurface( loadedSurface );
  }
  return optimizedSurface;
}


int main( int argc, char* args[] )
{
  if( !init() )
  {
    printf("Failed to initialize!\n" );
  }
  else
  {
    //Main loop flag
    bool quit = false;

    //Event handler
    SDL_Event e;

    //While application is running
    while( !quit )
    {
      //Handle events on queue
      while( SDL_PollEvent( &e ) != 0 )
      {
        //User requests quit
        if( e.type == SDL_QUIT )
        {
          quit = true;
        }
      }

      if( !loadMedia( "09_the_viewport/viewport.png" ) )
      {
        printf( "Failed to load media!\n" );
      }
      else
      {
        //Clear screen
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( gRenderer );

        //Top left corner viewport
        SDL_Rect topLeftViewport;
        topLeftViewport.x = 0;
        topLeftViewport.y = 0;
        topLeftViewport.w = SCREEN_WIDTH / 2;
        topLeftViewport.h = SCREEN_HEIGHT / 2;
        SDL_RenderSetViewport( gRenderer, &topLeftViewport );

        //Render texture to screen
        SDL_RenderCopy( gRenderer, gTexture, NULL, NULL );

        if( !loadMedia( "/home/adam/Pictures/otb.png" ) )
        {
          printf( "Unable to load media!\n" );
        }
        else
        {
          //Top right viewport
          SDL_Rect topRightViewport;
          topRightViewport.x = SCREEN_WIDTH / 2;
          topRightViewport.y = 0;
          topRightViewport.w = SCREEN_WIDTH / 2;
          topRightViewport.h = SCREEN_HEIGHT / 2;
          SDL_RenderSetViewport( gRenderer, &topRightViewport );

          //Render texture to screen
          SDL_RenderCopy( gRenderer, gTexture, NULL, NULL );
        }

        if( !loadMedia( "/home/adam/Pictures/logo-abe1.png" ) )
        {
          printf( "Unable to load media!\n" );
        }
        else
        {
          //Bottom viewport
          SDL_Rect bottomViewport;
          bottomViewport.x = 0;
          bottomViewport.y = SCREEN_HEIGHT / 2;
          bottomViewport.w = SCREEN_WIDTH;
          bottomViewport.h = SCREEN_HEIGHT / 2;
          SDL_RenderSetViewport( gRenderer, &bottomViewport );

          //Render texture to screen
          SDL_RenderCopy( gRenderer, gTexture, NULL, NULL );
        }

        //Update screen
        SDL_RenderPresent( gRenderer );
      }
    }
  }

  //Free resources and close SDL
  close();

  return 0;
}
