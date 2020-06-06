// Defs.h
// Defines usados na Caixa Preta
// 10/01/2020

#define TRUE  1
#define FALSE 0

// BITS - definições
#define BIT0  0x01
#define BIT1  0x02
#define BIT2  0x04
#define BIT3  0x08
#define BIT4  0x10
#define BIT5  0x20
#define BIT6  0x40
#define BIT7  0x80

// Endereços da SRAM
#define MPU_ADR_INI 0x00000L  //Início área MPU (12.720 msg x 18 = 228.960 Bytes) = 12,7 seg
#define MPU_ADR_FIM 0x37E60L  //Fim    área MPU
#define CXP_ADR_INI 0x37E60L  //Início área de conficuração da Caixa Preta (sobra 416 bytes)
#define CXP_ADR_FIM 0x38000L  //Fim    área de conficuração da Caixa Preta
#define GPS_ADR_INI 0x38000L  //Início área GPS (256 msg x 128 = 32.768) = 25,6 seg
#define GPS_ADR_FIM 0x40000L  //Fim    área GPS
#define MPU_PASSO   18        //Tamanho de uma mensagem do MPU (Acel, Giro, Mag)
#define GPS_PASSO   128       //Tamanho da mensagem do GPS (gps_dados[])

// LCD - Constantes para os bits de controle
#define LCD_BL  8 //Back Light
#define LCD_RW  4 //R/#W
#define LCD_RS  2 //RS
#define LCD_E   1 //Enable

// LCD - Bits para indicar qual linha mudou
#define LCD_LINHA0 BIT0 //Linha 0 foi alterada
#define LCD_LINHA1 BIT1 //Linha 1 foi alterada
#define LCD_LINHA2 BIT2 //Linha 0 foi alterada
#define LCD_LINHA3 BIT3 //Linha 1 foi alterada

// Timer 1 
#define FREQ_T1 100  //Freq de interrupção do timer 1

// Timer 2
#define FREQ_T2 1000  //Freq de interrupção do timer 1
//#define FREQ_T2 5000  //Freq de interrupção do timer 1
//#define FREQ_T2 25000  //Freq de interrupção do timer 1

// TESTE
#define TESTE_TOT 17     //Modos de teste: 1, 2 , ..., 17
#define TESTE_0    0     //Não tem
#define TESTE_1    1     //LEDs
#define TESTE_2    2     //LCD
#define TESTE_3    3     //Teclado
#define TESTE_4    4     //TWI
#define TESTE_5    5     //Acel e giro
#define TESTE_6    6     //Magnetometro
#define TESTE_7    7     //SRAM
#define TESTE_8    8     //FLAH
#define TESTE_9    9     //GPS Tudo
#define TESTE_10  10     //GPS RMC GSA
#define TESTE_11  11     //GPS U-Center
#define TESTE_12  12     //MPU-->Matlab
#define TESTE_13  13     //BlueTooth
#define TESTE_14  14     //Livre
#define TESTE_15  15     //Livre
#define TESTE_16  16     //Livre
#define TESTE_17  17     //Livre

// OPERA
#define OPERA_TOT  7     //Modos de teste: 1, 2 , ..., 7
#define OPERA_0    0     //Não tem
#define OPERA_1    1     //Livre
#define OPERA_2    2     //Livre
#define OPERA_3    3     //Livre
#define OPERA_4    4     //Livre
#define OPERA_5    5     //Livre
#define OPERA_6    6     //Livre
#define OPERA_7    7     //Livre

// TECLADO - Parametros para leitura das chaves
#define  SW_FILA_TAM  10  // tamanho da fila do teclado
#define  SW_TOL     20    // tolerancia para identificar chave
// Codigos para as chaves
#define  SW_NADA    7
#define  SW_INF     6
#define  SW_DIR     4
#define  SW_SUP     3
#define  SW_ESQ     2
#define  SW_SEL     0
#define  SW_SEQ1    8
#define  SW_SEQ2    9
#define  SW_NAOSEI 10

// Aleat - Constantes para o gerador pseudo-aleatório
#define ALEAT_SUGEST_M   53  //m sugerido
#define ALEAT_SUGEST_D   109 //d sugerido
#define ALEAT_SUGEST_U   13  //u sugerido (semente)


