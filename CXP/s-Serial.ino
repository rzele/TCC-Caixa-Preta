// Serial - Porta Serial 0
// CXP - Caixa Preta
// 10/01/2019


////////////////////////////////////////////////////
/////////// Não usam Fila: ser_xxx /////////////////
////////////////////////////////////////////////////

////////////////////////////////////////////////////
/////////// float (double) /////////////////////////
////////////////////////////////////////////////////

// Imprimir float = + xxx xxx xxx , ddd ddd ddd ddd (24 posições)
//  9 posições = limite da parte inteira
// 12 posições = limite da parte fracionária
// Caso ultrapasse os limites imprime ### , ###
// No Arduino, double e float têm a mesma precisão
void ser_float(float fx, byte prec){
  char msg[24];
  str_float(fx,prec,msg);
  ser_str(msg);
}

////////////////////////////////////////////////////
//////////////// 32 bits ///////////////////////////
////////////////////////////////////////////////////

// Escrever 32 bits em Decimal, com sinal e com zeros à esq
void ser_dec32(long dt){
  char msg[12];
  str_dec32(dt, msg);
  ser_str(msg);
}

// Escrever 32 bits em Decimal, sem sinal e com zeros à esq
void ser_dec32u(long dt){
  char msg[12];
  str_dec32u(dt, msg);
  ser_str(msg);
}

// Escrever 32 bits em Decimal, com sinal e sem zeros à esq
void ser_dec32nz(long dt){
  char msg[12];
  byte i;
  str_dec32(dt, msg);
  str_rmvz_s(msg);
  ser_str(msg);
}

// Escrever 32 bits em Decimal, sem sinal e sem zeros à esq
void ser_dec32unz(long dt){
  char msg[12];
  str_dec32u(dt, msg);
  str_rmvz_u(msg);
  ser_str(msg);
}

// Escrever word (32 bits) em Hexa na posição atual
void ser_hex32(long dt){
  char msg[9];
  str_hex32(dt, msg);
  ser_str(msg);
}
////////////////////////////////////////////////////
//////////////// 16 bits ///////////////////////////
////////////////////////////////////////////////////

// Escrever 16 bits em Decimal, com sinal e com zeros à esq
void ser_dec16(int dt){
  char msg[7];
  str_dec16(dt, msg);
  ser_str(msg);
}

// Escrever 16 bits em Decimal, sem sinal e com zeros à esq
void ser_dec16u(word dt){
  char msg[7];
  str_dec16u(dt, msg);
  ser_str(msg);
}

// Escrever 16 bits em Decimal, com sinal e sem zeros à esq
void ser_dec16nz(int dt){
  char msg[7];
  byte i;
  str_dec8(dt, msg);
  str_rmvz_s(msg);
  ser_str(msg);
}

// Escrever 16 bits em Decimal, sem sinal e sem zeros à esq
void ser_dec16unz(word dt){
  char msg[7];
  str_dec16u(dt, msg);
  str_rmvz_u(msg);
  ser_str(msg);
}

// Escrever word (16 bits) em Hexa na posição atual
void ser_hex16(word dt){
  char msg[5];
  str_hex16(dt, msg);
  ser_str(msg);
}

////////////////////////////////////////////////////
///////////////// 8 bits ///////////////////////////
////////////////////////////////////////////////////

// Escrever 8 bits em Decimal, com sinal e com zeros à esq
void ser_dec8(byte dt){
  char msg[5];
  str_dec8(dt, msg);
  ser_str(msg);
}

// Escrever 8 bits em Decimal, sem sinal e com zeros à esq
void ser_dec8u(byte dt){
  char msg[5];
  str_dec8u(dt, msg);
  ser_str(msg);
}

// Escrever 8 bits em Decimal, com sinal e sem zeros à esq
void ser_dec8nz(byte dt){
  char msg[5];
  byte i;
  str_dec8(dt, msg);
  str_rmvz_s(msg);
  ser_str(msg);
}

// Escrever 8 bits em Decimal, sem sinal e sem zeros à esq
void ser_dec8unz(byte dt){
  char msg[5];
  str_dec8u(dt, msg);
  str_rmvz_u(msg);
  ser_str(msg);
}

// Escrever byte em Hexa
void ser_hex8(byte dt){
  char msg[3];
  str_hex8(dt, msg);
  ser_str(msg);
}

// Imprimir qtd de pares CR e LF
void ser_crlf(byte qtd){
  int i;
  for (i=0; i<qtd; i++){
    ser_char('\r'); //0xD
    ser_char('\n'); //0xA
  }
}

// Imprimir qtd Carriage Return
void ser_cr(byte qtd){
  int i;
  for (i=0; i<qtd; i++)
    ser_char('\r'); //0xD
}

// Imprimir qtd Line Feed
void ser_lf(byte qtd){
  int i;
  for (i=0; i<qtd; i++)
    ser_char('\n'); //0xA
}

// Imprimir qtd brancos
void ser_spc(byte qtd){
  int i;
  for (i=0; i<qtd; i++)
    ser_char(' ');
}

