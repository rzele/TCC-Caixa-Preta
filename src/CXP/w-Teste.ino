// Modos de Teste
// CXP - Caixa Preta
// 06/04/2020

byte linha[21];

void teste(void){
  byte modo;    //Modo selecionado
  byte i;
  word w;
  long z;
  float f;
  while(TRUE){
    lcd_apaga();
    lcd_str(0,0,"TESTE");
    ser_str("\nModo Teste\n");
    while(sw_val<0x10);  //Esperar soltar tecla SEL
    lcd_str(0,5," - Selecionar");
    ser_str("Selecionar com LCD\n");
    modo=sel_modo(teste_msg, TESTE_TOT);
    //modo=10;
    lcd_apaga();
    ser_crlf(1);
    switch(modo){
      case TESTE_1:  teste_1(modo);  break;
      case TESTE_2:  teste_2(modo);  break;
      case TESTE_3:  teste_3(modo);  break;
      case TESTE_4:  teste_4(modo);  break;
      case TESTE_5:  teste_5(modo);  break;
      case TESTE_6:  teste_6(modo);  break;
      case TESTE_7:  teste_7(modo);  break;
      case TESTE_8:  teste_8(modo);  break;
      case TESTE_9:  teste_9(modo);  break;
      case TESTE_10: teste_10(modo); break;
      case TESTE_11: teste_11(modo); break;
      case TESTE_12: teste_12(modo); break;
      case TESTE_13: teste_13(modo); break;
      case TESTE_14: teste_14(modo); break;
      case TESTE_15: teste_15(modo); break;
      case TESTE_16: teste_16(modo); break;
      case TESTE_17: teste_17(modo); break;
    }
  }
  while(1);
}

// 1 - LEDs
char teste_1(char md){
  char *msg="[1] TESTE dos LEDs";
  byte cont=0,x;
  lcd_str(0,0,msg);
  ser_str(msg); ser_crlf(1);
  while(TRUE){
    leds_cont(cont++);
    if (sw_tira(&x))     break;
    delay(500);
  }
  led_amo();
  led_vd();
  led_am();
  led_vm();
  ser_str(msg);
  ser_str("\n--- Fim ---\n");
  return md;
}

// 2 - LCD
char teste_2(char md){
  byte j,lin,ct=0,base;
  char *msg="[2] TESTE do LCD";
  lcd_str(0,0,msg);
  ser_str(msg);
  delay(1000);
  base=0;
  while(TRUE){
    ct=base;
    for (lin=0; lin<NRL; lin++){
      lcd_hex8(lin,0,ct);
      lcd_str(lin,2,": ");
      for (j=0; j<16; j++)  lcd_char(lin,j+4,ct++);
    }
    base+=16;
    delay(1000);
    if (sw_tira(&ct) == TRUE) break;
  }
  ser_str("\n--- Fim ---\n");
  return md;
}

// 3 - Teclado
char teste_3(char md){
  byte i;   //Para repetições com for
  byte x;   //Último valor lido pela ADC
  byte cod; //Código da tecla (x>>5)
  byte novo=99,velho=99;  //Evitar múltipla impressões porta serial
  char *msg="[3] TESTE das Teclas";
  lcd_apaga();
  lcd_str(0,0,msg);
  ser_str(msg);
  ser_crlf(1);
  while (TRUE){
    x=sw_val;
    cod=x>>5;
    novo=cod;
    lcd_float(1,0,5.0*x/256,2);
    lcd_str(1,5,"V=");
    lcd_hex8(1,7,x);
    lcd_str(1,9,"=> ");
    lcd_dec8unz(1,12,cod);
    lcd_str(1,13," = ");
    lcd_str(1,16,sw_nome[cod]);
    lcd_str(2,0,"Fila: pin=");
    lcd_dec8unz(2,10,sw_pin);
    lcd_str(2,12," pout=");
    lcd_dec8unz(2,18,sw_pout);
    i=1;
    while(i<sw_pin && i<6){
      lcd_str(3,4*(i-1),sw_nome[sw_fila[i]]);
      i++;
    }
    if (novo != velho){
      ser_str("A0 = ");
      ser_float(5.0*x/256,2);
      ser_str("V ==> ");
      ser_hex8(x);
      ser_str(" ==> ");
      ser_dec8unz(cod);
      ser_str(" = ");
      ser_str(sw_nome[cod]);
      ser_crlf(1);
      ser_str("Fila: pin=");  ser_dec8unz(sw_pin);
      ser_str(" pout=");  ser_dec8unz(sw_pout);
      ser_str(" [ ");
      i=1;
      while(i<sw_pin && i<SW_FILA_TAM){
        ser_str(sw_nome[sw_fila[i]]);
        ser_spc(1);
        i++;
      }
      if (sw_pin == sw_pout){
        for (i=0; i<SW_FILA_TAM; i++){
          ser_str(sw_nome[sw_fila[i]]);
          ser_spc(1);
          
        }
      }
      ser_char(']');
      if (sw_pin == sw_pout)  ser_str(" Fila Cheia!");
      ser_str("\n\n");
      velho=novo;
    }
  }
  sw_qq_tecla();
  ser_str("\n--- Fim ---\n");
  return md;
}

