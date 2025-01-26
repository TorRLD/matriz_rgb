#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

// Biblioteca gerada pelo arquivo .pio durante compilacao.
#include "ws2818b.pio.h" 

// ------------------- Definições dos Pinos GPIO -------------------

// Definição dos pinos GPIO do teclado matricial
#define ROW_1    5
#define ROW_2    18
#define ROW_3    19
#define ROW_4    20
#define COL_1    21
#define COL_2    6
#define COL_3    22
#define COL_4    26


// Dimensões do teclado
#define NUM_LINHAS 4
#define NUM_COLUNAS 4

// ------------------- Inicialização dos GPIOs -------------------
void setup_gpio() {
    // ------------ Configuração das linhas (saída) ------------
    gpio_init(ROW_1);
    gpio_init(ROW_2);
    gpio_init(ROW_3);
    gpio_init(ROW_4);

    gpio_set_dir(ROW_1, GPIO_OUT);
    gpio_set_dir(ROW_2, GPIO_OUT);
    gpio_set_dir(ROW_3, GPIO_OUT);
    gpio_set_dir(ROW_4, GPIO_OUT);

    // Define todas as linhas em HIGH (1) inicialmente
    gpio_put(ROW_1, 1);
    gpio_put(ROW_2, 1);
    gpio_put(ROW_3, 1);
    gpio_put(ROW_4, 1);

    // ------------ Configuração das colunas (entrada, pull-up) ------------
    gpio_init(COL_1);
    gpio_init(COL_2);
    gpio_init(COL_3);
    gpio_init(COL_4);

    gpio_set_dir(COL_1, GPIO_IN);
    gpio_set_dir(COL_2, GPIO_IN);
    gpio_set_dir(COL_3, GPIO_IN);
    gpio_set_dir(COL_4, GPIO_IN);

    // Pull-up em cada coluna (0 = pressionado)
    gpio_pull_up(COL_1);
    gpio_pull_up(COL_2);
    gpio_pull_up(COL_3);
    gpio_pull_up(COL_4);

}

// ------------------- Leitura do Teclado Matricial -------------------
const char KEYPAD[NUM_LINHAS][NUM_COLUNAS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

char read_keypad() {
    // Arrays de linhas e colunas
    const uint rows[] = {ROW_1, ROW_2, ROW_3, ROW_4};
    const uint cols[] = {COL_1, COL_2, COL_3, COL_4};

    // Faz a varredura linha a linha
    for (int linha = 0; linha < 4; linha++) {
        // Coloca a linha atual em LOW (0) para "ativar"
        gpio_put(rows[linha], 0);

        // Lê cada coluna
        for (int coluna = 0; coluna < 4; coluna++) {
            // Se a coluna estiver em 0, significa tecla pressionada
            if (gpio_get(cols[coluna]) == 0) {
                sleep_ms(50); // Debounce

                if (gpio_get(cols[coluna]) == 0) {
                    // Espera o botão ser solto
                    while (gpio_get(cols[coluna]) == 0) {
                        sleep_ms(10);
                    }
                    // Desativa a linha (retorna a HIGH)
                    gpio_put(rows[linha], 1);

                    // Retorna o caractere correspondente
                    return KEYPAD[linha][coluna];
                }
            }
        }
        // Desativa a linha atual (HIGH)
        gpio_put(rows[linha], 1);
    }

    return '\0'; // Nenhuma tecla pressionada
}

//DEV 1
void tecla_zero();

//DEV 2
void tecla_um();
void funcao_A();

//DEV 3
void tecla_dois();
void funcao_B();

//DEV 4
void tecla_tres();
void funcao_C();

//DEV 5
void tecla_quatro();
void funcao_D();

//DEV 6
void tecla_cinco();
void funcao_sharp();


int main() {
  stdio_init_all();
  setup_gpio();

  printf("Sistema iniciado. Pressione uma tecla no teclado matricial.\n");
  
  while (true) {
      char tecla = read_keypad();
      if (tecla != '\0'){ 
          printf("Tecla '%c' pressionada.\n", tecla);
          // Aqui você pode adicionar a lógica para o que fazer com a tecla pressionada
      }
      // Verifica se a tecla pressionada é '2'
      if (tecla == '2')
      {
          tecla_dois();
      }
      if (tecla == '4')
      {
          tecla_quatro();
      }

      // Verifica se a tecla pressionada é 'B'
      if (tecla == 'B')
      {
          funcao_B();
      }

      if (tecla == 'D') {
        funcao_D();
      }

        
        
  }
  
  sleep_ms(100); // Pequena pausa para evitar sobrecarga do loop
  return 0;
}

// Dados e funções gerais:

#include "hardware/pio.h"
#include "hardware/clocks.h"

// Definição do número de LEDs e pino.
#define LED_COUNT 25
#define LED_PIN 7

// Definição de pixel GRB
struct pixel_t
{
  uint8_t G, R, B; // Três valores de 8-bits compõem um pixel.
};
typedef struct pixel_t pixel_t;
typedef pixel_t npLED_t; // Mudança de nome de "struct pixel_t" para "npLED_t" por clareza.

// Declaração do buffer de pixels que formam a matriz.
npLED_t leds[LED_COUNT];

// Variáveis para uso da máquina PIO.
PIO np_pio;
uint sm;

/**
 * Inicializa a máquina PIO para controle da matriz de LEDs.
 */
void npInit(uint pin)
{
  // Cria programa PIO.
  uint offset = pio_add_program(pio0, &ws2818b_program);
  np_pio = pio0;

  // Toma posse de uma máquina PIO.
  sm = pio_claim_unused_sm(np_pio, false);
  if (sm < 0)
  {
    np_pio = pio1;
    sm = pio_claim_unused_sm(np_pio, true); // Se nenhuma máquina estiver livre, panic!
  }

  // Inicia programa na máquina PIO obtida.
  ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);

  // Limpa buffer de pixels.
  for (uint i = 0; i < LED_COUNT; ++i)
  {
    leds[i].R = 0;
    leds[i].G = 0;
    leds[i].B = 0;
  }
}

