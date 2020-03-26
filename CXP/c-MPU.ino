// MPU - Rotinas para acesso
// CXP - Caixa Preta
// 22/01/2019

////////////////////////////////////////////////////
/////////////// Magnetômetro   /////////////////////
////////////////////////////////////////////////////

// Ler Magnetômetro
// Retorna vetor de 3 words [hx hy hz]
void mpu_rd_mg(word *vetor){
  byte i,vet[6];
  mpu_rd_blk(MAG_XOUT_L, vet, 6);
  vetor[0] = (int)((vet [1] << 8) | vet[0]);    //Montar Mag X
  vetor[1] = (int)((vet [3] << 8) | vet[2]);    //Montar Mag Y
  vetor[2] = (int)((vet [5] << 8) | vet[4]);    //Montar Mag Z
}

// Inicializar Magnetômetro
void mpu_mag_config(void){
  // Despertar MAG, Modo 1 continuo e 16 bits
  mpu_wr(MAG_CNTL_1, 0x12);
  delay(200);       //200ms  
}



////////////////////////////////////////////////////
/////////// Aceleração e Giro      /////////////////
////////////////////////////////////////////////////

// Ler Aceleração, temperatura e giro
// Retorna vetor de 7 words [ax ay az tp gx gy gz]
void mpu_rd_ac_tp_gi(word *vetor){
  byte i,vet[14];
  mpu_rd_blk(ACCEL_XOUT_H, vet, 14);
  vetor[0] = (int)((vet [0] << 8) | vet[1]);    //Montar Acel X
  vetor[1] = (int)((vet [2] << 8) | vet[3]);    //Montar Acel Y
  vetor[2] = (int)((vet [4] << 8) | vet[5]);    //Montar Acel Z
  vetor[3] = (int)((vet [6] << 8) | vet[7]);    //Montar Temp
  vetor[4] = (int)((vet [8] << 8) | vet[9]);    //Montar Giro x
  vetor[5] = (int)((vet[10] << 8) | vet[11]);   //Montar Giro y
  vetor[6] = (int)((vet[12] << 8) | vet[13]);   //Montar Giro z
}

// Ler Aceleração e temperatura
void mpu_rd_ac_tp(void){
  byte vet[8];
  mpu_rd_blk(ACCEL_XOUT_H, vet, 8);
  axi = (int)((vet[0] << 8) | vet[1]);    //Montar Acel X
  ayi = (int)((vet[2] << 8) | vet[3]);    //Montar Acel Y
  azi = (int)((vet[4] << 8) | vet[5]);    //Montar Acel Z
  tpi = (int)((vet[6] << 8) | vet[7]);    //Montar Temp
  //serial_str("MPU = ");
  //serial_xyzt(ax,ay,az,tp);
  //serial_crlf();
}
  
// Acordar o MPU e programar para usar relógio Giro X
void mpu_acorda(void) {
  mpu_wr(PWR_MGMT_1, 1);
}

// Dormir o MPU e programar para usar relógio Giro X
void mpu_dorme(void) {
  mpu_wr(PWR_MGMT_1, 0x21);  //SLEEP=1 e PLL com Giro X
}

// Ler o registrador WHO_AM_I
byte mpu_whoami(void) {
  return mpu_rd(WHO_AM_I);
}

// Colocar o MPU num estado conhecido
// Algumas operações são redundantes dependendo de
// quando esta função é chamada
void mpu_inicializa(void) {

  // Despertar MPU, Relógio = PLL do Giro-x
  mpu_wr(PWR_MGMT_1, 0x01);
  delay(200);       //200ms - Esperar PLL estabilizar

  // 6 ==> Taxa = 1 kHz, Banda: Acel=5 Hz e Giro=5 Hz e delay=19 mseg
  mpu_wr(CONFIG, 6);

  // Taxa de amostragem =  taxa/(1+SMPLRT_DIV) = 1k/10 = 100Hz
  mpu_wr(SMPLRT_DIV, 9);  //Taxa = 100 Hz
  //mpu_wr(SMPLRT_DIV, 4);  //Taxa = 1k/5 = 500 Hz
}

// Selecionar Fundo de Escalas para o MPU
// Acel: 0=+/-2g, 1=+/-4g, 2=+/-8g, 3=+/-16g 
// Gyro: 0=+/-250gr/s, 1=+/-500gr/s, 2=+/-1000gr/s, 3=+/-2000gr/s,  
void mpu_escalas(byte gfs, byte afs) {
  mpu_wr(GYRO_CONFIG, gfs << 3); //FS do Giro
  mpu_wr(ACCEL_CONFIG, afs << 3); //FS do Acel
}


///////////////// Rotinas Básicas para MPU

// (10) Escrever num registrador do MPU
void mpu_wr(byte reg, byte dado) {
  twi_start(10);          //START
  twi_er(MPU_EWR, 11);    //Endereçar MPU para escrita
  twi_dado_er(reg, 12);   //Informar acesso ao PWR_MGMT_1 (0x6B)
  twi_dado_er(dado, 13);  //Selecionar PLL eixo X como referência
  twi_stop();             //Gerar STOP para finalizar
}

// (20) Ler um registrador do MPU
byte mpu_rd(byte reg) {
  uint8_t dado;
  twi_start(20);                //START
  twi_er(MPU_EWR, 21);           //Endereçar MPU para escrita
  twi_dado_er(reg, 22);         //Informar registrador
  twi_start_rep(23);            //START Repetido
  twi_et(MPU_ERD, 24);           //Endereçar MPU para leitura
  dado = twi_dado_et_nack(25);  //Receber dado do MPU com NACK
  twi_stop();                    //Gerar STOP para finalizar
  return dado;
}

// (30) Escrever um bloco de dados no MPU a partir de um registrador
void mpu_wr_blk(byte reg, byte *dado, byte qtd) {
  uint8_t i;
  twi_start(30);                //START
  twi_er(MPU_EWR, 31);          //Endereçar MPU para escrita
  twi_dado_er(reg, 32);         //Informar acesso ao PWR_MGMT_1 (0x6B)
  for (i = 0; i < qtd; i++)
    twi_dado_er(dado[i], 33);   //Selecionar PLL eixo X como referência
  twi_stop();                   //Gerar STOP para finalizar
}

// (40) Ler um bloco do MPU a partir de um registrador
void mpu_rd_blk(byte reg, byte *dado, byte qtd) {
  byte i;
  twi_start(40);                //START
  twi_er(MPU_EWR, 41);          //Endereçar MPU para escrita
  twi_dado_er(reg, 42);         //Informar registrador
  twi_start_rep(43);            //START Repetido
  twi_et(MPU_ERD, 44);          //Endereçar MPU para leitura
  for (i=0; i<qtd; i++)
    dado[i] = twi_dado_et_ack(45);  //Receber dados e gerar ACK
  dado = twi_dado_et_nack(46);  //Receber último dado e gerar NACK
  twi_stop();                   //Gerar STOP para finalizar
}