// 4 - TWI
// Verificar se os escravos TWI geram ACK
// Resultados no LCD e na serial
byte teste_4(char md){
  char *msg="[4] TWI (I2C)";
  lcd_apaga();
  lcd_str(0,0,msg);
  ser_str(msg);
  ser_crlf(1);

  twi_scan();
  lcd_str(1,0,"MPU9250 = ");
  if (twi_er_ok(MPU_ADR)) lcd_str(1,10,"OK "); 
  else                    lcd_str(1,10,"NOK");
  lcd_str(2,0,"FLASH 1 = ");
  if (twi_er_ok(FLASH1_ADR)) lcd_str(2,10,"OK "); 
  else                       lcd_str(2,10,"NOK");
  lcd_str(3,0,"FLASH 2 =  ");
  if (twi_er_ok(FLASH2_ADR)) lcd_str(3,10,"OK "); 
  else                       lcd_str(3,10,"NOK");
  sw_qq_tecla();
  ser_str("\n--- Fim ---\n");
  return;
}

// 5 - MPU - Acelerômetro e giroscópio
char teste_5(char md){
  byte who,i;
  int vt[7];
  float vtf[7];
  char *msg="[5] MPU6050 Acel e Giro";
  word ac_esc, giro_esc;
  lcd_apaga();
  lcd_str(0,0,msg);
  ser_str(msg);
  ser_crlf(1);
  
  mpu_acorda();     //Acordar MPU
  who=mpu_whoami();
  lcd_str(1,0,"Who am I = ");
  lcd_hex8(1,10,who);
  ser_str("MPU retornou Who am I = ");
  ser_hex8(who);
  if (who == 0x73){
    lcd_str(1,15," OK");  //MPU respondendo
    ser_str("h ==> OK!");
  }
  else{
    lcd_str(1,15," NOK");  //MPU respondendo
    ser_str("h ==> NOK! ERRO");
  }

  mpu_inicializa();     //Inicializar
  mpu_escalas(0,0);     //+/- 2g e +/-250gr/seg
  ac_esc=2;
  giro_esc=250;
  delay(2000);
  lcd_apaga();
  lcd_str(0,0,"Acel");
  lcd_str(1,3,"g");
  lcd_str(2,0,"Giro");
  lcd_str(3,0,"gr/s");
  while(TRUE){
    mpu_rd_ac_tp_gi(vt);
    vtf[0]=ac_esc*((float)vt[0]/32767);
    vtf[1]=ac_esc*((float)vt[1]/32767);
    vtf[2]=ac_esc*((float)vt[2]/32767);
    vtf[3]=36.53+(vt[3]/340);
    vtf[4]=giro_esc*((float)vt[4]/32767);
    vtf[5]=giro_esc*((float)vt[5]/32767);
    vtf[6]=giro_esc*((float)vt[6]/32767);
    
    lcd_hex16(0,5,vt[0]);  lcd_hex16(0,10,vt[1]);  lcd_hex16(0,15,vt[2]);
    lcd_float(1,5,vtf[0],1);
    lcd_float(1,10,vtf[1],1);
    lcd_float(1,15,vtf[2],1);
    
    lcd_hex16(2,5,vt[4]);  lcd_hex16(2,10,vt[5]);  lcd_hex16(2,15,vt[6]);
    lcd_apaga_lin(3);
    lcd_str(3,0,"gr/s");
    lcd_dec16nz(3,5,vtf[4]);
    lcd_dec16nz(3,10,vtf[5]);
    lcd_dec16nz(3,15,vtf[6]);
    
    ser_str("\nax=");   ser_hex16(vt[0]);   //ax
    ser_char('(');      ser_float(vtf[0],2);

    ser_str("g)  ay="); ser_hex16(vt[1]);   //ay
    ser_char('(');      ser_float(vtf[1],2);

    ser_str("g)  az="); ser_hex16(vt[2]); //az
    ser_char('(');      ser_float(vtf[2],2);
    
    ser_str("g)    tp=");   ser_hex16(vt[3]);
    ser_char('(');      ser_float(vtf[3],1);
    
    ser_str("C)    gx=");   ser_hex16(vt[4]);    //gx
    ser_char('(');      ser_float(vtf[4],2);
    
    ser_str("gr/s)  gy="); ser_hex16(vt[5]);  //gy
    ser_char('(');      ser_float(vtf[5],2);

    ser_str("gr/s)  gz="); ser_hex16(vt[6]);  //gz
    ser_char('(');      ser_float(vtf[6],2);
    ser_str("gr/s)");
    
    if (sw_tira(&who)){
      ser_str("\n--- Fim ---\n");
      return md;
    }
    delay(500);
  }
  return md;
}

