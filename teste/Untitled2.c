#include <stdio.h>
#include <SDL3/SDL.h>
#include <stdbool.h>

int main(int argc, char* argv[])
{
    //1. Inicializa o subsistema de vídeo
    //Se o SDL_Init retornar falso(falhou), entra no bloco de erro
    if(!SDL_Init(SDL_INIT_VIDEO))
    {
        //%s imprime a string retornada por SDL_GetError()
        //A função SDL_GetError() recupera a mensagem de erro par a ser exibida
        printf("Erro ao inicializar SDL: %s \n", SDL_GetError());
        //return 1 indica que o ocorreu um erro
        return 1;
    }

    //2.Cria a janela
    //A função devolve um ponteiro para o objeto da janela criada
    SDL_Window* window = SDL_CreateWindow(
        "Snake SDL3", //1. Título
        640, 480,          //2. Largura x Altura (em pixels)
        0                       //3. Flags; 0 Significa uma janela normal, sem nada especial
        );

    if(!window)
    {
        printf("Erro ao criar janela: %s \n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    //3. Cria o renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

    if(!renderer)
    {
        printf("Erro ao criar renderer: %s \n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    //4. Loop principal
    bool running = true;
    SDL_Event event;

    while(running)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
        }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

//5.Limpeza
SDL_DestroyRenderer(renderer);
SDL_DestroyWindow(window);
SDL_Quit();

return 0;
}