/**
 * Atribui uma cor RGB a um LED.
 */
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b)
{
  leds[index].R = r;
  leds[index].G = g;
  leds[index].B = b;
}

/**
 * Limpa o buffer de pixels.
 */
void npClear()
{
  for (uint i = 0; i < LED_COUNT; ++i)
    npSetLED(i, 0, 0, 0);
}

/**
 * Escreve os dados do buffer nos LEDs.
 */
void npWrite()
{
  // Escreve cada dado de 8-bits dos pixels em sequência no buffer da máquina PIO.
  for (uint i = 0; i < LED_COUNT; ++i)
  {
    pio_sm_put_blocking(np_pio, sm, leds[i].G);
    pio_sm_put_blocking(np_pio, sm, leds[i].R);
    pio_sm_put_blocking(np_pio, sm, leds[i].B);
  }
  sleep_us(100); // Espera 100us, sinal de RESET do datasheet.
}

// Modificado do github: https://github.com/BitDogLab/BitDogLab-C/tree/main/neopixel_pio
// Função para converter a posição do matriz para uma posição do vetor.
int getIndex(int x, int y)
{
  // Se a linha for par (0, 2, 4), percorremos da esquerda para a direita.
  // Se a linha for ímpar (1, 3), percorremos da direita para a esquerda.
  if (y % 2 == 0)
  {
    return 24 - (y * 5 + x); // Linha par (esquerda para direita).
  }
  else
  {
    return 24 - (y * 5 + (4 - x)); // Linha ímpar (direita para esquerda).
  }
}

