#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include "user_data.h"

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h" 
      
#include "lwip/pbuf.h"           
#include "lwip/tcp.h"            
#include "lwip/netif.h" 

#define LED_PIN CYW43_WL_GPIO_LED_PIN 

void user_request(char *html, size_t html_size); // Função para lidar com a requisição do usuário
static err_t tcp_server_accept(void *arg, struct tcp_pcb *newpcb, err_t err); // Função de callback para aceitar conexões TCP
static err_t http_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err); // Função de callback para receber dados TCP
void server_init(void); // Inicializa o servidor TCP
bool handle_http_request(const char *request, size_t request_size, char *response, size_t response_size); // Função para lidar com a requisição HTTP

#endif