// 6 - MPU - Magnetômetro
char teste_6(char md){
  byte who;
  int vetor[7];
  char *msg="[6] MPU6050 Magneto";
  lcd_apaga();
  lcd_str(0,0,msg);
  ser_str(msg);
  ser_crlf(1);

  while(TRUE){
    mpu_rd_mg(vetor);
    lcd_hex16(1,0,vetor[0]);
    lcd_hex16(1,6,vetor[1]);
    lcd_hex16(1,12,vetor[2]);
    ser_str("\nhx=");      ser_hex16(vetor[0]);
    ser_str("  hy=");      ser_hex16(vetor[1]);
    ser_str("  hz=");      ser_hex16(vetor[2]);
    if (sw_tira(&who))     break;
  }
  ser_str("\n--- Fim ---\n");
  return;
}

// 7 - SRAM  (23LC1024)
char teste_7(char md){
  word x,cont=0;
  byte y;
  char *msg="[7]U7=SRAM0 U5=SRAM1";
  lcd_apaga();
  lcd_str(0,0,msg);
  ser_str(msg);
  ser_crlf(1);
  ser_str("(23LC1024) U7 = SRAM0, PAG0 = 0x0 0000 --> 0x0 FFFF\n");
  ser_str("(23LC1024) U7 = SRAM0, PAG1 = 0x1 0000 --> 0x1 FFFF\n");
  ser_str("(23LC1024) U5 = SRAM1, PAG0 = 0x2 0000 --> 0x2 FFFF\n");
  ser_str("(23LC1024) U5 = SRAM1, PAG1 = 0x3 0000 --> 0x3 FFFF\n");
  x=TCNT0;  x=x^TCNT1;  x=x^TCNT2;
  x=x&0xF;
  if (x==0) x=13; //Semente
  rrand_inic_tudo(x,17,11);   //Inicializar gerador aleat
  lcd_str(1,5,"PG0 PG1 PG2 PG3");
  lcd_str(2,0,"Byte");
  lcd_str(3,0," Blk");
  
  while(TRUE){
    ser_crlf(1);
    cont++;
    ser_str("Ensaio = ");   ser_dec16unz(cont); ser_crlf(1);
    if (teste_7_8_byte(0,0))  lcd_str(2, 5," OK");
    else                       lcd_str(2, 5,"NOK");
    if (teste_7_8_byte(0,1))  lcd_str(2, 9," OK");
    else                       lcd_str(2, 9,"NOK");
    if (teste_7_8_byte(0,2))  lcd_str(2,13," OK");
    else                       lcd_str(2,13,"NOK");
    if (teste_7_8_byte(0,3))  lcd_str(2,17," OK");
    else                       lcd_str(2,17,"NOK");
    if (teste_7_8_bloco(0,0)) lcd_str(3, 5," OK");
    else                       lcd_str(3, 5,"NOK");
    if (teste_7_8_bloco(0,1)) lcd_str(3, 9," OK");
    else                       lcd_str(3, 9,"NOK");
    if (teste_7_8_bloco(0,2)) lcd_str(3,13," OK");
    else                       lcd_str(3,13,"NOK");
    if (teste_7_8_bloco(0,3)) lcd_str(3,17," OK");
    else                       lcd_str(3,17,"NOK");
    lcd_hex16(1,0,cont);
    if (sw_tira(&y))     break;        
    delay(1000);
  }
  ser_str("\n--- Fim ---\n");
  return;
}

