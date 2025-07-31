#include "network/web_server.h"

/**
 * @brief HTML para ser enviado ao cliente
 */
const char HTML[] =
"<!DOCTYPE html>"
"<html lang=\"pt-BR\">"
    "<head>"
        "<meta charset=\"UTF-8\">"
        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
        "<title>"
        "</title>"
        "<style>"
        "</style>"
    "</head>"
    "<body>"
        "<script>"
        "</script>"
    "</body>"
"</html>";

/**
 * @brief Função de callback para receber dados TCP
 * @param arg Ponteiro para os dados do cliente
 * @param tpcb Ponteiro para o PCB TCP
 * @param p Ponteiro para o buffer de dados
 * @param err Código de erro
 */
static err_t http_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
    if (!p)
    {
        tcp_close(tpcb);
        return ERR_OK;
    }

    char *req = (char *)p->payload;

    const char *header_html =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Connection: close\r\n"
        "\r\n";

    const char *header_json =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "Connection: close\r\n"
        "\r\n";

    char json[64];

    if (p->len >= 6 && strncmp(req, "GET / ", 6) == 0){
        tcp_write(tpcb, header_html, strlen(header_html), TCP_WRITE_FLAG_COPY);
        tcp_write(tpcb, HTML, strlen(HTML), TCP_WRITE_FLAG_COPY);
    }else if(handle_http_request(req, p->len, json, sizeof(json))) {
        tcp_write(tpcb, header_json, strlen(header_json), TCP_WRITE_FLAG_COPY);
        tcp_write(tpcb, json, strlen(json), TCP_WRITE_FLAG_COPY);
    } else {
        const char *not_found =
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/plain\r\n"
            "Connection: close\r\n"
            "\r\n"
            "Recurso não encontrado.";
        tcp_write(tpcb, not_found, strlen(not_found), TCP_WRITE_FLAG_COPY);
    }
    tcp_recved(tpcb, p->tot_len);
    pbuf_free(p);
    tcp_close(tpcb); // Fecha a conexão após envio da resposta
    return ERR_OK;
}

/**
 * @brief Função de callback para aceitar conexões TCP
 * @param arg Ponteiro para os dados do cliente
 * @param newpcb Ponteiro para o novo PCB TCP
 * @param err Código de erro
 */
static err_t tcp_server_accept(void *arg, struct tcp_pcb *newpcb, err_t err){
    tcp_recv(newpcb, http_recv);
    return ERR_OK;
}

/**
 * @brief Inicializa o servidor TCP
 */
void server_init(void) {
    //Inicializa a arquitetura do cyw43
    if (cyw43_arch_init()) {
        panic("Failed to initialize CYW43");
    }

    // GPIO do CI CYW43 em nível baixo
    cyw43_arch_gpio_put(LED_PIN, 1);

    // Ativa o Wi-Fi no modo Station, de modo a que possam ser feitas ligações a outros pontos de acesso Wi-Fi.
    cyw43_arch_enable_sta_mode();

    // Conectar à rede WiFI 
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        panic("Failed to connect to WiFi");
    }

    // Caso seja a interface de rede padrão - imprimir o IP do dispositivo.
    if (netif_default){
        printf("IP do dispositivo: %s\n", ipaddr_ntoa(&netif_default->ip_addr));
    }

    // Configura o servidor TCP - cria novos PCBs TCP. É o primeiro passo para estabelecer uma conexão TCP.
    struct tcp_pcb *server = tcp_new();
    if (!server){
        panic("Failed to create TCP PCB\n");
    }

    //vincula um PCB (Protocol Control Block) TCP a um endereço IP e porta específicos.
    if (tcp_bind(server, IP_ADDR_ANY, 80) != ERR_OK){
        panic("Failed to bind TCP PCB\n");
    }

    // Coloca um PCB (Protocol Control Block) TCP em modo de escuta, permitindo que ele aceite conexões de entrada.
    server = tcp_listen(server);

    // Define uma função de callback para aceitar conexões TCP de entrada. É um passo importante na configuração de servidores TCP.
    tcp_accept(server, tcp_server_accept);
    printf("Servidor ouvindo na porta 80\n");
}

