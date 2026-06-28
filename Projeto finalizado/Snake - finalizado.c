// --- Definição das bibliotecas a serem utilizadas ---

#include <stdio.h>
#include <SDL3/SDL.h>
#include <stdlib.h>
#include <time.h>
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --- Estados do jogo ---

#define TELA_INICIO   0
#define TELA_JOGANDO    1
#define TELA_GAME_OVER  2
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --- Definição das dimensões e título da janela ---

#define LARGURA_JANELA 640                                                                             //Definição da largura da janela em pixels
#define ALTURA_JANELA 640                                                                                //Definição da altura da janela em pixels
#define TITULO "Super jogo da cobrinha da MAN 1.0"                                    //Definição do título da janela
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --- Definições: tamanho da célula, quantidade de linhas e colunas ---

#define TAMANHO_CELULA 32
#define COLUNAS (LARGURA_JANELA / TAMANHO_CELULA)                          //  640 / 32 = 20
#define LINHAS     (ALTURA_JANELA / TAMANHO_CELULA)                             //   640 / 32 = 20
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --- Valores que uma célula pode ter ---

#define VAZIO 0
#define COBRA 1
#define COMIDA 2
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//Controlando a velocidade do jogo

#define VELOCIDADE_MS 150              //A cobra se move a cada 150ms
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --- Função para zerar todas as células do "tabuleiro" ---
    //A função percorre a matriz inserida como parâmetro, atribuindo a constante vazio (0) aos elementos da matriz