// 8 - FLASH (24LC1025)
char teste_8(char md){
  word x,cont=0;
  byte y;
  char *msg="[8] U4=FSH0 U6=FSH1";
  lcd_apaga();
  lcd_str(0,0,msg);
  ser_str(msg);
  ser_crlf(1);
  ser_str("(24LC1025) U4 = FLASH0, PAG0 = 0x0 0000 --> 0x0 FFFF\n");
  ser_str("(24LC1025) U4 = FLASH0, PAG1 = 0x1 0000 --> 0x1 FFFF\n");
  ser_str("(24LC1025) U6 = FLASH1, PAG0 = 0x2 0000 --> 0x2 FFFF\n");
  ser_str("(24LC1025) U6 = FLASH1, PAG1 = 0x3 0000 --> 0x3 FFFF\n");
  x=TCNT0;  x=x^TCNT1;  x=x^TCNT2;
  x=x&0xF;
  if (x==0) x=13; //Semente
  rrand_inic_tudo(x,17,11);   //Inicializar gerador aleat
  lcd_str(1,5,"PG0 PG1 PG2 PG3");
  lcd_str(2,0,"Byte");
  lcd_str(3,0," Blk");
  
  while(TRUE){
    ser_crlf(1);
    cont++;
    ser_str("Ensaio = ");   ser_dec16unz(cont); ser_crlf(1);
    if (teste_7_8_byte(1,0))  lcd_str(2, 5," OK");
    else                       lcd_str(2, 5,"NOK");
    if (teste_7_8_byte(1,1))  lcd_str(2, 9," OK");
    else                       lcd_str(2, 9,"NOK");
    if (teste_7_8_byte(1,2))  lcd_str(2,13," OK");
    else                       lcd_str(2,13,"NOK");
    if (teste_7_8_byte(1,3))  lcd_str(2,17," OK");
    else                       lcd_str(2,17,"NOK");
    if (teste_7_8_bloco(1,0)) lcd_str(3, 5," OK");
    else                       lcd_str(3, 5,"NOK");
    if (teste_7_8_bloco(1,1)) lcd_str(3, 9," OK");
    else                       lcd_str(3, 9,"NOK");
    if (teste_7_8_bloco(1,2)) lcd_str(3,13," OK");
    else                       lcd_str(3,13,"NOK");
    if (teste_7_8_bloco(1,3)) lcd_str(3,17," OK");
    else                       lcd_str(3,17,"NOK");
    lcd_hex16(1,0,cont);
    if (sw_tira(&y))     break;        
    delay(1000);
  }
  ser_str("\n--- Fim ---\n");
  return;
}