// Imprimir string
void ser_str(byte *msg){
  int i=0;
  while( msg[i] != '\0')
    ser_char(msg[i++]);
}

// Imprimir um char, prende esperando o envio
void ser_char(byte dt){
  while (ser_tx_ok == FALSE);
  ser_tx_ok=FALSE;
  UDR0=dt;
}


////////////////////////////////////////////////////
////////////// Usam Fila: serb_xxx /////////////////
////////////////////////////////////////////////////

////////////////////////////////////////////////////
/////////// float (double) /////////////////////////
////////////////////////////////////////////////////

// Imprimir float = + xxx xxx xxx , ddd ddd ddd ddd (24 posições)
//  9 posições = limite da parte inteira
// 12 posições = limite da parte fracionária
// Caso ultrapasse os limites imprime ### , ###
// No Arduino, double e float têm a mesma precisão
void serb_float(float fx, byte prec){
  char msg[24];
  str_float(fx,prec,msg);
  serb_str(msg);
}

////////////////////////////////////////////////////
//////////////// 32 bits ///////////////////////////
////////////////////////////////////////////////////

// Escrever 32 bits em Decimal, com sinal e com zeros à esq
void serb_dec32(long dt){
  char msg[12];
  str_dec32(dt, msg);
  serb_str(msg);
}

// Escrever 32 bits em Decimal, sem sinal e com zeros à esq
void serb_dec32u(long dt){
  char msg[12];
  str_dec32u(dt, msg);
  serb_str(msg);
}

// Escrever 32 bits em Decimal, com sinal e sem zeros à esq
void serb_dec32nz(long dt){
  char msg[12];
  byte i;
  str_dec32(dt, msg);
  str_rmvz_s(msg);
  serb_str(msg);
}

// Escrever 32 bits em Decimal, sem sinal e sem zeros à esq
void serb_dec32unz(long dt){
  char msg[12];
  str_dec32u(dt, msg);
  str_rmvz_u(msg);
  serb_str(msg);
}

// Escrever word (32 bits) em Hexa na posição atual
void serb_hex32(long dt){
  char msg[9];
  str_hex32(dt, msg);
  serb_str(msg);
}
////////////////////////////////////////////////////
//////////////// 16 bits ///////////////////////////
////////////////////////////////////////////////////

// Escrever 16 bits em Decimal, com sinal e com zeros à esq
void serb_dec16(int dt){
  char msg[7];
  str_dec16(dt, msg);
  serb_str(msg);
}

// Escrever 16 bits em Decimal, sem sinal e com zeros à esq
void serb_dec16u(word dt){
  char msg[7];
  str_dec16u(dt, msg);
  serb_str(msg);
}

// Escrever 16 bits em Decimal, com sinal e sem zeros à esq
void serb_dec16nz(int dt){
  char msg[7];
  byte i;
  str_dec8(dt, msg);
  str_rmvz_s(msg);
  serb_str(msg);
}

// Escrever 16 bits em Decimal, sem sinal e sem zeros à esq
void serb_dec16unz(word dt){
  char msg[7];
  str_dec16u(dt, msg);
  str_rmvz_u(msg);
  serb_str(msg);
}

// Escrever word (16 bits) em Hexa na posição atual
void serb_hex16(word dt){
  char msg[5];
  str_hex16(dt, msg);
  serb_str(msg);
}

////////////////////////////////////////////////////
///////////////// 8 bits ///////////////////////////
////////////////////////////////////////////////////

// Escrever 8 bits em Decimal, com sinal e com zeros à esq
void serb_dec8(byte dt){
  char msg[5];
  str_dec8(dt, msg);
  serb_str(msg);
}

// Escrever 8 bits em Decimal, sem sinal e com zeros à esq
void serb_dec8u(byte dt){
  char msg[5];
  str_dec8u(dt, msg);
  serb_str(msg);
}

// Escrever 8 bits em Decimal, com sinal e sem zeros à esq
void serb_dec8nz(byte dt){
  char msg[5];
  byte i;
  str_dec8(dt, msg);
  str_rmvz_s(msg);
  serb_str(msg);
}

// Escrever 8 bits em Decimal, sem sinal e sem zeros à esq
void serb_dec8unz(byte dt){
  char msg[5];
  str_dec8u(dt, msg);
  str_rmvz_u(msg);
  serb_str(msg);
}

// Escrever byte em Hexa na posição atual
void serb_hex8(byte dt){
  char msg[3];
  str_hex8(dt, msg);
  serb_str(msg);
}

// Colocar na fila qtd de pares CR e LF
void serb_crlf(byte qtd){
  int i;
  for (i=0; i<qtd; i++)
    serb_char('\r'); //0xD
    serb_char('\n'); //0xA
}

// Colocar na fila qtd Carriage Return
void serb_cr(byte qtd){
  int i;
  for (i=0; i<qtd; i++)
    serb_char('\r'); //0xD
}

// Colocar na fila qtd Line Feed
void serb_lf(byte qtd){
  int i;
  for (i=0; i<qtd; i++)
    serb_char('\n'); //0xA
}


// Colocar na fila qtd brancos
void serb_spc(byte qtd){
  int i;
  for (i=0; i<qtd; i++)
    serb_char(' ');
}

