#include "shell.h"
#include "terminal.h"
#include "util.h"
#include "pong.h"

#define SHELL_MAX_INPUT 128

static char shell_buffer[SHELL_MAX_INPUT];
static size_t shell_pos = 0;

static void shell_add_char(char c) {
    if (shell_pos < SHELL_MAX_INPUT - 1) {
        shell_buffer[shell_pos++] = c;
        terminal_putchar(c);
    }
}

static void shell_backspace(void) {
    if (shell_pos > 0) {
        shell_pos--;
        terminal_column--;
        terminal_putchar(' ');
        terminal_column--;
    }
}

static void shell_execute(void) {
    shell_buffer[shell_pos] = '\0';
    terminal_putchar('\n');

    if (strcmp(shell_buffer, "help") == 0) {
        terminal_writestring("Commands: help, clear, echo, kernel\n");
    } 
    else if (strcmp(shell_buffer, "clear") == 0) {
        terminal_initialize();
    } 
    else if (starts_with(shell_buffer, "echo ")) {
        terminal_writestring(shell_buffer + 5);
        terminal_putchar('\n');
    } 
    else if (strcmp(shell_buffer, "echo") == 0) {
        terminal_putchar('\n');
    } 
    else if (strcmp(shell_buffer, "kernel") == 0) {
        terminal_writestring("myos v0.0.1\n");
    } 
    else if (strcmp(shell_buffer, "pong") == 0) {
        terminal_initialize();
        pong();
    } 
    else {
        terminal_writestring("Comando desconhecido\nTente 'help' para conhecer os comandos existentes!\n");
    } 

    shell_pos = 0;
    terminal_writestring("> ");
}

void shell_init(void) {
    shell_pos = 0;
    terminal_writestring("> ");
}

void shell_handle_char(char c) {
    if (c == '\n') {
        shell_execute();
    } else if (c == '\b') {
        shell_backspace();
    } else {
        shell_add_char(c);
    }
}