// Testear acesso byte
// Tipo=0==>SRAM  Tipo=1==>FLASH
// qual = 0 -> (U7) SRAM0, PAG0 (0 0000 -> 0 FFFF) 
// qual = 1 -> (U7) SRAM0, PAG1 (1 0000 -> 1 FFFF) 
// qual = 2 -> (U5) SRAM1, PAG2 (2 0000 -> 2 FFFF) 
// qual = 3 -> (U5) SRAM1, PAG3 (3 0000 -> 3 FFFF) 
char teste_7_8_byte(char tipo, char qual){
  long adr;
  byte d1,d2;
  adr=rrand_fx(15);
  adr=(adr<<4) | rrand_fx(15);
  adr=(adr<<4) | rrand_fx(15);
  adr=(adr<<4) | rrand_fx(15);
  adr=adr | ((long)qual<<16);

  // Gravar e ler
  if (tipo==0){        //SRAM
    d1=rrand_fx(15);
    sram_wr(adr,d1);
    d2=sram_rd(adr);
    ser_str("SRAM");
  }
  else{               //FLASH
    flash_wr(adr,d1); 
    flash_espera(adr);  //Esperar gravação
    d2=flash_rd(adr);
    ser_str("FLASH");
  }
  ser_dec8unz(qual>>1);

  ser_str(", PAG");  ser_dec8unz(qual&1);
  ser_str(", ADR=");  ser_hex32(adr);
  ser_str(" ==> WR("); ser_hex8(d1);
  ser_str(") RD("); ser_hex8(d2);
  if (d1==d2) { ser_str(") ==> Byte OK\n");  return TRUE; }
  else        { ser_str(") ==> Byte NOK\n"); return FALSE;}
}

// Testear SRAM acesso em bloco
// Tipo=0==>SRAM  Tipo=1==>FLASH
// 0 = (U7) SRAM0, PAG0 (0 0000 -> 0 FFFF) 
// 1 = (U7) SRAM0, PAG1 (1 0000 -> 1 FFFF) 
// 2 = (U5) SRAM1, PAG2 (2 0000 -> 2 FFFF) 
// 3 = (U5) SRAM1, PAG3 (3 0000 -> 3 FFFF) 
char teste_7_8_bloco(char tipo, char qual){
  long adr;
  byte i,erro,d1[16],d2[16];
  adr=rrand_fx(15);
  adr=(adr<<4) | rrand_fx(15);
  adr=(adr<<4) | rrand_fx(15);
  adr=(adr<<4) | rrand_fx(15);
  adr=adr | ((long)qual<<16);
  adr &= 0xFFFF0; //Abrir espaço para 16 endereços
  for (i=0; i<16; i++)  d1[i]=rrand_fx(15);

  // Gravar e ler
  if (tipo==0){              //SRAM
    sram_wr_blk(adr,d1,16);
    sram_rd_blk(adr,d2,16);
    ser_str("SRAM");
  }
  else{                    //FLASH
    flash_wr_blk(adr,d1,16);
    flash_espera(adr);    //Esperar gravação
    flash_rd_blk(adr,d2,16);
    ser_str("FLASH");  
  }

  erro=0;
  for (i=0; i<16; i++){
    if (d1[i]!=d2[i]) erro++;
  }
  ser_dec8unz(qual>>1);
  
  ser_str(", PAG");  ser_dec8unz(qual&1);
  ser_str(", ADR=");  ser_hex32(adr);
  ser_str(" ==> WR(");
  for (i=0; i<15; i++){
    ser_hex8(d1[i]);
    ser_char(',');
  }
  ser_hex8(d1[15]);
  ser_str(") RD(");
  for (i=0; i<15; i++){
    ser_hex8(d2[i]);
    ser_char(',');
  }
  ser_hex8(d2[15]);
  ser_str(") Erros=");
  ser_dec8unz(erro);
  if (erro==0){ ser_str(" ==> Bloco OK\n");  return TRUE; }
  else        { ser_str(" ==> Bloco NOK\n"); return FALSE;}
}

