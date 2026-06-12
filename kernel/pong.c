#include "terminal.h"
#include "keyboard.h"

void desenhar_borda(void) {
    int cor = 7;

    for (int x = 0; x < VGA_WIDTH; x++) {
        terminal_putentryat('-', cor, x, 0);
        terminal_putentryat('-', cor, x, VGA_HEIGHT - 1);
    }

    for (int y = 1; y < VGA_HEIGHT - 1; y++) {
        terminal_putentryat('|', cor, 0, y);
        terminal_putentryat('|', cor, VGA_WIDTH - 1, y);
    }

    terminal_putentryat('+', cor, 0, 0);
    terminal_putentryat('+', cor, VGA_WIDTH - 1, 0);
    terminal_putentryat('+', cor, 0, VGA_HEIGHT - 1);
    terminal_putentryat('+', cor, VGA_WIDTH - 1, VGA_HEIGHT - 1);
}


void pong(){

    int xPlayerUm = 15;
    int yPlayerUm = VGA_HEIGHT/2;
    int xPlayerDois = VGA_WIDTH-15;
    int yPlayerDois = VGA_HEIGHT/2;
    int xBolinha = VGA_WIDTH/2;
    int yBolinha = VGA_HEIGHT/2;
    int xPlacarUm = xPlayerUm+5;
    int xPlacarDois = xPlayerDois-5;
    int yPlacar = 5;
    int contador = 0;
    int umX = 1;
    int umY = 1;
    int playerUm = 0;
    int playerDois = 0;

    terminal_putentryat('0', 5, xPlacarUm,yPlacar);
    terminal_putentryat('0', 5, xPlacarDois,yPlacar);
    terminal_putentryat('o', 2, xBolinha,yBolinha);
    terminal_putentryat('|', 3, xPlayerUm,yPlayerUm);
    terminal_putentryat('|', 3, xPlayerUm,yPlayerUm-1);
    terminal_putentryat('|', 3, xPlayerUm,yPlayerUm-2);
    terminal_putentryat('|', 4, xPlayerDois,yPlayerDois);
    terminal_putentryat('|', 4, xPlayerDois,yPlayerDois-1);
    terminal_putentryat('|', 4, xPlayerDois,yPlayerDois-2);

    desenhar_borda();

    disable_cursor();
    while(1){
        char c = keyboard_getchar();

        contador++;
        if (contador >= 900000) {
            terminal_putentryat(' ', 2, xBolinha, yBolinha);
            contador = 0;

            if(yBolinha >= VGA_HEIGHT -2 || yBolinha <= 0){
                umY = -umY;
            }

            if(xBolinha >= VGA_WIDTH -2 || xBolinha <= 0){
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

            if(xBolinha > xPlayerDois){
                xBolinha = VGA_WIDTH/2;
                yBolinha = VGA_HEIGHT/2;
                umY = -umY;
                playerUm++;
                terminal_putentryat('0'+playerUm,5,xPlacarUm,yPlacar);

            }
            
            if(xBolinha < xPlayerUm){
                xBolinha = VGA_WIDTH/2;
                yBolinha = VGA_HEIGHT/2;
                playerDois++;
                umX = -umX;
                terminal_putentryat('0'+playerDois,5,xPlacarDois,yPlacar);
                contador = -1000000;
            }

            xBolinha+=umX;
            yBolinha+=umY;
            terminal_putentryat('o', 2, xBolinha, yBolinha);
            
        }

        if(c == 'w' && yPlayerUm > 2){
            terminal_putentryat(' ', 3, xPlayerUm, yPlayerUm);
            yPlayerUm--;
            terminal_putentryat('|', 3, xPlayerUm,yPlayerUm-2);
        }
        if(c == 's' && VGA_HEIGHT > yPlayerUm+1 ){
            terminal_putentryat(' ', 3, xPlayerUm, yPlayerUm-2);
            yPlayerUm++;
            terminal_putentryat('|', 3, xPlayerUm,yPlayerUm);
        }
        if(c == 'i' && yPlayerDois > 2){
            terminal_putentryat(' ', 4, xPlayerDois, yPlayerDois);
            yPlayerDois--;
            terminal_putentryat('|', 4, xPlayerDois,yPlayerDois-2);
        }
        if(c == 'k' && VGA_HEIGHT > yPlayerDois+1 ){
            terminal_putentryat(' ', 4, xPlayerDois, yPlayerDois-2);
            yPlayerDois++;
            terminal_putentryat('|', 4, xPlayerDois,yPlayerDois);
        }
        if(c == 'b'){
            terminal_initialize();
            enable_cursor();
            break;
        }
    }

}
