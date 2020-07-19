// CXP - Caixa Preta
// 06/04/2020

#include "b-Defs.h"
#include "b-Globs.h"

void setup() {
  timer1_config();
  sw_config();
  leds_config();
  scope_config();
  lcd_config();
  ser_config(115200L);    //RX0 e TX0 bps
  gps_config(9600);
  ser_tx_fila_config();   //TX fila serial
  ser_rx_fila_config();   //RX fila serial
  bt_config(115200);      //BT: Configurar Serial 2
  bt_tx_EN();             //BT: Habilitar Transmissão
  bt_tx_int_EN();         //BT: Habilitar interrup por transmissão

  timer2_config();
  timer1_config();
  //twi_config_100k();       //Configurar TWI
  twi_config_400k();       //Configurar TWI
  //spi_config(SPI_500K);  //Configurar SPI
  spi_config(SPI_8M);      //Configurar SPI
  mpu_config();            //Configura MPU +/- 2g
  //mpu_mag_config();       //Configurar o magnetômetro

}

void loop() {

  /*
  // - Teste LAT e LONG
  float f;
  byte msg[24];
  byte lat[]="1548.63128";      //12,5761315
  byte longi[]="04748.655572";  //112,5761315
  
  str_lat(lat,'S',msg);
  ser_str("\nLat = ");
  ser_str(msg);
  ser_crlf(1);

  str_long(longi,'E',msg);
  ser_str("\nLong = ");
  ser_str(msg);
  ser_crlf(1);

  while(TRUE);
  */


  //teste();  //Forçar a entrada  no modo teste
  if (sw_val < 10)  teste();
  else              opera();
  
  while(TRUE);  //Para em loop infinito
}


// Selecionar o modo
// Serve selecionar modo de Operação
// Serve selecionar modo de Teste
// A linha 0 é preparada por quem chama
// Usa as linhas 1, 2 e 3

byte sel_modo(char *msg[], byte total){
  byte prov=total;    //provisório = 1, 2, ..., OPERA_TOT
  byte tecla,aux;
  byte nr,ser;
  byte fase;    //Indicar se estava recebendo nr pela serial
  fase=0;
  while(TRUE){
    lcd_apaga_lin(1);
    lcd_apaga_lin(2);
    lcd_apaga_lin(3);
    lcd_str(2,0,"-->");
    aux=prov;
    lcd_str(1,3,msg[aux++]);
    if (aux>total)  aux=1;    
    lcd_str(2,3,msg[aux]);
    ser_str(msg[aux++]);  ser_crlf(1);
    if (aux>total)  aux=1;    
    lcd_str(3,3,msg[aux]);
    //Esperar tecla ou serial
    while(TRUE){
      if ( sw_tira(&tecla) == TRUE) break;
      if (ser_rx_tira(&ser) == TRUE){
        if (fase==1){
          if (ser>0x2F && ser<0x3A) nr=10*nr + (ser-0x30);
          if (ser==CR || ser==LF){
            if (nr<=total)   return(nr);    //nr dentro da faixa, retorna    
            else            fase=0;        //nr fora da faixa, anula
          }
        }
        else{
          if (ser>0x2F && ser<0x3A){
            fase=1;                 //Marcar que está recebendo nr
            nr=ser-0x30;
          }
          if (ser=='t' || ser=='T'){
            ser&=~(0x20);   //Transformar em T maiúscula
            return(ser);
          }
          if (ser=='o' || ser=='O'){
            ser&=~(0x20);   //Transformar em O maiúscula
            return(ser);
          }
        }
      }
    }
    //while ( sw_tira(&tecla) == FALSE);
    switch(tecla){
      case SW_SUP:  prov--;  break;
      case SW_INF:  prov++;  break;
      case SW_SEL:  if (++prov>total)  prov=1;
                    return prov;
    }
    if (prov>total)  prov=1;
    if (prov==0)     prov=total;
  }
}


byte sel_modo_velho(char *msg[], byte total){
  byte prov=total;    //provisório = 1, 2, ..., OPERA_TOT
  byte tecla,aux;
  while(TRUE){
    lcd_apaga_lin(1);
    lcd_apaga_lin(2);
    lcd_apaga_lin(3);
    lcd_str(2,0,"-->");
    aux=prov;
    lcd_str(1,3,msg[aux++]);
    if (aux>total)  aux=1;    
    lcd_str(2,3,msg[aux]);
    ser_str(msg[aux++]);  ser_crlf(1);
    if (aux>total)  aux=1;    
    lcd_str(3,3,msg[aux]);
    //Esperar tecla
    while ( sw_tira(&tecla) == FALSE);
    switch(tecla){
      case SW_SUP:  prov--;  break;
      case SW_INF:  prov++;  break;
      case SW_SEL:  if (++prov>total)  prov=1;
                    return prov;
    }
    if (prov>total)  prov=1;
    if (prov==0)     prov=total;
  }
}