void inicializar_tabuleiro(int tabuleiro[LINHAS][COLUNAS])
{
    for(int i = 0; i < LINHAS; i++)
        for(int j = 0; j < COLUNAS; j++)
            tabuleiro[i][j] = VAZIO;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --- Função para "desenhar" o tabuleiro na tela ---

void desenhar_tabuleiro(SDL_Renderer *renderer, int tabuleiro[LINHAS][COLUNAS])
{
    SDL_FRect celula;                                   //retângulo que representa uma célula
    celula.w = TAMANHO_CELULA - 1;     //Cria uma borda fina entre células
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
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --- A struct da cobra ---

typedef struct  //Representa uma posição (linha, coluna) no tabuleiro
{
        int linha;
        int coluna;
} Posicao;

typedef enum //Direções possíveis
{
    CIMA,
    BAIXO,
    ESQUERDA,
    DIREITA
} Direcao;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --- Representa o tamanho máximo da cobra ---

#define TAMANHO_MAXIMO_COBRA (LINHAS * COLUNAS)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

typedef struct
{
    Posicao corpo[TAMANHO_MAXIMO_COBRA];    //corpo[0] = cabeça
    int tamanho;                                                             // tamanho representa quantos segmentos estão ativos
    Direcao direcao;
} Cobra;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --- Inicializar a cobra com 3 segmentos no centro do tabuleiro ---

void inicializar_cobra(Cobra *cobra)
{
    int centro_linha = LINHAS / 2;
    int centro_coluna = COLUNAS / 2;

    cobra -> tamanho = 3;
    cobra -> direcao = DIREITA;

    //Cabeça no centro
    cobra -> corpo[0].linha = centro_linha;
    cobra ->corpo[0].coluna = centro_coluna;

    //Corpo se estende para a esquerda da cabeça
    cobra -> corpo[1].linha = centro_linha;
    cobra -> corpo[1].coluna = centro_coluna - 1;

    cobra -> corpo[2].linha = centro_linha;
    cobra -> corpo[2].coluna = centro_coluna - 2;

}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --- Marca as células da cobra na matriz do tabuleiro ---
void atualizar_tabuleiro_com_cobra(int tabuleiro[LINHAS][COLUNAS], Cobra *cobra)
{
    for(int i = 0; i < cobra -> tamanho; i++)
    {
        int l = cobra -> corpo[i].linha;
        int c = cobra -> corpo[i].coluna;
        tabuleiro[l][c] = COBRA;
    }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --- Spawna a comida em uma posição aleatória VAZIA ---

Posicao spawnar_comida(int tabuleiro[LINHAS][COLUNAS])
{
    int l, c;

    do {
        l = rand() % LINHAS;
        c = rand() % COLUNAS;
    } while (tabuleiro[l][c] != VAZIO);

    tabuleiro[l][c] = COMIDA;

    // Retorna a posição sorteada
    Posicao pos;
    pos.linha  = l;
    pos.coluna = c;
    return pos;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --- Verifica se a cobra colidiu com algo ---

    // Recebe a posição FUTURA da cabeça e verifica se é válida
    // Retorna 1 se houve colisão, 0 se está tudo bem

int checar_colisao(int tabuleiro[LINHAS][COLUNAS], Cobra *cobra)
{
    // Calcula a próxima posição da cabeça
    Posicao proxima = cobra->corpo[0];

    switch (cobra->direcao)
    {
        case CIMA:
            proxima.linha--;
        break;

        case BAIXO:
            proxima.linha++;
        break;

        case ESQUERDA:
            proxima.coluna--;
        break;

        case DIREITA:
            proxima.coluna++;
        break;
    }

    // Colisão com as paredes
    if (proxima.linha  < 0 || proxima.linha  >= LINHAS ||
        proxima.coluna < 0 || proxima.coluna >= COLUNAS)
        return 1;

    // Colisão com o próprio corpo (ignora o índice 0)
    for (int i = 1; i < cobra->tamanho; i++)
    {
        if (cobra->corpo[i].linha  == proxima.linha &&
            cobra->corpo[i].coluna == proxima.coluna)
            return 1;
    }

    return 0;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int mover_cobra(Cobra *cobra, int tabuleiro[LINHAS][COLUNAS])
{
    /*  Move a cabeça na direção atual
        Após deslocar o corpo, a cabeça recebe uma nova posição baseada na direção atual
        0, 0) -------> A coluna cresce para a direita
        |
        |
        v
        A linha cresce pra baixo

    */
    // 1. Calcula nova posição da cabeça ANTES de mover
    Posicao nova_cabeca = cobra->corpo[0];              //A cobra é armazenada num vetor de posições, onde o índice 0 é sempre a cabeça


    switch (cobra->direcao)
    {
        case CIMA:
                 nova_cabeca.linha--;       // sobe = linha diminui
        break;

        case BAIXO:                             // desce = linha aumenta
            nova_cabeca.linha++;
        break;

        case ESQUERDA:                  // esquerda = coluna diminui
            nova_cabeca.coluna--;
        break;

        case DIREITA:                       // direita =  coluna aumenta
            nova_cabeca.coluna++;
        break;
    }

    // 2. Verifica se a nova posição tem comida
    int comeu = 0;
    if (nova_cabeca.linha  >= 0 && nova_cabeca.linha  < LINHAS &&
        nova_cabeca.coluna >= 0 && nova_cabeca.coluna < COLUNAS)
    {
        if (tabuleiro[nova_cabeca.linha][nova_cabeca.coluna] == COMIDA)
            comeu = 1;
    }

    // 3. Desloca o corpo de trás para frente
    if (comeu)
    {
        // Cresce: desloca SEM descartar a cauda
        for (int i = cobra->tamanho; i > 0; i--)
            cobra->corpo[i] = cobra->corpo[i - 1];
        cobra->tamanho++;
    }
    else
    {
        // Move normalmente: descarta a cauda
        for (int i = cobra->tamanho - 1; i > 0; i--)
            cobra->corpo[i] = cobra->corpo[i - 1];
    }

    // 4. Insere a nova cabeça
    cobra->corpo[0] = nova_cabeca;

    return comeu;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --- Salva o recorde em arquivo ---
void salvar_recorde(int recorde)
{
    FILE *arquivo = fopen("recorde.txt", "w");  // "w" = escrita

    if (arquivo == NULL)
    {
        printf("Erro ao salvar recorde!\n");
        return;
    }

    fprintf(arquivo, "%d", recorde);  // escreve o número no arquivo
    fclose(arquivo);
}

// --- Carrega o recorde do arquivo ---
int carregar_recorde()
{
    FILE *arquivo = fopen("recorde.txt", "r");  // "r" = leitura

    if (arquivo == NULL)
        return 0;  // arquivo não existe ainda → recorde é 0

    int recorde = 0;
    fscanf(arquivo, "%d", &recorde);  // lê o número do arquivo
    fclose(arquivo);

    return recorde;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --- Desenha a tela de início ---

void desenhar_tela_inicio(SDL_Renderer *renderer)
{
    // Fundo azul escuro
    SDL_SetRenderDrawColor(renderer, 0, 0, 80, 255);
    SDL_RenderClear(renderer);

    // Borda da tela
    SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255);
    SDL_FRect borda = { 10.0f, 10.0f, 620.0f, 620.0f };
    SDL_RenderRect(renderer, &borda);

    // Cobra decorativa — 8 blocos verdes enfileirados
    for(int i = 0; i < 8; i++)
    {
        SDL_SetRenderDrawColor(renderer, 0, 180, 0, 255);
        SDL_FRect bloco = { 100.0f + i * 36.0f, 240.0f, 30.0f, 30.0f };
        SDL_RenderFillRect(renderer, &bloco);
    }

    // Cabeça da cobra (verde mais claro)
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_FRect cabeca = { 100.0f + 8 * 36.0f, 240.0f, 30.0f, 30.0f };
    SDL_RenderFillRect(renderer, &cabeca);

    // Olho da cabeça (ponto branco)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_FRect olho = { 100.0f + 8 * 36.0f + 18.0f, 247.0f, 6.0f, 6.0f };
    SDL_RenderFillRect(renderer, &olho);

    // Comida (bloco vermelho à frente da cobra)
    SDL_SetRenderDrawColor(renderer, 220, 0, 0, 255);
    SDL_FRect comida = { 100.0f + 10 * 36.0f, 240.0f, 30.0f, 30.0f };
    SDL_RenderFillRect(renderer, &comida);

    // Seta ▶ feita de retângulos (indica pressionar ENTER)
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    // Corpo da seta (triângulo aproximado com 4 barras)
    SDL_FRect seta1 = { 290.0f, 380.0f, 10.0f, 40.0f };
    SDL_FRect seta2 = { 300.0f, 385.0f, 10.0f, 30.0f };
    SDL_FRect seta3 = { 310.0f, 390.0f, 10.0f, 20.0f };
    SDL_FRect seta4 = { 320.0f, 395.0f, 10.0f, 10.0f };
    SDL_RenderFillRect(renderer, &seta1);
    SDL_RenderFillRect(renderer, &seta2);
    SDL_RenderFillRect(renderer, &seta3);
    SDL_RenderFillRect(renderer, &seta4);

    // Seta piscante (segunda seta ao lado)
    Uint64 tempo = SDL_GetTicks();
    if((tempo / 500) % 2 == 0)
    {
        SDL_SetRenderDrawColor(renderer, 255, 200, 0, 255);
        SDL_FRect seta5 = { 340.0f, 380.0f, 10.0f, 40.0f };
        SDL_FRect seta6 = { 350.0f, 385.0f, 10.0f, 30.0f };
        SDL_FRect seta7 = { 360.0f, 390.0f, 10.0f, 20.0f };
        SDL_FRect seta8 = { 370.0f, 395.0f, 10.0f, 10.0f };
        SDL_RenderFillRect(renderer, &seta5);
        SDL_RenderFillRect(renderer, &seta6);
        SDL_RenderFillRect(renderer, &seta7);
        SDL_RenderFillRect(renderer, &seta8);
    }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --- Desenha a tela de game over ---

void desenhar_tela_game_over(SDL_Renderer *renderer, int score, int recorde)
{
    // Fundo vermelho escuro
    SDL_SetRenderDrawColor(renderer, 60, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Borda da tela
    SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
    SDL_FRect borda = { 10.0f, 10.0f, 620.0f, 620.0f };
    SDL_RenderRect(renderer, &borda);

    // Cruz ✕ centralizada
    SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
    for(int i = 0; i < 8; i++)
    {
        SDL_FRect bloco1 = { 270.0f + i * 14.0f, 220.0f + i * 14.0f, 12.0f, 12.0f };
        SDL_FRect bloco2 = { 270.0f + i * 14.0f, 318.0f - i * 14.0f, 12.0f, 12.0f };
        SDL_RenderFillRect(renderer, &bloco1);
        SDL_RenderFillRect(renderer, &bloco2);
    }

    // Seta piscante centralizada
    Uint64 tempo = SDL_GetTicks();
    if((tempo / 500) % 2 == 0)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_FRect s1 = { 290.0f, 430.0f, 10.0f, 40.0f };
        SDL_FRect s2 = { 300.0f, 435.0f, 10.0f, 30.0f };
        SDL_FRect s3 = { 310.0f, 440.0f, 10.0f, 20.0f };
        SDL_FRect s4 = { 320.0f, 445.0f, 10.0f, 10.0f };
        SDL_RenderFillRect(renderer, &s1);
        SDL_RenderFillRect(renderer, &s2);
        SDL_RenderFillRect(renderer, &s3);
        SDL_RenderFillRect(renderer, &s4);
    }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --- Capturando o teclado ---

    //SDL_Event é uma estrutura do SDL que representa qualquer coisa que aconteceu — fechar janela, mover mouse, pressionar tecla, etc.
    //Passamos um ponteiro (*evento) para evitar copiar a estrutura inteira na memória.

void processar_teclado(SDL_Event *evento, Cobra *cobra)
{
    //Se não for uma tecla pressionada, a função encerra
    if(evento -> type != SDL_EVENT_KEY_DOWN)
        return;

    //scancode representa a posição física da tecla
    switch(evento -> key.scancode)
    {
        // Seta cima
        // Só muda para cima se não estiver indo pra baixo
        case SDL_SCANCODE_UP:
            if(cobra -> direcao != BAIXO)
                cobra -> direcao = CIMA;
        break;

        // Seta baixo
        // Só muda para baixo se não estiver indo para cima
        case SDL_SCANCODE_DOWN:
            if(cobra -> direcao != CIMA)
                cobra -> direcao = BAIXO;
        break;

        // Seta esquerda
        // Só muda para a esquerda se não estiver indo para a direita
        case SDL_SCANCODE_LEFT:
            if(cobra ->direcao != DIREITA)
                cobra -> direcao = ESQUERDA;
        break;

        // Seta direita
        // Só muda para a direita se não estiver para a esquerda
        case SDL_SCANCODE_RIGHT:
            if(cobra -> direcao != ESQUERDA)
                cobra -> direcao = DIREITA;
        break;
    }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --- Reinicia todos os elementos do jogo ---
void reiniciar_jogo(int tabuleiro[LINHAS][COLUNAS], Cobra *cobra,
                    int *score, int *game_over, Posicao *pos_comida)
{
    inicializar_tabuleiro(tabuleiro);
    inicializar_cobra(cobra);
    atualizar_tabuleiro_com_cobra(tabuleiro, cobra);
    *score = 0;
    *game_over = 0;
    *pos_comida = spawnar_comida(tabuleiro);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//-- Função principal do código ---

int main(int argc, char* argv[])
{
    // --- Inicializção da biblioteca SDL  ---

    if(!SDL_Init(SDL_INIT_VIDEO))
    {
            printf("Erro ao inicializar a biblioteca SDL: %s \n", SDL_GetError());      //1. A função SDL_GetError() obtém o erro ocorrido da execução
            return 1;                                                                                                            //2. o return 1 serve para indicar o erro de execução ao usuário
    }
   //--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    // ---  Criação da janela   ---

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
    //--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

     // ---  Criação do renderer  ---

     //O renderer é responsável por "desenhar" o conteúdo na janela do jogo
     SDL_Renderer *renderer = SDL_CreateRenderer(janela, NULL);                      //1. Cria o "pincel" que desenha dentro da janela

     if(!renderer)
     {
         printf("Erro ao criar renderer: %s \n", SDL_GetError());                               //  1. A função SDL_GetError() obtém o erro ocorrido da execução
         SDL_DestroyWindow(janela);                                                                            //  2. SDL_DestroyWindow(janela) fecha a janela aberta
         SDL_Quit();                                                                                                            //  3. SDL_Quit() encerra a execução da biblioteca
         return 1;                                                                                                                //  4. o return 1 serve para indicar o erro de execução ao usuário
     }
     //--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

      // --- Inicialização dos elementos do tabuleiro ---

      int tabuleiro[LINHAS][COLUNAS];

      //Inicializa o tabuleiro com zeros
      inicializar_tabuleiro(tabuleiro);

      // --- Declara e inicializa a cobra  ---
      Cobra cobra;
      inicializar_cobra(&cobra);

      // Marca a cobra no tabuleiro
      atualizar_tabuleiro_com_cobra(tabuleiro, &cobra);

      // --- Estado do jogo ---
        int game_over = 0;
        int score = 0;
        Posicao pos_comida;     //Variável para guardar a posição da comida
        int recorde   = carregar_recorde();  //  carrega ao iniciar
        int tela = TELA_INICIO; //Começa na tela de início

        // Inicializa o gerador de números aleatórios
        srand((unsigned int)time(NULL));

       //Spawna a comida em uma posição vazia
       pos_comida = spawnar_comida(tabuleiro);
      //--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

      // --- Controle de tempo ---
            //O SDL usa Uint64 para medir tempo porque o valor cresce sem parar desde que o programa iniciou
            // SDL_GetTicks() retorna quantos milissegundos se passaram desde que o SDL foi iniciado
      Uint64 tempo_anterior = SDL_GetTicks();

      // --- 4. Loop principal do jogo  ---

        int rodando = 1;                                                                                                       //A var. rodando atua como condição para manter o loop funcionando
        SDL_Event evento;

        while(rodando)                                                                                                     // Enquanto a condição for verdadeira (1), o loop é mantido
        {
            while(SDL_PollEvent(&evento))                                                                    // Processa todos os eventos pendentes, verificando se algo aconteceu
            {
                if(evento.type == SDL_EVENT_QUIT)                                                          // Caso o usuário encerre a janela...
                    rodando = 0;
                if(evento.type == SDL_EVENT_KEY_DOWN)
                {
                    //ENTER avança ou reinicia conforme a tela atual
                    if(evento.key.scancode == SDL_SCANCODE_RETURN)
                    {
                        if(tela == TELA_INICIO)
                            tela = TELA_JOGANDO;

                        else if(tela == TELA_GAME_OVER)
                        {
                            reiniciar_jogo(tabuleiro, &cobra, &score, &game_over, &pos_comida);
                            tela = TELA_INICIO;
                        }
                    }
                }

                if(tela == TELA_JOGANDO)
                    processar_teclado(&evento, &cobra);
            }

            //Só move a cobra quando o tempo passou
            Uint64 tempo_atual = SDL_GetTicks();
            if(tela == TELA_JOGANDO)
            {
                if (tempo_atual - tempo_anterior >= VELOCIDADE_MS)
                {

                    if (!game_over)
                   {
                       // 1. Checa colisão ANTES de mover
                        if (checar_colisao(tabuleiro, &cobra))
                        {
                            game_over = 1;
                            tela = TELA_GAME_OVER;

                            //Atualiza e salva o recorde se bateu o anterior
                            if (score > recorde)
                            {
                                recorde = score;
                                salvar_recorde(recorde);
                                printf("Novo recorde: %d ! \n", recorde);
                            }

                            printf("Game Over! Score %d | Recorde %d \n", score, recorde);
                        }

                        else
                        {
                            // 2. Só move se não houve colisão
                            int comeu = mover_cobra(&cobra, tabuleiro);

                            // 3. Atualiza o tabuleiro
                            inicializar_tabuleiro(tabuleiro);
                            atualizar_tabuleiro_com_cobra(tabuleiro, &cobra);
                            tabuleiro[pos_comida.linha][pos_comida.coluna] = COMIDA;

                            // 4. Verifica se comeu
                            if (comeu)
                            {
                                score += 10;
                                pos_comida = spawnar_comida(tabuleiro);
                                printf("Score: %d\n", score);
                             }
                            }
                        }
                    tempo_anterior = tempo_atual;
                    }
                }

        if(tela == TELA_INICIO)
        {
            desenhar_tela_inicio(renderer);
        }
        else if(tela == TELA_JOGANDO)
        {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            desenhar_tabuleiro(renderer, tabuleiro);
        }
        else if(tela == TELA_GAME_OVER)
        {
            desenhar_tela_game_over(renderer, score, recorde);
        }

        SDL_RenderPresent(renderer);

    }
     //--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // --- 5.  Encerra o jogo  ---
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(janela);
    SDL_Quit();

    return 0;
}
