#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdbool.h>

int main(int argc, char* argv[]) {

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("Erro: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Snake SDL3 - Etapa 3", 640, 480, 0);
    if (!window) { SDL_Quit(); return 1; }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) { SDL_DestroyWindow(window); SDL_Quit(); return 1; }

    // --- Posição inicial do bloco ---
    float x = 300.0f;
    float y = 220.0f;
    float velocidade = 1.0f;

    // --- Direção atual (1 = sim, 0 = não) ---
    bool mover_cima    = false;
    bool mover_baixo   = false;
    bool mover_esq     = false;
    bool mover_dir     = false;

    bool running = true;
    SDL_Event event;

    while (running) {

        // --- Leitura de eventos ---
        while (SDL_PollEvent(&event)) {

            if (event.type == SDL_EVENT_QUIT)
                running = false;

            // Tecla PRESSIONADA
            if (event.type == SDL_EVENT_KEY_DOWN) {
                switch (event.key.key) {
                    case SDLK_UP:    mover_cima  = true; break;
                    case SDLK_DOWN:  mover_baixo = true; break;
                    case SDLK_LEFT:  mover_esq   = true; break;
                    case SDLK_RIGHT: mover_dir   = true; break;
                    case SDLK_ESCAPE: running    = false; break;
                }
            }

            // Tecla SOLTA
            if (event.type == SDL_EVENT_KEY_UP) {
                switch (event.key.key) {
                    case SDLK_UP:    mover_cima  = false; break;
                    case SDLK_DOWN:  mover_baixo = false; break;
                    case SDLK_LEFT:  mover_esq   = false; break;
                    case SDLK_RIGHT: mover_dir   = false; break;
                }
            }
        }

        // --- Atualiza posição com base nas teclas ---
        if (mover_cima)  y -= velocidade;
        if (mover_baixo) y += velocidade;
        if (mover_esq)   x -= velocidade;
        if (mover_dir)   x += velocidade;

        // --- Renderização ---
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Desenha o bloco na posição atual
        SDL_FRect bloco = { x, y, 40.0f, 40.0f };
        SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
        SDL_RenderFillRect(renderer, &bloco);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
