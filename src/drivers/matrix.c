#include "drivers/matrix.h"

//Cores 
const RGB RED = {0.1, 0, 0};
const RGB GREEN = {0, 0.1, 0};
const RGB BLUE = {0, 0, 0.1};
const RGB YELLOW = {0.1, 0.1, 0};
const RGB CYAN = {0, 0.1, 0.1};
const RGB MAGENTA = {0.1, 0, 0.1};
const RGB WHITE = {0.1, 0.1, 0.1};
const RGB BLACK = {0, 0, 0};

PIO pio; //Variável para armazenar a configuração da PIO
uint sm; //Variável para armazenar o estado da máquina

/**
 * @brief Inicializa a matriz de LEDs RGB
 */
uint matrix_init(void) {
   //Configurações da PIO
   pio = pio0; 
   uint offset = pio_add_program(pio, &pio_matrix_program);
   sm = pio_claim_unused_sm(pio, true);
   pio_matrix_program_init(pio, sm, offset, WS2812_PIN);
}

/**
 * @brief Converte as intensidades de cor para um valor RGB
 * @param r Intensidade da cor vermelha
 * @param g Intensidade da cor verde
 * @param b Intensidade da cor azul
 * @return Valor RGB
 */
uint32_t matrix_rgb(double r, double g, double b){
  unsigned char R, G, B;
  R = r * 100;
  G = g * 100;
  B = b * 100;
  return (G << 24) | (R << 16) | (B << 8);
}

/**
 * @brief Define as cores dos LEDs
 * @param pio Configuração da PIO
 * @param sm Estado da máquina
 * @param r Intensidade da cor vermelha
 * @param g Intensidade da cor verde
 * @param b Intensidade da cor azul
 */
void set_leds(PIO pio, uint sm, double r, double g, double b) {
    uint32_t valor_led;
    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        valor_led = matrix_rgb(r, g, b);
        pio_sm_put_blocking(pio, sm, valor_led);
    }
}

/**
 * @brief Obtém o índice do LED RGB
 * @param x Coordenada x
 * @param y Coordenada y
 * @return Índice do LED RGB
 */
int getIndex(int x, int y) {
    // Se a linha for par (0, 2, 4), percorremos da esquerda para a direita.
    // Se a linha for ímpar (1, 3), percorremos da direita para a esquerda.
    if (y % 2 == 0) {
        return 24-(y * 5 + x); // Linha par (esquerda para direita).
    } else {
        return 24-(y * 5 + (4 - x)); // Linha ímpar (direita para esquerda).
    }
}

/**
 * @brief Obtém as coordenadas do LED RGB
 * @param index Índice do LED RGB
 * @param x Ponteiro para armazenar a coordenada x
 * @param y Ponteiro para armazenar a coordenada y
 */
void getCoordinates(int index, int *x, int *y) {
    *x = (index % 5);
    *y = (index / 5);
}

/**
 * @brief Converte as coordenadas para o índice do LED RGB
 * @param x Coordenada x
 * @param y Coordenada y
 * @return Índice do LED RGB
 */
uint coordenates_to_index(int x, int y) {
    return x * 5 + y;
}

/**
 * @brief Desenha a matriz de LEDs RGB
 * @param pixels Array com as cores dos LEDs
 * @param pio Configuração da PIO
 * @param sm Estado da máquina
 */
void draw_matrix(RGB pixels[NUM_PIXELS], PIO pio, uint sm) {
    for (int i = 0; i < NUM_PIXELS; i++) {
        int x = i % 5;
        int y = i / 5;
        int index = getIndex(x, y);
        pio_sm_put_blocking(pio, sm, matrix_rgb(pixels[index].R, pixels[index].G, pixels[index].B));
    }
}

/**
 * @brief Apaga a matriz de LEDs RGB
 */
void clear_matrix(void){
    RGB pixels[NUM_PIXELS];
    for (int i = 0; i < NUM_PIXELS; i++) {
        pixels[i] = BLACK;
    }
    draw_matrix(pixels, pio0, 0);
}

