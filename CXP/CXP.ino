// CXP - Caixa Preta
// 10/01/2020

#include "a-Defs.h"
#include "b-Globs.h"


void setup() {
  word x; //teste
  long i;
  long z;
  float f;
  char c;
  int sw1,sw2,sw3,sw4,sw5;
  timer1_config();
  sw_config();
  leds_config();
  scope_config();
  lcd_config();
  lcdb_config();
  ser_config(115200L);
  gps_config(9600);
  ser_tx_fila_config();   //TX fila serial
  ser_rx_fila_config();   //RX fila serial
  gps_rx_fila_config();   //GPS RX fila serial
  timer1_config();
  twi_config_100k();       //Configurar TWI
  spi_config(SPI_500K);    //Configurar SPI
}

void loop() {
  while(TRUE){
    //modo=modo_sel();
    modo=MODO_9;
    if      (modo==MODO_0) modo_0();
    else if (modo==MODO_1) modo_1();
    else if (modo==MODO_2) modo_2();
    else if (modo==MODO_3) modo_3();
    else if (modo==MODO_4) modo_4();
    else if (modo==MODO_5) modo_5();
    else if (modo==MODO_6) modo_6();
    else if (modo==MODO_7) modo_7();
    else if (modo==MODO_8) modo_8();
    else                   modo_9();
    
    /*
    // Por que este switch/case não funciona? Seleciona opção anterior!
    switch(modo){
      case MODO_0:  ser_str("\n0)M="); ser_dec8unz(modo); ser_crlf(1);  modo_0(); break;
      case MODO_1:  ser_str("\n1)M="); ser_dec8unz(modo); ser_crlf(1);  modo_1(); break;
      case MODO_2:  ser_str("\n2)M="); ser_dec8unz(modo); ser_crlf(1);  modo_2(); break;
      case MODO_3:  ser_str("\n3)M="); ser_dec8unz(modo); ser_crlf(1);  modo_3(); break;
      case MODO_4:  ser_str("\n4)M="); ser_dec8unz(modo); ser_crlf(1);  modo_4(); break;
      case MODO_5:  ser_str("\n5)M="); ser_dec8unz(modo); ser_crlf(1);  modo_5(); break;
      case MODO_6:  ser_str("\n6)M="); ser_dec8unz(modo); ser_crlf(1);  modo_6(); break;
      case MODO_7:  ser_str("\n7)M="); ser_dec8unz(modo); ser_crlf(1);  modo_7(); break;
      default:      ser_str("\nd)Def M="); ser_dec8unz(modo); ser_crlf(1);  modo=0;   break;
    }
    */
  }
}

// Selecionar o modo de operação
byte modo_sel(void){
  byte tecla,prov=0;
  lcd_apaga();
  lcd_str("Selecionar Modo");
  ser_str("Esperando Seleção:\n");

  while(TRUE){
    lcd_cursor_lc(1,0);
    lcd_spc(20);
    lcd_str_lc(1,0,modo_msg[prov]);
    while ( sw_tira(&tecla) == FALSE);
    switch(tecla){
      case SW_CIMA:  prov++;  break;
      case SW_BAIXO: prov--;  break;
      case SW_SEL:   return prov;
    }
    if (prov==MODO_TOTAL) prov=0;
    if (prov==255)        prov=MODO_TOTAL-1;
    //ser_str("prov = "); ser_dec8unz(prov);  ser_crlf(1);
  }
}