// Colocar string no buffer serial
void serb_str(byte *msg){
  int i=0;
  while( msg[i] != '\0')
    serb_char(msg[i++]);
}

// Colocar um char no buffer serial
// Gera alerta se fila serial encher
void serb_char(byte dt){
  if ( (ser_tx_poe(dt) == FALSE))
    lcd_str(0,0,"Fila Serial TX Cheia"); //Mensagem de erro no LCD
}

// Fica preso até esvaziar toda a fila serial
void serb_esvazia_tx(void){
  byte dado;
  while ( ser_tx_tira(&dado) == TRUE){
    while (ser_tx_ok == FALSE);
    ser_tx_ok=FALSE;
    UDR0=dado;
  }
}

////////////////////////////////////////////////////
////////////////// Fila TX /////////////////////////
////////////////////////////////////////////////////

// Inicializar fila TX serial
void ser_tx_fila_config(void){
  ser_tx_pin=1;
  ser_tx_pout=0;
}

// Colocar um byte na fila TX
char ser_tx_poe(char cha){
  if (ser_tx_pin == ser_tx_pout){
    ser_fila_tx_cheia();  //Msg de fila cheia
    return FALSE;         // fila cheia
  }
  else{
      ser_tx_fila[ser_tx_pin++]=cha;
      if (ser_tx_pin==SER_TX_FILA_TAM)
          ser_tx_pin=0;
      return TRUE;
  }
}

// Mensagem de Fila TX Cheia
void ser_fila_tx_cheia(void){
  char msg[]="\nERRO: Fila Serial TX Cheia!\n";
  byte i=0;
  while(msg[i]!='\0'){
    UDR0=msg[i++];
    while (ser_tx_ok == FALSE);
    ser_tx_ok=FALSE;
  }
}

// Tirar um byte da fila TX
char ser_tx_tira(char *cha){
  char pout_aux;
  pout_aux=ser_tx_pout+1;
  if (pout_aux==SER_TX_FILA_TAM)    pout_aux=0;
  if (pout_aux == ser_tx_pin)       return(FALSE);
  else{
      *cha=ser_tx_fila[pout_aux];
      ser_tx_pout=pout_aux;
      return TRUE;
  }
}

////////////////////////////////////////////////////
////////////////// Fila RX /////////////////////////
////////////////////////////////////////////////////

// Inicializar fila RX serial
void ser_rx_fila_config(void){
  ser_rx_pin=1;
  ser_rx_pout=0;
}

// Colocar um byte na fila RX
char ser_rx_poe(char cha){
  if (ser_rx_pin == ser_rx_pout){
    ser_fila_rx_cheia();  //Msg de fila cheia
    return FALSE;         // fila cheia
  }
  else{
      ser_rx_fila[ser_rx_pin++]=cha;
      if (ser_rx_pin==SER_RX_FILA_TAM)
          ser_rx_pin=0;
      return TRUE;
  }
}

// Mensagem de Fila RX Cheia
void ser_fila_rx_cheia(void){
  char msg[]="\nERRO: Fila Serial RX Cheia!\n";
  byte i=0;
  while(msg[i]!='\0'){
    UDR0=msg[i++];
    while (ser_tx_ok == FALSE);
    ser_tx_ok=FALSE;
  }
}

// Tirar um byte da fila RX
char ser_rx_tira(char *cha){
  char pout_aux;
  pout_aux=ser_rx_pout+1;
  if (pout_aux==SER_RX_FILA_TAM)    pout_aux=0;
  if (pout_aux == ser_rx_pin)       return(FALSE);
  else{
      *cha=ser_rx_fila[pout_aux];
      ser_rx_pout=pout_aux;
      return TRUE;
  }
}

////////////////////////////////////////////////////
/////////////// Configuração ///////////////////////
////////////////////////////////////////////////////

// Configurar porta serial
// Habilitar interrupção por transmissão
void ser_config(long br){
  word dv;
  dv=(2000000L/br)-1;
  //lcd_cursor(0);
  //lcd_dec32unz(br);
  //lcd_spc(1);
  //lcd_dec8unz(dv);
  //lcd_spc(1);

  UBRR0H = dv>>8;     //MSB
  UBRR0L = dv&0xFF;   //LSB
  // Configurar 8 bits, sem paridade, 1 stop
  UCSR0A = (1<<TXC0) | (1<<U2X0);
  UCSR0B = (1<<RXCIE0) | (1<<TXCIE0) | (1<<RXEN0) | (1<<TXEN0);  //RXIE=TXIE=1, hab TX e RX
  UCSR0C = (1<<UCSZ01)| (1<<UCSZ00);
  ser_tx_ok=TRUE;        //Já pode começar a transmitir
}

// TX: Interrupção por dado enviado
ISR(USART0_TX_vect){
  ser_tx_ok=TRUE;  //Marcar que já transmitiu
}


// RX: Interrupção por dado recebido
ISR(USART0_RX_vect){
  ser_rx_poe(UDR0); //Por na fila
  ser_rx_ok=TRUE;   //Marcar que recebeu
}
