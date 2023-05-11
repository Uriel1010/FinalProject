#ifndef TFC_UART_H_
#define TFC_UART_H_

#define SDA_SERIAL_BAUD1 2400
#define SDA_SERIAL_BAUD2 9600
#define SDA_SERIAL_BAUD3 19200
#define SDA_SERIAL_BAUD4 38400
#define CORE_CLOCK 48000000

void Uart0_Br_Sbr(int sysclk, int baud);
void InitUARTs();
char uart_getchar (UART_MemMapPtr channel);
void uart_putchar (UART_MemMapPtr channel, char ch);
void UARTprintf(UART_MemMapPtr channel,char* str);
int uart_getchar_present(UART_MemMapPtr channel);
#endif /* TFC_UART_H_ */
