#include "drivers/joystick.h"

/**
 * @brief Inicializa o joystick
 */
void joystick_init(void){
    adc_init();
    adc_gpio_init(JOYSTICK_X_PIN);
    adc_gpio_init(JOYSTICK_Y_PIN);
}

/**
 * @brief Lê o valor do eixo x do joystick
 * @return Valor do eixo x
 */
uint joystick_read_x(void){
    adc_select_input(1);
    return adc_read();
}

/**
 * @brief Lê o valor do eixo y do joystick
 * @return Valor do eixo y
 */
uint joystick_read_y(void){
    adc_select_input(0);
    return adc_read();
}

/**
 * @brief Lê o valor do joystick
 * @param x Ponteiro para armazenar o valor do eixo x
 * @param y Ponteiro para armazenar o valor do eixo y
 */
void reading_joystick(uint16_t *x, uint16_t *y){
    *x = joystick_read_x();
    *y = joystick_read_y();
}
