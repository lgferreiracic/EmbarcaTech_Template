#include "sensors/mpu6050.h"

/**
 * @brief Reseta o MPU6050
 */
void mpu6050_reset()
{
    // Dois bytes para reset: primeiro o registrador, segundo o dado
    uint8_t buf[] = {0x6B, 0x80};
    i2c_write_blocking(I2C_PORT, addr, buf, 2, false);
    sleep_ms(100); // Aguarda reset e estabilização

    // Sai do modo sleep (registrador 0x6B, valor 0x00)
    buf[1] = 0x00;
    i2c_write_blocking(I2C_PORT, addr, buf, 2, false);
    sleep_ms(10); // Aguarda estabilização após acordar
}

/**
 * @brief Lê os valores crus de aceleração, giroscópio e temperatura do MPU6050
 * @param accel Array de 3 elementos para armazenar os valores de aceleração
 * @param gyro Array de 3 elementos para armazenar os valores de giroscópio
 * @param temp Ponteiro para armazenar o valor de temperatura
 */
void mpu6050_read_raw(int16_t accel[3], int16_t gyro[3], int16_t *temp)
{
    uint8_t buffer[6];

    // Lê aceleração a partir do registrador 0x3B (6 bytes)
    uint8_t val = 0x3B;
    i2c_write_blocking(I2C_PORT, addr, &val, 1, true);
    i2c_read_blocking(I2C_PORT, addr, buffer, 6, false);

    for (int i = 0; i < 3; i++)
    {
        accel[i] = (buffer[i * 2] << 8) | buffer[(i * 2) + 1];
    }

    // Lê giroscópio a partir do registrador 0x43 (6 bytes)
    val = 0x43;
    i2c_write_blocking(I2C_PORT, addr, &val, 1, true);
    i2c_read_blocking(I2C_PORT, addr, buffer, 6, false);

    for (int i = 0; i < 3; i++)
    {
        gyro[i] = (buffer[i * 2] << 8) | buffer[(i * 2) + 1];
    }

    // Lê temperatura a partir do registrador 0x41 (2 bytes)
    val = 0x41;
    i2c_write_blocking(I2C_PORT, addr, &val, 1, true);
    i2c_read_blocking(I2C_PORT, addr, buffer, 2, false);

    *temp = (buffer[0] << 8) | buffer[1];
}

/**
 * @brief Inicializa o MPU6050
 */
void init_mpu6050(){
    // Inicialização da I2C do MPU6050
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Declara os pinos como I2C na Binary Info
    bi_decl(bi_2pins_with_func(I2C_SDA, I2C_SCL, GPIO_FUNC_I2C));
    mpu6050_reset();
}

/**
 * @brief Lê os valores crus de aceleração, giroscópio e temperatura do MPU6050
 * @return Estrutura MPU6050_Data com os valores lidos
 */
MPU6050_Data get_mpu6050_data(void){
    int16_t acc[3], gyro[3], temp;
    MPU6050_Data data;
    mpu6050_read_raw(acc, gyro, &temp);
    data.accel_x = acc[0];
    data.accel_y = acc[1];
    data.accel_z = acc[2];
    data.gyro_x = gyro[0];
    data.gyro_y = gyro[1];
    data.gyro_z = gyro[2];
    data.temp = temp;
    return data;
}