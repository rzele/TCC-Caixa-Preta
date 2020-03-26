// LCD
// CXP - Caixa Preta
// 10/01/2019

// Pinos do LCD
// PC1 = RS
// PC2 = R/#W
// PC0 = E
// PA4..7 = D4..7
//PC3 = Back Light

////////////////////////////////////////////////////
/////////// LCD com buffer /////////////////////////
////////////////////////////////////////////////////

////////////////////////////////////////////////////
/////////// float (double) com buffer //////////////
////////////////////////////////////////////////////

// Imprimir float = + xxx xxx xxx , ddd ddd ddd ddd (24 posições)
//  9 posições = limite da parte inteira
// 12 posições = limite da parte fracionária
// Caso ultrapasse os limites imprime ### , ###
// No Arduino, double e float têm a mesma precisão
void lcdb_float(byte lin, byte col, float fx, byte prec){
  char msg[24];
  str_float(fx,prec,msg);
  lcdb_str(lin,col,msg);
}

////////////////////////////////////////////////////
//////////// 32 bits com buffer ////////////////////
////////////////////////////////////////////////////

// Escrever 32 bits em Decimal, com sinal e com zeros à esq
void lcdb_dec32(byte lin, byte col, long dt){
  char msg[12];
  str_dec32(dt, msg);
  lcdb_str(lin,col,msg);
}

// Escrever 32 bits em Decimal, sem sinal e com zeros à esq
void lcdb_dec32u(byte lin, byte col, long dt){
  char msg[12];
  str_dec32u(dt, msg);
  lcdb_str(lin,col,msg);
}

// Escrever 32 bits em Decimal, com sinal e sem zeros à esq
void lcdb_dec32nz(byte lin, byte col, long dt){
  char msg[12];
  byte i;
  str_dec32(dt, msg);
  str_rmvz_s(msg);
  lcdb_str(lin,col,msg);
}

// Escrever 32 bits em Decimal, sem sinal e sem zeros à esq
void lcdb_dec32unz(byte lin, byte col, long dt){
  char msg[12];
  str_dec32u(dt, msg);
  str_rmvz_u(msg);
  lcdb_str(lin,col,msg);
}

// Escrever word (32 bits) em Hexa na posição atual
void lcdb_hex32(byte lin, byte col, long dt){
  char msg[9];
  str_hex32(dt, msg);
  lcdb_str(lin,col,msg);
}

////////////////////////////////////////////////////
//////////// 16 bits com buffer ////////////////////
////////////////////////////////////////////////////

// Escrever 16 bits em Decimal, com sinal e com zeros à esq
void lcdb_dec16(byte lin, byte col, int dt){
  char msg[7];
  str_dec16(dt, msg);
  lcdb_str(lin,col,msg);
}

// Escrever 16 bits em Decimal, sem sinal e com zeros à esq
void lcdb_dec16u(byte lin, byte col, word dt){
  char msg[7];
  str_dec16u(dt, msg);
  lcdb_str(lin,col,msg);
}

// Escrever 16 bits em Decimal, com sinal e sem zeros à esq
void lcdb_dec16nz(byte lin, byte col, int dt){
  char msg[7];
  byte i;
  str_dec8(dt, msg);
  str_rmvz_s(msg);
  lcdb_str(lin,col,msg);
}

// Escrever 16 bits em Decimal, sem sinal e sem zeros à esq
void lcdb_dec16unz(byte lin, byte col, word dt){
  char msg[7];
  str_dec16u(dt, msg);
  str_rmvz_u(msg);
  lcdb_str(lin,col,msg);
}

// Escrever word (16 bits) em Hexa na posição atual
void lcdb_hex16(byte lin, byte col, word dt){
  char msg[5];
  str_hex16(dt, msg);
  lcdb_str(lin,col,msg);
}

////////////////////////////////////////////////////
//////////// 8 bits com buffer /////////////////////
////////////////////////////////////////////////////

// Escrever 8 bits em Decimal, com sinal e com zeros à esq
void lcdb_dec8(byte lin, byte col, byte dt){
  char msg[5];
  str_dec8(dt, msg);
  lcdb_str(lin,col,msg);
}

// Escrever 8 bits em Decimal, sem sinal e com zeros à esq
void lcdb_dec8u(byte lin, byte col, byte dt){
  char msg[5];
  str_dec8u(dt, msg);
  lcdb_str(lin,col,msg);
}