// Modo 9 - MPU ==> Matlab
byte modo_9(void){
  byte who;
  word vetor[7];
  lcd_apaga();
  lcd_cursor_lc(0,0);
  lcd_str("[9] MPU==>MATLAB");
  mpu_acorda();     //Acordar MPU
  who=mpu_whoami();
  if (who != 0x73){
    lcd_cursor_lc(2,1);
    lcd_str("MPU nao responde!");  //MPU respondendo
    sw_qq_tecla();
    return  FALSE;
  }

  lcd_cursor_lc(1,0);
  lcd_str("Qq tecla inicia TX");
  while(sw_tira(&who)==FALSE);
  mpu_inicializa();     //Inicializar
  mpu_escalas(0,0);     //+/- 2g e +/-250gr/seg
  delay(2000);
  lcd_apaga();
  while(TRUE){
    mpu_rd_ac_tp_gi(vetor);
    lcd_cursor_lc(0,0);    lcd_hex16(vetor[0]);
    lcd_cursor_lc(0,6);    lcd_hex16(vetor[1]);
    lcd_cursor_lc(0,12);   lcd_hex16(vetor[2]);
    lcd_cursor_lc(1,0);    lcd_hex16(vetor[4]);
    lcd_cursor_lc(1,6);    lcd_hex16(vetor[5]);
    lcd_cursor_lc(1,12);   lcd_hex16(vetor[6]);
    ser_dec16(vetor[0]);   ser_crlf(1);           //ax
    ser_dec16(vetor[1]);   ser_crlf(1);           //ay
    ser_dec16(vetor[2]);   ser_crlf(1);           //az
    ser_dec16(vetor[3]);   ser_crlf(1);           //temperatura
    ser_dec16(vetor[4]);   ser_crlf(1);           //gx
    ser_dec16(vetor[5]);   ser_crlf(1);           //gy
    ser_dec16(vetor[6]);   ser_crlf(1);           //gz
    if (sw_tira(&who))     break;
  }
  ser_crlf(1);           ser_dec16(1);
  ser_crlf(1);           ser_dec16(2);
  ser_crlf(1);           ser_dec16(3);
  ser_crlf(1);           ser_dec16(99);
  ser_crlf(1);           ser_dec16(99);
  ser_crlf(1);           ser_dec16(99);
  ser_crlf(1);           ser_dec16(99);
  return;
}

// Modo 8 - Expedito
byte modo_8(void){
  byte dado;
  long adr;
  word x,i;
  ser_str("\n<<<Modo 8>>>\n"); 
  ser_str("Modo ");
  ser_dec8unz(modo);
  ser_str(": Testar SRAM (24LC1025)\n");
  lcd_apaga();
  lcd_str("[8] Expedito FLASH ");
  ser_str("Escravos receptores:\n");
  for (x=0; x<256; x++){
    ser_str("\nTestando ");
    ser_dec8u(x);
    twi_start(5);
    TWDR = x;                       //Endereço do  escravo
    TWCR = (1<<TWINT) | (1<<TWEN);  //Enviar endereço
    while ( !(TWCR & (1<<TWINT)));  //Esperar TWINT = 1
    if ( (TWSR & 0xF8) == TWI_SLA_WR_ACK){
      ser_str("Encontrado: ");
      ser_hex8(x);
      ser_str("\n");
    }
   twi_stop();
  }
  ser_str("Terminou!\n\nQualquer tecla para finalizar.");
  sw_qq_tecla();
  ser_str("\nSaindo do Modo 8!\n");
  return;
  

  
  dado=flash_rd(0x10000L);
  ser_hex32(adr);
  ser_str("-->");
  ser_hex8(dado);
  ser_crlf(1);

  flash_wr(0x10000L,0xAA);
  flash_espera(0);

  dado=flash_rd(0x10000L);
  ser_hex32(adr);
  ser_str("-->");
  ser_hex8(dado);
  ser_crlf(1);

  ser_str("Terminou!\n\nQualquer tecla para finalizar.");
  sw_qq_tecla();
  ser_str("\nSaindo do Modo 8!\n");
  return;

}

