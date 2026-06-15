#include <stdio.h>
#include <stdbool.h>
#include <SDL3/SDL.h>

int main(int argc, char* argv[])
{
    //1. Criação da janela
    if(!SDL_Init(SDL_INIT_VIDEO))
    {
        printf("Erro: %s \n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
                                          "Snake SDL3 - Etapa 2", //1. Título
                                          640, 480,                          //2. Largura x altura em pixels
                                          0                                         //3.Flag (0 = normal);
                                          );
    if(!window)
    {
        printf("Erro: %s \n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    //2.Criação do renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

    if(!renderer)
    {
        printf("Erro: %s \n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event event;

    while(running)
    {
        //--- Eventos ---
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_EVENT_QUIT)
                running = false;
        }
    //--- Fundo preto ---
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // --- Desenhando retângulos ---

    //SDL_Rect: retângulo com floats {x, y, largura, altura}

    //Bloco verde (simulando um pedaço da cobra)
    SDL_FRect bloco_cobra = {100.0f, 100.0f, 40.0f, 40.0f};
    SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255); //Verde
    SDL_RenderFillRect(renderer, &bloco_cobra);

    //Bloco vermelho
    SDL_FRect bloco_comida = {300.0f, 200.0f, 40.0f, 40.0f};
    SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255); //vermelho
    SDL_RenderFillRect(renderer, &bloco_comida);

    //Borda do bloco (apenas contorno, sem preenchimento)
    SDL_FRect bloco_borda = {500.0f, 300.0f, 40.0f, 40.0f};
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); //amarelo
    SDL_RenderRect(renderer, &bloco_borda);

    //--- Apresenta o frame ---
    SDL_RenderPresent(renderer);

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
