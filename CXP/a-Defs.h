// Defs.h
// Defines usados na Caixa Preta
// 10/01/2020

#define TRUE  1
#define FALSE 0

//////////////////////// CXP ////////////////////////
// Modos de operação
#define MODO_0  0     //
#define MODO_1  1     //
#define MODO_2  2     //
#define MODO_3  3     //
#define MODO_4  4     //
#define MODO_5  5     //
#define MODO_6  6     //
#define MODO_7  7     //
#define MODO_8  8     //
#define MODO_9  9     //

#define MODO_TOTAL 10

// SRAM 23LC1024

// Instruções da SRAM
#define SRAM_READ    3   //Ler dado da memória
#define SRAM_WRITE   2   //Escrever dado da memória
#define SRAM_RDMR    5   //Ler Registrador de Modo
#define SRAM_WRMR    1   //Escrever no Registrador de Modo

// Registrador de Modo: Modos de Operação
#define SRAM_MODO_BYTE   0x00  //Modo Byte
#define SRAM_MODO_PAG    0x80  //Modo Página
#define SRAM_MODO_SEQ    0x40  //Modo Sequencial

// Velocidades SPI, verificar o dobrador (SPI2X)
#define SPI_125K   0 //SCL=125KHz, SPI2X=0
#define SPI_250K   1 //SCL=250KHz, SPI2X=0
#define SPI_500K   2 //SCL=500KHz, SPI2X=1
#define SPI_1M     3 //SCL=1MHz,   SPI2X=0
#define SPI_2M     4 //SCL=2MHz    SPI2X=1
#define SPI_4M     5 //SCL=4MHz    SPI2X=0
#define SPI_8M     6 //SCL=8MHz    SPI2X=1

#define MISO  50  //Master Input
#define MOSI  51  //Master Output      
#define SCK   52  //Saída do relógio
#define CS0   49  //(PL0) Controla o estado do #CS0 (0x0 0000 -> 0x1 FFFF)
#define CS1   48  //(PL1) Controla o estado do #CS1 (0x2 0000 -> 0x3 FFFF)      
#define CS2   47  //(PL2) Controla o estado do #CS2      
#define SS    53  //Para Mestre é preciso SS como saída      

// LCD
#define NRL 4   //Qtd de linhas do LCD
#define NRC 20  //Qtd de colunas do LCD

// GPS - Serial 3
#define GPS_TX_FILA_TAM 10 //Tamanho da fila circular de TX
#define GPS_RX_FILA_TAM 300 //Tamanho da fila circular de RX

#define SER_TX_FILA_TAM 100 //Tamanho da fila circular de TX
#define SER_RX_FILA_TAM 100 //Tamanho da fila circular de RX
#define CR 0xD    //Carriage Return
#define LF 0xA    //Line Feed
#define SPC 0x20  //Espaço

/////////////////// TECLAS ///////////////////////////

// Parametros para leitura das chaves

#define  SW_FILA_TAM  10  // tamanho da fila do teclado
#define  SW_TOL     20    // tolerancia para identificar chave

// (8 bits) Caixa Preta: Valores lidos pelo conversor AD0
#define  SW_VAL_DIR    147      //antigo 0
#define  SW_VAL_CIMA   113      //antigo 33
#define  SW_VAL_BAIXO  210      //antigo 80
#define  SW_VAL_ESQ     81      //antigo 121
#define  SW_VAL_SEL      0      //antigo 186
#define  SW_VAL_NADA   255      //antigo 255

/*
// (8 bits) Controlador Cerveja: Valores lidos pelo conversor AD0
#define  SW_VAL_DIR      0      //antigo 0
#define  SW_VAL_CIMA    33      //antigo 33
#define  SW_VAL_BAIXO   80      //antigo 80
#define  SW_VAL_ESQ    121      //antigo 121
#define  SW_VAL_SEL    186      //antigo 186
#define  SW_VAL_NADA   255      //antigo 255
*/

// (10 bits) Valores lidos pelo conversor AD0
#define  SW_VAL_DIR_10      0      //antigo 0
#define  SW_VAL_CIMA_10   135      //antigo 135
#define  SW_VAL_BAIXO_10  323      //antigo 323
#define  SW_VAL_ESQ_10    487      //antigo 487
#define  SW_VAL_SEL_10    745      //antigo 745
#define  SW_VAL_NADA_10  1023      //antigo 1023

// Codigos para as chaves
#define  SW_NADA   0
#define  SW_CIMA   1
#define  SW_ESQ    2
#define  SW_SEL    3
#define  SW_DIR    4
#define  SW_BAIXO  5
#define  SW_SEQ1   6
#define  SW_SEQ2   7

#define  SW_NAOSEI 99