// Modo 7 - Testar SRAM 23LC1024 e W25Q64 (SPI)
byte modo_7(void){
  byte i,j,dado,tx[64],rx[64];;
  long erro,vet[16],incr;
  word x;
  ser_str("\n<<<Modo 7>>>\n"); 
  ser_str("Modo ");
  ser_dec8unz(modo);
  ser_str(": Testar SRAM\n");
  lcd_apaga();
  lcd_str("[7] SRAM");

  // Gerar a sequencia
  // Só usa as 16 primeiras posições
  for (i=0; i<16; i++)  tx[i]=i;
  
  // Sortear 4 números para gerar 16 endereços
  x=TCNT1;
  for (i=0; i<16; i+=4){
    incr=(i/4)*0x10000L;
    //incr=0;
    vet[i]=x+incr;
    vet[i+1]=((x+0x1234)&0xFFFF)+incr;
    vet[i+2]=((x+0x3456)&0xFFFF)+incr;
    vet[i+3]=((x+0x789A)&0xFFFF)+incr;
  }

  // Escritas/leituras isoladas 
  ser_str("\nSRAM: Escritas e leituras isoladas.\nGravar Padrão:\n");
  for (i=0; i<16; i++){
    ser_dec8u(i);
    ser_str(") Gravar ");
    ser_hex8(tx[i]);
    ser_str(" no endereço ");
    ser_hex32(vet[i]);
    ser_spc(1);
    sram_wr(vet[i],tx[i]);
    ser_str(" Gravado!\n");
  }

  ser_str("\nConferir Padrão:\n");
  for (i=0; i<16; i++){
    ser_dec8u(i);
    ser_str(") Ler ");
    ser_hex8(tx[i]);
    ser_str(" no endereço ");
    ser_hex32(vet[i]);
    ser_spc(1);
    dado=sram_rd(vet[i]);
    if (dado == tx[i])  ser_str(" OK!\n");
    else                ser_str(" NOK!\n");
  }
  
  ////////////////////////////////////
  // Escrita de blocos
  /////////////////////////////////
  ser_str("\nEscrita e leitura em blocos.\n");
  
  //Sortear 4 endereços, um em cada bloco
  x=TCNT1&0xFFF0;   //Garantir página de 16 endereços
  vet[0]=x+0x00000;
  vet[1]=x+0x10000;
  vet[2]=x+0x20000;
  vet[3]=x+0x30000;

  // Gerar padrão a ser gravado
  for (i=0; i<64; i++)  tx[i]=i;

  // Conteúdo inicial da memória  
  sram_rd_blk(vet[0], &rx[0], 16);
  sram_rd_blk(vet[1], &rx[16], 16);
  sram_rd_blk(vet[2], &rx[32], 16);
  sram_rd_blk(vet[3], &rx[48], 16);

  ser_str("\nConteudo original da SRAM:\n");
  for (j=0; j<4; j++){
    ser_str("Endereço ");
    ser_hex32(vet[j]);
    ser_str(": ");    
    for (i=0; i<16; i++){
      ser_hex8(rx[(16*j)+i]);
      ser_spc(1); 
    }
    ser_crlf(1);
  }

  ser_str("\nEscrita dos blocos na SRAM:\n");
  for (j=0; j<4; j++){
    ser_str("No endereço ");
    ser_hex32(vet[j]);
    ser_str(" escrever: ");    
    for (i=0; i<16; i++){
      ser_hex8(tx[(16*j)+i]);
      ser_spc(1); 
    }
    sram_wr_blk(vet[j],&tx[16*j], 16);
    ser_str(" Escrito !\n");    
  }

  // Conteúdo posterior da memória  
  sram_rd_blk(vet[0], &rx[0], 16);
  sram_rd_blk(vet[1], &rx[16], 16);
  sram_rd_blk(vet[2], &rx[32], 16);
  sram_rd_blk(vet[3], &rx[48], 16);

  erro=FALSE;
  ser_str("\nConteúdo da SRAM após escrita:\n");
  for (j=0; j<4; j++){
    ser_str("Endereço ");
    ser_hex32(vet[j]);
    ser_str(": ");    
    for (i=0; i<16; i++){
      ser_hex8(rx[(16*j)+i]);
      ser_spc(1);
      if ((rx[(16*j)+i]) != (tx[(16*j)+i])){
        ser_str("<-Erro ");
        erro=TRUE;
      }
    }
    ser_crlf(1);
  }
  if(erro==TRUE)  ser_str("\nTem erro na escrita.\n");
  else            ser_str("\nEscrita sem erros.\n");
  
  ser_str("Terminou!\n\nQualquer tecla para finalizar.");
  sw_qq_tecla();
  ser_str("\nSaindo do Modo 7!\n");
  return;
}

