#include <8051.h>
#include <stdio.h>
#include <stdint.h>
#include <at89c51ed2.h>

#define MAX_DIGITS 5
#define CARRIAGE_RETURN 13
#define BACKSPACE 8
#define SPACE 32

#define ENTER_KEY (13)
#define DIVIDE_BY_16 (16)
#define ASCII_SPACE (32)
#define NUMBER_BASE (16)
#define DATA_MAX (255)
#define ADDRESS_MAX (0x7FFF)
#define CODE_START_ADDRESS (0x7fff)

// Function Prototypes
void display_help(void);
int getchar(void);
int putchar(int c);
void uart_initialization(void);
char int_to_char(int num);
unsigned char char_to_int(unsigned char ch);
unsigned int parse_user_input(unsigned char base);
void xram_write(unsigned int address, unsigned char data);
unsigned char xram_read(unsigned int address);
void read_memory(void);
void memory_read(unsigned int start_address, unsigned int end_address);
void print_hex_number(uint32_t num, unsigned char width);
void write_memory(void);
void read_code_memory(void);
void code_memory_read(unsigned int start_address, unsigned int end_address);
void handle_command(void);
void main(void);

// Function Definitions
void display_help(void) {
    printf("\r\n========================================\r\n");
    printf("\r\n");
    printf("         MEMORY EDITOR \r\n");
    printf("\r\n");
    printf("============================================\r\n");
    printf("\r\n");
    printf(" Available Commands:\r\n");
    printf("\r\n");
    printf("< R >  Read Data Memory\r\n");
    printf("\r\n");
    printf("< W >  Write Data Memory\r\n");
    printf("\r\n");
    printf("< C >  Read Code Memory\r\n");
    printf("\r\n");
    printf("< H >  Display This Help Menu\r\n");
    printf("\r\n");
    printf("< X >  Exit \r\n");
    printf("\r\n");
    printf("===========================================\r\n");
}
int getchar(void) {
    while (!RI);
    RI = 0;
    return SBUF;
}

int putchar(int c) {
    while (!TI);
    SBUF = c;
    TI = 0;
    return c;
}

void uart_initialization(void) {
    TMOD |= 0x20;
    SCON = 0x50;
    TH1 = 0xFD;
    TR1 = 1;
    ES = 1;
    EA = 1;
    TI = 1;
}

void initialize_xram(void) {
    unsigned int address;
    for (address = 0x0000; address <= ADDRESS_MAX; address++) {
        xram_write(address, 0xFF);
    }
    //printf("\r\nXRAM Initialized from 0x0000 to 0x7FFF\r\n");
}



void print_hex_number(uint32_t num, unsigned char width) {
    unsigned char buffer[10];
    int8_t digit_count = 0;
    do {
        buffer[digit_count++] = int_to_char(num % 16);
        num /= 16;
    } while (num > 0);

    while (digit_count < width) {
        putchar('0');
        width--;
    }

    for (int i = digit_count - 1; i >= 0; i--) {
        putchar(buffer[i]);
    }
}

void write_memory(void) {
    unsigned int start_address, end_address;
    unsigned char data;

    printf("\r\n Enter Start Address to Write (Hex):\r\n");
    printf("\r\n");
    start_address = parse_user_input(NUMBER_BASE);
     printf("\r\n");
     if (start_address > 0xFFFF) {
        printf("\r\n Error: Start Address must be between 0x0000 and 0xFFFF.\r\n");
        return;
    }
    printf("\r\n Enter End Address to Write (Hex):\r\n");
    printf("\r\n");
    end_address = parse_user_input(NUMBER_BASE);
    if (end_address > 0xFFFF) {
        printf("\r\n Error: End Address must be between 0x0000 and 0xFFFF.\r\n");
        return;
    }
    if (end_address < start_address) {
        printf("\r\n Error: End Address must be greater than or equal to Start Address.\r\n");
        return;
    }
  printf("\r\n");
    printf("\r\n Enter Data to Write (Hex):\r\n");
      printf("\r\n");
    data = parse_user_input(NUMBER_BASE);
    printf("\r\n");
    printf("\r\n---------------------------XRAM WRITE----------------------------\r\n");
    printf("\r\n");
    printf("Addr: +0  +1  +2  +3  +4  +5  +6  +7  +8  +9  +A  +B  +C  +D  +E  +F\r\n");

    for (unsigned int addr = start_address; addr <= end_address; addr++) {
        if ((addr - start_address) % DIVIDE_BY_16 == 0) {
            printf("\r\n%04X: ", addr);
        }
        printf("%02X  ", data);
        xram_write(addr, data);
    }
      printf("\r\n");

    printf("\r\n---------------------------------------------------------------------\r\n");
    printf("\r\n Data 0x%02X written to addresses 0x%04X to 0x%04X.\r\n", data, start_address, end_address);
}

char int_to_char(int num) {
    if (num >= 0 && num <= 9)
        return '0' + num;
    else if (num >= 10 && num <= 15)
        return 'A' + (num - 10);
    return '0';
}

unsigned char char_to_int(unsigned char ch) {
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    else if (ch >= 'A' && ch <= 'F')
        return ch - 'A' + 10;
    else if (ch >= 'a' && ch <= 'f')
        return ch - 'a' + 10;
    return 0;
}