// Mostar tudo que chega do GPS
char teste_9(char md){
  char *msg="[9] GPS Tudo";
  word tipos[7];    //Contar tipos de mensagens
  byte dado;
  byte tp;
  byte cont=0,cmax=0;
  gps_st=0;
  for (dado=0; dado<7; dado++)  tipos[dado]=0;
  lcd_apaga();
  lcd_str(0,0,msg);
  ser_str(msg);
  ser_crlf(1);
  delay(2000);
  lcd_str(0,0,"GPRMC  GPVTG  GPGGA");
  lcd_str(2,0,"GPGSA  GPGSV  GPGLL");
  //gps_rx_fila_config();   //GPS RX fila serial
  UCSR3B |= (1<<RXEN3) ;      //habilitar RX
  while(TRUE){
    while( (UCSR3A&(1<<RXC3)) == 0);   //Esperar chegar
    dado=UDR3;
    if (dado == '$'){
      if (cont>cmax)  cmax=cont;
      ser_crlf(1);
      ser_dec8u(cont);
      ser_spc(1);
      ser_dec8u(cmax);
      cont=0;
      ser_crlf(1);
    }
    cont++;
    ser_hex8(dado);
    ser_spc(1);
    
    //ser_char(dado);
    tp=teste_9_gps_msg(dado);
    tipos[tp]++;
    lcd_dec16u(1,0,tipos[1]);
    lcd_dec16u(1,7,tipos[2]);
    lcd_dec16u(1,14,tipos[3]);
    lcd_dec16u(3,0,tipos[4]);
    lcd_dec16u(3,7,tipos[5]);
    lcd_dec16u(3,14,tipos[6]);
    if (sw_tira(&dado))   break;
 }
  ser_str("\n--- Fim ---\n");
  return;
}

//Identificar tipos de mensagens do GPS
byte teste_9_gps_msg(char letra){
  switch(gps_st){
    case  0: if (letra=='$') gps_st=1;
             return GPS_NADA;

    case  1: if (letra=='G') gps_st=2;
             else            gps_st=0;
             return GPS_NADA;
    
    case  2: if (letra=='P') gps_st=3;
             else             gps_st=0;
             return GPS_NADA;
    
    case  3: if (letra=='R')     gps_st=4;
             else if(letra=='V') gps_st=6;
             else if(letra=='G') gps_st=8;
             else                gps_st=0;
             return GPS_NADA;
             
    case  4: if (letra=='M') gps_st=5;
             else            gps_st=0;
             return GPS_NADA;
    
    case  5: gps_st=0;
             if (letra=='C') return GPS_RMC;
             return GPS_NADA;

    case  6: if (letra=='T') gps_st=7;
             else            gps_st=0;
             return GPS_NADA;

    case  7: gps_st=0;
             if (letra=='G') return GPS_VTG;
             return GPS_NADA;

    case  8: if (letra=='L')     gps_st=9;
             else if(letra=='G') gps_st=10;
             else if(letra=='S') gps_st=11;
             else                gps_st=0;
             return GPS_NADA;
    
    case  9: gps_st=0;
             if (letra=='L') return GPS_GLL;
             return GPS_NADA;

    case 10: gps_st=0;
             if (letra=='A') return GPS_GGA;
             return GPS_NADA;
             
    case 11: gps_st=0;
             if (letra=='A')     return GPS_GSA;
             else if(letra=='V') return GPS_GSV;
             return GPS_NADA;
  }
}