void funcao_sharp()
{
  // Inicializa entradas e saídas.
  stdio_init_all();

  // Inicializa matriz de LEDs NeoPixel.
  npInit(LED_PIN);
  npClear();

  // Declaração das matrizes
  int matrizes[23][5][5][3] = {
      {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{12, 199, 0}, {12, 199, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}},
      {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {12, 199, 0}, {12, 199, 0}, {0, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}},
      {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {12, 199, 0}, {12, 199, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}},
      {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {12, 199, 0}, {12, 199, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {12, 199, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}},
      {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {12, 199, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {12, 199, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {12, 199, 0}, {0, 0, 0}},
       {{0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}},
      {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {12, 199, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {12, 199, 0}, {0, 0, 0}},
       {{0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {12, 199, 0}, {0, 0, 0}}},
      {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {12, 199, 0}, {0, 0, 0}},
       {{0, 0, 0}, {255, 0, 0}, {12, 199, 0}, {12, 199, 0}, {0, 0, 0}}},
      {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{255, 0, 0}, {0, 0, 0}, {0, 0, 0}, {12, 199, 0}, {0, 0, 0}},
       {{0, 0, 0}, {12, 199, 0}, {12, 199, 0}, {12, 199, 0}, {0, 0, 0}}},
      {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{255, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{12, 199, 0}, {12, 199, 0}, {12, 199, 0}, {12, 199, 0}, {0, 0, 0}}},
      {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{12, 199, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{12, 199, 0}, {12, 199, 0}, {12, 199, 0}, {12, 199, 0}, {0, 0, 0}}},
      {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{12, 199, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{12, 199, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{12, 199, 0}, {12, 199, 0}, {12, 199, 0}, {0, 0, 0}, {0, 0, 0}}},
      {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{12, 199, 0}, {12, 199, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{12, 199, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{12, 199, 0}, {12, 199, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}},
      {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{12, 199, 0}, {12, 199, 0}, {12, 199, 0}, {0, 0, 0}, {0, 0, 0}},
       {{12, 199, 0}, {0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}},
       {{12, 199, 0}, {12, 199, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}},
      {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{12, 199, 0}, {12, 199, 0}, {12, 199, 0}, {12, 199, 0}, {0, 0, 0}},
       {{12, 199, 0}, {0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}},
       {{12, 199, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}},
      {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{12, 199, 0}, {12, 199, 0}, {12, 199, 0}, {12, 199, 0}, {0, 0, 0}},
       {{12, 199, 0}, {255, 0, 0}, {0, 0, 0}, {12, 199, 0}, {0, 0, 0}},
       {{12, 199, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}},
      {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{12, 199, 0}, {12, 199, 0}, {12, 199, 0}, {12, 199, 0}, {0, 0, 0}},
       {{12, 199, 0}, {255, 0, 0}, {0, 0, 0}, {12, 199, 0}, {0, 0, 0}},
       {{12, 199, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}},
      {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{12, 199, 0}, {12, 199, 0}, {12, 199, 0}, {12, 199, 0}, {0, 0, 0}},
       {{12, 199, 0}, {255, 0, 0}, {0, 0, 0}, {12, 199, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {12, 199, 0}, {0, 0, 0}}},
      {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{12, 199, 0}, {12, 199, 0}, {12, 199, 0}, {12, 199, 0}, {0, 0, 0}},
       {{0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {12, 199, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {12, 199, 0}, {12, 199, 0}, {0, 0, 0}}},
      {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {12, 199, 0}, {12, 199, 0}, {12, 199, 0}, {0, 0, 0}},
       {{0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {12, 199, 0}, {0, 0, 0}},
       {{0, 0, 0}, {12, 199, 0}, {12, 199, 0}, {12, 199, 0}, {0, 0, 0}}},
      {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {12, 199, 0}, {12, 199, 0}, {12, 199, 0}, {0, 0, 0}},
       {{0, 0, 0}, {12, 199, 0}, {0, 0, 0}, {12, 199, 0}, {0, 0, 0}},
       {{0, 0, 0}, {12, 199, 0}, {12, 199, 0}, {12, 199, 0}, {0, 0, 0}}},
      {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}},
      {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {12, 199, 0}, {12, 199, 0}, {12, 199, 0}, {0, 0, 0}},
       {{0, 0, 0}, {12, 199, 0}, {0, 0, 0}, {12, 199, 0}, {0, 0, 0}},
       {{0, 0, 0}, {12, 199, 0}, {12, 199, 0}, {12, 199, 0}, {0, 0, 0}}},
      {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
       {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}}};

  // Loop para alternar entre as matrizes
  while (true)
  {
    for (int i = 0; i < 23; i++) // Itera pelas 23 matrizes
    {
      for (int linha = 0; linha < 5; linha++)
      {
        for (int coluna = 0; coluna < 5; coluna++)
        {
          int posicao = getIndex(linha, coluna);
          npSetLED(posicao, matrizes[i][linha][coluna][0], matrizes[i][linha][coluna][1], matrizes[i][linha][coluna][2]);
        }
      }
      npWrite();      // Atualiza os LEDs
      sleep_ms(1000); // Espera 1 segundo
      npClear();      // Limpa os LEDs
    }
  }
}

void tecla_dois()
{
    // Inicializa entradas e saídas.
    stdio_init_all();

    // Inicializa matriz de LEDs NeoPixel.
    npInit(LED_PIN);
    npClear();

    // Declaração das matrizes
    int point_game[18][5][5][3] = {
        {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{32, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}},
        {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {32, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}},
        {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {32, 0, 255}, {0, 0, 0}}},
        {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {32, 0, 255}, {0, 0, 0}, {0, 0, 0}}},
        {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {32, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}},
        {{{0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {32, 0, 255}, {0, 0, 0}, {0, 0, 0}}},
        {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{255, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {32, 0, 255}, {0, 0, 0}}},
        {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {32, 0, 255}, {0, 0, 0}, {0, 0, 0}}},
        {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {32, 0, 255}, {0, 0, 0}, {0, 0, 0}}},
        {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {32, 0, 255}, {0, 0, 0}, {0, 0, 0}}},
        {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {32, 0, 255}, {0, 0, 0}}},
        {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {32, 0, 255}, {0, 0, 0}, {0, 0, 0}}},
        {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {32, 0, 255}, {0, 0, 0}, {0, 0, 0}}},
        {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {32, 0, 255}, {0, 0, 0}, {0, 0, 0}}},
        {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{255, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {32, 0, 255}, {0, 0, 0}, {0, 0, 0}}},
        {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {255, 0, 0}, {32, 0, 255}, {0, 0, 0}, {0, 0, 0}}},
        {{{0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}}},
        {{{32, 0, 255}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {32, 0, 255}},
         {{0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
         {{32, 0, 255}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {32, 0, 255}}}};

    // Loop para alternar entre as matrizes
    while (true)
    {
        for (int i = 0; i < 18; i++) // Itera pelas 18 matrizes
        {
            for (int linha = 0; linha < 5; linha++)
            {
                for (int coluna = 0; coluna < 5; coluna++)
                {
                    int posicao = getIndex(linha, coluna);
                    npSetLED(posicao, point_game[i][linha][coluna][0], point_game[i][linha][coluna][1], point_game[i][linha][coluna][2]);
                }
            }
            npWrite();      // Atualiza os LEDs
            sleep_ms(1000); // Espera 1 segundo
            npClear();      // Limpa os LEDs
        }
    }
}

