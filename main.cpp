#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include "UART.h"
#include "GPIO.h"
#include "Timer.h"
#include "FIFO.h"

UART uart(57600,UART::DATABITS_8,UART::PARITY_NONE,UART::STOPBITS_1);

GPIO led(11,GPIO::OUTPUT);
GPIO botao(12,GPIO::INPUT);

Timer timer(1000);
char message[8];

void acende_led(){
	//PORTB = PORTB | led_mask;
	led.set(true);
}

void apaga_led(){
	//PORTB = PORTB & ~led_mask;
	led.set(false);
}

bool ler_botao(){
	//return (PINB & botao_mask);
	return botao.get();
}

void setup(){
	sei();
}

void loop(){
	//uart.put('a');
	uart.put(uart.get() + 1);

//	//if((PINB & (1<<PB4))) acende_led();
//	if(ler_botao()){
//		//printf("Acendendo led");
//		acende_led();
//	}
//	else{
//		//printf("Apagando led");
//		apaga_led();
//	}
//
//
//	//sprintf(message, "LED: %d\n",botao.get());
//	//uart.puts(message);
//	//_delay_ms(100);
////
//	timer.delay(1000);
//	sprintf(message, "Timer: %d\n",timer.millis());
//	//sprintf(message, "Timer: %ull\n",timer.micros());
//	uart.puts(message);
//	//_delay_ms(100);

}

static const int MAX = 4;
typedef FIFO<MAX,char> MyFIFO;
MyFIFO fifo_test;

void teste_fila(){
	char x = 0, y = 0;

	uart.puts("Test 1: insert/remove 1.\r\n");
	//fifo_test.clear();

	uart.puts("\t Push A. ");
	fifo_test.push('A');
	uart.puts(" => OK.\r\n");

	uart.puts("\t Pop: ");
	x = fifo_test.pop();
	uart.put(x);
	if(x == 'A') uart.puts(". => OK.\r\n");
	else uart.puts(". => NOK.\r\n");


	uart.puts("Test 2: insert/remove MAX.\r\n");
	//fifo_test.clear();

	x = 'A';
	for (int i = 0; i < MAX; i++) {
		uart.puts("\t Push ");
		uart.put(x);
		uart.puts(". ");
		fifo_test.push(x);
		uart.puts(" => OK.\r\n");
		x++;
	}

	y = 'A';
	for (int i = 0; i < MAX; i++) {
		uart.puts("\t Pop: ");
		x = fifo_test.pop();
		uart.put(x);
		if(x == y) uart.puts(". => OK.\r\n");
		else uart.puts(". => NOK.\r\n");
		y++;
	}


	uart.puts("Test 3: remove from empty.\r\n");
	//fifo_test.clear();

	for (int i = 0; i < MAX; i++) {
		uart.puts("\t Pop: ");
		x = fifo_test.pop();
		if(x == (char)MyFIFO::FIFO_ERROR_EMPTY) uart.puts(". => OK.\r\n");
		else uart.puts(". => NOK.\r\n");
	}

	uart.puts("Test 4: insert in full.\r\n");
	//fifo_test.clear();

	x = 'A';
	for (int i = 0; i < MAX; i++) {
		uart.puts("\t Push ");
		uart.put(x);
		uart.puts(". ");
		fifo_test.push(x);
		uart.puts(" => OK.\r\n");
		x++;
	}

	uart.puts("\t Add extra item: ");
	x = fifo_test.push('X');
	if(x == (char)MyFIFO::FIFO_ERROR_FULL) uart.puts(". => OK.\r\n");
	else uart.puts(". => NOK.\r\n");

	y = 'A';
	for (int i = 0; i < MAX; i++) {
		uart.puts("\t Pop: ");
		x = fifo_test.pop();
		uart.put(x);
		if(x == y) uart.puts(". => OK.\r\n");
		else uart.puts(". => NOK.\r\n");
		y++;
	}

	timer.delay(10000);
}

int main(){
	setup();

	while(true){
		loop();
		//teste_fila();
	}
}