// Escrever 8 bits em Decimal, com sinal e sem zeros à esq
void lcdb_dec8nz(byte lin, byte col, byte dt){
  char msg[5];
  byte i;
  str_dec8(dt, msg);
  str_rmvz_s(msg);
  lcdb_str(lin,col,msg);
}

// Escrever 8 bits em Decimal, sem sinal e sem zeros à esq
void lcdb_dec8unz(byte lin, byte col, byte dt){
  char msg[5];
  str_dec8u(dt, msg);
  str_rmvz_u(msg);
  lcdb_str(lin,col,msg);
}

// Escrever byte em Hexa a partir da posição (lin,col)
void lcdb_hex8(byte lin, byte col, byte dt){
  char msg[3];
  str_hex8(dt, msg);
  lcdb_str(lin,col,msg);
}

// Escrever brancos em todo o LCD
void lcdb_apaga(void){
  lcdb_spc(0,0,20);
  lcdb_spc(1,0,20);
  lcdb_spc(2,0,20);
  lcdb_spc(3,0,20);
}

// Escrever uma qtd de espaços a partir da posição (lin,col)
void lcdb_spc(byte lin, byte col, byte qtd){
  byte i,j;
  j=lin*NRC+col;
  i=0;
  while ( (i<qtd) && (j<(lin+1)*NRC) ){
    lcdb_buf[j++]=' ';
    i++;  
  }
  lcdb_flags[lin]=TRUE;
}

// Escrever uma string a partir da posição (lin,col)
void lcdb_str(byte lin, byte col, char *msg){
  byte i,j;
  j=lin*NRC+col;
  i=0;
  while ( (msg[i]!='\0') && (j<(lin+1)*NRC) ){
    lcdb_buf[j++]=msg[i++];  
  }
  lcdb_flags[lin]=TRUE;
}

// Escrever um char, na posição (lin,col)
void lcdb_char(byte lin, byte col, byte dt){
  if ( (lin*NRC+col) < (NRL*NRC) ){
    lcdb_buf[lin*NRC+col]=dt;  
    lcdb_flags[lin]=TRUE;
  }
}


// Atualizar LCD com os dados do buffer
void lcdb_atualiza(void){
  byte i;
  for (i=0; i<NRL; i++){
    if (lcdb_flags[i] == TRUE){
      lcdb_flags[i]=FALSE;
      lcdb_copia(i); }  
  }
}

// Copiar uma linha do buffer para o LCD
void lcdb_copia(byte linha){
  byte i;
  lcd_cursor_lc(linha,0);
  i=NRC*linha;
  for (i; i<NRC*(linha+1); i++)
    lcd_char(lcdb_buf[i]);
}

// Configurar o buffer para o LCD
void lcdb_config(void){
  byte i;
  for (i=0; i<NRL*NRC; i++) lcdb_buf[i]=' ';
  for (i=0; i<NRL; i++)     lcdb_flags[i]=TRUE;
}

////////////////////////////////////////////////////
/////////// float (double) /////////////////////////
////////////////////////////////////////////////////

// Imprimir float = + xxx xxx xxx , ddd ddd ddd ddd (24 posições)
//  9 posições = limite da parte inteira
// 12 posições = limite da parte fracionária
// Caso ultrapasse os limites imprime ### , ###
// No Arduino, double e float têm a mesma precisão
void lcd_float(float fx, byte prec){
  char msg[24];
  str_float(fx,prec,msg);
  lcd_str(msg);
}

////////////////////////////////////////////////////
//////////////// 32 bits ///////////////////////////
////////////////////////////////////////////////////

// Escrever 32 bits em Decimal, com sinal e com zeros à esq
void lcd_dec32(long dt){
  char msg[12];
  str_dec32(dt, msg);
  lcd_str(msg);
}

// Escrever 32 bits em Decimal, sem sinal e com zeros à esq
void lcd_dec32u(long dt){
  char msg[12];
  str_dec32u(dt, msg);
  lcd_str(msg);
}

// Escrever 32 bits em Decimal, com sinal e sem zeros à esq
void lcd_dec32nz(long dt){
  char msg[12];
  byte i;
  str_dec32(dt, msg);
  str_rmvz_s(msg);
  lcd_str(msg);
}

