# EmbarcaTech Project Template

<p align="center">
  <img src="EmbarcaTechLogo.png" alt="EmbarcaTech" width="300">
</p>

## Projeto: Template de Projeto com Raspberry Pi Pico W

![C](https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge\&logo=c\&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-%23008FBA.svg?style=for-the-badge\&logo=cmake\&logoColor=white)
![Raspberry Pi](https://img.shields.io/badge/-Raspberry_Pi-C51A4A?style=for-the-badge\&logo=Raspberry-Pi)
![FreeRTOS](https://img.shields.io/badge/FreeRTOS-%23000000.svg?style=for-the-badge\&logo=freertos\&logoColor=white)
![GitHub](https://img.shields.io/badge/github-%23121011.svg?style=for-the-badge\&logo=github\&logoColor=white)
![Windows 11](https://img.shields.io/badge/Windows%2011-%230079d5.svg?style=for-the-badge\&logo=Windows%2011\&logoColor=white)

## Descrição do Projeto

Este repositório fornece um **modelo base personalizável** para projetos desenvolvidos no **EmbarcaTech** com o **Raspberry Pi Pico W (RP2040)**. A estrutura modular permite ativar ou remover componentes como **FreeRTOS**, **web server**, **cliente MQTT**, **sensores**, **display OLED**, **drivers**, e muito mais.

A flexibilidade do projeto permite que você inicie rapidamente um sistema embarcado completo, com suporte a multitarefa, rede e periféricos externos, usando uma organização limpa e escalável.

## Estrutura Modular do Projeto

O template organiza os arquivos em módulos separados para facilitar a leitura, manutenção e customização:

```
project_template/
├── src/                   # Código-fonte principal
│   ├── core/              # Tarefas e lógica principal
│   ├── display/           # Display OLED SSD1306 (opcional)
│   ├── drivers/           # Drivers de hardware (botão, buzzer, etc.)
│   ├── network/           # MQTT/Web Server + user_data.h
│   ├── sensors/           # Sensores I2C (MPU6050, BMP280, etc.)
│   ├── CMakeLists.txt     # Configuração local da build
│   └── main.c             # Ponto de entrada do programa
├── include/               # Headers organizados por módulo
├── lib/                   # Bibliotecas externas (FreeRTOS, LWIP, FatFS)
└── CMakeLists.txt         # Build principal do projeto
```

## Componentes Suportados

Este projeto suporta os seguintes módulos e periféricos (ativados conforme necessidade):

* ✅ **FreeRTOS** (kernel multitarefa)
* ✅ **Display OLED SSD1306** (I2C)
* ✅ **Web Server** baseado em LWIP
* ✅ **Cliente MQTT** com TLS
* ✅ **Drivers de Hardware**:

  * Botões (A, B, Joystick)
  * Buzzers
  * LED RGB
  * Matriz WS2812 (com suporte a `.pio`)
  * SDCard via SPI
* ✅ **Sensores**:

  * **MPU6050** (Acelerômetro + Giroscópio)
  * **BMP280** (Pressão e Temperatura)
  * **AHT20** (Umidade e Temperatura)

## Ambiente de Desenvolvimento

* **VS Code** com extensão do Raspberry Pi Pico
* **CMake** como sistema de build
* **Pico SDK**
* **FreeRTOS** (opcional)
* **LWIP** (para Web/MQTT)
* **FatFS** (para SDCard)

## user\_data.h

Quando **Web Server** ou **MQTT** são utilizados, as credenciais são configuradas no arquivo:

```c
#define WIFI_SSID "SuaRedeWiFi"
#define WIFI_PASSWORD "SuaSenhaWiFi"

#define MQTT_SERVER "000.000.0.000"
#define MQTT_USERNAME "usuario"
#define MQTT_PASSWORD "senha"
```

## Como Utilizar

1. **Clone o repositório**

```bash
git clone [URL_DO_REPOSITORIO]
```

2. **Configure o ambiente**

   * Configure o Pico SDK
   * Se necessário, baixe e configure FreeRTOS e LWIP

3. **Personalize o projeto**

   * Remova/adicione arquivos conforme os periféricos utilizados
   * Edite o `CMakeLists.txt` para ativar os módulos desejados

4. **Compile o projeto**

   * Use VS Code (ou CLI) para compilar:

   ```bash
   cmake -B build
   cd build && make
   ```

5. **Grave o firmware no Pico W**

   * Copie o `.uf2` gerado para o dispositivo via BOOTSEL

## Licença

Distribuído sob a licença MIT.

