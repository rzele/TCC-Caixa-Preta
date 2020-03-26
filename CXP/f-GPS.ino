// GPS - Rotinas para acesso
// CXP - Caixa Preta
// 22/01/2019


// GP RMC - Recbe uma string, se for RMC, a imprime
// Retorna TRUE se era RMC (imprime)
// Retorna FALSE se não era (nada imprime);
// Seq Vazia = $GPRMC,,V,,,,,,,,,,N*53
//             01234567890123456789012
byte gps_rmc_ser(char *vt){
  char aux[]="$GPRMC,";
  int i=0;
  byte ck1,ck2;
  for (i=0; i<7; i++){
    if (vt[i] != aux[i]) return FALSE;
  }
  //ser_str(vt);  //Imprimir a seq recebida
  //ser_crlf(1);

  //Início da mensagem
  ser_str("GP RMC ==> ");

  // HORA
  ser_str("Hora=");
  if (vt[i] != ','){
    ser_char(vt[i++]); ser_char(vt[i++]); ser_char(':');  //HH
    ser_char(vt[i++]); ser_char(vt[i++]); ser_char(':');  //MM
    while( vt[i] != ',') ser_char(vt[i++]);
  }
  i++;
  
  // Status
  ser_str("  St=");  ser_char(vt[i++]);
  i++;  

  // Latitude
  ser_str("  Lat=");
  if (vt[i] != ','){
    ser_char(vt[i++]); ser_char(vt[i++]); ser_char(' ');  //dd
    while( vt[i] != ',') ser_char(vt[i++]);
  }
  i++;
  
  // N/S
  if (vt[i] != ','){
    ser_spc(1);
    ser_char(vt[i++]);
  }  
  i++;

  // Longitude
  ser_str("  Long=");
  if (vt[i] != ','){
    ser_char(vt[i++]); ser_char(vt[i++]); ser_char(vt[i++]);  ser_char(' ');  //gg
    while( vt[i] != ',') ser_char(vt[i++]);
  }
  i++;

  // E/W
  if (vt[i] != ','){
    ser_spc(1);
    ser_char(vt[i++]);
  }  
  i++;
  
  // Speed
  ser_str("  Speed=");
  if (vt[i] != ','){  
    while( vt[i] != ',') ser_char(vt[i++]);
  }
  i++;

  // Curso
  ser_str("  Curso=");
  if (vt[i] != ','){  
    while( vt[i] != ',') ser_char(vt[i++]);
  }
  i++;
  
  // Data: dd/mm/yy
  ser_str("  Data=");
  if (vt[i] != ','){  
    ser_char(vt[i++]);    ser_char(vt[i++]);    ser_char('/');  
    ser_char(vt[i++]);    ser_char(vt[i++]);    ser_char('/');  
    ser_char(vt[i++]);    ser_char(vt[i++]);  
  }
  i++;

  // mv
  ser_str("  mv=");
  if (vt[i] != ','){  
    while( vt[i] != ',') ser_char(vt[i++]);
  }
  i++;
  
  // mvE
  ser_str("  mvE=");
  if (vt[i] != ','){  
    while( vt[i] != ',') ser_char(vt[i++]);
  }
  i++;

  // Modo
  ser_str("  Modo=");
  if (vt[i] != ','){  
    ser_char(vt[i++]);
  }
  i++;

  // Check Sum
  ser_str("  CHK="); ser_char(vt[i++]); ser_char(vt[i++]);
  i++;
  ck1=(asc_nib(vt[i-3])<<4)+asc_nib(vt[i-2]);
  ck2=0;
  i=1;
  while(vt[i] != '*') ck2^=vt[i++];
  if ( (ck1^ck2) == 0)  ser_str(" OK");
  else                  ser_str(" NOK");

  ser_crlf(2);
  return TRUE;
}

// GP GSA - Recbe uma string, se for GSA, a imprime
// Retorna TRUE se era GSA (imprime)
// Retorna FALSE se não era (nada imprime);
// Seq Vazia = $GPGSA,A,3,,,,,,,,,,,,N*53
//             01234567890123456789012
byte gps_gsa_ser(char *vt){
  char aux[]="$GPGSA,";
  int i=0;
  byte ck1,ck2;
  for (i=0; i<7; i++){
    if (vt[i] != aux[i]) return FALSE;
  }
  //ser_str(vt);  //Imprimir a seq recebida
  //ser_crlf(1);

  //Início da mensagem
  ser_str("GP GSA ==> ");

  // Modo
  ser_str("Smode=");  ser_char(vt[i++]);
  i++;  

  // Fixo
  ser_str("  FS=");  ser_char(vt[i++]);
  i++;  

  // Satélites
  for (ck1=1; ck1<13; ck1++){
    ser_str(" S");
    ser_dec8unz(ck1);
    ser_str("=");
    if (vt[i] != ','){
      ser_char(vt[i++]);
      ser_char(vt[i++]);
    }
    i++;
  }

  // PDOP
  ser_str("  PDOP=");
  if (vt[i] != ','){  
    while( vt[i] != ',') ser_char(vt[i++]);
  }
  i++;

  // HDOP
  ser_str("  HDOP=");
  if (vt[i] != ','){  
    while( vt[i] != ',') ser_char(vt[i++]);
  }
  i++;

  // VDOP
  ser_str("  HDOP=");
  if (vt[i] != ','){  
    while( vt[i] != '*') ser_char(vt[i++]);
  }
  i++;

  // Check Sum
  ser_str("  CHK="); ser_char(vt[i++]); ser_char(vt[i++]);
  i++;
  ck1=(asc_nib(vt[i-3])<<4)+asc_nib(vt[i-2]);
  ck2=0;
  i=1;
  while(vt[i] != '*') ck2^=vt[i++];
  if ( (ck1^ck2) == 0)  ser_str(" OK");
  else                  ser_str(" NOK");

  ser_crlf(2);
  return TRUE;
}