// Escrever 32 bits em Decimal, sem sinal e sem zeros à esq
void lcd_dec32unz(long dt){
  char msg[12];
  str_dec32u(dt, msg);
  str_rmvz_u(msg);
  lcd_str(msg);
}

// Escrever word (32 bits) em Hexa na posição atual
void lcd_hex32(long dt){
  char msg[9];
  str_hex32(dt, msg);
  lcd_str(msg);
}

////////////////////////////////////////////////////
//////////////// 16 bits ///////////////////////////
////////////////////////////////////////////////////

// Escrever 16 bits em Decimal, com sinal e com zeros à esq
void lcd_dec16(int dt){
  char msg[7];
  str_dec16(dt, msg);
  lcd_str(msg);
}

// Escrever 16 bits em Decimal, sem sinal e com zeros à esq
void lcd_dec16u(word dt){
  char msg[7];
  str_dec16u(dt, msg);
  lcd_str(msg);
}

// Escrever 16 bits em Decimal, com sinal e sem zeros à esq
void lcd_dec16nz(int dt){
  char msg[7];
  byte i;
  str_dec8(dt, msg);
  str_rmvz_s(msg);
  lcd_str(msg);
}

// Escrever 16 bits em Decimal, sem sinal e sem zeros à esq
void lcd_dec16unz(word dt){
  char msg[7];
  str_dec16u(dt, msg);
  str_rmvz_u(msg);
  lcd_str(msg);
}

// Escrever word (16 bits) em Hexa na posição atual
void lcd_hex16(word dt){
  char msg[5];
  str_hex16(dt, msg);
  lcd_str(msg);
}

////////////////////////////////////////////////////
///////////////// 8 bits ///////////////////////////
////////////////////////////////////////////////////

// Escrever 8 bits em Decimal, com sinal e com zeros à esq
void lcd_dec8(byte dt){
  char msg[5];
  str_dec8(dt, msg);
  lcd_str(msg);
}

// Escrever 8 bits em Decimal, sem sinal e com zeros à esq
void lcd_dec8u(byte dt){
  char msg[5];
  str_dec8u(dt, msg);
  lcd_str(msg);
}

// Escrever 8 bits em Decimal, com sinal e sem zeros à esq
void lcd_dec8nz(byte dt){
  char msg[5];
  byte i;
  str_dec8(dt, msg);
  str_rmvz_s(msg);
  lcd_str(msg);
}

// Escrever 8 bits em Decimal, sem sinal e sem zeros à esq
void lcd_dec8unz(byte dt){
  char msg[5];
  str_dec8u(dt, msg);
  str_rmvz_u(msg);
  lcd_str(msg);
}

// Escrever byte em Hexa na posição atual
void lcd_hex8(byte dt){
  char msg[3];
  str_hex8(dt, msg);
  lcd_str(msg);
}

// Escrever brancos em todo o LCD
void lcd_apaga(void){
  lcd_cursor_lc(0,0);   lcd_spc(20);
  lcd_cursor_lc(1,0);   lcd_spc(20);
  lcd_cursor_lc(2,0);   lcd_spc(20);
  lcd_cursor_lc(3,0);   lcd_spc(20);
  lcd_cursor(0);
}

// Escrever string na posição "lin" e "col"
void lcd_str_lc(byte lin, byte col, char *pt){
  lcd_cursor_lc(lin,col);
  lcd_str(pt);
}

// Escrever string
void lcd_str(char *pt){
  while ( *pt != '\0'){
    lcd_char(*pt);
    pt++;
  }
}

// Escrever qtd de espaços
// Máximo de 20 espaços
void lcd_spc(byte qtd){
  char msg[21];
  if (qtd>20) qtd=20;
  str_spc(qtd,msg);
  lcd_str(msg);
}

// Posicionar cursor. lin=0,1, ... e col=0,1,2...
// Usar NRL (linhas) e NRC (colunas)
void lcd_cursor_lc(byte lin, byte col){
  byte aux;
  switch(lin){
    case 0:  aux=col;           break;
    case 1:  aux=0x40+col;      break;
    case 2:  aux=NRC+col;       break;
    case 3:  aux=0x40+NRC+col;  break;
    default: aux=0;
  }
  lcd_cmdo(0x80|aux);
}

