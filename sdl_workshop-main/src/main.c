#include "SDL.h"
#include "SDL_image.h"
#include <stdio.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>







const int SCREEN_WIDTH = 656;
const int SCREEN_HEIGHT = 400;


SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;








// Load an image and return an SDL_Texture
SDL_Texture* load_texture(const char* filename) {
    SDL_Surface* surface = IMG_Load(filename);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

// Draw a texture at the specified location
void draw_texture(SDL_Texture* texture, int x, int y) {
    SDL_Rect rect = { x, y, 0, 0 };
    SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}

struct box{
    int x;
    int y;
    int w;
    int h;
};

struct box start = { 248, 100, 160, 60};
struct box options = { 248, 180, 160, 60};
struct box close = { 248, 260, 160, 60};

int check_box(int x, int y){
    if((x >= start.x && x <= start.x + start.w) && (y >= start.y && y <= start.y + start.h)){
        return 1;
    }else if((x >= options.x && x <= options.x + options.w) && (y >= options.y && y <= options.y + options.h)){
        return 2;
    }else if((x >= close.x && x <= close.x + close.w) && (y >= close.y && y <= close.y + close.h)){
        return 3;
    }
    return 0;
}

void draw_volume_Bar(int volume, SDL_Texture* red, SDL_Texture* white){
    volume /= 10;
    int i = 0;
    while(i < volume){
        draw_texture(red, 128 + 180 + i * 10, 50 + 125);
        SDL_RenderPresent(renderer);
        i++;
    }
    i = volume;
    while(i < 10){
        draw_texture(white, 128 + 180 + i * 10, 50 + 125);
        SDL_RenderPresent(renderer);
        i++;
    }
}



int main(int argc, char* argv[]) {

    (void) argc;
    (void) argv;

    // Initialize SDL2 and create a window and renderer
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Init(SDL_INIT_AUDIO);
    Mix_Init(MIX_INIT_MP3);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);


    window = SDL_CreateWindow("The Big 3", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture* homepage_texture = load_texture("C:/Users/htrik/Downloads/homepage2/sdl_workshop-main/assets/homepage/homepage.png");
    SDL_Texture* options_texture = load_texture("C:/Users/htrik/Downloads/homepage2/sdl_workshop-main/assets/homepage/options.png");
    SDL_Texture* redBar_texture = load_texture("C:/Users/htrik/Downloads/homepage2/sdl_workshop-main/assets/homepage/redBar.png");
    SDL_Texture* whiteBar_texture = load_texture("C:/Users/htrik/Downloads/homepage2/sdl_workshop-main/assets/homepage/whiteBar.png");

    int volume = 70;

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);

    Mix_Music* music = Mix_LoadMUS("C:/Users/htrik/Downloads/homepage2/sdl_workshop-main/assets/homepage/musicHome.mp3");
    Mix_PlayMusic(music, -1);
    Mix_VolumeMusic(((float)volume / 100) * 128);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    draw_texture(homepage_texture, 0, 0);

    SDL_RenderPresent(renderer);



    int quit = 0;

    int setting = 0;

    int x;
    int y;



    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            Mix_VolumeMusic(((float)volume / 100) * 128);
            if (event.type == SDL_QUIT) {
                quit = 1;
            }else if (event.type == SDL_MOUSEBUTTONDOWN){
                x = event.button.x;
                y = event.button.y;
                if(setting == 0){
                    if(check_box(x, y) != 0){
                        int choice = check_box(x, y);
                        switch(choice){
                            case 1:
                                // start
                                break;
                            case 2:
                                // options
                                draw_texture(options_texture, 128, 50);

                                draw_volume_Bar(volume, redBar_texture, whiteBar_texture);

                                setting = 1;
                                break;
                            case 3:
                                // exit
                                quit = 1;
                        }
                    }
                }else{
                        if((x >= 145 + 128 && x <= 145 + 24 + 128) && (y >= 125 +50 && y <= 125 + 24 + 50)){
                            if(volume != 0){
                                volume -= 10;
                                draw_volume_Bar(volume, redBar_texture, whiteBar_texture);
                            }
                        }else if((x >= 286 + 128 && x <= 286 + 24 + 128) && (y >= 125 +50 && y <= 125 + 24 + 50)){
                            if(volume != 100){
                                volume += 10;
                                draw_volume_Bar(volume, redBar_texture, whiteBar_texture);
                            }
                        }else if((x >= 340 + 128  && x <= 340 + 20 + 128) && (y >= 20 +50 && y <= 20 + 30 + 50)){
                            SDL_RenderClear(renderer);
                            draw_texture(homepage_texture, 0, 0);
                            SDL_RenderPresent(renderer);
                            setting = 0;
                        }
                    }
                }
            }


        }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();
    return 0;
}