// Enviar msg para GPS
void gps_str(byte *msg){
  int i=0;
  while( msg[i] != '\0')
    ser_char(msg[i++]);
}

// Enviar um char para o GPS
// Sem usar interrupção
void gps_char(byte dt){
  UDR3=dt;
  while( (UCSR3A&(1<<TXC3)) == 0);  //Esperar flag tx
  UCSR3A |= 1<<TXC3;                //Apagar flag tx
}





////////////////////////////////////////////////////
////////////////// GPS Fila RX /////////////////////
////////////////////////////////////////////////////

// Inicializar fila GPS RX serial
void gps_rx_fila_config(void){
  gps_rx_pin=1;
  gps_rx_pout=0;
}

// Colocar um byte na fila RX
char gps_rx_poe(char cha){
  if (gps_rx_pin == gps_rx_pout){
    gps_fila_rx_cheia();  //Msg de fila cheia
    return FALSE;         // fila cheia
  }
  else{
      gps_rx_fila[gps_rx_pin++]=cha;
      if (gps_rx_pin==GPS_RX_FILA_TAM)
          gps_rx_pin=0;
      return TRUE;
  }
}

// Mensagem de Fila RX Cheia
void gps_fila_rx_cheia(void){
  char msg[]="\n********ERRO: Fila GPS Cheia!\n";
  byte i=0;
  UCSR3B &= ~(1<<RXEN3) ;      //desabilitar e RX
  led_AM();
  ser_str(msg);
}

// Tirar um byte da fila RX
char gps_rx_tira(char *cha){
  char pout_aux;
  pout_aux=gps_rx_pout+1;
  if (pout_aux==GPS_RX_FILA_TAM)    pout_aux=0;
  if (pout_aux == gps_rx_pin)       return(FALSE);
  else{
      *cha=gps_rx_fila[pout_aux];
      gps_rx_pout=pout_aux;
      return TRUE;
  }
}


////////////////////////////////////////////////////
/////////////// Configuração ///////////////////////
////////////////////////////////////////////////////

// Configurar porta serial 3
// Não habilita TX e nem RX
// Não habilita interrupções
void gps_config(long br){
  word dv;
  dv=(2000000L/br)-1;
  UBRR3H = dv>>8;     //MSB
  UBRR3L = dv&0xFF;   //LSB
  // Configurar 8 bits, sem paridade, 1 stop
  UCSR3A = (1<<TXC3) | (1<<U2X3);
  UCSR3B = 0;     //desab RX e TX 
//  UCSR3B = (1<<RXCIE3) | (1<<TXCIE3) | (1<<RXEN3) | (1<<TXEN3);  //RXIE=TXIE=1, hab TX e RX
//  UCSR3B = (1<<RXCIE3) | (1<<TXCIE3) | (1<<TXEN3);                 //RXIE=TXIE=1, hab TX e não RX
  UCSR3C = (1<<UCSZ31)| (1<<UCSZ30);
  gps_tx_ok=TRUE;        //Já pode começar a transmitir
}

// TX3: Interrupção por dado enviado
ISR(USART3_TX_vect){
  gps_tx_ok=TRUE;  //Marcar que já transmitiu
}


// RX3: Interrupção por dado recebido
ISR(USART3_RX_vect){
  byte x;
  x=UDR3;
  if (gps_msg_fase == 0){
    gps_msg_0[gps_msg_ix++]=x;
    if (x == '\n'){
      gps_msg_0[gps_msg_ix-2]='\0';
      gps_msg_fase=1;
      gps_msg_ix=0;
      gps_msg_ok=TRUE;
    }
  }
  else{
    gps_msg_1[gps_msg_ix++]=x;
    if (x == '\n'){
      gps_msg_1[gps_msg_ix-2]='\0';
      gps_msg_fase=0;
      gps_msg_ix=0;
      gps_msg_ok=TRUE;
    }
  }
  gps_rx_ok=TRUE;   //Marcar que recebeu
}
