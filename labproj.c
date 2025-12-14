#include <lpc214x.h>

#define MOTOR_PIN (1 << 10)  // P0.10 for DC Motor
#define LED_PIN   (1 << 11)  // P0.11 for LED

void UART0_Init(void);
char UART0_ReadChar(void);
void UART0_WriteString(char *str);
void delay(void);

int main(void) {
    UART0_Init();                         // Initialize UART
    IO0DIR |= MOTOR_PIN | LED_PIN;       // Set P0.10 and P0.11 as output

    UART0_WriteString("UART Interface Ready\r\n");

    while (1) {
        char cmd = UART0_ReadChar();     // Read a command from UART

        switch (cmd) {
            case '1':
                IO0SET = MOTOR_PIN;
                UART0_WriteString("Motor ON\r\n");
                break;
            case '2':
                IO0CLR = MOTOR_PIN;
                UART0_WriteString("Motor OFF\r\n");
                break;
            case '3':
                IO0SET = LED_PIN;
                UART0_WriteString("LED ON\r\n");
                break;
            case '4':
                IO0CLR = LED_PIN;
                UART0_WriteString("LED OFF\r\n");
                break;
            default:
                UART0_WriteString("Invalid Command\r\n");
        }

        delay(); // Small delay
    }
}

// UART0 Initialization: 9600 baud, 15MHz PCLK
void UART0_Init(void) {
    PINSEL0 |= 0x00000005;  // Enable TxD0 (P0.0) and RxD0 (P0.1)
    U0LCR = 0x83;           // Enable DLAB, 8-bit word length
    U0DLL = 0x6E;           // Baud rate = 9600 @ 15MHz PCLK
    U0DLM = 0x00;
    U0LCR = 0x03;           // Disable DLAB
}

// Read one character from UART0
char UART0_ReadChar(void) {
    while (!(U0LSR & 0x01)); // Wait until data is received
    return U0RBR;
}

// Send a string over UART0
void UART0_WriteString(char *str) {
    while (*str) {
        while (!(U0LSR & 0x20)); // Wait until THR is empty
        U0THR = *str++;
    }
}

// Simple delay function
void delay(void) {
    int i;
    for (i = 0; i < 10000; i++);
}
