// SRAM (SPI) - Rotinas para acesso
// CXP - Caixa Preta
// 22/01/2019
//
// 23LC1024 - SRAM SPI de 128 KB

// Escrever uma sequência
// se adr = 0x0 0000 -> 0x1 FFFF, SRAM0 #CS0
// se adr = 0x2 0000 -> 0x3 FFFF, SRAM1 #CS1
void sram_wr_blk(long adr, byte *vet, word qtd){
  word cont;
  if ((adr&0xF0000L)<0x20000L)  spi_cs0();  //Selecionar SRAM0
  else                          spi_cs1();  //Selecionar SRAM1
  spi_transf(SRAM_WRITE);      //Indicar escrita
  spi_transf((byte)(adr >> 16) & 0xff); //ender (23..16)
  spi_transf((byte)(adr >> 8) & 0xff);  //ender (15..8)
  spi_transf((byte)adr);                //ender (7..0)
  // Escrever os bytes indicados
  for (cont=0; cont<qtd; cont++)  spi_transf(vet[cont]);
  spi_CS0();  //Mais fácil desabilitar as duas
  spi_CS1();  //Mais fácil desabilitar as duas
}

// Ler uma sequência
// se adr = 0x0 0000 -> 0x1 FFFF, SRAM0 #CS0
// se adr = 0x2 0000 -> 0x3 FFFF, SRAM1 #CS1
void sram_rd_blk(long adr, byte *vet, word qtd){
  word cont;
  if ((adr&0xF0000L)<0x20000L)  spi_cs0();  //Selecionar SRAM0
  else                          spi_cs1();  //Selecionar SRAM1
  spi_transf(SRAM_READ);       //Indicar leitura
  spi_transf((byte)(adr >> 16) & 0xff); //ender (23..16)
  spi_transf((byte)(adr >> 8) & 0xff);  //ender (15..8)
  spi_transf((byte)adr);                //ender (7..0)
  // Ler os bytes indicados
  for (cont=0; cont<qtd; cont++)  vet[cont] = spi_transf(0x00);
  spi_CS0();  //Mais fácil desabilitar as duas
  spi_CS1();  //Mais fácil desabilitar as duas
}

// Escrever o dado no endereço adr da SRAM
// se adr = 0x0 0000 -> 0x1 FFFF, SRAM0 #CS0
// se adr = 0x2 0000 -> 0x3 FFFF, SRAM1 #CS1
void sram_wr(long adr, byte dado){
  if ((adr&0xF0000L)<0x20000L)  spi_cs0();  //Selecionar SRAM0
  else                          spi_cs1();  //Selecionar SRAM1
  spi_transf(SRAM_WRITE);      //Indicar escrita
  spi_transf((byte)(adr >> 16) & 0xff); //ender (23..16)
  spi_transf((byte)(adr >> 8) & 0xff);  //ender (15..8)
  spi_transf((byte)adr);                //ender (7..0)
  spi_transf(dado);       //Escrever um Byte
  spi_CS0();  //Mais fácil desabilitar as duas
  spi_CS1();  //Mais fácil desabilitar as duas
}

// Ler o endereço adr da SRAM
// se adr = 0x0 0000 -> 0x1 FFFF, SRAM0 #CS0
// se adr = 0x2 0000 -> 0x3 FFFF, SRAM1 #CS1
byte sram_rd(long adr){
  byte dado;
  if ((adr&0xF0000L)<0x20000L)  spi_cs0();  //Selecionar SRAM0
  else                          spi_cs1();  //Selecionar SRAM1
  spi_transf(SRAM_READ);       //Indicar leitura
  spi_transf((uint8_t)(adr  >> 16) & 0xff); //ender (23..16)
  spi_transf((uint8_t)(adr >> 8) & 0xff);  //ender (15..8)
  spi_transf((uint8_t)adr);                //ender (7..0)
  dado = spi_transf(0);    //Ler um Byte
  spi_CS0();  //Mais fácil desabilitar as duas
  spi_CS1();  //Mais fácil desabilitar as duas
  return dado;
}

