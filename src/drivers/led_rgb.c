#include "drivers/led_rgb.h"

/**
 * @brief Inicializa o LED no pino especificado
 * @param led_pin Pino do LED
 */
void led_init(uint led_pin){
    gpio_init(led_pin);
    gpio_set_dir(led_pin, GPIO_OUT);
    gpio_put(led_pin, 0);
}

/**
 * @brief Inicializa todos os LEDs
 */
void led_init_all(void){
    led_init(LED_RED_PIN);
    led_init(LED_GREEN_PIN);
    led_init(LED_BLUE_PIN);
}

/**
 * @brief Acende o LED vermelho
 */
void red(void){
    gpio_put(LED_RED_PIN, 1);
    gpio_put(LED_GREEN_PIN, 0);
    gpio_put(LED_BLUE_PIN, 0);
}

/**
 * @brief Acende o LED verde
 */
void green(void){
    gpio_put(LED_RED_PIN, 0);
    gpio_put(LED_GREEN_PIN, 1);
    gpio_put(LED_BLUE_PIN, 0);
}

/**
 * @brief Acende o LED azul
 */
void blue(void){
    gpio_put(LED_RED_PIN, 0);
    gpio_put(LED_GREEN_PIN, 0);
    gpio_put(LED_BLUE_PIN, 1);
}

/**
 * @brief Acende o LED amarelo (vermelho + verde)
 */
void yellow(void){
    gpio_put(LED_RED_PIN, 1);
    gpio_put(LED_GREEN_PIN, 1);
    gpio_put(LED_BLUE_PIN, 0);
}

/**
 * @brief Acende o LED ciano (verde + azul)
 */
void cyan(void){
    gpio_put(LED_RED_PIN, 0);
    gpio_put(LED_GREEN_PIN, 1);
    gpio_put(LED_BLUE_PIN, 1);
}

/**
 * @brief Acende o LED magenta (vermelho + azul)
 */
void magenta(void){
    gpio_put(LED_RED_PIN, 1);
    gpio_put(LED_GREEN_PIN, 0);
    gpio_put(LED_BLUE_PIN, 1);
}


/**
 * @brief Acende o LED branco (vermelho + verde + azul)
 */
void white(void){
    gpio_put(LED_RED_PIN, 1);
    gpio_put(LED_GREEN_PIN, 1);
    gpio_put(LED_BLUE_PIN, 1);
}

/**
 * @brief Apaga todos os LEDs
 */
void black(void){
    gpio_put(LED_RED_PIN, 0);
    gpio_put(LED_GREEN_PIN, 0);
    gpio_put(LED_BLUE_PIN, 0);
}