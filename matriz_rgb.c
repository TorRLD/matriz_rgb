#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

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
void funcaao_B();

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
        if (tecla != '\0') {
            printf("Tecla '%c' pressionada.\n", tecla);
            // Aqui você pode adicionar a lógica para o que fazer com a tecla pressionada
        }
        sleep_ms(100); // Pequena pausa para evitar sobrecarga do loop
    }
    
    return 0;
}
