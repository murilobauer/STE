/*
 * UART.cpp
 *
 *  Created on: 15 de mar de 2017
 *      Author: aluno
 */

#include "UART.h"
#include <avr/io.h>
#include <avr/interrupt.h>

UART * UART::__singletonUART  = 0;

UART::UART(unsigned long bd, DataBits_t db, ParityBits_t pr, StopBits_t sb)
: _baudrate(bd), _databits(db), _paridade(pr), _stopbits(sb)
{
	__singletonUART = this;

	//Configura o baud_rate
	UBRR0 = (F_CPU / (16ul * this->_baudrate)) -1;
	//Liga TX e RX
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);

	//Set databits
	if(_databits == DATABITS_9){
		//UCSR0B = (UCSR0B & ~(1 << UCSZ02)) | (_databits << UCSZ02);
	}else{
		UCSR0C = (UCSR0C & ~(3 << UCSZ00)) | (_databits << UCSZ00);
	}


	//Set parity
	UCSR0C = (UCSR0C & ~(3 << UPM00)) | (_paridade << UPM00);

	//Set stopbits
	//unsigned char reg = UCSR0C; //read
	//reg = (reg & ~(1 << USBS0)) | (_stopbits << USBS0); //modify
	//UCSR0C = reg; //update
	UCSR0C = (UCSR0C & ~(1 << USBS0)) | (_stopbits << USBS0);

	//Configura o frame para 8N1
	//UCSR0C = (3<<UCSZ00);

//-------------------------------------------------------------------------------------------
	 UCSR0B |= 0X80; //Liga interrupcao de RX

}

//UART::~UART() {
	// TODO Auto-generated destructor stub
//}

void UART::put(unsigned char dado){
	//wait until UDR0 is empty
//	while(!(UCSR0A & (1<<UDRE0)));
	//Envia o dado
//	UDR0 = dado;

	self()->_tx_fifo.push(dado);
	 UCSR0B |= 0X20; //Liga interrupcao de TX
}

unsigned char UART::get(){
	//wait until dado is received
	//while(!(UCSR0A & (1<<RXC0)));
	//Retorna dado
	//return UDR0;
	while(self()->_rx_fifo.size() == 0);
	return self()->_rx_fifo.pop();
}

void  UART::puts(const char * str){
	while(*str != '\0'){
		this->put(*str);
		str++;
	}
}

void UART::isr_rx_handler(){
	self()->_rx_fifo.push(UDR0);
}

void UART::isr_tx_handler(){
	if(self()->_tx_fifo.size()!=0){
		UDR0 = self()->_tx_fifo.pop();
	}
	else {
		 UCSR0B &= ~0X20; //Desliga interrupcao de TX
	}
}

//ISR(USART_RX_vect){
//	UART::isr_rx_handler();
//}
//
//ISR(USART_UDRE_vect){
//	UART::isr_tx_handler();
//}

#ifdef __AVR_ATmega2560__
ISR(USART0_RX_vect)
#else
ISR(USART_RX_vect)
#endif
{
	UART::isr_rx_handler();
}

#ifdef __AVR_ATmega2560__
ISR(USART0_UDRE_vect)
#else
ISR(USART_UDRE_vect)
#endif
{
	UART::isr_tx_handler();
}