// GPS
// Identificar tipos de mensagens
#define GPS_NADA  0
#define GPS_RMC   1
#define GPS_VTG   2
#define GPS_GGA   3
#define GPS_GSA   4
#define GPS_GSV   5
#define GPS_GLL   6
#define GPS_MSG_TAM 200   //Tamanho max de uma msg do GPS
#define GPS_DADOS_TAM 128   //Tamanho do vetor para os dados extraídos do GPS

// Marcar posição de cada um dos parâmetros guardados em gps_dados[GPS_DADOS_TAM]
#define GPS_STATUS     0                 //2 bytes
#define GPS_HORA      (GPS_STATUS+2)    //11 bytes
#define GPS_DATA      (GPS_HORA+11)     //7 bytes
#define GPS_LAT       (GPS_DATA+7)      //11 bytes
#define GPS_NS        (GPS_LAT+11)      //2 bytes
#define GPS_LONG      (GPS_NS+2)        //12 bytes
#define GPS_EW        (GPS_LONG+12)     //2 bytes
#define GPS_VEL_NOS   (GPS_EW+2)        //8 bytes
#define GPS_CURSO     (GPS_VEL_NOS+8)   //8 bytes
#define GPS_PDOP      (GPS_CURSO+8)     //6 bytes
#define GPS_HDOP      (GPS_PDOP+6)      //6 bytes
#define GPS_VDOP      (GPS_HDOP+6)      //6 bytes
#define GPS_VEL_KPH   (GPS_VDOP+6)      //7 bytes
#define GPS_VEL_UN    (GPS_VEL_KPH+7)   //2 bytes
#define GPS_FIX       (GPS_VEL_UN+2)    //2 bytes ?sem uso? - não me lembro por que
#define GPS_QTD_SAT   (GPS_FIX+2)       //3 bytes
#define GPS_ALT       (GPS_QTD_SAT+3)   //7 bytes
#define GPS_ALT_UN    (GPS_ALT+7)       //2 bytes
#define GPS_ADR_SRAM  (GPS_ALT_UN+2)    //5 bytes

/////////////// MPU 6050 - Constantes
#define MPU_ADR  0x68  //Endereço MPU-6050
#define MPU_EWR  0xD0  //MPU para escrita (0x68<<1)
#define MPU_ERD  0xD1  //MPU para leitura (0x68<<1 + 1)

#define MPU9250_WHO 0x73  //MPU Who am I

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

// Valores para o Sample Rate, Registrador SMPLRT_DIV
// Considerando Taxa = 1kHz (Registrador CONFIG)
#define SAMPLE_RT_1kHz     0   // 1.000/(0+1) = 1000
#define SAMPLE_RT_500Hz    1   // 1.000/(1+1) = 500
#define SAMPLE_RT_333Hz    2   // 1.000/(2+1) = 333,33
#define SAMPLE_RT_250Hz    3   // 1.000/(3+1) = 250
#define SAMPLE_RT_200Hz    4   // 1.000/(4+1) = 200
#define SAMPLE_RT_166Hz    5   // 1.000/(5+1) = 166,66
#define SAMPLE_RT_142Hz    6   // 1.000/(6+1) = 142,85
#define SAMPLE_RT_125Hz    7   // 1.000/(7+1) = 125
#define SAMPLE_RT_111Hz    8   //1.000 /(8+1) = 111,11
#define SAMPLE_RT_100Hz    9   //1.000 /(9+1) = 100

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
#define MAG_I2C_ADDR    0x0C //endereço i2c do magnetometro
#define MAG_I2C_ADDR_WR 0x18 //0x0c << 1
#define MAG_I2C_ADDR_RD 0x19 //(0x0c << 1) + 1
#define MAG_ASAX        0x10 //endereço do registrador ASAX do magnetometro
#define MAG_ASAY        0x11 //endereço do registrador ASAY do magnetometro  
#define MAG_ASAZ        0x12 //endereço do registrador ASAZ do magnetometro  
#define MAG_ST1         0x02 //endereço do registrador ST1 do magnetometro que indica se o dado está pronto

//////////////////////////////////////////////////////////////////////////////////
/////////////////////// 06/04/2020 ///////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////



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
