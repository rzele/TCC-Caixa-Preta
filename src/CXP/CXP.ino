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
  ser_config(500000L);    //RX0 e TX0 bps
  gps_config(9600);
  ser_tx_fila_config();   //TX fila serial
  ser_rx_fila_config();   //RX fila serial
  timer2_config();
  timer1_config();
  twi_config_100k();       //Configurar TWI
  spi_config(SPI_500K);    //Configurar SPI
  mpu_config();         //Configura MPU +/- 2g    
}

void loop() {
  
  teste();  //Forçar a entrada  no modo teste
  
  if (ADCH < 10)  teste();
  else            opera();
  
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