// Extrair e interpretar as mensagens do GPS
char teste_10(char md){
  char *msg="[10] GPS Interpreta";
  byte i,dado;
  byte msg_tipo;
  lcd_apaga();
  lcd_str(0,0,msg);
  ser_str(msg);
  ser_crlf(1);
  delay(2000);
  lcd_apaga_lin(0);

  //Zerar todo o vetor com os dados do extraídos do GPS
  for (i=0; i<GPS_DADOS_TAM; i++)  gps_dados[i]=0;
  
  // RX3: Habilitar recepção e sua interrupção
  UCSR3B = (1<<RXCIE3) | (1<<RXEN3);  //RXIE=1, RXEN=1

  gps_msg_ok=FALSE;
  while(TRUE){
    while(gps_msg_ok == FALSE);
    gps_msg_ok=FALSE;
    if (gps_msg_fase==0){
      ser_str(gps_msg_1);
      ser_crlf(1);
      gps_extrai(gps_msg_1);
    }
    else{
      ser_str(gps_msg_0);
      ser_crlf(1);
      gps_extrai(gps_msg_0);
    }
    ser_gps_dados(gps_dados);
    ser_crlf(2);
    lcd_gps_data(0,0);
    lcd_gps_hora(0,9);
    lcd_gps_lat(1,1);
    lcd_gps_long(2,0);
    lcd_gps_vel_kph(3,0);
    lcd_gps_dado(0,18,GPS_QTD_SAT);
    lcd_gps_dado(1,14,GPS_PDOP);
    lcd_gps_dado(2,14,GPS_ALT);
    lcd_gps_dado(2,19,GPS_ALT_UN);
    lcd_gps_dado(3,11,GPS_CURSO);
    lcd_gps_dado(3,19,GPS_STATUS);

    if (sw_tira(&dado))     break;    

  }
  // RX3: Desabilitar recepção e sua interrupção
  UCSR3B &= ~((1<<RXCIE3) | (1<<RXEN3));  //RXIE=0, RXEN=0

  sw_qq_tecla();
  return md;
}


// 11 - GPS ==> u-Center
char teste_11(char md){
  char *msg="[11] GPS-->u-Center";
  byte dado;
  byte flag_pc=0, flag_gps=0; //Marcar TX ou RX
  word cpc=0,cgps=0;  //Contadores de tráfego
  gps_st=0;
  lcd_apaga();
  lcd_str(0,0,msg);
  lcd_str(2,0,"GPS <== PC");
  lcd_str(3,0,"GPS ==> PC");
  lcd_dec16u(2,12,cpc);
  lcd_dec16u(3,12,cgps);
  ser_str(msg);
  ser_crlf(1);
  UCSR0B = (1<<RXEN0) | (1<<TXEN0);  //Só habilita TX e RX (Desab Interrup)
  UCSR3B = (1<<RXEN3) | (1<<TXEN3);  //Só habilita TX e RX

  while(TRUE){
    
    // PC --> GPS
    if ( (UCSR0A&(1<<RXC0)) == (1<<RXC0)){
      dado=UDR0;
      UDR3=dado;
      UCSR0A |= 1<<RXC0;  //Apagar flag RXC0
      cpc++;
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
      lcd_dec16u(2,12,cpc);
    }

    if (flag_gps == TRUE){
      flag_gps = FALSE;
      lcd_dec16u(3,12,cgps);
    }
  
    if (sw_tira(&dado))     break;    
  }
  // Voltar habilitar interrupções
  UCSR0B = (1<<RXCIE0) | (1<<TXCIE0) | (1<<RXEN0) | (1<<TXEN0);  //RXIE=TXIE=1, hab TX e RX
  ser_str("\n--- Fim ---\n");
  return md;
}

