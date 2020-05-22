// EEPROM - Rotinas básicas
// CXP - Caixa Preta
// 19/04/2020
// ATMEGA2560 tem 4KB (4.906) bytes de EEPROM 0x0000 --> 0x3FFF

// Imprimir só os dados da Calibração de Fábrica gravada na EEPROM
byte eeprom_cf_dados(void){
  int x;
  long z;
  byte msg[30];

  x=eeprom_rd_16b(CF_OK);   //Calibração de Fábrica ?
  ser_dec16(x); ser_crlf(1);

  if (x != 0x5353) return FALSE; //Sem calibração, retorna

  eeprom_rd_str(CF_DATA,msg,26);            //Data
  ser_str(msg); ser_crlf(1);
  
  eeprom_rd_str(CF_LOCAL,msg,26);           //Local
  ser_str(msg); ser_crlf(1);

  eeprom_rd_str(CFG_PADRAO,msg,26);        //g Padrao
  ser_str(msg); ser_crlf(1);
  
  eeprom_rd_str(CFG_LOCAL,msg,26);        //g Local
  ser_str(msg); ser_crlf(1);

  x=eeprom_rd_16b(CFG_PADRAO_BIN);        //g Padrao binário
  ser_dec16u(x); ser_crlf(1);

  x=eeprom_rd_16b(CFG_LOCAL_BIN);        //g Local binário
  ser_dec16u(x); ser_crlf(1);

  x=eeprom_rd_16b(CF_WHO);      //Resposta ao Who am I
  ser_hex16(x); ser_crlf(1);
  
  x=eeprom_rd_16b(CF_FA);               //Freq de amostragem
  ser_dec16unz(x); ser_crlf(1);

  eeprom_rd_blk(CF_AX,msg,14);        //Médias = Erro intrínseco
  ser_lin_ac_tp_gi(msg); //ser_crlf(1);

  x=eeprom_rd_16b(CF_FA);               //Qtd de medidas para a média
  ser_dec16unz(x); ser_crlf(1);

  eeprom_rd_blk(CF_AX_SOMA,msg,28);   //Somatorio por eixo (32 bits): 
  for (x=0; x<28; x+=4){
    z=msg[x];
    z = (z<<8)+msg[x+1];
    z = (z<<8)+msg[x+2];
    z = (z<<8)+msg[x+3];
    ser_dec32(z);
    ser_spc(1);
  }
  ser_crlf(1);

  eeprom_rd_blk(CF_AX_PRI,msg,14);    //Primeira medida
  ser_lin_ac_tp_gi(msg);

  eeprom_rd_blk(CF_AX_ULT,msg,14);    //Ultima medida
  ser_lin_ac_tp_gi(msg);
 
  x=eeprom_rd_16b(CF_ST_OK);    //Self Test 
  ser_dec16(x); ser_crlf(1);

  eeprom_rd_blk(CF_ST_OFF_AX,msg,12); //Leituras Self Test OFF
  ser_lin_ac_gi(msg);

  eeprom_rd_blk(CF_ST_ON_AX,msg,12); //Leituras Self Test ON
  ser_lin_ac_gi(msg);

  eeprom_rd_blk(CF_ST_REG_AX,msg,12); //Leituras Regs de Self Test
  ser_lin_ac_gi(msg);
  
  eeprom_rd_blk(CF_ST_TOL_AX,msg,12); //Resultado Self test
  ser_lin_ac_gi(msg);

  return TRUE;
}

// Mostrar Calibração de Fábrica gravada na EEPROM
byte eeprom_cf_mostra(void){
  int x;
  long z;
  byte msg[30];

  ser_str("\n--- EEPROM: Dados da Calibração de Fábrica ---");
  // Verificar se já fez calibração ASCII(S)=0x53
  x=eeprom_rd_16b(CF_OK);
  if (x != 0x5353){
    ser_str("\nNunca fez calibragem!\n");
    //eeprom_dump(0,256);
    return FALSE;                    
  }

  eeprom_rd_str(CF_DATA,msg,26);       
  ser_str("\nData = ");   ser_str(msg);           //Data
  eeprom_rd_str(CF_LOCAL,msg,26);       
  ser_str("\nLocal = ");   ser_str(msg);          //Local
  eeprom_rd_str(CFG_PADRAO,msg,26);        
  ser_str("\ng Padrao = ");   ser_str(msg);       //g Padrao
  x=eeprom_rd_16b(CFG_PADRAO_BIN);                
  ser_str(" (");  ser_dec16u(x);  ser_str(")");
  eeprom_rd_str(CFG_LOCAL,msg,26);        
  ser_str("\ng Local = ");   ser_str(msg);        //g Local
  x=eeprom_rd_16b(CFG_LOCAL_BIN);                
  ser_str(" (");  ser_dec16u(x);  ser_str(")");
  x=eeprom_rd_16b(CF_WHO);                        
  ser_str("\nWho am I = 0x");   ser_hex16(x);     //Resposta ao Who am I
  x=eeprom_rd_16b(CF_FA);                    
  ser_str("\n\nFreq (Hz) Amost = ");   ser_dec16unz(x);  //Freq de amostragem

  ser_crlf(1);
  ser_spc(20);
  ser_str("ax     ay     az     tp     gx     gy     gz");
  ser_str("\nErro intinseco:   ");
  eeprom_rd_blk(CF_AX,msg,14);
  ser_lin_ac_tp_gi(msg);

  ser_str("Primeira medida:  ");
  eeprom_rd_blk(CF_AX_PRI,msg,14);
  ser_lin_ac_tp_gi(msg);

  ser_str("Ultima medida:    ");
  eeprom_rd_blk(CF_AX_ULT,msg,14);
  ser_lin_ac_tp_gi(msg);

  ser_str("Somatorio por eixo (32 bits): ");
  eeprom_rd_blk(CF_AX_SOMA,msg,28);
  for (x=0; x<28; x+=4){
    z=msg[x];
    z = (z<<8)+msg[x+1];
    z = (z<<8)+msg[x+2];
    z = (z<<8)+msg[x+3];
    ser_dec32(z);
    ser_spc(1);
  }
  ser_crlf(1);
  ser_dec16unz(eeprom_rd_16b(CF_QTD));
  ser_str(" = Medidas para a Media\n");

  
  ser_str("\nSelf Test = ");
  x=eeprom_rd_16b(CF_ST_OK);
  if (x==COD_SIM) ser_str("OK!\n");
  else            ser_str("FALHOU!\n");

  ser_spc(17);
  ser_str("ax     ay     az     gx     gy     gz\n");
  ser_str("Self Test OFF: ");
  eeprom_rd_blk(CF_ST_OFF_AX,msg,12);
  ser_lin_ac_gi(msg);
  
  ser_str("Self Test ON:  ");
  eeprom_rd_blk(CF_ST_ON_AX,msg,12);
  ser_lin_ac_gi(msg);

  ser_str("Registradores: ");
  eeprom_rd_blk(CF_ST_REG_AX,msg,12);
  ser_lin_ac_gi(msg);
  
  ser_str("Result (<14%): ");
  eeprom_rd_blk(CF_ST_TOL_AX,msg,12);
  ser_lin_ac_gi(msg);

  ser_str("--- EEPROM: Fim dos Dados da Calibração de Fábrica ---\n\n");
  //sram_op_mostra();
  //sram_dump(CXP_ADR_INI,256);

  return TRUE;
}

