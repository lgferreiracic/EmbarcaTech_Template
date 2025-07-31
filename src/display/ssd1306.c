#include "display/ssd1306.h"
#include "display/font.h"

/**
 * @brief Inicializa o display OLED SSD1306
 * @param ssd Ponteiro para a estrutura do display
 * @param width Largura do display
 * @param height Altura do display
 * @param external_vcc Tensão de alimentação externa
 * @param address Endereço do display
 * @param i2c Ponteiro para a instância I2C
 */
void ssd1306_init(ssd1306_t *ssd, uint8_t width, uint8_t height, bool external_vcc, uint8_t address, i2c_inst_t *i2c) {
  ssd->width = width;
  ssd->height = height;
  ssd->pages = height / 8U;
  ssd->address = address;
  ssd->i2c_port = i2c;
  ssd->bufsize = ssd->pages * ssd->width + 1;
  ssd->ram_buffer = calloc(ssd->bufsize, sizeof(uint8_t));
  ssd->ram_buffer[0] = 0x40;
  ssd->port_buffer[0] = 0x80;
}

/**
 * @brief Configura o display OLED SSD1306
 * @param ssd Ponteiro para a estrutura do display
 */
void ssd1306_config(ssd1306_t *ssd) {
  ssd1306_command(ssd, SET_DISP | 0x00);
  ssd1306_command(ssd, SET_MEM_ADDR);
  ssd1306_command(ssd, 0x01);
  ssd1306_command(ssd, SET_DISP_START_LINE | 0x00);
  ssd1306_command(ssd, SET_SEG_REMAP | 0x01);
  ssd1306_command(ssd, SET_MUX_RATIO);
  ssd1306_command(ssd, HEIGHT - 1);
  ssd1306_command(ssd, SET_COM_OUT_DIR | 0x08);
  ssd1306_command(ssd, SET_DISP_OFFSET);
  ssd1306_command(ssd, 0x00);
  ssd1306_command(ssd, SET_COM_PIN_CFG);
  ssd1306_command(ssd, 0x12);
  ssd1306_command(ssd, SET_DISP_CLK_DIV);
  ssd1306_command(ssd, 0x80);
  ssd1306_command(ssd, SET_PRECHARGE);
  ssd1306_command(ssd, 0xF1);
  ssd1306_command(ssd, SET_VCOM_DESEL);
  ssd1306_command(ssd, 0x30);
  ssd1306_command(ssd, SET_CONTRAST);
  ssd1306_command(ssd, 0xFF);
  ssd1306_command(ssd, SET_ENTIRE_ON);
  ssd1306_command(ssd, SET_NORM_INV);
  ssd1306_command(ssd, SET_CHARGE_PUMP);
  ssd1306_command(ssd, 0x14);
  ssd1306_command(ssd, SET_DISP | 0x01);
}

/**
 * @brief Envia um comando para o display OLED SSD1306
 * @param ssd Ponteiro para a estrutura do displayyy
 * @param command Comando a ser enviado
 */
void ssd1306_command(ssd1306_t *ssd, uint8_t command) {
  ssd->port_buffer[1] = command;
  i2c_write_blocking(
    ssd->i2c_port,
    ssd->address,
    ssd->port_buffer,
    2,
    false
  );
}

/**
 * @brief Envia dados para o display OLED SSD1306
 * @param ssd Ponteiro para a estrutura do display
 */
void ssd1306_send_data(ssd1306_t *ssd) {
  ssd1306_command(ssd, SET_COL_ADDR);
  ssd1306_command(ssd, 0);
  ssd1306_command(ssd, ssd->width - 1);
  ssd1306_command(ssd, SET_PAGE_ADDR);
  ssd1306_command(ssd, 0);
  ssd1306_command(ssd, ssd->pages - 1);
  i2c_write_blocking(
    ssd->i2c_port,
    ssd->address,
    ssd->ram_buffer,
    ssd->bufsize,
    false
  );
}

