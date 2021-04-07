// BT - Bluetooth
// CXP - Caixa Preta
// 29/06/2020

// COM0 --> Arduino
// COM1 --> 
// COM2 --> Bluetooth
// COM3 --> GPS

// Imprimir LATITUDE
// Recebe no vet = ddmm.mmmmm
//           ns = N ou S
// Imprime ==> +/-dd.ddddddd
void bt_lat(byte *vet, byte ns){
  byte msg[24];
  str_lat(vet,ns,msg);
  bt_str(msg);
}

// Imprimir LONGITUDE
// Recebe no vet = dddmm.mmmmm
//           ew = E ou W
// Imprime ==> +/-ddd.ddddddd
void bt_long(byte *vet, byte ew){
  byte msg[24];
  str_long(vet,ew,msg);
  bt_str(msg);
}

// Imprimir GPS em uma só linha, dados no pacote gps_dados
// Apenas separa os campos por espaços
void bt_gps_dados_lin(char *gps_vt){
    bt_str(&gps_vt[GPS_STATUS]);   bt_spc(1);     //Status

    gps_vt[GPS_HORA+6]='\0';  //Remover fração de segundos
    bt_str(&gps_vt[GPS_HORA]);     bt_spc(1);     //Hora
    bt_str(&gps_vt[GPS_DATA]);     bt_spc(1);     //Data
    bt_str(&gps_vt[GPS_LAT]);      bt_spc(1);     //Lat
    bt_str(&gps_vt[GPS_NS]);       bt_spc(1);     //N-S
    bt_str(&gps_vt[GPS_LONG]);     bt_spc(1);     //Long
    bt_str(&gps_vt[GPS_EW]);       bt_spc(1);     //E-W
    bt_str(&gps_vt[GPS_VEL_KPH]);  bt_spc(1);     //Vel km/h
    bt_str(&gps_vt[GPS_VEL_UN]);   bt_spc(1);     //Unidade da velocidade
    bt_str(&gps_vt[GPS_VEL_NOS]);  bt_spc(1);     //Vel Nós
    bt_str(&gps_vt[GPS_CURSO]);    bt_spc(1);     //Curso
    bt_str(&gps_vt[GPS_ALT]);      bt_spc(1);     //Altitude
    bt_str(&gps_vt[GPS_ALT_UN]);   bt_spc(1);     //Unidade da Altitude
    bt_str(&gps_vt[GPS_QTD_SAT]);  bt_spc(1);     //Qtd de Satélites
    bt_str(&gps_vt[GPS_PDOP]);     bt_spc(1);     //PDOP
    bt_str(&gps_vt[GPS_HDOP]);     bt_spc(1);     //HDOP
    bt_str(&gps_vt[GPS_VDOP]);     bt_spc(1);     //VDOP
    bt_str(&gps_vt[GPS_ADR_SRAM]); bt_crlf(1);    //ADR SRAM-FLASH
}

// Imprimir GPS, dados no pacote gps_dados
void bt_gps_dados(char *gps_vt){
    bt_str("gps_dados: Status=");  bt_str(&gps_vt[GPS_STATUS]);
    bt_str(" Hora=");      bt_str(&gps_vt[GPS_HORA]);
    bt_str(" Data=");    bt_str(&gps_vt[GPS_DATA]);
    bt_str(" Lat=");     bt_str(&gps_vt[GPS_LAT]);
    bt_str(" N/S=");     bt_str(&gps_vt[GPS_NS]);
    bt_str(" Long=");    bt_str(&gps_vt[GPS_LONG]);
    bt_str(" E/W=");     bt_str(&gps_vt[GPS_EW]);
    bt_str(" Vel Nos="); bt_str(&gps_vt[GPS_VEL_NOS]);
    bt_str(" Curso=");   bt_str(&gps_vt[GPS_CURSO]);
    bt_str(" PDOP=");   bt_str(&gps_vt[GPS_PDOP]);
    bt_str(" HDOP=");   bt_str(&gps_vt[GPS_HDOP]);
    bt_str(" VDOP=");   bt_str(&gps_vt[GPS_VDOP]);
    bt_str(" Vel_kph=");   bt_str(&gps_vt[GPS_VEL_KPH]);
    bt_str(" Vel_uni=");   bt_str(&gps_vt[GPS_VEL_UN]);
    //bt_str(" Fix=");   bt_str(&gps_vt[GPS_FIX);        //Sem uso
    bt_str(" Qtd_Sat=");   bt_str(&gps_vt[GPS_QTD_SAT]);
    bt_str(" Alt=");   bt_str(&gps_vt[GPS_ALT]);
    bt_str(" Alt_uni=");   bt_str(&gps_vt[GPS_ALT_UN]);
    bt_str(" Adr_MPU=");   bt_str(&gps_vt[GPS_ADR_SRAM]);
}

// Compor e imprimir uma linha com acel, temp e giro
// Recebe vetor com bytes e monta words para imprimir
void bt_lin_ac_tp_gi(byte *vet){
  byte i;
  for (i=0; i<14; i+=2){
    bt_dec16( ( (word)vet[i]<<8) | vet[i+1]);
    bt_spc(1);
  }
  bt_crlf(1);
}

// Compor e imprimir uma linha com acel e giro
// Recebe vetor com bytes e monta words para imprimir
void bt_lin_ac_gi(byte *vet){
  byte i;
  for (i=0; i<12; i+=2){
    bt_dec16( ( (word)vet[i]<<8) | vet[i+1]);
    bt_spc(1);
  }
  bt_crlf(1);
}