// Modo 5 - Testar GPS com u-Center
byte modo_5(void){
  long adr;       //Endereço para gravar na SRAM as msg do PC (u-Center)
  word cpc,cgps;  //Contadores de tráfego
  byte who,flag_pc,flag_gps,dado;
  ser_str("\n<<<Modo 5>>>\n"); 
  ser_str("Modo ");
  ser_dec8unz(modo);
  ser_str(": GPS com u-Center\n");
  lcd_apaga();
  lcd_str("[5] GPS u-Center");

  ser_str("20 primeiros bytes da SRAM\n");
  delay(1000);
  adr=0;
  for (who=0; who<20; who++){
    ser_hex8(sram_rd(adr++));
    ser_spc(1);
  }

  for (who=0; who<20; who++){
    sram_wr((long)who,0x55);
  }

  
  // Reconfigurar portas serais
  // Serial 0 --> conversar com o u-Center
  // Serial 3 --> conversar com o GPS
  // Habilitar TX e RX, mas sem interrupções
  //ser_config(460800L);  //OK com u-Center
  //ser_config(230400L);  //Não funciona com u-Center
  ser_config(115200L);    //OK com u-Center
  gps_config(9600L);

  UCSR0B = (1<<RXEN0) | (1<<TXEN0);  //Só habilita TX e RX
  UCSR3B = (1<<RXEN3) | (1<<TXEN3);  //Só habilita TX e RX


  flag_pc=flag_gps=cpc=cgps=0;
  adr=0;
  while(TRUE){

    // PC --> GPS
    if ( (UCSR0A&(1<<RXC0)) == (1<<RXC0)){
      dado=UDR0;
      UDR3=dado;
      UCSR0A |= 1<<RXC0;  //Apagar flag RXC0
      cpc++;
      sram_wr(adr++,dado);
      flag_pc=TRUE;
    }

    // GPS --> PC
    if ( (UCSR3A&(1<<RXC3)) == (1<<RXC3)){
      UDR0=UDR3;
      UCSR3A |= 1<<RXC3;  //Apagar flag RXC3
      cgps++;
      flag_gps=TRUE;
    }

    if (flag_pc == TRUE){
      flag_pc = FALSE;
      lcd_cursor_lc(1,8);
      lcd_dec16unz(cpc);
    }

    if (flag_gps == TRUE){
      flag_gps = FALSE;
      lcd_cursor_lc(1,0);
      lcd_dec16unz(cgps);
    }

    if (sw_tira(&who))     break;
  }
  ser_str("100 primeiros bytes da SRAM\n");
  adr=0;
  for (who=0; who<20; who++){
    //UDR0=sram_rd(adr++);
    UDR0='A'+who;
    while ( (UCSR0A&(1<<TXC0)) == (1<<TXC0));
    UCSR0A |= (1<<TXC0);
  }
  ser_str("\nSaindo do Modo 5!\n");
  return TRUE;;
}


