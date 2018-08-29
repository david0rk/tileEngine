#include "SDL/SDL.h"
#include <iostream>
#include <fstream>
using namespace std;


//Globals
char* WINDOW_TITLE = "The Tile Whisperer (TM)";
int WINDOW_WIDTH = 1280;
int WINDOW_HEIGHT = 640;
int tileSize = 64; // sets tile size for the renderer.
int tilePad=1;



//config file example
/*
 * WINDOW_TITLE
 * WINDOW_WIDTH
 * WINDOW_HEIGHT
 * tileSize
 * tilePad
 *
 */
int main(int argc, char **argv)
{
    /*
    ifstream confFile ("engine.cfg");
    // need to break out configs from strings
    //can use stoi
    while(!confFile.eof())
        {
            getline(confFile,WINDOW_WIDTH);
            getline(confFile,WINDOW_HEIGHT);
            getline(confFile,tileSize);
            getline(confFile,tilePad);
        }
    confFile.close();
    */

//SETUP SDL
    SDL_Init( SDL_INIT_VIDEO );
    SDL_Surface* screen = SDL_SetVideoMode( WINDOW_WIDTH,WINDOW_HEIGHT, 0, SDL_HWSURFACE | SDL_DOUBLEBUF );
    SDL_WM_SetCaption( WINDOW_TITLE, 0 );

//load spritesheet.
    SDL_Surface* spriteSrcImg = SDL_LoadBMP("sprites.bmp");
    SDL_Surface* spriteSheet = SDL_DisplayFormat(spriteSrcImg);
    SDL_FreeSurface(spriteSrcImg); // unload bitmap.
    if( spriteSheet != NULL ) {
        //Map the color key
        Uint32 colorkey = SDL_MapRGB( spriteSheet->format, 0xff, 0, 0xFF );
        //Set all pixels of color R 0, G 0xFF, B 0xFF to be transparent
        SDL_SetColorKey( spriteSheet, SDL_SRCCOLORKEY, colorkey );
    }

//load tilesheet.
    SDL_Surface* tileSrcImg = SDL_LoadBMP("tiles.bmp");
    SDL_Surface* tileSheet = SDL_DisplayFormat(tileSrcImg);
    SDL_FreeSurface(tileSrcImg); // unload bitmap.
    if( tileSheet != NULL ) {
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
    SDL_Rect curTile;
    curTile.x = 0;
    curTile.y = 0;
    curTile.w = tileSize;
    curTile.h = tileSize;

//output surface
    SDL_Rect drawLoc;
    drawLoc.x = 0;
    drawLoc.y = 0;
    drawLoc.w = 0;
    drawLoc.h = 0;



    SDL_Event event;

    bool gameRunning = true;

//initialize map (fill with first tile)
    int stepY = WINDOW_HEIGHT/tileSize;
    int stepX = WINDOW_WIDTH/tileSize;
    for (int i=0; i<=stepY; i++) { //fill columns with tile
        for (int j=0; j<=stepX; j++) { //fill rows with tile
            drawLoc.x=j*tileSize;
            SDL_BlitSurface(tileSheet, &curTile, screen, &drawLoc);
        }
        drawLoc.y=i*tileSize;
        SDL_BlitSurface(tileSheet, &curTile, screen, &drawLoc);
    }

//reset drawing position to start at tile 0 and initial sprite draw
    drawLoc.x=0;
    drawLoc.y=0;
    SDL_BlitSurface(spriteSheet, &curTile, screen, &drawLoc);
//Display Drawing.
    SDL_Flip(screen);

    while (gameRunning) {
        if (SDL_PollEvent(&event)) {
            int moveX =0;
            int moveY =0;
            switch( event.type ) {
            case SDL_KEYDOWN:  // if key is pressed, draw character sprite.  each press moves character diagonally.
                SDLKey keyPressed = event.key.keysym.sym;
                switch (keyPressed) {
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

                //Redraw clean floor over current sprite location.
                SDL_BlitSurface(tileSheet, &curTile, screen, &drawLoc);

                //ADD BOUNDARY CHECK HERE TO KEEP PLAYER FROM RUNNING OFF SCREEN

                //Move drawing location based on user input & draw sprite.
                drawLoc.x=drawLoc.x+moveX;
                drawLoc.y=drawLoc.y+moveY;
                SDL_BlitSurface(spriteSheet, &curTile, screen, &drawLoc); // draw

                //debugging/testing info
                int spriteX = drawLoc.x/tileSize;
                int spriteY = drawLoc.y/tileSize;
                cout << "sprite moved " << spriteX << " , " <<  spriteY  << endl;
                cout << "tileno " << ((spriteY*mapHeight) + (spriteX)) << endl;
                //output to screen
                SDL_Flip(screen);
                break;
            }

            if (event.type == SDL_QUIT) {
                gameRunning = false;
            }
        }

    }

    SDL_FreeSurface(spriteSheet); //free spritesheet
    SDL_FreeSurface(tileSheet); //free tilesheet
    SDL_Quit();
    return 0;
}
