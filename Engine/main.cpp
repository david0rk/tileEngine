#include "SDL/SDL.h"
#include <iostream>
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 640;
const char* WINDOW_TITLE = "I am the tile whisperer";
int tilesize = 16; // sets tile size for the renderer. 

int main(int argc, char **argv)
{
SDL_Init( SDL_INIT_VIDEO );
SDL_Surface* screen = SDL_SetVideoMode( WINDOW_WIDTH,WINDOW_HEIGHT, 0, SDL_HWSURFACE | SDL_DOUBLEBUF );
SDL_WM_SetCaption( WINDOW_TITLE, 0 );

SDL_Surface* bitmap = SDL_LoadBMP("art.bmp"); // spritesheet load
SDL_Surface* spriteSheet = SDL_DisplayFormat(bitmap);  // spritesheet convert/optimize.
SDL_FreeSurface(bitmap); // unload bitmap.

 //If the image was optimized just fine
if( spriteSheet != NULL )
	{
		//Map the color key
		Uint32 colorkey = SDL_MapRGB( spriteSheet->format, 0xff, 0, 0xFF );
		//Set all pixels of color R 0, G 0xFF, B 0xFF to be transparent
		SDL_SetColorKey( spriteSheet, SDL_SRCCOLORKEY, colorkey );
	}
	
// source tile
SDL_Rect thisTile;   //  SPRITE LAYER!  LAYER 2 
thisTile.x = 0;
thisTile.y = 0;
thisTile.w = tilesize;
thisTile.h = tilesize;

// Part of the screen we want to draw the tile to
SDL_Rect bgLayer; //  BACKGROUND LAYER!  0
bgLayer.x = 0;
bgLayer.y = 0;
bgLayer.w = 0;
bgLayer.h = 0;


/* 
 * NEW STUFF FOR NEXT REVISION STARTS HERE!
 * drawing layers!
 * 0 = background.  Sky, noninteractive shit.
 * 1 = decorations. decorations, possibly interactive/movable 
 * 2 = sprites. Player, NPCs, items
 * 3 = fx and overlays  shadows, sprite effects.  Fade out 50% black for menus
 * 4 = hud/menus  menus (options/etc) and HUD (points, lives, etc)
 * 
 * basically the screen is drawn from back to front (painters algorithm) 
 * would suffer from overdraw inefficiency (drawing background to have foreground elements over them) but only once.
 * composited layers will be stored in ram and can be blitted to the screen at once once inital draw in ram is done.
 * redraw can be done by blitting the entire "assmbled" rectangle the clean copy in ram (i think)
 * however it may not be needed if only one tile changes, could just draw that one tile instead of the whole thing, but if more than one tile changes it may be faster than instead of iterating through the whole thing just redrawing the whole layer in one pass.
 * 
 * loading/generating layers is gonna take a bit of work.
 * essentially a numeric array.  
 * 0, 0 ,0 ,0
 * 0, 0, 0, 0
 * 0, 0, 0, 0
 * 1, 1, 1, 1
 * screen is 4x4 tile grid (in this example)
 * top three rows are blank,  bottom row is first tile. 
 * 2, 2 ,2, 2
 * 0, 0, 0, 0
 * 0, 0, 0, 0
 * 2, 2, 2, 2
 * screen is 4x4 tile grid (in this example)
 * top and bottom row is second tile, middle two rows are blank. 
 * essentially multiply tilesize x number = tile location, may need to do some math to make rows in spritesheets work instead of one long ass row. 
 * 
 * KEEP IN MIND, DRAWING STARTS IN THE TOP LEFT CORNER, AND ENDS IN THE BOTTOM RIGHT CORNER.
 * 
 * tiler tool needs to be reworked into a mapping tool, load functions and such for loading spritesheets.. will autogen map files.
 * 
 * map file format  rough draft.
 * first line = spritesheet name.
 * second line begins raw tile data array.
 * endline = next row in  drawing (maybe should specify mapsize on second line?  we'll cross that bridge when we get to it) 
 * may need to generate configs for the engine to specify a range ofwhat tiles/sprites should be interactive (doors, keys/etc)
 * may want to make a spritesheet assembler (uniform spacing/enumeration and labeling FTW!)
 * 
 * essentially the goal is a event drive (turn based) tiling engine that can be edited with the game, 1 map/tile assembler tool, notepad and paint.  Its gonna take FOREVER-ish probably. 
 */

SDL_Rect decoLayer; // DECORATION LAYER!  1
decoLayer.x = 0;
decoLayer.y = 0;
decoLayer.w = 0;
decoLayer.h = 0;

SDL_Rect fxLayer;  // FX LAYER! 3
fxLayer.x = 0; 
fxLayer.y = 0; 
fxLayer.w = 0; 
fxLayer.h = 0; 

SDL_Rect uiLayer; // UI LAYER!  4
uiLayer.x = 0; 
uiLayer.y = 0; 
uiLayer.w = 0; 
uiLayer.h = 0; 

/*
  // prototype for "drawlayer" 
 
  void drawlayer (int layerToDraw)
  {
  	int tilecount = layer.GetTileCount(int layerToDraw);
  	for int i =0 i <= tilecount; i++
  {
   Draw(tiles)
  }
  Blit screen 
 }
 */






SDL_Event event;

bool gameRunning = true;

// rudimentary "how many tiles fit in the window we're drawing to"
int stepY = WINDOW_HEIGHT/tilesize;
int stepX = WINDOW_WIDTH/tilesize;

	for (int i=0;i<=stepY;i++) //fill columns with tile
{
		for (int j=0;j<=stepX;j++) //fill rows with tile
{
	bgLayer.x=j*tilesize;
	SDL_BlitSurface(spriteSheet, &thisTile, screen, &bgLayer);
} 
	bgLayer.y=i*tilesize;
	SDL_BlitSurface(spriteSheet, &thisTile, screen, &bgLayer);
	
} 
SDL_Flip(screen);


while (gameRunning)
{
	
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
				moveY=tilesize;
				moveX=0;
				break;
				case SDLK_UP:
				moveY=tilesize*-1;
				moveX=0;
				break;
				case SDLK_LEFT:
				moveY=0;
				moveX=tilesize*-1;
				break;
				case SDLK_RIGHT:
				moveY=0;
				moveX=tilesize;
				break;
		   }
		   thisTile.y=0; // redraw floor before moving character < needs to be updated to draw from data structure in ram.
		   SDL_BlitSurface(spriteSheet, &thisTile, screen, &bgLayer); // draw
		   thisTile.y=tilesize; //position of next sprite  (down the sheet) 
		   
		   if (bgLayer.y >= WINDOW_HEIGHT || bgLayer.x >= WINDOW_WIDTH)
		   {
			   bgLayer.x = 0;
			   bgLayer.y = 0;
			}
			else
			{
				bgLayer.x=bgLayer.x+moveX;
				bgLayer.y=bgLayer.y+moveY;
			}
		
			SDL_BlitSurface(spriteSheet, &thisTile, screen, &bgLayer); // draw
			SDL_Flip(screen); // spriteSheets/drawing are upside down.
			break;
		}
	if (event.type == SDL_QUIT)
	{
		gameRunning = false;
	}
}

//SDL_BlitSurface(spriteSheet, &thisTile, screen, &bgLayer);
//SDL_Flip(screen);

}
SDL_FreeSurface(spriteSheet); // clear drawing.
SDL_Quit();
return 0;
}

