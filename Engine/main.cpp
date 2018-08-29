#include "SDL/SDL.h"
#include <iostream>
using namespace std;


//Globals
const char* WINDOW_TITLE = "The Tile Whisperer (TM) NOW WITH MOTHERFUCKIN LAYERS";
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 640;
int tileSize = 32; // sets tile size for the renderer. 
int tilePadding=1;
//ifstream configFile (engine.cfg);

//config file example
/*
 * WINDOW_TITLE
 * WINDOW_WIDTH, WINDOW_HEIGHT
 * tileSize,tilePaddinG
 * 
 */

int main(int argc, char **argv)
{
//SETUP SDL
SDL_Init( SDL_INIT_VIDEO );
SDL_Surface* screen = SDL_SetVideoMode( WINDOW_WIDTH,WINDOW_HEIGHT, 0, SDL_HWSURFACE | SDL_DOUBLEBUF );
SDL_WM_SetCaption( WINDOW_TITLE, 0 );

//load spritesheet.
SDL_Surface* spriteSRC = SDL_LoadBMP("sprites.bmp"); 
SDL_Surface* spriteSheet = SDL_DisplayFormat(spriteSRC);
SDL_FreeSurface(spriteSRC); // unload bitmap.

if( spriteSheet != NULL )
	{
		//Map the color key
		Uint32 colorkey = SDL_MapRGB( spriteSheet->format, 0xff, 0, 0xFF );
		//Set all pixels of color R 0, G 0xFF, B 0xFF to be transparent
		SDL_SetColorKey( spriteSheet, SDL_SRCCOLORKEY, colorkey );
	}



//load tilesheet.
SDL_Surface* tileSRC = SDL_LoadBMP("tiles.bmp"); 
SDL_Surface* tileSheet = SDL_DisplayFormat(tileSRC); 
SDL_FreeSurface(tileSRC); // unload bitmap.

if( tileSheet != NULL )
	{
		//Map the color key
		Uint32 colorkey = SDL_MapRGB( tileSheet->format, 0xff, 0, 0xFF );
		//Set all pixels of color R 0, G 0xFF, B 0xFF to be transparent
		SDL_SetColorKey( tileSheet, SDL_SRCCOLORKEY, colorkey );
	}
    
    
//beginnings of sprite coord system, need size of sheet to enumerate based on tileSize
int sheetWidth = spriteSheet -> w; 
int sheetHeight = spriteSheet -> h;
cout << "spritesheet width is: " << sheetWidth  << endl;
cout << "spridesheet height is: " << sheetHeight << endl;
cout << "spritesheet is " << sheetWidth/ tileSize  << " tiles wide, by " << sheetHeight/tileSize << " tiles tall" <<endl;

int mapWidth=WINDOW_WIDTH/tileSize;
int mapHeight=WINDOW_HEIGHT/tileSize;

cout << "map is " << mapWidth << "tiles wide by " << mapHeight << " tiles tall " << mapWidth*mapHeight << " total tiles"<< endl;
    
// tile to draw
SDL_Rect thisTile;
thisTile.x = 0;
thisTile.y = 0;
thisTile.w = tileSize;
thisTile.h = tileSize;

//output surface
SDL_Rect bgLayer;
bgLayer.x = 0;
bgLayer.y = 0;
bgLayer.w = 0;
bgLayer.h = 0;



SDL_Event event;

bool gameRunning = true;

//initialize/populate display 

int stepY = WINDOW_HEIGHT/tileSize;
int stepX = WINDOW_WIDTH/tileSize;
	
    for (int i=0;i<=stepY;i++) //fill columns with tile
{
		for (int j=0;j<=stepX;j++) //fill rows with tile
{
	bgLayer.x=j*tileSize;
	SDL_BlitSurface(tileSheet, &thisTile, screen, &bgLayer);
} 
	bgLayer.y=i*tileSize;
	SDL_BlitSurface(tileSheet, &thisTile, screen, &bgLayer);
} 

//reset drawing position to start at tile 0;
bgLayer.x=0;
bgLayer.y=0;
//first sprite draw
SDL_BlitSurface(spriteSheet, &thisTile, screen, &bgLayer);

//drawing from ram to display
SDL_Flip(screen);

while (gameRunning)
{
	// big block of hardcoded input handling, break out into config file.
if (SDL_PollEvent(&event))
{
		int moveX =0;
		int moveY =0;
	   switch( event.type )
	   {
		   case SDL_KEYDOWN:  // if key is pressed, draw character sprite.  each press moves character diagonally.
		   SDLKey keyPressed = event.key.keysym.sym;
		   switch (keyPressed)
		   {
				case SDLK_DOWN:
				moveY=tileSize;
				moveX=0;
				break;
				case SDLK_UP:
				moveY=tileSize*-1;
				moveX=0;
				break;
				case SDLK_LEFT:
				moveY=0;
				moveX=tileSize*-1;
				break;
				case SDLK_RIGHT:
				moveY=0;
				moveX=tileSize;
				break;
		   }
		   thisTile.y=0; // redraw floor before moving character < needs to be updated to draw from data structure in ram.
		   //redraw tile before moving sprite
           SDL_BlitSurface(tileSheet, &thisTile, screen, &bgLayer); // draw
           /*TODO
            * ADD BOUNDARY CHECK HERE TO KEEP PLAYER FROM RUNNING OFF SCREEN 
           */
           //move drawing location
           bgLayer.x=bgLayer.x+moveX;
           bgLayer.y=bgLayer.y+moveY;
           //draw sprite
           SDL_BlitSurface(spriteSheet, &thisTile, screen, &bgLayer); // draw
           
           int spriteX = bgLayer.x/tileSize;
           int spriteY =bgLayer.y/tileSize;
            
           cout << "sprite moved " << spriteX << " , " <<  spriteY  << endl;
           cout << "tileno " << ((spriteY*mapHeight) + (spriteX)) << endl;
           SDL_Flip(screen); // spriteSheets/drawing are upside down.
           break;
		}
	if (event.type == SDL_QUIT)
	{
		gameRunning = false;
	}
}

}

SDL_FreeSurface(spriteSheet); //free spritesheet
SDL_FreeSurface(tileSheet); //free tilesheet

SDL_Quit();
return 0;
}

