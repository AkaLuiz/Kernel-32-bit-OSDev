#include "terminal.h"
#include "keyboard.h"

int xPlayerUm = 15;
int yPlayerUm = VGA_HEIGHT/2;
int xPlayerDois = VGA_WIDTH-15;
int yPlayerDois = VGA_HEIGHT/2;
int xBolinha = VGA_WIDTH/2;
int yBolinha = VGA_HEIGHT/2;
int contador = 0;
int umX = 1;
int umY = 1;


void pong(){
    disable_cursor();
    while(1){
        char c = keyboard_getchar();
        terminal_putentryat('o', 2, xBolinha,yBolinha);
        terminal_putentryat('|', 3, xPlayerUm,yPlayerUm);
        terminal_putentryat('|', 3, xPlayerUm,yPlayerUm-1);
        terminal_putentryat('|', 3, xPlayerUm,yPlayerUm-2);
        terminal_putentryat('|', 4, xPlayerDois,yPlayerDois);
        terminal_putentryat('|', 4, xPlayerDois,yPlayerDois-1);
        terminal_putentryat('|', 4, xPlayerDois,yPlayerDois-2);
        
        contador++;

        if (contador >= 70000) {
            terminal_putentryat(' ', 2, xBolinha, yBolinha);

            if(yBolinha >= VGA_HEIGHT || yBolinha <= 0){
                umY = -umY;
            }

            if(xBolinha >= VGA_WIDTH || xBolinha <= 0){
                umX = -umX;
            }

            if (xBolinha == xPlayerUm + 1 &&
                yBolinha >= yPlayerUm - 2 &&
                yBolinha <= yPlayerUm) {
                umX = 1;
            }

            if (xBolinha == xPlayerDois - 1 &&
                yBolinha >= yPlayerDois - 2 &&
                yBolinha <= yPlayerDois) {
                umX = -1;
            }

            xBolinha+=umX;
            yBolinha+=umY;
            contador = 0;
        }

        if(c == 'w' && yPlayerUm > 2){
            yPlayerUm--;
            terminal_initialize();
        }
        if(c == 's' && VGA_HEIGHT > yPlayerUm+1 ){
            yPlayerUm++;
            terminal_initialize();
        }
        if(c == 'i' && yPlayerDois > 2){
            yPlayerDois--;
            terminal_initialize();
        }
        if(c == 'k' && VGA_HEIGHT > yPlayerDois+1 ){
            yPlayerDois++;
            terminal_initialize();
        }
        if(c == 'b'){
            terminal_initialize();
            enable_cursor();
            break;
        }
    }

}
