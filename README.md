# Kernel-32-bit-OSDev

# 🧱 Bare Bones – Kernel Mínimo Inspirado na OSDev Wiki

Este projeto é uma implementação educacional baseada no tutorial Bare Bones, da OSDev Wiki.
Ele demonstra como um kernel mínimo pode ser carregado por um bootloader compatível com Multiboot (ex.: GRUB) e como a execução é transferida para código C.

📚 **Fonte original**: https://wiki.osdev.org/Bare_Bones

🎯 **Objetivo**: entender todo o fluxo — bootloader → assembly → C → escrita direta na memória de vídeo (VGA).

```vbnet
/
├── boot.s       → Código Assembly com o header Multiboot e o ponto de entrada (_start)
├── kernel.c     → Kernel mínimo escrito em C
├── linker.ld    → Script do linker que organiza as seções na memória
└── grub.cfg     → Configuração do GRUB para criar ISO bootável

```

### 🚀 **1. boot.s** — Inicialização e Multiboot

O arquivo boot.s contém:

- Header Multiboot, necessário para o GRUB reconhecer o kernel.
- Configuração da stack
- Ponto de entrada _start, que chama kernel_main() em C.
- Loop final que mantém o kernel ativo.

🧩 **Header Multiboot**
```asm
.set ALIGN,    1<<0
.set MEMINFO,  1<<1
.set FLAGS,    ALIGN | MEMINFO
.set MAGIC,    0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM
```

**Explicação**:

| Campo        | Função                                   |
| ------------ | ---------------------------------------- |
| **MAGIC**    | Identificador obrigatório para Multiboot |
| **FLAGS**    | Diz ao bootloader o que fornecer         |
| **CHECKSUM** | Torna `MAGIC + FLAGS + CHECKSUM = 0`     |

🧵 **Stack**

```asm
.section .bss
.align 16
stack_bottom:
    .skip 16384
stack_top:

```

▶️ **Ponto de entrada**

```asm
.section .text
.global _start
_start:
    mov $stack_top, %esp
    call kernel_main
    cli

halt_loop:
    hlt
    jmp halt_loop

```

### 💻 2. **kernel.c** — Kernel Mínimo em C

O kernel é compilado com um cross-compiler i686-elf, pois não há libc disponível.

Isso significa:

- ❌ printf, malloc, memcpy → não existem
- ✔️ você implementa tudo do zero

O modo texto da VGA usa o endereço 0xB8000, com 80×25 caracteres.

#### **Cores VGA**

```c
enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    ...
    VGA_COLOR_WHITE = 15,
};
```
#### **Combinação de cores**

```c
uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}
```
#### **Caractere em formato VGA**

```c
uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t)uc | (uint16_t)color << 8;
}
```
#### **Inicialização do terminal**

```c
void terminal_initialize(void) {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

    for (size_t y = 0; y < VGA_HEIGHT; y++)
        for (size_t x = 0; x < VGA_WIDTH; x++)
            terminal_buffer[y * VGA_WIDTH + x] = vga_entry(' ', terminal_color);
}
```
#### **Suporte ao caractere** 
(Mini desafio da wiki OSDev)

```c
if (data[i] == '\n') {
    terminal_column = 0;
    terminal_row++;
}
```
#### **Entrada principal do kernel**

```c
void kernel_main(void) {
    terminal_initialize();
    terminal_writestring("Hello, kernel World!\nOla mundo!");
}
```
### 🧭 3. linker.ld — Layout da Memória

O linker script controla onde cada seção do kernel é colocada na memória.

```ld
ENTRY(_start)

SECTIONS {
    . = 2M;

    .text BLOCK(4K) : ALIGN(4K) {
        *(.multiboot)
        *(.text)
    }

    .rodata BLOCK(4K) : ALIGN(4K) {
        *(.rodata)
    }

    .data BLOCK(4K) : ALIGN(4K) {
        *(.data)
    }

    .bss BLOCK(4K) : ALIGN(4K) {
        *(COMMON)
        *(.bss)
    }
}
```
Seções:
| Seção       | Conteúdo                            |
| ----------- | ----------------------------------- |
| **.text**   | Código executável (Assembly + C)    |
| **.rodata** | Dados somente leitura               |
| **.data**   | Variáveis globais inicializadas     |
| **.bss**    | Variáveis globais não inicializadas |


### 🔥 4. Criando uma imagem ISO bootável com GRUB

Depois de compilar o kernel para myos.bin, você pode gerar uma imagem .iso inicializável.

#### 📁 Estrutura de diretórios

```sh
mkdir -p isodir/boot/grub
cp myos.bin isodir/boot/myos.bin
cp grub.cfg isodir/boot/grub/grub.cfg
```
#### Resultado:

```markdown
isodir/
└── boot/
    ├── myos.bin
    └── grub/
        └── grub.cfg

```

#### 📝 Arquivo grub.cfg

```cfg
menuentry "myos" {
    multiboot /boot/myos.bin
}
```

#### ⚙️ Gerando a ISO

```
grub-mkrescue -o myos.iso isodir
```

#### ▶️ Testando a ISO

```
qemu-system-i386 -cdrom myos.iso
```