// Modo 6 - Testar FLASH 24LC1025 (TWI)
byte modo_6(void){
  byte i,j,dado,tx[64],rx[64];;
  long erro,vet[16],incr;
  word x;
  ser_str("\n<<<Modo 6>>>\n"); 
  ser_str("Modo ");
  ser_dec8unz(modo);
  ser_str(": Testar SRAM (24LC1025)\n");
  lcd_apaga();
  lcd_str("[6] FLASH ");

  // Gerar a sequencia
  // Só usa as 16 primeiras posições
  for (i=0; i<16; i++)  tx[i]=i;
  
  // Sortear 4 números para gerar 16 endereços
  x=TCNT1;
  for (i=0; i<16; i+=4){
    incr=(i/4)*0x10000L;
    //incr=0;
    vet[i]=x+incr;
    vet[i+1]=((x+0x1234)&0xFFFF)+incr;
    vet[i+2]=((x+0x3456)&0xFFFF)+incr;
    vet[i+3]=((x+0x789A)&0xFFFF)+incr;
  }


  // Gravação
  ser_str("\nEscrita e leitura isoladas.\nGravar Padrão:\n");
  for (i=0; i<16; i++){
    ser_dec8u(i);
    ser_str(") Gravar ");
    ser_hex8(tx[i]);
    ser_str(" no endereço ");
    ser_hex32(vet[i]);
    ser_spc(1);
    flash_wr(vet[i],tx[i]);
    flash_espera(vet[i]);
    ser_str(" Gravado!\n");
  }

  ser_str("\nConferir Padrão:\n");
  for (i=0; i<16; i++){
    ser_dec8u(i);
    ser_str(") Ler ");
    ser_hex8(tx[i]);
    ser_str(" no endereço ");
    ser_hex32(vet[i]);
    ser_spc(1);
    dado=flash_rd(vet[i]);
    if (dado == tx[i])  ser_str(" OK!\n");
    else                ser_str(" NOK!\n");
  }

  ////////////////////////////////////
  // Gravação de blocos
  /////////////////////////////////

  ser_str("\nEscrita e leitura em blocos.\n");

  //Sortear 4 endereços, um em cada bloco
  x=TCNT1&0xFFF0;   //Garantir página de 16 endereços
  vet[0]=x+0x00000;
  vet[1]=x+0x10000;
  vet[2]=x+0x20000;
  vet[3]=x+0x30000;

  // Gerar padrão a ser gravado
  for (i=0; i<64; i++)  tx[i]=i;

  // Conteúdo inicial da memória  
  flash_rd_blk(vet[0], &rx[0], 16);
  flash_rd_blk(vet[1], &rx[16], 16);
  flash_rd_blk(vet[2], &rx[32], 16);
  flash_rd_blk(vet[3], &rx[48], 16);

  ser_str("\nConteudo original da Flash:\n");
  for (j=0; j<4; j++){
    ser_str("Endereço ");
    ser_hex32(vet[j]);
    ser_str(": ");    
    for (i=0; i<16; i++){
      ser_hex8(rx[(16*j)+i]);
      ser_spc(1); 
    }
    ser_crlf(1);
  }

  ser_str("\nGravação dos blocos:\n");
  for (j=0; j<4; j++){
    ser_str("No endereço ");
    ser_hex32(vet[j]);
    ser_str(" gravar: ");    
    for (i=0; i<16; i++){
      ser_hex8(tx[(16*j)+i]);
      ser_spc(1); 
    }
    flash_wr_blk(vet[j],&tx[16*j], 16);
    flash_espera(vet[j]);
    ser_str(" Gravado !\n");    
  }

  // Conteúdo posterior da memória  
  flash_rd_blk(vet[0], &rx[0], 16);
  flash_rd_blk(vet[1], &rx[16], 16);
  flash_rd_blk(vet[2], &rx[32], 16);
  flash_rd_blk(vet[3], &rx[48], 16);

  erro=FALSE;
  ser_str("\nConteúdo da Flash após gravação:\n");
  for (j=0; j<4; j++){
    ser_str("Endereço ");
    ser_hex32(vet[j]);
    ser_str(": ");    
    for (i=0; i<16; i++){
      ser_hex8(rx[(16*j)+i]);
      ser_spc(1);
      if ((rx[(16*j)+i]) != (tx[(16*j)+i])){
        ser_str("<-Erro ");
        erro=TRUE;
      }
    }
    ser_crlf(1);
  }
  if(erro==TRUE)  ser_str("\nTem erro na gravação.\n");
  else            ser_str("\nGravação sem erros.\n");
  
  ser_str("Terminou!\n\nQualquer tecla para finalizar.");
  sw_qq_tecla();
  return;
}



// Modo 0 - Testar TWI
// Verificar se os escravos TWI geram ACK
// Resultados no LCD e na serial
byte modo_0(void){
  ser_str("\n<<<Modo 0>>>\n"); 
  ser_str("Modo ");
  ser_dec8unz(modo);
  ser_str(": Testar TWI\n");
  lcd_apaga();
  lcd_str("[0] TWI");
  twi_scan();
  lcd_str_lc(0,8,"MPU ");
  if (twi_er_ok(MPU_ADR)) lcd_str("OK "); 
  else                    lcd_str("NOK");
  lcd_str_lc(1,0,"FSH1 ");
  if (twi_er_ok(FLASH1_ADR)) lcd_str("OK "); 
  else                     lcd_str("NOK");
  lcd_str_lc(1,8,"FSH2 ");
  if (twi_er_ok(FLASH2_ADR)) lcd_str("OK "); 
  else                     lcd_str("NOK");
  sw_qq_tecla();
  ser_str("\nSaindo do Modo 0!\n");
  return;
}