/**
 * @brief Desenha um pixel no display OLED SSD1306
 * @param ssd Ponteiro para a estrutura do display
 * @param x Posição x do pixel
 * @param y Posição y do pixel
 * @param value Valor do pixel
 */
void ssd1306_pixel(ssd1306_t *ssd, uint8_t x, uint8_t y, bool value) {
  uint16_t index = (y >> 3) + (x << 3) + 1;
  uint8_t pixel = (y & 0b111);
  if (value)
    ssd->ram_buffer[index] |= (1 << pixel);
  else
    ssd->ram_buffer[index] &= ~(1 << pixel);
}

/**
 * @brief Preenche o display OLED SSD1306
 * @param ssd Ponteiro para a estrutura do display
 * @param value Valor do pixel
 */
void ssd1306_fill(ssd1306_t *ssd, bool value) {
    // Itera por todas as posições do display
    for (uint8_t y = 0; y < ssd->height; ++y) {
        for (uint8_t x = 0; x < ssd->width; ++x) {
            ssd1306_pixel(ssd, x, y, value);
        }
    }
}

/**
 * @brief Desenha um retângulo no display OLED SSD1306
 * @param ssd Ponteiro para a estrutura do display
 * @param top Posição y do topo do retângulo
 * @param left Posição x da esquerda do retângulo
 * @param width Largura do retângulo
 * @param height Altura do retângulo
 * @param value Valor do pixel
 * @param fill Preenche o retângulo
 */
void ssd1306_rect(ssd1306_t *ssd, uint8_t top, uint8_t left, uint8_t width, uint8_t height, bool value, bool fill) {
  for (uint8_t x = left; x < left + width; ++x) {
    ssd1306_pixel(ssd, x, top, value);
    ssd1306_pixel(ssd, x, top + height - 1, value);
  }
  for (uint8_t y = top; y < top + height; ++y) {
    ssd1306_pixel(ssd, left, y, value);
    ssd1306_pixel(ssd, left + width - 1, y, value);
  }

  if (fill) {
    for (uint8_t x = left + 1; x < left + width - 1; ++x) {
      for (uint8_t y = top + 1; y < top + height - 1; ++y) {
        ssd1306_pixel(ssd, x, y, value);
      }
    }
  }
}

/**
 * @brief Desenha uma linha no display OLED SSD1306
 * @param ssd Ponteiro para a estrutura do display
 * @param x0 Posição x do início da linha
 * @param y0 Posição y do início da linha
 * @param x1 Posição x do fim da linha
 * @param y1 Posição y do fim da linha
 * @param value Valor do pixel
 */
void ssd1306_line(ssd1306_t *ssd, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, bool value) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);

    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;

    int err = dx - dy;

    while (true) {
        ssd1306_pixel(ssd, x0, y0, value); // Desenha o pixel atual

        if (x0 == x1 && y0 == y1) break; // Termina quando alcança o ponto final

        int e2 = err * 2;

        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }

        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

/**
 * @brief Desenha uma linha horizontal no display OLED SSD1306
 * @param ssd Ponteiro para a estrutura do display
 * @param x0 Posição x do início da linha
 * @param x1 Posição x do fim da linha
 * @param y Posição y da linha
 * @param value Valor do pixel
 */
void ssd1306_hline(ssd1306_t *ssd, uint8_t x0, uint8_t x1, uint8_t y, bool value) {
  for (uint8_t x = x0; x <= x1; ++x)
    ssd1306_pixel(ssd, x, y, value);
}

/**
 * @brief Desenha uma linha vertical no display OLED SSD1306
 * @param ssd Ponteiro para a estrutura do display
 * @param x Posição x da linha
 * @param y0 Posição y do início da linha
 * @param y1 Posição y do fim da linha
 * @param value Valor do pixel
 */