/////////////// MPU 6050 - Constantes
#define MPU_ADR  0x68  //Endereço MPU-6050
#define MPU_EWR  0xD0  //MPU para escrita (0x68<<1)
#define MPU_ERD  0xD1  //MPU para leitura (0x68<<1 + 1)

//Escalas para Giroscópio
#define GIRO_FS_250  0   // +/- 250 graus/seg
#define GIRO_FS_500  1   // +/- 500 graus/seg
#define GIRO_FS_1000 2   // +/- 1000 graus/seg
#define GIRO_FS_2000 3   // +/- 2000 graus/seg

//Escalas para Acelerômetro
#define ACEL_FS_2G  0   // +/- 2g
#define ACEL_FS_4G  1   // +/- 4g
#define ACEL_FS_8G  2   // +/- 8g
#define ACEL_FS_16G 3   // +/- 16g

// Registradores do MPU-9250 que foram usados
#define SELF_TEST_X      0x0D
#define SELF_TEST_Y      0x0E
#define SELF_TEST_Z      0x0F
#define SELF_TEST_A      0x10
#define SMPLRT_DIV       0x19
#define CONFIG           0x1A
#define GYRO_CONFIG      0x1B
#define ACCEL_CONFIG     0x1C
#define FIFO_EN          0x23
#define INT_PIN_CFG      0x37
#define INT_ENABLE       0x38
#define INT_STATUS       0x3A
#define ACCEL_XOUT_H     0x3B
#define USER_CTRL        0x6A
#define PWR_MGMT_1       0x6B
#define FIFO_COUNTH      0x72
#define FIFO_COUNTL      0x73
#define FIFO_R_W         0x74
#define WHO_AM_I         0x75

/////////////// MPU 9250 - Magnetômetro
#define MAG_CNTL_1      0x0A  //Controle 1
#define MAG_CNTL_2      0x0B  //Controle 2
#define MAG_XOUT_L      0x03  //MAG XL seq:[XL XH YL YH ZL ZH]

// Endereços da FLASH 24LC1025 (128 KB) TWI
#define FLASH1_ADR     0x50  //FLASH1
#define FLASH2_ADR     0x51  //FLASH2
#define FLASH1_ADR_B0  FLASH1_ADR+0  //FLASH1, Bloco 0: 64KB (0x00000 -> 0x0FFFF)
#define FLASH1_ADR_B1  FLASH1_ADR+4  //FLASH1, Bloco 1: 64KB (0x10000 -> 0x1FFFF)
#define FLASH2_ADR_B0  FLASH2_ADR+0  //FLASH2, Bloco 0: 64KB (0x00000 -> 0x0FFFF)
#define FLASH2_ADR_B1  FLASH2_ADR+4  //FLASH2, Bloco 1: 64KB (0x10000 -> 0x1FFFF)
#define FLASH_PAG   128              //Tamanho da página para gravação         

/////////////// TWI - Códigos de Status
#define TWI_START_OK      8     //Start OK
#define TWI_START_REP_OK  0x10  //Start Repetido OK
#define TWI_SLA_WR_ACK    0x18  //EET enviado e ACK recebido
#define TWI_SLA_WR_NACK   0x20  //EET enviado e NACK recebido
#define TWI_TX_DATA_ACK   0x28  //Dado enviado e ACK recebido
#define TWI_SLA_RD_ACK    0x40  //EER enviado e ACK recebido
#define TWI_SLA_RD_NACK   0x48  //EER enviar e NACK recebido 
#define TWI_RX_DATA_NACK  0x58  //Dado recebido e NACK gerado
#define TWI_RX_DATA_ACK   0x50  //Dado recebido e ACK gerado
#define TWI_TMI_OUT       10000 //Time out

// Códigos de erro no trabalho com TWI
#define TWI_ERRO_1  1   //Erro ao gerar START
#define TWI_ERRO_2  2   //Erro ao gerar START Repetido
#define TWI_ERRO_3  3   //Erro Escravo Receptor endereçado (ER) não enviou ACK
#define TWI_ERRO_4  4   //Erro Escravo Transmissor endereçado (ET) não enviou ACK
#define TWI_ERRO_5  5   //Erro Escravo Receptor (ER) não enviou ACK após envio do dado
#define TWI_ERRO_6  6   //Erro ao receber um dado do Escravo Transmissor (ET) e gerar um ACK
#define TWI_ERRO_7  7   //Erro ao receber um dado do Escravo Transmissor (ET) e gerar um NACK
#define TWI_ERRO_8  8   //Erro ao esperar TWINT - Timeout esperando TWINT ir para 1


// Timer 1 
#define FREQ_T1 100  //Freq de interrupção do timer 1
