#include "terminal.h"
#include "keyboard.h"

size_t x = 15;
size_t y = 15;

void pong(){
    
    while(1){
        char c = keyboard_getchar();
        terminal_putentryat('|', 3, x,y);
        terminal_putentryat('|', 3, x,y-1);
        if(c == 'w' && y > 1){
            y--;
            terminal_initialize();
        }
        if(c == 's' && VGA_HEIGHT > y+1 ){
            y++;
            terminal_initialize();
        }
    }

}
