// Flash (I2C) - Rotinas para acesso Usa TWI
// CXP - Caixa Preta
// 22/01/2019
//
// 24LC1025 - Flash TWI de 128 KB

// (90) Escrever um bloco com FF
// qtd --> precisa estar dentro de página de 128 bytes
void flash_apg_blk(long adr, byte qtd){
  byte ct;
  byte er,et;

  // Selecionar endereço do escravo
  er=flash_qual(adr);
  er=er<<1;   //Escravo receptor
  et=er+1;    //Escravo transmissor

  twi_start(90);                //START
  twi_er(er, 91);        //Endereçar 24LCxxx para escrita
  twi_dado_er(adr>>8, 92);      //MSB do endereço
  twi_dado_er(adr&0xFF, 93);    //LSB do endereço
  for (ct=0; ct<qtd; ct++)
    twi_dado_er(0xFF, 94);      //0xFF a ser escrito
  twi_stop();                   //Gerar STOP para finalizar
}

// (80) Ler um bloco a partir de um endereço
// qtd --> não há limitação dentro dos 16 bits (64 K)
void flash_rd_blk(long adr,  byte *vet, word qtd){
  word ct;
  byte er,et;

  //ser_str("\nEntrou flash rd adr = ");
  //ser_hex32(adr);

  // Selecionar endereço do escravo
  er=flash_qual(adr);
  er=er<<1;   //Escravo receptor
  et=er+1;    //Escravo transmissor

  twi_start(80);                //START
  twi_er(er, 81);        //Endereçar 24LCxxx para escrita
  twi_dado_er(adr>>8, 82);      //MSB do endereço
  twi_dado_er(adr&0xFF, 83);    //LSB do endereço
  twi_start_rep(84);            //START Repetido
  twi_et(et, 85);        //Endereçar 24LCxxx para leitura
  for (ct=0; ct<qtd-1; ct++)
    vet[ct]=twi_dado_et_ack(86);//Ler um dado com ACK
  vet[ct]=twi_dado_et_nack(87); //Ler último dado com NACK
  twi_stop();                   //Gerar STOP para finalizar
  //ser_str(" Saiu flash rd\n");
}

// (70) Escrever um bloco a partir de um endereço
// qtd --> precisa estar dentro de página de 128 bytes
void flash_wr_blk(long adr, byte *vet, byte qtd){
  byte ct;
  byte er,et;

//  ser_str("\nEntrou flash wr adr = ");
//  ser_hex32(adr);
  // Selecionar endereço do escravo
  er=flash_qual(adr);
  er=er<<1;   //Escravo receptor
  et=er+1;    //Escravo transmissor

  twi_start(70);                //START
  twi_er(er, 71);        //Endereçar 24LCxxx para escrita
  twi_dado_er(adr>>8, 72);      //MSB do endereço
  twi_dado_er(adr&0xFF, 73);    //LSB do endereço
  for (ct=0; ct<qtd; ct++)
    twi_dado_er(vet[ct], 74);   //Dado  a ser escrito
  twi_stop();                   //Gerar STOP para finalizar
//  ser_str(" Saiu flash wr\n");
}

// (50) Ler um endereço da memória
byte flash_rd(long adr) {
  byte dado,er,et;

  // Selecionar endereço do escravo
  er=flash_qual(adr);
  //ser_str("er=");
  //ser_hex8(er);
  //ser_crlf(1);
  er=er<<1;   //Escravo receptor
  et=er+1;    //Escravo transmissor
  
  twi_start(50);                //START
  twi_er(er, 51);               //Endereçar Escravo receptor
  twi_dado_er(adr>>8, 52);      //MSB do endereço
  twi_dado_er(adr&0xFF, 53);    //LSB do endereço
  twi_start_rep(54);            //START Repetido
  twi_et(et, 55);               //Endereçar Escravo transmissor
  dado = twi_dado_et_nack(56);  //Receber dado da 24LCxxx com NACK
  twi_stop();                   //Gerar STOP para finalizar
  return dado;
}

// (60) Escrever num endereço da memória
void flash_wr(long adr, byte dado) {
  byte er,et;

  // Selecionar endereço do escravo
  er=flash_qual(adr);
  //ser_str("er=");
  //ser_hex8(er);
  //ser_crlf(1);
  er=er<<1;   //Escravo receptor
  et=er+1;    //Escravo transmissor
  
  twi_start(60);                //START
  twi_er(er, 61);        //Endereçar 24LCxxx para escrita
  twi_dado_er(adr>>8, 62);      //MSB do endereço
  twi_dado_er(adr&0xFF, 63);    //LSB do endereço
  twi_dado_er(dado, 64);        //Dado  a ser escrito
  twi_stop();                   //Gerar STOP para finalizar
}

// Retornar endereço TWI da flash em função do endereço a ser acessado.
byte flash_qual(long adr){
  byte qual;
  qual=(adr>>16);
  switch(qual){
    case 0: return  FLASH1_ADR+0;  //0 0000 --> 0 FFFF
    case 1: return  FLASH1_ADR+4;  //1 0000 --> 1 FFFF
    case 2: return  FLASH2_ADR+0;  //2 0000 --> 2 FFFF
    case 3: return  FLASH2_ADR+4;  //3 0000 --> 3 FFFF
  }
}

// (100) Esperar Flash terminar gravação
// Ficar acessando com WR até ela responder
void flash_espera(long adr){
  byte er;
  er=flash_qual(adr);
  er=er<<1;
  while(TRUE){
    twi_start(100);                //START
    if (twi_er_check(er, 101) == TRUE){
      twi_stop();
      break;
    }
    twi_stop();
  }
}
