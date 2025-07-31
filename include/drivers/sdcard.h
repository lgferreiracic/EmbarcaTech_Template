#ifndef SDCARD_H
#define SDCARD_H

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "hardware/rtc.h"
#include "pico/stdlib.h"

#include "ff.h"
#include "diskio.h"
#include "f_util.h"
#include "hw_config.h"
#include "my_debug.h"
#include "rtc.h"
#include "sd_card.h"

typedef void (*p_fn_t)();
typedef struct
{
    char const *const command;
    p_fn_t const function;
    char const *const help;
} cmd_def_t;

sd_card_t *sd_get_by_name(const char *const name); // Obtém o cartão SD pelo nome
FATFS *sd_get_fs_by_name(const char *name); // Obtém o sistema de arquivos pelo nome
void run_setrtc(void); // Configura a data e hora do RTC
void run_format(void); // Formata o cartão SD
void run_mount(void); // Monta o cartão SD
void run_unmount(void); // Desmonta o cartão SD
void run_getfree(void); // Obtém o espaço livre no cartão SD
void run_ls(void); // Lista os arquivos do cartão SD
void run_cat(void); // Lê o conteúdo de um arquivo
void read_file(const char *filename); // Lê o conteúdo de um arquivo 

#endif