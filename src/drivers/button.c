#include "drivers/button.h"

/**
 * @brief Inicializa o botão no pino especificado
 * @param gpio Pino do botão
 */
void button_init(uint gpio){
    gpio_init(gpio);
    gpio_set_dir(gpio, GPIO_IN);
    gpio_pull_up(gpio);
}

/**
 * @brief Inicializa todos os botões
 */
void button_init_all(void){
    button_init(BUTTON_A_PIN);
    button_init(BUTTON_B_PIN);
    button_init(JOYSTICK_BUTTON_PIN);
}

/**
 * @brief Função para debouncing
 * @param last_time Ponteiro para a última vez que o botão foi pressionado
 * @return true se o botão foi pressionado, false caso contrário
 */
bool debounce(volatile uint32_t *last_time){
    uint32_t current_time = to_ms_since_boot(get_absolute_time());
    if (current_time - *last_time > 250){ 
        *last_time = current_time;
        return true;
    }
    return false;
}