// Posicionar cursor usando endereço da DDRAM
void lcd_cursor(byte dt){
  lcd_cmdo(0x80|dt);
}

// Escrever um char, espera Busy = 0
void lcd_char(byte dt){
  lcd_busy_hold();
  PORTC &= ~((1<<DDC2)|(1<<DDC1)|(1<<DDC0));  //R/W=RS=E = LOW
  lcd_RS();
  lcd_E();  lcd_E();  //2X para gerar um delay
  PORTA &= ~((1<<DDA7)|(1<<DDA6)|(1<<DDA5)|(1<<DDA4));  //D7=D6=D5=D4 = LOW
  PORTA |= dt&0xF0;
  lcd_e();  lcd_e();  //2X para gerar um delay
  lcd_E();  lcd_E();  //2X para gerar um delay
  PORTA &= ~((1<<DDA7)|(1<<DDA6)|(1<<DDA5)|(1<<DDA4));  //D7=D6=D5=D4 = LOW
  PORTA |= (dt&0xF)<<4;
  lcd_e();  lcd_e();  //2X para gerar um delay
}

// Escrever um char
// Não testa o Busy, por isso usa lcd_nib_wr() com delays
void lcd_char_nbusy(byte dt){
  PORTC &= ~((1<<DDC2)|(1<<DDC1)|(1<<DDC0));  //R/W=RS=E = LOW
  lcd_RS();
  lcd_nib_wr(dt>>4);
  lcd_nib_wr(dt&0xF);
}

// Escrever um comando
// Espera Busy = 0
void lcd_cmdo(byte dt){
  lcd_busy_hold();
  PORTC &= ~((1<<DDC2)|(1<<DDC1)|(1<<DDC0));  //R/W=RS=E = LOW
  lcd_rs();
  lcd_E();    lcd_E();    lcd_E();    //2X para gerar um delay
  PORTA &= ~((1<<DDA7)|(1<<DDA6)|(1<<DDA5)|(1<<DDA4));  //D7=D6=D5=D4 = LOW
  PORTA |= dt&0xF0;
  lcd_e();  lcd_e();  lcd_e();    //2X para gerar um delay
  lcd_E();    lcd_E();  lcd_E();    //2X para gerar um delay
  PORTA &= ~((1<<DDA7)|(1<<DDA6)|(1<<DDA5)|(1<<DDA4));  //D7=D6=D5=D4 = LOW
  PORTA |= (dt&0xF)<<4;
  lcd_e();  lcd_e();  lcd_e();    //2X para gerar um delay
}

// Esperar BUSY = 0
void lcd_busy_hold(void){
  int x;
  DDRA  &= ~((1<<DDA7)|(1<<DDA6)|(1<<DDA5)|(1<<DDA4));  //D7=D6=D5=D4 = Entradas
  PORTA |=  ((1<<DDA7)|(1<<DDA6)|(1<<DDA5)|(1<<DDA4));  //D7=D6=D5=D4 = PULL UP
  lcd_RW(); //Leitura
  lcd_rs(); //Status
  while(TRUE){
    led_VM();
    lcd_E();  lcd_E();  //2X para gerar um delay
    x=PINA&0xF0;
    lcd_e();  lcd_e();
    lcd_E();  lcd_E();  //2X para gerar um delay
    x|=(PINA&0xF0)>>4;
    lcd_e();  lcd_e();
    //Serial.println(x,HEX);
    if ( (x&0x80) == 0)  break;
  }
  led_vm();
  DDRA  |=  ((1<<DDA7)|(1<<DDA6)|(1<<DDA5)|(1<<DDA4));  //D7=D6=D5=D4 = Saídas
  PORTA &= ~((1<<DDA7)|(1<<DDA6)|(1<<DDA5)|(1<<DDA4));  //D7=D6=D5=D4 = LOW
  lcd_rs();
  lcd_rw();
}