void ssd1306_vline(ssd1306_t *ssd, uint8_t x, uint8_t y0, uint8_t y1, bool value) {
  for (uint8_t y = y0; y <= y1; ++y)
    ssd1306_pixel(ssd, x, y, value);
}

/**
 * @brief Desenha um caractere no display OLED SSD1306
 * @param ssd Ponteiro para a estrutura do display
 * @param c Caractere a ser desenhado
 * @param x Posição x do caractere
 * @param y Posição y do caractere
 */
void ssd1306_draw_char(ssd1306_t *ssd, char c, uint8_t x, uint8_t y){
  uint16_t index = (c - ' ') * 8;
  char ver=c;
  
  for (uint8_t i = 0; i < 8; ++i)
  {
    uint8_t line = font[index + i];
    for (uint8_t j = 0; j < 8; ++j)
    {
      ssd1306_pixel(ssd, x + i, y + j, line & (1 << j));
    }
  }
}

/**
 * @brief Desenha uma string no display OLED SSD1306
 * @param ssd Ponteiro para a estrutura do display
 * @param str String a ser desenhada
 * @param x Posição x da string
 * @param y Posição y da string
 */
void ssd1306_draw_string(ssd1306_t *ssd, const char *str, uint8_t x, uint8_t y)
{
  while (*str)
  {
    ssd1306_draw_char(ssd, *str++, x, y);
    x += 8;
    if (x + 8 >= ssd->width)
    {
      x = 0;
      y += 8;
    }
    if (y + 8 >= ssd->height)
    {
      break;
    }
  }
}

/**
 * @brief Inicializa o display OLED SSD1306
 * @param ssd Ponteiro para a estrutura do display
 */
void display_init(ssd1306_t *ssd){
  i2c_init(I2C_PORT_DISP, 400000);
  gpio_set_function(I2C_SDA_DISP, GPIO_FUNC_I2C);
  gpio_set_function(I2C_SCL_DISP, GPIO_FUNC_I2C);
  gpio_pull_up(I2C_SDA_DISP);
  gpio_pull_up(I2C_SCL_DISP);
  ssd1306_init(ssd, SSD1306_WIDTH, SSD1306_HEIGHT, false, SSD1306_ADDR, I2C_PORT_DISP);
  ssd1306_config(ssd);
  ssd1306_fill(ssd, false);
  ssd1306_send_data(ssd);
}

/**
 * @brief Limpa o display OLED SSD1306
 * @param ssd Ponteiro para a estrutura do display
 */
void ssd1306_clear(ssd1306_t *ssd) {
  ssd1306_fill(ssd, false); // Preenche o buffer com zeros
  ssd1306_send_data(ssd);   // Envia o buffer para o display
}

/**
 * @brief Inicia a exibição da tela de início
 * @param ssd Ponteiro para a estrutura do display
 */
void start_display(ssd1306_t *ssd){
  ssd1306_fill(ssd, false); // Limpa o display
  ssd1306_draw_string(ssd, "CEPEDI   TIC37", 8, 10); // Desenha uma string
  ssd1306_draw_string(ssd, "Estabelecendo", 12, 30); // Desenha uma string
  ssd1306_draw_string(ssd, "Conexao", 33, 48); // Desenha uma string      
  ssd1306_send_data(ssd); // Atualiza o display
}

/**
 * @brief Inicia a exibição da tela de status
 * @param ssd Ponteiro para a estrutura do display
 * @param string1 Primeira string a ser desenhada
 * @param string2 Segunda string a ser desenhada
 */
void status_display(ssd1306_t *ssd, const char *string1, const char *string2){
  ssd1306_fill(ssd, false); // Limpa o display
  ssd1306_draw_string(ssd, "CEPEDI   TIC37", 8, 10); // Desenha uma string
  ssd1306_draw_string(ssd, string1, 12, 30); // Desenha uma string
  if(string2 != NULL)
    ssd1306_draw_string(ssd, string2, 17, 48); // Desenha uma string      
  ssd1306_send_data(ssd); // Atualiza o display
}