// Escrever no registrador de modo
// qual=0 --> SRAM0 #CS0 (0x0 0000 -> 0x1 FFFF)
// qual=1 --> SRAM1 #CS1 (0x2 0000 -> 0x3 FFFF)
void sram_modo_wr(byte qual, byte dado){
  if (qual==0)  spi_cs0();   //Selecionar SRAM0
  else          spi_CS1();   //Selecionar SRAM1
  spi_transf(SRAM_WRMR);  //Indicar escrita no Reg de Modo
  //spi_transf(modo);        //Escrever o Modo
  spi_transf(dado);        //Escrever o Modo
  spi_CS0();  //Mais fácil desabilitar as duas
  spi_CS1();  //Mais fácil desabilitar as duas
}

// Ler o registrador de modo
// qual=0 --> SRAM0 #CS0 (0x0 0000 -> 0x1 FFFF)
// qual=1 --> SRAM1 #CS1 (0x2 0000 -> 0x3 FFFF)
byte sram_modo_rd(byte qual){
  byte dado;
  if (qual==0)  spi_cs0();   //Selecionar SRAM0
  else          spi_CS1();   //Selecionar SRAM1
  spi_transf(SRAM_RDMR);    //Indicar escrita no Reg de Modo
  dado=spi_transf(0);       //Ler o Modo
  spi_CS0();  //Mais fácil desabilitar as duas
  spi_CS1();  //Mais fácil desabilitar as duas
  return dado;
}

// Enviar um Byte pela porta SPI
// Rotina envia e recebe um Byte ao mesmo tempo
byte spi_transf(byte dado){
  SPDR = dado;
  while ( !(SPSR & (1<<SPIF)));
  return SPDR;
}

// CS0 (PL0) = SRAM 0 (23LC1024)
void spi_cs0(void){ PORTL &= ~(1<<DDL0);  }  //CS0=LOW
void spi_CS0(void){ PORTL |=  (1<<DDL0);  }  //CS0=HIGH

// CS1 (PL1) = SRAM 1 (23LC1024)
void spi_cs1(void){ PORTL &= ~(1<<DDL1);  }  //CS1=LOW
void spi_CS1(void){ PORTL |=  (1<<DDL1);  }  //CS1=HIGH

// CS2 (PL2) = SRAM 2 (W25Q64)
void spi_cs2(void){ PORTL &= ~(1<<DDL2);  }  //CS2=LOW
void spi_CS2(void){ PORTL |=  (1<<DDL2);  }  //CS2=HIGH

// Inicializar porta SPI do Mega
void spi_config(byte clk){

  DDRB  |= (1<<DDB2)|(1<<DDB1)|(1<<DDB0);   //MOSI=SCK=SS=Saída
  PORTB |= (1<<DDB2)|(1<<DDB1)|(1<<DDB0);   //MOSI=SCK=SS=1
  DDRB  &= ~(1<<DDB3);                      //MISO=Entrada
  PORTB |= (1<<DDB3);                       //MISO=com pullup
  DDRL  |= (1<<DDL2)|(1<<DDL1)|(1<<DDL0);   //CS2=CS1=CS0=Saída
  PORTL |= (1<<DDL2)|(1<<DDL1)|(1<<DDL0);   //CS2=CS1=CS0=1

  // Incializar porta SPI
  SPCR = ((1<<SPE) | (1<<MSTR));  //Hab. SPI Mestre, (SPR1=SPR0=0)
  SPSR = 0;                       //Desab Int e SPI2X=0
  switch(clk){
    case SPI_125K:                      SPCR |= (1<<SPR1) | (1<<SPR0);  break;
    case SPI_250K:                      SPCR |= (1<<SPR1);              break;
    case SPI_500K: SPSR |= (1<<SPI2X);  SPCR |= (1<<SPR1);              break;
    case SPI_1M:                        SPCR |= (1<<SPR0);              break;
    case SPI_2M:   SPSR |= (1<<SPI2X);  SPCR |= (1<<SPR0);              break;
    case SPI_4M:                                                        break;
    case SPI_8M:   SPSR |= (1<<SPI2X);                                  break;
    defualt:                            SPCR |= (1<<SPR1) | (1<<SPR0);  break;
  }
}
