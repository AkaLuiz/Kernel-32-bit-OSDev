#include "terminal.h"
#include "keyboard.h"
#include "shell.h"

void kernel_main(void) {
    terminal_initialize();
    terminal_writestring("MeuOS v0.1\n");

    shell_init();

    while (1) {
        char c = keyboard_getchar();
        if (c) {
            shell_handle_char(c);
        }
    }
}