// Retorna status (Busy + AC)
byte lcd_status(void){
  byte x;
  DDRA  &= ~((1<<DDA7)|(1<<DDA6)|(1<<DDA5)|(1<<DDA4));  //D7=D6=D5=D4 = Entradas
  PORTA |=  ((1<<DDA7)|(1<<DDA6)|(1<<DDA5)|(1<<DDA4));  //D7=D6=D5=D4 = PULL UP
  lcd_RW(); //Leitura
  lcd_rs(); //Status
  lcd_E();  lcd_E();  //2X para gerar um delay
  x=PINA&0xF0;
  lcd_e();
  lcd_E();  lcd_E();  //2X para gerar um delay
  x|=(PINA&0xF0)>>4;
  lcd_e();
  DDRA  |=  ((1<<DDA7)|(1<<DDA6)|(1<<DDA5)|(1<<DDA4));  //D7=D6=D5=D4 = Saídas
  PORTA &= ~((1<<DDA7)|(1<<DDA6)|(1<<DDA5)|(1<<DDA4));  //D7=D6=D5=D4 = LOW
  lcd_rs();
  lcd_rw();
  return x;
}

// Configurar LCD
void lcd_config(void){
  lcd_pinos();  //Configurar pinos para usar com LCD
  lcd_inic();   //Inicializar LCD
  lcd_BL();
}

// LCD - Configurar pinos
void lcd_pinos(void){
  DDRC  |=   (1<<DDC3)|(1<<DDC2)|(1<<DDC1)|(1<<DDC0);   //BL=R/W=RS=E = Saídas
  PORTC &= ~((1<<DDC3)|(1<<DDC2)|(1<<DDC1)|(1<<DDC0));  //BL=R/W=RS=E = LOW
  DDRA  |=   (1<<DDA7)|(1<<DDA6)|(1<<DDA5)|(1<<DDA4);   //D7=D6=D5=D4 = Saídas
  PORTA &= ~((1<<DDA7)|(1<<DDA6)|(1<<DDA5)|(1<<DDA4));  //D7=D6=D5=D4 = LOW
}

// LCD - Inicializar
void lcd_inic(void){
  delay(400);
  lcd_nib_wr(3);  //3
  delay(20);
  lcd_nib_wr(3);  //3
  delay(10);
  lcd_nib_wr(3);  //3
  delay(1);
  lcd_nib_wr(2);  //2
  delay(1);

  /*
  lcd_nib_wr(2);  lcd_nib_wr(8);    delay(1); //0x28
  lcd_nib_wr(0);  lcd_nib_wr(8);    delay(1);   //0x08
  lcd_nib_wr(0);  lcd_nib_wr(1);    delay(1);   //0x01
  lcd_nib_wr(0);  lcd_nib_wr(6);    delay(1);   //0x06
  lcd_nib_wr(0);  lcd_nib_wr(0xF);  delay(1); //0x0F 
  */
  
  lcd_cmdo(0x28); delay(1);
  lcd_cmdo(0x08); delay(1);
  lcd_cmdo(0x01); delay(1);
  lcd_cmdo(0x06); delay(1);
  lcd_cmdo(0x0F); delay(1);
}

// Escrever um nibble no LCD
// Recebe nibble = 0000 xxxx B
// Não altera RS e RW
void lcd_nib_wr(byte nib){
  volatile int i;
  lcd_E();  lcd_E();  //2X para gerar um delay
  //delayMicroseconds(400);
  PORTA &= ~((1<<DDA7)|(1<<DDA6)|(1<<DDA5)|(1<<DDA4));  //D7=D6=D5=D4 = LOW
  PORTA |= (nib&0xF)<<4;
  //delayMicroseconds(500);
  lcd_e();  lcd_e();  //2X para gerar um delay
  //delayMicroseconds(500);
}


// RW/RS/E - Sinais de controle do LCD
void lcd_e(void) { PORTC &= ~(1<<DDC0);  }  //E=0
void lcd_E(void) { PORTC |=  (1<<DDC0);  }  //E=1
void lcd_rs(void){ PORTC &= ~(1<<DDC1);  }  //RS=0 - Cmdo
void lcd_RS(void){ PORTC |=  (1<<DDC1);  }  //RS=1 - Dado
void lcd_rw(void){ PORTC &= ~(1<<DDC2);  }  //RW=0 - Escrita
void lcd_RW(void){ PORTC |=  (1<<DDC2);  }  //RW=1 - Leitura

// BL (PC3) = Back Light do LCD: Apagar / Acender / Inverter
void lcd_bl(void){ PORTC &= ~(1<<DDC3);  }  //BL = Apagado
void lcd_BL(void){ PORTC |=  (1<<DDC3);  }  //BL = Aceso
void lcd_Bl(void){ PORTC ^=  (1<<DDC3);  }  //BL = Invertido