// Compor e imprimir uma linha com cel, giro e mag
void bt_lin_ac_gi_mg(byte *vet){
  byte i;
  for (i=0; i<18; i+=2){
    bt_dec16u( ( (word)vet[i]<<8) | vet[i+1]);
    bt_spc(1);
  }
  bt_crlf(1);
}

// Compor acel, giro e mag e imprimir 1 por linha 
void bt_ac_gi_mg(byte *vet){
  byte i;
  for (i=0; i<18; i+=2){
    bt_dec16u( ( (word)vet[i]<<8) | vet[i+1]);
    bt_crlf(1);
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
void bt_float(float fx, byte prec){
  char msg[24];
  str_float(fx,prec,msg);
  bt_str(msg);
}

////////////////////////////////////////////////////
//////////////// 32 bits ///////////////////////////
////////////////////////////////////////////////////

// Escrever 32 bits em Decimal, com sinal e com zeros à esq
void bt_dec32(long dt){
  char msg[12];
  str_dec32(dt, msg);
  bt_str(msg);
}

// Escrever 32 bits em Decimal, sem sinal e com zeros à esq
void bt_dec32u(long dt){
  char msg[12];
  str_dec32u(dt, msg);
  bt_str(msg);
}

// Escrever 32 bits em Decimal, com sinal e sem zeros à esq
void bt_dec32nz(long dt){
  char msg[12];
  byte i;
  str_dec32(dt, msg);
  str_rmvz_s(msg);
  bt_str(msg);
}

// Escrever 32 bits em Decimal, sem sinal e sem zeros à esq
void bt_dec32unz(long dt){
  char msg[12];
  str_dec32u(dt, msg);
  str_rmvz_u(msg);
  bt_str(msg);
}

// Escrever word (32 bits) em Hexa na posição atual
void bt_hex32(long dt){
  char msg[9];
  str_hex32(dt, msg);
  bt_str(msg);
}

////////////////////////////////////////////////////
//////////////// 16 bits ///////////////////////////
////////////////////////////////////////////////////

// Escrever 16 bits em Decimal, com sinal e com zeros à esq
void bt_dec16(int dt){
  char msg[7];
  str_dec16(dt, msg);
  bt_str(msg);
}

// Escrever 16 bits em Decimal, sem sinal e com zeros à esq
void bt_dec16u(word dt){
  char msg[7];
  str_dec16u(dt, msg);
  bt_str(msg);
}

// Escrever 16 bits em Decimal, com sinal e sem zeros à esq
void bt_dec16nz(int dt){
  char msg[7];
  byte i;
  str_dec8(dt, msg);
  str_rmvz_s(msg);
  bt_str(msg);
}

// Escrever 16 bits em Decimal, sem sinal e sem zeros à esq
void bt_dec16unz(word dt){
  char msg[7];
  str_dec16u(dt, msg);
  str_rmvz_u(msg);
  bt_str(msg);
}

// Escrever word (16 bits) em Hexa na posição atual
void bt_hex16(word dt){
  char msg[5];
  str_hex16(dt, msg);
  bt_str(msg);
}

////////////////////////////////////////////////////
///////////////// 8 bits ///////////////////////////
////////////////////////////////////////////////////

// Escrever 8 bits em Decimal, com sinal e com zeros à esq
void bt_dec8(byte dt){
  char msg[5];
  str_dec8(dt, msg);
  bt_str(msg);
}

// Escrever 8 bits em Decimal, sem sinal e com zeros à esq
void bt_dec8u(byte dt){
  char msg[5];
  str_dec8u(dt, msg);
  bt_str(msg);
}

// Escrever 8 bits em Decimal, com sinal e sem zeros à esq
void bt_dec8nz(byte dt){
  char msg[5];
  byte i;
  str_dec8(dt, msg);
  str_rmvz_s(msg);
  bt_str(msg);
}

// Escrever 8 bits em Decimal, sem sinal e sem zeros à esq
void bt_dec8unz(byte dt){
  char msg[5];
  str_dec8u(dt, msg);
  str_rmvz_u(msg);
  bt_str(msg);
}

// Escrever byte em Hexa
void bt_hex8(byte dt){
  char msg[3];
  str_hex8(dt, msg);
  bt_str(msg);
}

// Imprimir qtd de pares CR e LF
void bt_crlf(byte qtd){
  int i;
  for (i=0; i<qtd; i++){
    bt_char('\r'); //0xD
    bt_char('\n'); //0xA
  }
}

// Imprimir qtd Carriage Return
void bt_cr(byte qtd){
  int i;
  for (i=0; i<qtd; i++)
    bt_char('\r'); //0xD
}

// Imprimir qtd Line Feed
void bt_lf(byte qtd){
  int i;
  for (i=0; i<qtd; i++)
    bt_char('\n'); //0xA
}

// Imprimir qtd brancos
void bt_spc(byte qtd){
  int i;
  for (i=0; i<qtd; i++)
    bt_char(' ');
}

// Imprimir string
void bt_str(byte *msg){
  int i=0;
  while( msg[i] != '\0')
    bt_char(msg[i++]);
}

// Imprimir um char, prende esperando o envio
void bt_char(byte dt){
  while (bt_tx_ok == FALSE);
  bt_tx_ok=FALSE;
  UDR2=dt;
}


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
    while (bt_tx_ok == FALSE);
    bt_tx_ok=FALSE;
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
    while (bt_tx_ok == FALSE);
    bt_tx_ok=FALSE;
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
