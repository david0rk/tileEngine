/*
 * Apologies to anyone who happens to look at this code.
 * I'll clean it up eventually
 * -david.
*/

#include "SDL/SDL.h"
#include <iostream>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 640;
const char* WINDOW_TITLE = "Tile Organizer";
int tileSize = 16; // sets tile size (square) for the renderer.
//The demo sheet I'm using has 16x16 sprites, but a 1px margin.  It feels shitty, but it works and kind of makes sense.

int main(int argc, char **argv)
{
SDL_Init( SDL_INIT_VIDEO ); // AND HE SAYETH LET THERE BE VIDEO!
SDL_Surface* screen = SDL_SetVideoMode( WINDOW_WIDTH,WINDOW_HEIGHT, 0, SDL_HWSURFACE | SDL_DOUBLEBUF );
SDL_WM_SetCaption( WINDOW_TITLE, 0 ); 

// INPUT FILE
SDL_Surface* inputBMP = SDL_LoadBMP("source.bmp"); //load original from disk. 
SDL_Surface* sourceSheet = SDL_DisplayFormat(inputBMP);  //make SDL NATIVE 
SDL_FreeSurface(inputBMP); // unload bitmap.

int sourceWidth = sourceSheet->w; // width of loaded sheet.
int sourceHeight = sourceSheet->h; // height of loaded sheet.


/*
 * pretty much any std:cout in this is debugging info 
*/  
std::cout << "source surface is " << sourceWidth << "wide" <<std::endl;
std::cout << "source surface is " << sourceHeight << "tall" <<std::endl;

//tile from source sheet.
SDL_Rect loadedTile; 
loadedTile.x = 0;
loadedTile.y = 0;
loadedTile.w = tileSize;
loadedTile.h = tileSize;

// Part of the screen we want to draw the tile to
SDL_Rect drawDest;
drawDest.x = 0;
drawDest.y = 0;
drawDest.w = 0;
drawDest.h = 0;

// initial draw (ONE IN THE CHAMBER!) 
SDL_BlitSurface(sourceSheet, &loadedTile, screen, &drawDest); 
SDL_Flip(screen);

//Event loop stop flag.  if its false program disappears.
bool gameRunning = true;
//Events to handle
SDL_Event event;

while (gameRunning)
{	
	if (SDL_PollEvent(&event))
	{
		switch( event.type )
		{
			case SDL_KEYDOWN:  // if key is pressed, draw character sprite.  each press moves character diagonally.
			SDLKey keyPressed = event.key.keysym.sym;
			switch (keyPressed)
			{
				case SDLK_ESCAPE:
					gameRunning = false;
				break;
				
				case SDLK_s:
					SDL_SaveBMP(screen,"output.bmp" );
				break;

				case SDLK_RETURN: // next output row.
					if (drawDest.y < WINDOW_HEIGHT-(tileSize))
					{
						drawDest.y+=tileSize;
						drawDest.x=0;
						//draw
						SDL_BlitSurface(sourceSheet, &loadedTile, screen, &drawDest); 
						SDL_Flip(screen);  
						std::cout << "next blitrow" << std::endl;
					}
					else 
					{
						std::cout << "running off screen!" << std::endl;
						gameRunning =false;
					}
				break;
				/* Spidey sense is tingling like a motherfucker here
				 * I can say if there is anything that fucks up
				 * it will most likely come from these next four cases.
				 * it appears to work, but I have a nagging feeling 
				 * I've forgotten some edge case/weird situation.
				 * -david
				 */
				case SDLK_UP:// next tile
					if (loadedTile.x >= sourceWidth-(tileSize) && loadedTile.y < sourceHeight-(tileSize)) //right edge, not bottom row.
					{
						loadedTile.y+=tileSize; 
						loadedTile.x=0;
						std::cout << "row down" << std::endl;
					}
					else 
					{
						std::cout << "next tile" << std::endl;
						loadedTile.x+=tileSize; 
					}
				break;
				
				case SDLK_DOWN: // previous tile 
					if (loadedTile.x > (tileSize) && loadedTile.y >(tileSize)) //not on first tile/row of input sheet.
					{
						loadedTile.y-=tileSize;
						loadedTile.x=sourceWidth;
						std::cout << "row up" << std::endl;
					}
					else
					{
						std::cout << "prev tile" << std::endl;
						loadedTile.x=loadedTile.x-tileSize; 
					}
				break;
				
				case SDLK_SPACE: // draw. & update drawing location. 
					if (drawDest.x >= WINDOW_WIDTH-(tileSize))  // if we're not running out of the output window horizontally
					{
						drawDest.y += tileSize;  //..move to next row on output sheet
						
						if (drawDest.y >= WINDOW_HEIGHT-(tileSize))  //unless we run off the bottom of the screen 
						{
							std::cout << "running off output window!" << std::endl; //screaming in agony
							gameRunning = false; // and die.
						}
						drawDest.x=0; //otherwise, we reset to the first column.
					}
					else
					{
						std::cout << "blit tile" << std::endl;
						//draw
						SDL_BlitSurface(sourceSheet, &loadedTile, screen, &drawDest);
						SDL_Flip(screen);  
						//move to next horizontal position
					   drawDest.x+=tileSize;
					}
				break;
				
				case SDLK_BACKSPACE: // MOVE TO PREVIOUS LOCATION.
					if (drawDest.x >= WINDOW_WIDTH-(tileSize))
					{
						drawDest.y -= tileSize;  //move to next row on output sheet.
						if (drawDest.y <= WINDOW_HEIGHT-(tileSize))
						{
							std::cout << "running off output window!" << std::endl;
							gameRunning = false;
						}
						drawDest.x=0; //reset to first column.
					}
					else
					{
						std::cout << "blit tile" << std::endl;
						SDL_BlitSurface(sourceSheet, &loadedTile, screen, &drawDest); 
						SDL_Flip(screen); 
					   drawDest.x-=tileSize;
					}
				break;
				
				default:
					std::cout << "not a valid key" << std::endl;
				break;
		
		   }
		   SDL_BlitSurface(sourceSheet, &loadedTile, screen, &drawDest); //blit
		   SDL_Flip(screen);  // flip to screen
		}
		
		
	}
}

}