// Modo 1 - Testar MPU
byte modo_1(void){
  byte who;
  word vetor[7];
  ser_str("\n<<<Modo 1>>>\n"); 
  lcd_apaga();
  lcd_str("[1] Testar MPU");
  ser_str("Modo ");
  ser_dec8unz(modo);
  ser_str(": Testar MPU\n");
  mpu_acorda();     //Acordar MPU
  who=mpu_whoami();
  lcd_str_lc(1,0,"Who am I =");
  lcd_hex8(who);
  ser_str("MPU retornou Who am I = ");
  ser_hex8(who);
  if (who == 0x73){
    lcd_str(" OK");  //MPU respondendo
    ser_str("h ==> OK!");
  }
  else{
    lcd_str(" NOK");  //MPU respondendo
    ser_str("h ==> NOK! ERRO");
  }

  mpu_inicializa();     //Inicializar
  mpu_escalas(0,0);     //+/- 2g e +/-250gr/seg
  delay(2000);
  lcd_apaga();
  while(TRUE){
    mpu_rd_ac_tp_gi(vetor);
    lcd_cursor_lc(0,0);    lcd_hex16(vetor[0]);
    lcd_cursor_lc(0,6);    lcd_hex16(vetor[1]);
    lcd_cursor_lc(0,12);   lcd_hex16(vetor[2]);
    lcd_cursor_lc(1,0);    lcd_hex16(vetor[4]);
    lcd_cursor_lc(1,6);    lcd_hex16(vetor[5]);
    lcd_cursor_lc(1,12);   lcd_hex16(vetor[6]);
    ser_str("\nax=");      ser_hex16(vetor[0]);
    ser_str("  ay=");      ser_hex16(vetor[1]);
    ser_str("  az=");      ser_hex16(vetor[2]);
    ser_str("  tp=");      ser_hex16(vetor[3]);
    ser_str("  gx=");      ser_hex16(vetor[4]);
    ser_str("  gy=");      ser_hex16(vetor[5]);
    ser_str("  gz=");      ser_hex16(vetor[6]);
    if (sw_tira(&who))     break;
  }
  ser_str("\nSaindo do Modo 1!\n");
  return;
}

// Modo 2 - Testar Magnetômetro
byte modo_2(void){
  byte who;
  word vetor[7];
  ser_str("\n<<<Modo 2>>>\n"); 
 lcd_apaga();
  lcd_str("[2] Testar Mag");
  ser_str("Modo ");
  ser_dec8unz(modo);
  ser_str(": Testar Magnetômetro\n");
  mpu_mag_config();     //Configurar Mag
  lcd_apaga();
  while(TRUE){
    mpu_rd_mg(vetor);
    lcd_cursor_lc(0,0);    lcd_hex16(vetor[0]);
    lcd_cursor_lc(0,6);    lcd_hex16(vetor[1]);
    lcd_cursor_lc(0,12);   lcd_hex16(vetor[2]);
    ser_str("\nhx=");      ser_hex16(vetor[0]);
    ser_str("  hy=");      ser_hex16(vetor[1]);
    ser_str("  hz=");      ser_hex16(vetor[2]);
    if (sw_tira(&who))     break;
  }
  ser_str("\nSaindo do Modo 2!\n");
  return;
}

// Modo 3 - GPS
byte modo_3(void){
  byte dado;
  ser_str("\n<<<Modo 3>>>\n"); 
  lcd_apaga();
  lcd_str("[");
  lcd_dec8unz(modo);
  lcd_str("] GPS Tudo");
  ser_str("Modo ");
  ser_dec8unz(modo);
  ser_str(": Testar GPS\n");
  gps_rx_fila_config();   //GPS RX fila serial
  UCSR3B |= (1<<RXEN3) ;      //habilitar e RX
  while(TRUE){
    while( (UCSR3A&(1<<RXC3)) == 0);   //Esperar chegar
    ser_char(UDR3);
    if (sw_tira(&dado))   break;
 }
  ser_str("Saiu\n");
  return;
}

// Modo 4 GPS, imprime GPRMC
byte modo_4(void){
  byte dado;
  word ix=0;   //Indexador para guardar no buffer
  ser_str("\n<<<Modo 4>>>\n"); 
  lcd_apaga();
  lcd_str("[");
  lcd_dec8unz(modo);
  lcd_str("] GPS RMC e GSA");
  ser_str("Modo ");
  ser_dec8unz(modo);
  ser_str(": Mensagens GPRMC\n");
  gps_msg_fase=0;   //Interrup usar buffer 0
  gps_msg_ix=0;     //Zerar indexador do buffer
  gps_msg_ok=FALSE;
  UCSR3B |= (1<<RXCIE3)|(1<<RXEN3) ;  //habilitar e RX e int RX
  while(TRUE){
    while (gps_msg_ok == FALSE);
    gps_msg_ok = FALSE;
    if (gps_msg_fase == 0){
      gps_rmc_ser(gps_msg_1);
      gps_gsa_ser(gps_msg_1);
    }
    if (gps_msg_fase == 1){
      gps_rmc_ser(gps_msg_0);
      gps_gsa_ser(gps_msg_0);
    }
    if (sw_tira(&dado))   break;
  }  
  ser_str("Saiu\n");
  return;
}
