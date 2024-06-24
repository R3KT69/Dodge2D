#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <sstream>   
#include <iomanip>

#include "objects.h"
#include "text.h"
#include "random.h"

#define FPS 60
#define FRAMEDELAY 1000 / FPS

using namespace std;

bool event_manager(SDL_Event &_event, hierarchy &_main_instance, bool *_gameState)
{
    while (SDL_PollEvent(&_event))
    {
        if (_event.type == SDL_QUIT)
        {
            return true;
        }

        if (_event.type == SDL_KEYDOWN)
        {
            switch (_event.key.keysym.sym)
            {
                case SDLK_0:
                    _main_instance.objects[0].updateWIDTH(25);
                    cout << "Pressed 0" << endl;
                    break;

                case SDLK_9:
                    _main_instance.objects[0].updateHEIGHT(25);
                    cout << "Pressed 9" << endl;
                    break;

                case SDLK_MINUS:
                    _main_instance.objects[0].updateHEIGHT(-25);
                    _main_instance.objects[0].updateWIDTH(-25);
                    cout << "Pressed -" << endl;
                    break;
                
                case SDLK_p:
                    if (*_gameState == true)
                    {
                        *_gameState = false;
                    }

                    else if (*_gameState == false)
                    {
                        *_gameState = true;
                    }
                    
                
                default:
                    break;
            }
        }
    }
    return false;
}

void logic(hierarchy &_main_instance, int *_faceR)
{
    SDL_Rect player = _main_instance.objects[0].rect; 
    SDL_Rect wall = _main_instance.objects[1].rect;
    
    const Uint8 *currentKeyState = SDL_GetKeyboardState(NULL);

    if (currentKeyState[SDL_SCANCODE_W])
    {
        if (player.y > 25) 
        { 
            _main_instance.objects[0].updateY(-5);
        }
    }

    if (currentKeyState[SDL_SCANCODE_A])
    {
        *_faceR = 0;
        if (player.x > 0) 
        { 
            _main_instance.objects[0].updateX(-5);
        }
    }

    if (currentKeyState[SDL_SCANCODE_S])
    {
        if (player.y < 720) 
        { 
            _main_instance.objects[0].updateY(5);
        }
    }

    if (currentKeyState[SDL_SCANCODE_D])
    {
        *_faceR = 1;
        if (player.x < 751) 
        { 
            _main_instance.objects[0].updateX(5);
        }
    }
    
    
}

void spawn_enemy(hierarchy &_enemy, const int count)
{
    for (int i = 0; i < count; i++)
    {
        int randX = random(15,100);
        
        _enemy.objects[i] = object(0 + (125*i), -200, 48, 48, "enemy");
        _enemy.objects[i].rect.x += randX;

        // cout << to_string(_enemy.objects[i].rect.x) << endl;
        // cout << to_string(enemy.objects[i].rect.y) << endl;
    }
    
    
}

void level_logic(hierarchy &_main_instance, hierarchy &_enemy, const int count, float *speed, int *level)
{
    bool hitBottom = false;
    bool hitBlock = false;

    for (int i = 0; i < count; i++)
    {
        if (_enemy.objects[i].rect.y > 899)
        {
           hitBottom = true;
        }
        

        if (_enemy.objects[i].rect.y < 900)
        {
            _enemy.objects[i].rect.y += *speed;
            
        } 

        if (SDL_HasIntersection(&_main_instance.objects[0].rect, &_enemy.objects[i].rect))
        {
            hitBlock = true;
            break;
        }
        
        
        
    }

    if (hitBlock == true)
    {
        spawn_enemy(_enemy, count);
        _main_instance.objects[0]._health -= 1;
        if (*speed >= 7.0f)
        {
            *speed -= 2.5f;
        }

        if (*speed >= 10.0f)
        {
            *speed -= 5.0f;
        }
        
        
    }
    
    else if (hitBottom == true)
    {
        *level += 1;
        *speed += 0.5f;
        spawn_enemy(_enemy, count);
    }


    // cout << "CurrentSpeed: " << *speed << endl;


    
    
    
}

string convert_1f(float *_speed)
{
    ostringstream str1;

    str1 << fixed << setprecision(1) << *_speed;

    string final_str = str1.str();

    return final_str;
}


