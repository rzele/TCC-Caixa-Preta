// BT - Bluetooth
// CXP - Caixa Preta
// 28/05/2020


////////////////////////////////////////////////////
////////////////// Fila BT TX //////////////////////
////// Transmitir para o Bluetooth - Serial 2 //////
////////////////////////////////////////////////////

// Inicializar fila BT TX serial
void bt_tx_fila_config(void){
  bt_tx_pin=1;
  bt_tx_pout=0;
}

// Colocar um byte na fila TX
char bt_tx_poe(char cha){
  if (bt_tx_pin == bt_tx_pout){
    bt_fila_tx_cheia();   //Msg de fila cheia
    return FALSE;         // fila cheia
  }
  else{
      bt_tx_fila[bt_tx_pin++]=cha;
      if (bt_tx_pin==BT_TX_FILA_TAM)
          bt_tx_pin=0;
      return TRUE;
  }
}

// Mensagem de Fila BT Cheia
void bt_fila_tx_cheia(void){
  char msg[]="\nERRO: Fila BT TX Cheia!\n";
  byte i=0;
  while(msg[i]!='\0'){
    UDR0=msg[i++];
    while (ser_tx_ok == FALSE);
    ser_tx_ok=FALSE;
  }
}

// Tirar um byte da fila BT TX
char bt_tx_tira(char *cha){
  char pout_aux;
  pout_aux=bt_tx_pout+1;
  if (pout_aux==BT_TX_FILA_TAM)    pout_aux=0;
  if (pout_aux == bt_tx_pin)       return(FALSE);
  else{
      *cha=bt_tx_fila[pout_aux];
      bt_tx_pout=pout_aux;
      return TRUE;
  }
}

////////////////////////////////////////////////////
////////////////// Fila BT RX //////////////////////
//////// Receber do Bluetooth - Serial 2 ///////////
////////////////////////////////////////////////////

// Inicializar fila BT RX serial
void bt_rx_fila_config(void){
  bt_rx_pin=1;
  bt_rx_pout=0;
}

// Colocar um byte na fila RX
char bt_rx_poe(char cha){
  if (bt_rx_pin == bt_rx_pout){
    bt_fila_rx_cheia();   //Msg de fila cheia
    return FALSE;         // fila cheia
  }
  else{
      bt_rx_fila[bt_rx_pin++]=cha;
      if (bt_rx_pin==BT_RX_FILA_TAM)
          bt_rx_pin=0;
      return TRUE;
  }
}

// Mensagem de Fila BT Cheia
void bt_fila_rx_cheia(void){
  char msg[]="\nERRO: Fila BT RX Cheia!\n";
  byte i=0;
  while(msg[i]!='\0'){
    UDR0=msg[i++];
    while (ser_tx_ok == FALSE);
    ser_tx_ok=FALSE;
  }
}

// Tirar um byte da fila BT RX
char bt_rx_tira(char *cha){
  char pout_aux;
  pout_aux=bt_rx_pout+1;
  if (pout_aux==BT_RX_FILA_TAM)    pout_aux=0;
  if (pout_aux == bt_rx_pin)       return(FALSE);
  else{
      *cha=bt_rx_fila[pout_aux];
      bt_rx_pout=pout_aux;
      return TRUE;
  }
}


// Habilitar ou Desabilitar Interrupção TX e RX pelo Bluetooth
void bt_tx_int_EN(void){ UCSR2B |=  (1<<TXCIE2);}   //Hab Interrup TX
void bt_tx_int_en(void){ UCSR2B &= ~(1<<TXCIE2);}   //Desab Interrup TX
void bt_rx_int_EN(void){ UCSR2B |=  (1<<RXCIE2);}   //Hab Interrup RX
void bt_rx_int_en(void){ UCSR2B &= ~(1<<RXCIE2);}   //Desab Interrup RX

// Habilitar ou Desabilitar TX e RX pelo Bluetooth
void bt_tx_EN(void){ UCSR2B |= (1<<TXEN2);}    //Hab TX
void bt_tx_en(void){ UCSR2B &= ~(1<<TXEN2);}   //Desab TX
void bt_rx_EN(void){ UCSR2B |= (1<<RXEN2);}    //Hab RX
void bt_rx_en(void){ UCSR2B &= ~(1<<RXEN2);}   //Desab RX

// BT RX: Interrupção por dado recebido
ISR(USART2_RX_vect){
  bt_rx_poe(UDR2); //Por na fila
  bt_rx_ok=TRUE;
}

// BT TX: Interrupção por dado transmitido
ISR(USART2_TX_vect){
  bt_tx_ok=TRUE;
}

// Bluettoth - Configurar porta serial 2
// Não habilita TX e nem RX
// Não habilita interrupções
void bt_config(long br){
  word dv;
  dv=(2000000L/br)-1;
  //ser_dec16(dv);
  //ser_crlf(1);
  UBRR2H = dv>>8;     //MSB
  UBRR2L = dv&0xFF;   //LSB
  // Configurar 8 bits, sem paridade, 1 stop
  UCSR2A = (1<<TXC2) | (1<<U2X2);
  UCSR2B = 0;     //desab RX e TX
//  UCSR2B = (1<<RXCIE2) | (1<<TXCIE2) | (1<<RXEN2) | (1<<TXEN2);  //RXIE=TXIE=1, hab TX e RX
//  UCSR2B = (1<<RXCIE2) | (1<<TXCIE2) | (1<<TXEN2);               //RXIE=TXIE=1, hab TX e não RX
  UCSR2C = (1<<UCSZ21)| (1<<UCSZ20);
  bt_tx_ok=TRUE;        //Já pode começar a transmitir
}