unsigned int parse_user_input(unsigned char base) {
    unsigned int number = 0;
    unsigned int digits[MAX_DIGITS] = {0};
    unsigned int digit_count = 0;
    unsigned char current_char = 0;

    while (current_char != CARRIAGE_RETURN) {
        current_char = getchar();
        if (((current_char >= '0') && (current_char <= '9')) ||
            ((current_char >= 'a') && (current_char <= 'f')) ||
            ((current_char >= 'A') && (current_char <= 'F'))) {
            putchar( current_char );
            digits[digit_count++] = char_to_int(current_char);
        } else if (current_char == BACKSPACE && digit_count > 0) {
            putchar(BACKSPACE);
            putchar(SPACE);
            putchar(BACKSPACE);
            digit_count--;
        }
    }

    for (unsigned int i = 0; i < digit_count; i++) {
        number = number * base + digits[i];
    }
    return number;
}

void xram_write(unsigned int address, unsigned char data) {
    unsigned char __xdata *ptr = (unsigned char __xdata *)address;
    *ptr = data;
}

unsigned char xram_read(unsigned int address) {
    unsigned char __xdata *ptr = (unsigned char __xdata *)address;
    return *ptr;
}

void read_memory(void) {
    unsigned int start_address, end_address;

    printf("\r\n Enter Start Address to Read (Hex):\r\n");
    printf("\r\n");
    start_address = parse_user_input(NUMBER_BASE);
    printf("\r\n");
     if (start_address > 0xFFFF) {
        printf("\r\n Error: Start Address must be between 0x0000 and 0xFFFF.\r\n");
        return;
    }
    printf("\r\n Enter End Address to Read (Hex):\r\n");
    printf("\r\n");
    end_address = parse_user_input(NUMBER_BASE);
    if (end_address > 0xFFFF) {
        printf("\r\n Error: End Address must be between 0x0000 and 0xFFFF.\r\n");
        return;
    }
    if (end_address < start_address) {
        printf("\r\n Error: End Address must be greater than or equal to Start Address.\r\n");
        return;
    }

    printf("\r\n--------------------------XRAM CONTENTS--------------------------\r\n");
    printf("\r\n");
    printf("Addr: +0  +1  +2  +3  +4  +5  +6  +7  +8  +9  +A  +B  +C  +D  +E  +F\r\n");
    memory_read(start_address, end_address);
    printf("\r\n------------------------------------------------------------------\r\n");
}


void memory_read(unsigned int start_address, unsigned int end_address) {
    unsigned char count = 0;

    while (start_address <= end_address) {
        if (count % DIVIDE_BY_16 == 0) {
            printf("\r\n%04X: ", start_address);
        }
        printf("%02X  ", xram_read(start_address));
        start_address++;
        count++;
    }
    printf("\r\n");
}
void read_code_memory(void) {
    unsigned int start_address;
    unsigned int end_address;

    printf("\r\n Enter Start Address (Code Memory):\r\n");
     printf("\r\n");
    start_address = parse_user_input(NUMBER_BASE);
     printf("\r\n");
    if (start_address > CODE_START_ADDRESS) {
        printf("\r\n Invalid Code Memory Start Address. Must be < 0x7fff.\r\n");
        return;
    }

    printf("\r\n Enter End Address (Code Memory):\r\n");
     printf("\r\n");
    end_address = parse_user_input(NUMBER_BASE);
     printf("\r\n");
    if (end_address > CODE_START_ADDRESS) {
        printf("\r\n Invalid Code Memory End Address. Must be < 0x7fff.\r\n");
        return;
    }
    if (end_address < start_address) {
        printf("\r\n Error: End Address must be greater than or equal to Start Address.\r\n");
        return;
    }

    printf("\r\n-----------------------CODE MEMORY CONTENTS----------------------\r\n");
    printf("\r\n");
    printf("Addr: +0  +1  +2  +3  +4  +5  +6  +7  +8  +9  +A  +B  +C  +D  +E  +F\r\n");
    code_memory_read(start_address, end_address);
    printf("\r\n-------------------------------------------------------------------\r\n");

    
}

void code_memory_read(unsigned int start_address, unsigned int end_address) {
    unsigned char count = 0;
    unsigned char __code *ptr;

    for (unsigned int addr = start_address; addr <= end_address; addr++) {
        ptr = (unsigned char __code *)addr;

        // Start a new row for every 16 addresses
        if (count % DIVIDE_BY_16 == 0) {
            putchar('\n');
            putchar('\r');

            print_hex_number(addr, 4);
            printf(": ");
        }

        // Print the code memory content
        print_hex_number(*ptr, 2);
        printf("  "); // Add spacing between numbers

        count++;
    }

    
       
        printf("\r\n");
    
}
    

void handle_command(void) {
    char cmd;
    printf("\r\nEnter Command (H for help):\r\n");
    cmd = getchar();
    putchar(cmd);
    printf("\033[2J\033[H");
    printf("\r\n");

    switch (cmd) {
        case 'R':
        case 'r':
            read_memory();
            break;
        case 'W':
        case 'w':
            write_memory();
            break;
        case 'C':
        case 'c':
            read_code_memory();
            break;
        case 'H':
        case 'h':
            display_help();
            break;
        case 'X':
        case 'x': {
            // printf("\033[2J\033[H"); // Clear screen and reset cursor position
            printf("\r\nExiting to 0x0000...\r\n");

            // Reset to 0x0000
            void (*reset_to_0x0000)(void) = (void (*)(void))0x0000;
            reset_to_0x0000();
            break;
        }
        default:
            printf("\r\n Invalid Command. Press 'H' for help.\r\n");
            break;
    }
    printf("\r\n************************************************************************\r\n");
}


void main(void) {
    AUXR = 0x02;
    uart_initialization();
    display_help();
    initialize_xram();

    while (1) {
        handle_command();
    }
}