int main(int argc, char* argv[]) {
    
    SDL_Window *window = SDL_CreateWindow("Dodge2D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "Initialization Failed " << SDL_GetError() << endl;
        SDL_Quit();
        return 1; 
    }

    if (TTF_Init() < 0) {
        cout << "Initialization Failed " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }  

    if (IMG_Init(IMG_INIT_PNG) < 0)
    {
        cout << "Initialization Failed " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }
    
    
    
    text Hud(TTF_OpenFont("font.ttf", 24), "Default text", {0,255,0});
    text HelpText(TTF_OpenFont("font.ttf", 16), "Default text", {255,255,255});
    text postGame(TTF_OpenFont("font.ttf", 48), "Default text", {0,255,0});
    SDL_Texture *backgroundTexture = IMG_LoadTexture(renderer, "images\\Floor.png");
    SDL_Texture *playerTexture = IMG_LoadTexture(renderer, "images\\player.png");
    SDL_Texture *playerTexture_r = IMG_LoadTexture(renderer, "images\\player_reverse.png");
    SDL_Texture *healthTexture = IMG_LoadTexture(renderer, "images\\heart.png");
    SDL_Texture *anvilTexture = IMG_LoadTexture(renderer, "images\\anvil.png");
    SDL_Rect heart = {690,3,20,20};
    SDL_Rect screen = {0,0,800,800};
    
    

    hierarchy main_instance(2);
    main_instance.objects[0] = object(375, 600, 40, 80, "player", 3);
    main_instance.objects[1] = object(0, 0, 800, 25, "hud", 0); // hud

    const int ENEMY_COUNT = 6;
    hierarchy enemy(ENEMY_COUNT); // Initialize Enemy
    spawn_enemy(enemy, ENEMY_COUNT); // Spawn first set of enemy

    // some stuff
    
    int faceR = 1; // Player Direction 1=right 0=left
    int currentlevel = 0;
    int frametime;
    float speed = 5.0f;
    
    bool gamePause = false;
    bool gameOver = false; 

    SDL_Event event;
    Uint32 frameStart, frameEnd;

    bool quit = false;
    while (!quit)
    {
        frameStart = SDL_GetTicks();
        quit = event_manager(event, main_instance, &gamePause);

        if (gamePause == false && gameOver == false)
        {
            SDL_RenderClear(renderer);

            logic(main_instance, &faceR);
            level_logic(main_instance, enemy, ENEMY_COUNT, &speed, &currentlevel);
            
            // Screen
            SDL_RenderCopy(renderer, backgroundTexture, NULL, &screen);

            // Entities

            // Player
            if (faceR == true)
            {
                SDL_RenderCopy(renderer, playerTexture, NULL, &main_instance.objects[0].rect);
            } 
            else if (faceR == false)
            {
                SDL_RenderCopy(renderer, playerTexture_r, NULL, &main_instance.objects[0].rect);
            }
            

            // Enemy (blocks/anvil)
            for (int i = 0; i < ENEMY_COUNT; i++)
            {
                SDL_RenderCopy(renderer, anvilTexture, NULL, &enemy.objects[i].rect);
            }
            
            // Hud Rendering
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); 
            SDL_RenderFillRect(renderer, &main_instance.objects[1].rect); 

            Hud.DrawText(*renderer, 715, 2, "Life: " + to_string(main_instance.objects[0]._health));
            Hud.DrawText(*renderer, 15, 2, "Level: " + to_string(currentlevel));
            Hud.DrawText(*renderer, 300, 2, "Current Speed: " + convert_1f(&speed));
            SDL_RenderCopy(renderer, healthTexture, NULL, &heart);
            
            // Info rendering
            HelpText.DrawText(*renderer, 680, 30, "(info)");
            HelpText.DrawText(*renderer, 650, 50, "Movement: WASD");
            HelpText.DrawText(*renderer, 650, 65, "Pause/Unpause: P");
            HelpText.DrawText(*renderer, 675, 85, "(cheats)");
            HelpText.DrawText(*renderer, 650, 95 + 10, "Grow Vertically: 9");
            HelpText.DrawText(*renderer, 650, 110 + 10, "Grow Horizontally: 0");
            HelpText.DrawText(*renderer, 650, 125 + 10, "Become Smaller: -");

            
            // Refresh Screen
            SDL_RenderPresent(renderer);
        } 

        if (gamePause == true)
        {
            postGame.DrawText(*renderer, 260, 375, "[ GAME PAUSE ]");
            SDL_RenderPresent(renderer);
        }
        
        if (main_instance.objects[0]._health < 1)
        {
            gameOver = true;
            postGame.DrawText(*renderer, 260, 375, "[ GAME OVER ]");
            postGame.DrawText(*renderer, 175, 420, "[ Closing in 3..2..1 ]");
            SDL_RenderPresent(renderer);
            SDL_Delay(3000);
            break;
        }
        
        

        // cout << "gamepause: " << gamePause << endl;

        // Frame Displacement
        frametime = SDL_GetTicks() - frameStart;
        if (FRAMEDELAY > frametime)
        {
            SDL_Delay(FRAMEDELAY - frametime);
        }
        
    }
    
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyTexture(playerTexture_r);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();

    
    return 0;
}