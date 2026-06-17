// -- Definição das bibliotecas a serem utilizadas
#include <stdio.h>
#include <SDL3/SDL.h>

// --- Definição das dimensões e título da janela ---
#define LARGURA_JANELA 640                                                                               //Definição da largura da janela em pixels
#define ALTURA_JANELA 640                                                                                  //Definição da altura da janela em pixels
#define TITULO "Super jogo da cobrinha da MAN 1.0"                                      //Definição do título da janela
//----------------------------------------------------

//-- Definições: tamanho da célula, quantidade de linhas e colunas
#define TAMANHO_CELULA 32
#define COLUNAS (LARGURA_JANELA / TAMANHO_CELULA)                           //640 / 32 = 20
#define LINHAS     (ALTURA_JANELA / TAMANHO_CELULA)                             //640 / 32 = 20
//----------------------------------------------------

//-- Valores que uma célula pode ter -----
#define VAZIO 0
#define COBRA 1
#define COMIDA 2
//----------------------------------------------------

// --- Função para zerar todas as células do "tabuleiro"

//A função percorre a matriz inserida como parâmetro, atribuindo a constante vazio (0) aos elementos da matriz

void inicializar_tabuleiro(int tabuleiro[LINHAS][COLUNAS])
{
    for(int i = 0; i < LINHAS; i++)
        for(int j = 0; j < COLUNAS; j++)
            tabuleiro[i][j] = VAZIO;
}

// ---------------------------- Função para "desenhar" o tabuleiro na tela ----------------------------------------------------

void desenhar_tabuleiro(SDL_Renderer *renderer, int tabuleiro[LINHAS][COLUNAS])
{
    SDL_FRect celula;   //retângulo que representa uma célula
    celula.w = TAMANHO_CELULA - 1; //Cria uma borda fina entre células
    celula.h = TAMANHO_CELULA - 1;

    for(int i = 0; i < LINHAS; i++)
    {
        for(int j = 0; j < COLUNAS; j++)
        {
            //Posição em pixels da célula
            celula.x = j * TAMANHO_CELULA;
            celula.y = i * TAMANHO_CELULA;

            if(tabuleiro[i][j] == COBRA)
            {
                SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255); //Verde
                SDL_RenderFillRect(renderer, &celula);
            }
            else if (tabuleiro[i][j] == COMIDA)
            {
                SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255); //Vermelho
                SDL_RenderFillRect(renderer, &celula);
            }
        }
    }
}



int main(int argc, char* argv[])
{
    //-------------- 1. Inicializção da biblioteca SDL  ------------------------
    if(!SDL_Init(SDL_INIT_VIDEO))
    {
            printf("Erro ao inicializar a biblioteca SDL: %s \n", SDL_GetError());      //1. A função SDL_GetError() obtém o erro ocorrido da execução
            return 1;                                                                                                            //2. o return 1 serve para indicar o erro de execução ao usuário
    }
    // -------------------------------------------------------------------------------

    //-------------- 2.  Criação da janela  -------------------------------------
SDL_Window *janela = SDL_CreateWindow(TITULO,                                           // 1. Titulo da janela (definida como constante no início)
                                                                               LARGURA_JANELA,                      //  2. Largura da janela(const.)
                                                                                ALTURA_JANELA,                        //  3. Altura da janela (const.)
                                                                                0                                                    //  4. Flag 0: Janela padrão
                                                                                );

    if(!janela)
    {
        printf("Erro ao criar janela: %s \n", SDL_GetError());                              // 1. A função SDL_GetError() obtém o erro ocorrido da execução
        SDL_Quit();                                                                                                     //  2. SDL_Quit() encerra a execução da biblioteca
        return 1;                                                                                                         //  3. o return 1 serve para indicar o erro de execução ao usuário
    }
    // --------------------------------------------------------------------------------------------

     //-------------- 3. Criação do renderer  -----------------------------------------------
     //O renderer é responsável por "desenhar" o conteúdo na janela do jogo
     SDL_Renderer *renderer = SDL_CreateRenderer(janela, NULL);                      //1. Cria o "pincel" que desenha dentro da janela

     if(!renderer)
     {
         printf("Erro ao criar renderer: %s \n", SDL_GetError());                               //  1. A função SDL_GetError() obtém o erro ocorrido da execução
         SDL_DestroyWindow(janela);                                                                            //  2. SDL_DestroyWindow(janela) fecha a janela aberta
         SDL_Quit();                                                                                                            //  3. SDL_Quit() encerra a execução da biblioteca
         return 1;                                                                                                                //  4. o return 1 serve para indicar o erro de execução ao usuário
     }
      // -------------------------------------------------------------------------------------------------

      //------------ Declara e inicializa o tabuleiro criado -----------------------------------
      int tabuleiro[LINHAS][COLUNAS];
      inicializar_tabuleiro(tabuleiro);
      // -------------------------------------------------------------------------------------------------

      //Colocando valores de teste para visualizar:
      tabuleiro[2][3] = COBRA;
      tabuleiro[2][4] = COBRA;
      tabuleiro[2][5] = COBRA;
      tabuleiro[5][10] = COMIDA;
      // -------------------------------------------------------------------------------------------------

      //-------------- 4. Loop principal do jogo   ------------------------------------------------
        int rodando = 1;                                                                                                       //A var. rodando atua como condição para manter o loop funcionando
        SDL_Event evento;

        while(rodando)                                                                                                     // Enquanto a condição for verdadeira (1), o loop é mantido
        {
            while(SDL_PollEvent(&evento))                                                                    // Processa todos os eventos pendentes, verificando se algo aconteceu
            {
                if(evento.type == SDL_EVENT_QUIT)                                                          // Caso o usuário encerre a janela...
                    rodando = 0;                                                                                                // Usuário fechou a janela (0), o loop é encerrado
            }

            //Limpa a tela com a cor preta
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);                                                                          //Apaga tudo e "pinta" de preto
            // -------------------------------------------------------------------------------------------------

            //Desenha o tabuleiro:
            desenhar_tabuleiro(renderer, tabuleiro);
            // -------------------------------------------------------------------------------------------------

            // (Desenho do jogo)
            SDL_RenderPresent(renderer);                                                                     //Apresenta o que foi "desenhado" na tela


        }
    //-------------- 5.  Encerra o jogo  ------------------------------------------------
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(janela);
    SDL_Quit();

    return 0;
}