// 12 - MPU ==> MatLab
char teste_12(char md){
  char *msg="[12] MPU --> Matlab";
  byte who;
  word vt[7];
  float vtf[7];
  word ac_esc, giro_esc;
  lcd_apaga();
  lcd_str(0,0,msg);
  ser_str(msg);
  ser_crlf(1);

  mpu_acorda();     //Acordar MPU
  who=mpu_whoami();
  if (who != 0x73){
    lcd_str(1,0,"MPU nao responde!");  //MPU Não responde
    ser_str("MPU nao responde!");  //MPU Não responde
    sw_qq_tecla();
    ser_str("\n--- Fim ---\n");
    return  FALSE;
  }

  //lcd_cursor_lc(1,0);
  //lcd_str(2,0,"Qq tecla inicia TX");
  //while(sw_tira(&who)==FALSE);
  mpu_inicializa();     //Inicializar
  mpu_escalas(0,0);     //+/- 2g e +/-250gr/seg
  ac_esc=2;
  giro_esc=250;
  delay(500);
  lcd_apaga();
  lcd_str(0,0,"Acel");
  lcd_str(1,3,"g");
  lcd_str(2,0,"Giro ");
  lcd_str(3,0,"gr/s ");
  ser_dec16(0xFFFF);
  ser_crlf(1);

  while(TRUE){
    mpu_rd_ac_tp_gi(vt);  //Ler MPU

    // Calcular acelerações e giros
    vtf[0]=ac_esc*((float)vt[0]/32767);
    vtf[1]=ac_esc*((float)vt[1]/32767);
    vtf[2]=ac_esc*((float)vt[2]/32767);
    vtf[3]=36.53+(vt[3]/340);
    vtf[4]=giro_esc*((float)vt[4]/32767);
    vtf[5]=giro_esc*((float)vt[5]/32767);
    vtf[6]=giro_esc*((float)vt[6]/32767);

    // LCD
    lcd_hex16(0,5,vt[0]);  lcd_hex16(0,10,vt[1]);  lcd_hex16(0,15,vt[2]);
    lcd_float(1,5,vtf[0],1);
    lcd_float(1,10,vtf[1],1);
    lcd_float(1,15,vtf[2],1);
    
    lcd_hex16(2,5,vt[4]);  lcd_hex16(2,10,vt[5]);  lcd_hex16(2,15,vt[6]);
    lcd_apaga_lin(3);
    lcd_str(3,0,"gr/s");
    lcd_dec16nz(3,5,vtf[4]);
    lcd_dec16nz(3,10,vtf[5]);
    lcd_dec16nz(3,15,vtf[6]);

    //Enviar pela porta serial
    ser_dec16(vt[0]);   ser_crlf(1);           //ax
    ser_dec16(vt[1]);   ser_crlf(1);           //ay
    ser_dec16(vt[2]);   ser_crlf(1);           //az
    ser_dec16(vt[3]);   ser_crlf(1);           //temperatura
    ser_dec16(vt[4]);   ser_crlf(1);           //gx
    ser_dec16(vt[5]);   ser_crlf(1);           //gy
    ser_dec16(vt[6]);   ser_crlf(1);           //gz
    
    if (sw_tira(&who))     break;    
  }
  ser_str("\n--- Fim ---\n");
  return md;
}

// 13 - BlueTooth
char teste_13(char md){
  lcd_str(0,0,teste_msg[md]);
  ser_str(teste_msg[md]);
  sw_qq_tecla();
  return md;
}

// 14 - Vazio
char teste_14(char md){
  lcd_str(0,0,teste_msg[md]);
  ser_str(teste_msg[md]);
  sw_qq_tecla();
  return md;
}

// 15 - Vazio
char teste_15(char md){
  lcd_str(0,0,teste_msg[md]);
  ser_str(teste_msg[md]);
  sw_qq_tecla();
  return md;
}

// 16 - Vazio
char teste_16(char md){
  lcd_str(0,0,teste_msg[md]);
  ser_str(teste_msg[md]);
  sw_qq_tecla();
  return md;
}

// 17 - Vazio
char teste_17(char md){
  lcd_str(0,0,teste_msg[md]);
  ser_str(teste_msg[md]);
  sw_qq_tecla();
  return md;
}


// Selecionar o modo de operação
byte teste_sel(void){
  byte prov=TESTE_TOT;    //provisório = 1, 2, ..., TESTE_TOT
  byte tecla,aux;
  while(TRUE){
    lcd_apaga_lin(1);
    lcd_apaga_lin(2);
    lcd_apaga_lin(3);
    lcd_str(2,0,"-->");
    aux=prov;
    lcd_str(1,3,teste_msg[aux++]);
    if (aux>TESTE_TOT)  aux=1;    
    lcd_str(2,3,teste_msg[aux]);
    ser_str(teste_msg[aux++]);  ser_crlf(1);
    if (aux>TESTE_TOT)  aux=1;    
    lcd_str(3,3,teste_msg[aux]);
    //Esperar tecla
    while ( sw_tira(&tecla) == FALSE);
    switch(tecla){
      case SW_SUP:  prov--;  break;
      case SW_INF:  prov++;  break;
      case SW_SEL:  if (++prov>TESTE_TOT)  prov=1;
                    return prov;
    }
    if (prov>TESTE_TOT)  prov=1;
    if (prov==0)         prov=TESTE_TOT;
  }
}