void funcao_B()
{
    // Inicializa entradas e saídas.
    stdio_init_all();

    // Inicializa matriz de LEDs NeoPixel.
    npInit(LED_PIN);
    npClear();

    // Declaração das matrizes
    int full_blue[18][5][5][3] = {
    {{21, 0, 255}, {21, 0, 255}, {21, 0, 255}, {21, 0, 255}, {21, 0, 255}},
    {{21, 0, 255}, {21, 0, 255}, {21, 0, 255}, {21, 0, 255}, {21, 0, 255}},
    {{21, 0, 255}, {21, 0, 255}, {21, 0, 255}, {21, 0, 255}, {21, 0, 255}},
    {{21, 0, 255}, {21, 0, 255}, {21, 0, 255}, {21, 0, 255}, {21, 0, 255}},
    {{21, 0, 255}, {21, 0, 255}, {21, 0, 255}, {21, 0, 255}, {21, 0, 255}}};

     for(int linha = 0; linha < 5; linha++){
    for(int coluna = 0; coluna < 5; coluna++){
      int posicao = getIndex(linha, coluna);
      npSetLED(posicao, full_blue[coluna][linha][0], full_blue[coluna][linha][1], full_blue[coluna][linha][2]);
    }
  }
      npWrite();      // Atualiza os LEDs
      sleep_ms(1000); // Espera 1 segundo
      npClear();      // Limpa os LEDs 

}

void tecla_quatro() {
    // Definir as cores
    uint8_t pacmanColor[3] = {255, 255, 0};  
    uint8_t fundoColor[3] = {0, 0, 0};       

    // Limpar a matriz antes de começar a animação
    npClear();

    // Quadro 1: Pac-Man na posição (2, 2)
    npSetLED(getIndex(2, 2), pacmanColor[0], pacmanColor[1], pacmanColor[2]);
    npWrite();
    sleep_ms(200); // FPS (quadros por segundo)

    // Quadro 2: Pac-Man na posição (2, 3)
    npClear();
    npSetLED(getIndex(2, 3), pacmanColor[0], pacmanColor[1], pacmanColor[2]);
    npWrite();
    sleep_ms(200);

    // Quadro 3: Pac-Man na posição (2, 4)
    npClear();
    npSetLED(getIndex(2, 4), pacmanColor[0], pacmanColor[1], pacmanColor[2]);
    npWrite();
    sleep_ms(200);

    // Quadro 4: Pac-Man na posição (3, 4)
    npClear();
    npSetLED(getIndex(3, 4), pacmanColor[0], pacmanColor[1], pacmanColor[2]);
    npWrite();
    sleep_ms(200);

    // Quadro 5: Pac-Man na posição (4, 4)
    npClear();
    npSetLED(getIndex(4, 4), pacmanColor[0], pacmanColor[1], pacmanColor[2]);
    npWrite();
    sleep_ms(200);

    // Voltar à posição inicial (opcional)
    npClear();
    npWrite();
    sleep_ms(500); 
}

void funcao_D() {
    // Cor verde com 50% de intensidade
    uint8_t verde[3] = {0, 127, 0};  // RGB: 0 (vermelho), 127 (verde, 50% intensidade), 0 (azul)

    // Configura todos os LEDs para a cor verde
    for (int i = 0; i < 25; i++) {
        npSetLED(i, verde[0], verde[1], verde[2]);  // Define a cor verde nos LEDs da matriz
    }

    // Atualiza a matriz de LEDs
    npWrite();
}
