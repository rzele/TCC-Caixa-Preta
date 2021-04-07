// Serial - Porta Serial 0
// CXP - Caixa Preta
// 10/01/2019

// COM0 --> Arduino
// COM1 --> 
// COM2 --> Bluetooth
// COM3 --> GPS

// Auxiliar no DUMP de memórias
void ser_dump_memo(long adr, byte *vet){
  word i;
  ser_hex16(adr>>16);
  ser_spc(1);
  ser_hex16(adr&0xFFFF);
  ser_str(": ");
  for (i=0; i<16; i++){
    ser_hex8(vet[i]);
    ser_spc(1);
  }
  ser_str("- ");
  
  for (i=0; i<16; i++){
    if (vet[i]>0x1F)  ser_char(vet[i]);
    else              ser_char('.');
  }
  ser_crlf(1);
}

// Imprimir GPS em uma só linha, dados no pacote gps_dados
// Apenas separa os campos por espaços
void ser_gps_dados_lin(char *gps_vt){
    ser_str(&gps_vt[GPS_STATUS]);   ser_spc(1);     //Status

    /*
    ser_char(gps_vt[GPS_DATA+0]);  ser_char(gps_vt[GPS_DATA+1]);    ser_char('/');  //Data
    ser_char(gps_vt[GPS_DATA+2]);  ser_char(gps_vt[GPS_DATA+3]);    ser_char('/');
    ser_char(gps_vt[GPS_DATA+4]);  ser_char(gps_vt[GPS_DATA+5]);    ser_spc(1);

    ser_char(gps_vt[GPS_HORA+0]);  ser_char(gps_vt[GPS_HORA+1]);    ser_char(':');  //Hora
    ser_char(gps_vt[GPS_HORA+2]);  ser_char(gps_vt[GPS_HORA+3]);    ser_char(':');
    ser_char(gps_vt[GPS_HORA+4]);  ser_char(gps_vt[GPS_HORA+5]);    ser_spc(1);
    */

    gps_vt[GPS_HORA+6]='\0';  //Remover fração de segundos
    ser_str(&gps_vt[GPS_HORA]);     ser_spc(1);     //Hora
    ser_str(&gps_vt[GPS_DATA]);     ser_spc(1);     //Data
    ser_str(&gps_vt[GPS_LAT]);      ser_spc(1);     //Lat
    ser_str(&gps_vt[GPS_NS]);       ser_spc(1);     //N-S
    ser_str(&gps_vt[GPS_LONG]);     ser_spc(1);     //Long
    ser_str(&gps_vt[GPS_EW]);       ser_spc(1);     //E-W
    ser_str(&gps_vt[GPS_VEL_KPH]);  ser_spc(1);     //Vel km/h
    ser_str(&gps_vt[GPS_VEL_UN]);   ser_spc(1);     //Unidade da velocidade
    ser_str(&gps_vt[GPS_VEL_NOS]);  ser_spc(1);     //Vel Nós
    ser_str(&gps_vt[GPS_CURSO]);    ser_spc(1);     //Curso
    ser_str(&gps_vt[GPS_ALT]);      ser_spc(1);     //Altitude
    ser_str(&gps_vt[GPS_ALT_UN]);   ser_spc(1);     //Unidade da Altitude
    ser_str(&gps_vt[GPS_QTD_SAT]);  ser_spc(1);     //Qtd de Satélites
    ser_str(&gps_vt[GPS_PDOP]);     ser_spc(1);     //PDOP
    ser_str(&gps_vt[GPS_HDOP]);     ser_spc(1);     //HDOP
    ser_str(&gps_vt[GPS_VDOP]);     ser_spc(1);     //VDOP
    ser_str(&gps_vt[GPS_ADR_SRAM]); ser_crlf(1);    //ADR SRAM-FLASH
}


// Imprimir GPS, dados no pacote gps_dados
void ser_gps_dados(char *gps_vt){
    ser_str("gps_dados: Status=");  ser_str(&gps_vt[GPS_STATUS]);
    ser_str(" Hora=");      ser_str(&gps_vt[GPS_HORA]);
    ser_str(" Data=");    ser_str(&gps_vt[GPS_DATA]);
    ser_str(" Lat=");     ser_str(&gps_vt[GPS_LAT]);
    ser_str(" N/S=");     ser_str(&gps_vt[GPS_NS]);
    ser_str(" Long=");    ser_str(&gps_vt[GPS_LONG]);
    ser_str(" E/W=");     ser_str(&gps_vt[GPS_EW]);
    ser_str(" Vel Nos="); ser_str(&gps_vt[GPS_VEL_NOS]);
    ser_str(" Curso=");   ser_str(&gps_vt[GPS_CURSO]);
    ser_str(" PDOP=");   ser_str(&gps_vt[GPS_PDOP]);
    ser_str(" HDOP=");   ser_str(&gps_vt[GPS_HDOP]);
    ser_str(" VDOP=");   ser_str(&gps_vt[GPS_VDOP]);
    ser_str(" Vel_kph=");   ser_str(&gps_vt[GPS_VEL_KPH]);
    ser_str(" Vel_uni=");   ser_str(&gps_vt[GPS_VEL_UN]);
    //ser_str(" Fix=");   ser_str(&gps_vt[GPS_FIX);        //Sem uso
    ser_str(" Qtd_Sat=");   ser_str(&gps_vt[GPS_QTD_SAT]);
    ser_str(" Alt=");   ser_str(&gps_vt[GPS_ALT]);
    ser_str(" Alt_uni=");   ser_str(&gps_vt[GPS_ALT_UN]);
    ser_str(" Adr_MPU=");   ser_str(&gps_vt[GPS_ADR_SRAM]);
}



// Compor e imprimir uma linha com acel, temp e giro
// Recebe vetor com bytes e monta words para imprimir
void ser_lin_ac_tp_gi(byte *vet){
  byte i;
  for (i=0; i<14; i+=2){
    ser_dec16( ( (word)vet[i]<<8) | vet[i+1]);
    ser_spc(1);
  }
  ser_crlf(1);
}

// Compor e imprimir uma linha com acel e giro
// Recebe vetor com bytes e monta words para imprimir
void ser_lin_ac_gi(byte *vet){
  byte i;
  for (i=0; i<12; i+=2){
    ser_dec16( ( (word)vet[i]<<8) | vet[i+1]);
    ser_spc(1);
  }
  ser_crlf(1);
}

// Compor e imprimir uma linha com cel, giro e mag
void ser_lin_ac_gi_mg(byte *vet){
  byte i;
  for (i=0; i<18; i+=2){
    ser_dec16u( ( (word)vet[i]<<8) | vet[i+1]);
    ser_spc(1);
  }
  ser_crlf(1);
}

// Compor acel, giro e mag e imprimir 1 por linha 
void ser_ac_gi_mg(byte *vet){
  byte i;
  for (i=0; i<18; i+=2){
    ser_dec16u( ( (word)vet[i]<<8) | vet[i+1]);
    ser_crlf(1);
  }
}

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