// Dump da EEPROM
void eeprom_dump(word adr, word qtd){
  word i=0;
  byte vt[16];
  for (i=0; i<qtd; i+=16){
    if ( (i%256) == 0)  ser_crlf(1);
    eeprom_rd_blk(adr+i,vt,16);
    ser_dump_memo(adr+i,vt);
  }
}

// Ler um valor de 32 bits da EEPROM
long eeprom_rd_32b(word adr){
  long x;
  x=eeprom_rd(adr++);           //MSB
  x = (x<<8)|eeprom_rd(adr++);  
  x = (x<<8)|eeprom_rd(adr++);  
  x = (x<<8)|eeprom_rd(adr);    //LSB
  return x;
}

// Escrever um valor de 32 bits da EEPROM
void eeprom_wr_32b(word adr, long dado){
  eeprom_wr(adr++,(dado>>24)&0xFF); //MSB
  eeprom_wr(adr++,(dado>>16)&0xFF);
  eeprom_wr(adr++,(dado>>8)&0xFF);
  eeprom_wr(adr,dado&0xFF);         //LSB
}

// Ler um valor de 16 bits da EEPROM
int eeprom_rd_16b(word adr){
  int x;
  x=eeprom_rd(adr++);             //MSB
  x = (x<<8)|eeprom_rd(adr);      //LSB
  return x;
}

// Escrever um valor de 16 bits da EEPROM
void eeprom_wr_16b(word adr, word dado){
  int x;
  eeprom_wr(adr++,dado>>8);     //MSB
  eeprom_wr(adr,dado&0xFF);     //LSB
}

// Ler uma string da EEPROM
// O ponteiro msg deve ter espaço adequado
// Qtd indica a quantidade máxima, o último byte é o zero final
void eeprom_rd_str(word adr, byte *msg, word qtd){
  word i=0,cont=0;
  byte x;
  if (qtd > 0){
    do{
      x=eeprom_rd(adr++);
      msg[i++]=x;
      cont++;
    }
    while( x!='\0' && cont<qtd);
    if (cont==qtd)  msg[qtd-1]='\0';
  }
  else  msg[0]='\0';
}

// Gravar uma string na EEPROM
void eeprom_wr_str(word adr, byte *msg){
  word i=0;
  while( msg[i] != '\0')  eeprom_wr(adr++, msg[i++]);
  eeprom_wr(adr, '\0');   //Gravar o zero final
}

// Gravar 0xFF em toda a EEPROM
void eeprom_wr_ff(void){
  word adr;
  for (adr=0; adr<EEPROM_TAM; adr++)
    eeprom_wr(adr, 0xFF);
}

// Ler sequencia de bytes da EEPROM
void eeprom_rd_blk(word adr, byte *vet, word qtd){
  word cont;
  for (cont=0; cont<qtd; cont++)
    vet[cont]=eeprom_rd(adr+cont);
}

// Gravar sequencia de bytes na EEPROM
void eeprom_wr_blk(word adr, byte *vet, word qtd){
  word cont;
  for (cont=0; cont<qtd; cont++)
    eeprom_wr(adr+cont, vet[cont]);
}

// Ler um byte da EEPROM
byte eeprom_rd(word adr){
  while( EECR & (1<<EEPE));   //Esperar terminar gravação anterior
  EEAR = adr&0x3FFF;           //Endereço truncado em 12 bits
  EECR |= (1<<EERE);          //Programar leitura
  return EEDR;                //Retornar valor lico
}

// Gravar um byte na EEPROM
void eeprom_wr(word adr, byte dado){
  while( EECR & (1<<EEPE));   //Esperar gravação anterior
  EEAR = adr&0x3FFF;           //Endereço truncado em 12 bits
  EEDR = dado;                //Dado
  cli();                      //Desabilita interrupts
  EECR |= (1<<EEMPE);         //Master Prog = 1
  EECR |= (1<<EEPE);          //Gravar
  sei();                      //Reabilita interrupts
}
