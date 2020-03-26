// Global.h - Variáveis globais
// CXP - Caixa Preta
// 10/01/2019

// GPS - Não funcionou
char msg_gll_off[]={0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x01,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x2B,'\0'};
 
// Programa principal
volatile byte modo;        //Modo de operação

char *modo_msg[]={"0: Testar TWI",
                  "1: Testar MPU",
                  "2: Testar Mag",
                  "3: GPS Tudo",
                  "4: GPS RMC",
                  "5: GPS U-Center",
                  "6: Testar Flash",
                  "7: Testar SRAM",
                  "8: Expedito"};

// MPU
int axi,ayi,azi,tpi,gxi,gyi,gzi;      //Receber leituras instantâneas do MPU
int refx,refy,refz;                   //Referência de 1.000

// LCD
volatile char lcdb_buf[NRL*NRC];   //buffer para o LCD
volatile byte lcdb_flags[NRL];     //Flags para indicar linha a atualizar

// Serial 0
volatile char ser_tx_fila[SER_TX_FILA_TAM]; //Espaço para a fila serial de TX
volatile byte ser_tx_pin, ser_tx_pout;      //Ponteiros para usar a fila
volatile byte ser_tx_ok;                    //Indica que terminou transmissão 
volatile char ser_rx_fila[SER_RX_FILA_TAM]; //Espaço para a fila serial de RX
volatile byte ser_rx_pin, ser_rx_pout;      //Ponteiros para usar a fila
volatile byte ser_rx_ok;                    //Indica que terminou recepção

// GPS = Serial 3
volatile char gps_tx_fila[GPS_TX_FILA_TAM]; //Espaço para a fila serial de TX
volatile byte gps_tx_pin, gps_tx_pout;      //Ponteiros para usar a fila
volatile byte gps_tx_ok;                    //Indica que terminou transmissão
volatile char gps_rx_fila[GPS_RX_FILA_TAM]; //Espaço para a fila serial de RX
volatile byte gps_rx_pin, gps_rx_pout;      //Ponteiros para usar a fila
volatile byte gps_rx_ok;                    //Indica que terminou recepção
volatile byte gps_msg_0[200];               //(fase=0) Buffer 0 usado pela interrupção para receber GPRMC
volatile byte gps_msg_1[200];               //(fase=1) Buffer 1 usado pela interrupção para receber GPRMC
volatile byte gps_msg_ix;                   //Indexador para escrever nos buffers;
volatile byte gps_msg_fase;                 //Fase para receber GPRMC (0=gps_rmc_0 e 1=gps_rmc_1);
volatile byte gps_msg_ok;                   //Indica que completou o recebimento de uma mensagem;

// Teclas
volatile char sw_fila[SER_TX_FILA_TAM]; //Espaço para a fila teclado
volatile byte sw_pin, sw_pout;          //Ponteiros para usar a fila
volatile byte sw_1,sw_2,sw_n,sw_v;      //Variáveis para detectar teclas acionadas
volatile byte sw_st_seq1,sw_st_seq2;    //Maq Estados para buscar sequências

//ADC
volatile word adc_val;    //Valor lido pelo ADC (em 16 bits porque tem contas)
volatile byte adc_ok;     //Flag indica que terminou conversão
volatile byte sw_val;     //Teclado: valor lido pelo ADC
volatile byte vcar_val;   //VCAR: valor lido pelo ADC
volatile byte vcap_val;   //VCAP: valor lido pelo ADC

//Timer 1
byte timer1_cont;       //(0, ..., 49) Contar estados do Timer 1 para ativar ADC
