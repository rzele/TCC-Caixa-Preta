// MPU - Rotinas para acesso
// CXP - Caixa Preta
// 22/01/2019


// Ler Aceleração, Giro e Mag
// Retorna vetor de 18 bytes
// [axh axl ayh ayl azh azl gxh gxl gyh gyl gzh gzl hxh hxl hyh hyl hzh hzl]
void mpu_rd_ac_gi_mg(byte *vetor){
  byte i,vet[14];
  // Acel e Giro
  mpu_rd_blk(ACCEL_XOUT_H, vet, 14);
  vetor[ 0] = vet[ 0];  //axh
  vetor[ 1] = vet[ 1];  //axl
  vetor[ 2] = vet[ 2];  //ayh
  vetor[ 3] = vet[ 3];  //ayl
  vetor[ 4] = vet[ 4];  //azh
  vetor[ 5] = vet[ 5];  //azl
  // Pular temperatura vet[6] e vet[7]
  vetor[ 6] = vet[ 8];  //gxh
  vetor[ 7] = vet[ 9];  //gxl
  vetor[ 8] = vet[10];  //gyh
  vetor[ 9] = vet[11];  //gyl
  vetor[10] = vet[12];  //gzh
  vetor[11] = vet[13];  //gzl
  // Mag
  mpu_rd_blk(MAG_XOUT_L, vet, 6);
  vetor[12] = vet[ 0];  //hxh
  vetor[13] = vet[ 1];  //hxl
  vetor[14] = vet[ 2];  //hyh
  vetor[15] = vet[ 3];  //hyl
  vetor[16] = vet[ 4];  //hzh
  vetor[17] = vet[ 5];  //hzl
}



////////////////////////////////////////////////////
/////////////// Magnetômetro   /////////////////////
////////////////////////////////////////////////////

/*
// Ler Magnetômetro
// Retorna vetor de 3 words [hx hy hz]
void mpu_rd_mg(word *vetor){
  byte i,vet[6];
  mpu_rd_blk(MAG_XOUT_L, vet, 6);
  vetor[0] = (int)((vet [0] << 8) | vet[1]);    //Montar Mag X
  vetor[1] = (int)((vet [2] << 8) | vet[3]);    //Montar Mag Y
  vetor[2] = (int)((vet [4] << 8) | vet[5]);    //Montar Mag Z
}

// Inicializar Magnetômetro
void mpu_mag_config(void){
  // Despertar MAG, Modo 1 continuo e 16 bits
  mpu_wr(MAG_CNTL_1, 0x12);
  delay(200);       //200ms  
}*/

// Inicializar Magnetômetro
void mpu_mag_config(void){
  
  //desabilita modo mestre no mpu
  mpu_wr(USER_CTRL, 0x00);

  //habilita o bypass mode para acessarmos o magnetometro diretamente
  mpu_wr(INT_PIN_CFG, 0x02); 

//  //configurar modo FUSE ROM para magnetometro
//  mpu_wr_mg_reg(MAG_CNTL_1, 0x1F);
//  delay(100); //espera trocar de modo
//
//  //lê os ajustes de sensiblidade nos registradores do magnetometro asax, asay e asaz
//  mpu_rd_mg_blk(MAG_ASAX, mag_asa, 3);
//
//  //magnetometro no modo power down
//  mpu_wr_mg_reg(MAG_CNTL_1, 0x00);
//  delay(100); //espera trocar de modo
//
//  //magnetometro no modo continuo, 100hz e output de 16 bits
//  mpu_wr_mg_reg(MAG_CNTL_1, 0x16);
//  delay(100); //espera trocar de modo
}

// (50) lê registrador do magnetometro
byte mpu_rd_mg_reg(byte reg){

  byte dado;
  
  twi_start(50);                         //START
  twi_er(MAG_I2C_ADDR_WR, 51);           //Endereçar Magnetometro para escrita
  //twi_er(MAG_I2C_ADDR << 1, 51); 
  

  twi_dado_er(reg, 52);                  //Informar registrador
  twi_start_rep(53);                     //START Repetido
  twi_et(MAG_I2C_ADDR_RD, 54);           //Endereçar Magnetometro para leitura
  dado = twi_dado_et_nack(55);           //Receber dado do magnetometro com NACK
  
  twi_stop();                    //Gerar STOP para finalizar

  return dado;
}

//(60) lê em bloco os registradores do magnetometro
void mpu_rd_mg_blk(byte reg, byte *dado, byte qtd){
  byte i;
  twi_start(60);                  //START
  twi_er(MAG_I2C_ADDR_WR, 61);    //Endereçar MPU para escrita
  twi_dado_er(reg, 62);           //Informar registrador
  twi_start_rep(63);              //START Repetido
  twi_et(MAG_I2C_ADDR_RD, 64);     //Endereçar Magnetometro para leitura

  for (i=0; i<qtd; i++)
    dado[i] = twi_dado_et_ack(65);  //Receber dados e gerar ACK
    
  dado = twi_dado_et_nack(66);  //Receber último dado e gerar NACK
  twi_stop();                   //Gerar STOP para finalizar

}



// (70) escreve no magnetometro
byte mpu_wr_mg_reg(byte reg, byte dado){
  twi_start(70);                //start
  twi_er(MAG_I2C_ADDR_WR, 71);  //endereça magnetometro para escrita
  twi_dado_er(reg, 72);         //informa o registrador
  twi_dado_er(dado, 73);        //informa o dado
  twi_stop();                   //stop
}

//deve retornar 0x48
byte mag_whoami(){
  byte who = 0;
  who = mpu_rd_mg_reg(0x00);
  return who;
}

// Ler Magnetômetro
// Retorna vetor de 3 words [hx hy hz]
void mpu_rd_mg_out(word *vetor){
	byte i,vet[6];

	mpu_rd_mg_blk(MAG_XOUT_L, vet, 6);

	vetor[0] = (int)((int)(vet [1] << 8) | vet[0]);    //Montar Mag X
	vetor[1] = (int)((int)(vet [3] << 8) | vet[2]);    //Montar Mag Y
	vetor[2] = (int)((int)(vet [5] << 8) | vet[4]);    //Montar Mag Z

}

////////////////////////////////////////////////////
/////////// Aceleração e Giro      /////////////////
////////////////////////////////////////////////////

// Preparar para MPU usar INT4
// Pino PE4 entrada com pullup
// Habilitar INT4 para flanco de descida
// MPU: interrup em baixo com push-pull, pulso de 50 useg
// MPU: Habilitar interrup dado pronto
void mpu_int(void){

  // INT4 = PE4 = Pino 2 --> entrada com pullup 
  DDRE = DDRE & ~(1 << DDE4); //DDD4=0, entrada          
  PORTE = PORTE | (1 << PE4); //Pull-up ligado (PORTE4=1)
  //pinMode(2,INPUT_PULLUP);

  // Preparar interrupção INT4 por flanco de descida
  EICRB = (EICRB | (1<<ISC41)) & ~(1 << ISC40); //INT4 = flanco descida
  EIMSK = EIMSK | (1 << INT4);                  //INT4 habilitada  

  mpu_wr(INT_PIN_CFG,0x82);  //push-pull, pulso 50 useg e bypass mode
  mpu_wr(INT_ENABLE,1);     //DATA_RDY_EN
}

// Desabilitar MPU que usava INT4
// Desabilitar INT4 para 
// MPU: Desabilitar interrup dado pronto
void mpu_des_int(void){
  EIMSK &= ~(1 << INT4);    //INT4 Desabilitada  
  mpu_wr(INT_ENABLE,0);     //DATA_RDY_EN = 0 (desab)
}

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

// Ler Aceleração, giro
// Retorna vetor de 6 words [ax ay az gx gy gz]
void mpu_rd_ac_gi(word *vetor){
  byte i,vet[14];
  mpu_rd_blk(ACCEL_XOUT_H, vet, 14);
  vetor[0] = (int)((vet [0] << 8) | vet[1]);    //Montar Acel X
  vetor[1] = (int)((vet [2] << 8) | vet[3]);    //Montar Acel Y
  vetor[2] = (int)((vet [4] << 8) | vet[5]);    //Montar Acel Z
  vetor[4] = (int)((vet [6] << 8) | vet[7]);    //Montar Temp - Será sobrescrita
  vetor[4] = (int)((vet [8] << 8) | vet[9]);    //Montar Giro x
  vetor[5] = (int)((vet[10] << 8) | vet[11]);   //Montar Giro y
  vetor[6] = (int)((vet[12] << 8) | vet[13]);   //Montar Giro z
}

// Acordar o MPU e programar para usar relógio Giro X
void mpu_acorda(void) {
  mpu_wr(PWR_MGMT_1, 1);
}

// Dormir o MPU e programar para usar relógio Giro X
void mpu_dorme(void) {
//  mpu_wr(PWR_MGMT_1, 0x21);  //SLEEP=1 e PLL com Giro X
  mpu_wr(PWR_MGMT_1, 0x41);  //SLEEP=1 e PLL com Giro X
}

// Ler o registrador WHO_AM_I
byte mpu_whoami(void) {
  return mpu_rd(WHO_AM_I);
}

// Colocar o MPU num estado conhecido
// Taxa = 1 kHz, Banda: Acel=5 Hz e Giro=5 Hz e delay=19 mseg
// Taxa de amostragem =  taxa/(1+SMPLRT_DIV) = 1k/10 = 100Hz
//Escalas acel = +/2g e giro = +/-250 gr/s
void mpu_config(void) {

  // Despertar MPU, Relógio = PLL do Giro-x
  mpu_wr(PWR_MGMT_1, 0x01);
  delay(200);       //200ms - Esperar PLL estabilizar

  // 6 ==> Taxa = 1 kHz, Banda: Acel=5 Hz e Giro=5 Hz e delay=19 mseg
  mpu_wr(CONFIG, 6);

  // 9 ==> Taxa de amostragem =  taxa/(1+SMPLRT_DIV) = 1k/10 = 100Hz
  mpu_wr(SMPLRT_DIV, SAMPLE_RT_100Hz);  //Taxa de amostragem = 100 Hz
  //mpu_wr(SMPLRT_DIV, SAMPLE_RT_500Hz);  //Taxa de amostragem = 500 Hz

  // Definir escalas
  mpu_escalas(GIRO_FS_250, ACEL_FS_2G);   //Escalas acel = +/2g e giro = +/-250 gr/s

  //configura o magnetometro
  mpu_mag_config();
}

// Selecionar Fundo de Escalas para o MPU
// Acel: 0=+/-2g, 1=+/-4g, 2=+/-8g, 3=+/-16g 
// Gyro: 0=+/-250gr/s, 1=+/-500gr/s, 2=+/-1000gr/s, 3=+/-2000gr/s,  
void mpu_escalas(byte gfs, byte afs) {
  mpu_wr(GYRO_CONFIG, gfs << 3); //FS do Giro
  mpu_wr(ACCEL_CONFIG, afs << 3); //FS do Acel
}

// Selecionar Sample Rate
// Considerando Taxa = 1kHz (Registrador CONFIG)
void mpu_sample_rt(byte sample_rate) {
  mpu_wr(SMPLRT_DIV, sample_rate);  //Taxa de amostragem
}

// MPU: Calibrar
// Faz uma série de leituras e retorna a média
void mpu_calibra(word *vt, word qtd) {
  long sum[7];  //Acumular para calcular a média
  word aux[7];  //Leituras intermediárias
  word i,j;

  mpu_escalas(GIRO_FS_250, ACEL_FS_2G);  // Menores escalas para maior precisã
  mpu_sample_rt(SAMPLE_RT_500Hz);       // Taxa de amostragem em 500 Hz
  mpu_int();                            // Habilitar interrupção
  mpu_dado_ok=FALSE;
  for (i=0; i<7; i++)   sum[i]=0;       //Zerar acumulador
  mpu_des_int();                        //Desabilitar interrupção
  mpu_sample_rt(SAMPLE_RT_100Hz);       //Voltar Taxa de amostragem para 100 Hz

  for (i=0; i<qtd; i++){
    while(mpu_dado_ok == FALSE);
    mpu_dado_ok = FALSE;
    mpu_rd_ac_tp_gi(aux);
    for (j=0; j<7; j++) sum[i] += aux[i];
  }

  // Calcular as médias
  for (i=0; i<7; i++) vt[i] = sum[i]/qtd;

}


// MPU: Realizar Self-Test (ST)
// Retorna: TRUE  se passou no teste
//          FALSE se falhou no teste
byte mpu_self_test(void) {
  char msg[100];
  byte   aux[14];  //Auxiliar na leitura dos registradores
  word   gx1, gy1, gz1, ax1, ay1, az1; //Valores com Self-Test desabilitado
  word   gx2, gy2, gz2, ax2, ay2, az2; //Valores Self-Test habilitado
  byte   gx3, gy3, gz3, ax3, ay3, az3; //Valores reg de Self-Test
  float  gxf, gyf, gzf, axf, ayf, azf; //Factory Trim
  float  gxr, gyr, gzr, axr, ayr, azr; //Alteração em %

  // Desabilitar Self_Test
  mpu_wr(ACCEL_CONFIG, ACEL_FS_8G << 3);  //Escala 8g, Self-test Desabiliatdo
  mpu_wr(GYRO_CONFIG, GIRO_FS_250 << 3);  //Escala 250, Self-test Desabiliatdo
  delay(250);  //Aguardar cofiguração estabilizar

  // Ler valores com Self-Test Desabilitado
  mpu_rd_blk(ACCEL_XOUT_H, aux, 14);
  ax1 = (int16_t)((aux[0] << 8) | aux[1]) ;
  ay1 = (int16_t)((aux[2] << 8) | aux[3]) ;
  az1 = (int16_t)((aux[4] << 8) | aux[5]) ;
  gx1 = (int16_t)((aux[6] << 8) | aux[7]) ;
  gy1 = (int16_t)((aux[8] << 8) | aux[9]) ;
  gz1 = (int16_t)((aux[10] << 8) | aux[11]) ;

  // Habilitar Self_Test
  mpu_wr(ACCEL_CONFIG, 0xE0|(ACEL_FS_8G << 3));  //Escala 8g, Self-test Habilitado
  mpu_wr(GYRO_CONFIG, 0xE0|(GIRO_FS_250 << 3));  //Escala 250, Self-test Habilitado
  delay(250);  //Aguardar cofiguração estabilizar

  // Ler valores com Self-Test Habilitado.
  mpu_rd_blk(ACCEL_XOUT_H, aux, 14);
  ax2 = (int16_t)((aux[0] << 8) | aux[1]) ;
  ay2 = (int16_t)((aux[2] << 8) | aux[3]) ;
  az2 = (int16_t)((aux[4] << 8) | aux[5]) ;
  gx2 = (int16_t)((aux[6] << 8) | aux[7]) ;
  gy2 = (int16_t)((aux[8] << 8) | aux[9]) ;
  gz2 = (int16_t)((aux[10] << 8) | aux[11]) ;

  // Leitura dos resultados do self-test
  aux[0] = mpu_rd(SELF_TEST_X); //Eixo X: resultado self-test
  aux[1] = mpu_rd(SELF_TEST_Y); //Eixo Y: resultado self-test
  aux[2] = mpu_rd(SELF_TEST_Z); //Eixo Z: resultado self-test
  aux[3] = mpu_rd(SELF_TEST_A); //Restante dos bits dos resultados

  // Extrair dados do registradores de self-test
  ax3 = (aux[0] >> 3) | ((aux[3] >> 4) & 3) ; // XA_TEST
  ay3 = (aux[1] >> 3) | ((aux[3] >> 2) & 3) ; // YA_TEST
  az3 = (aux[2] >> 3) | (aux[3] & 3);         // ZA_TEST
  gx3 = aux[0]  & 0x1F ;  // XG_TEST
  gy3 = aux[1]  & 0x1F ;  // YG_TEST
  gz3 = aux[2]  & 0x1F ;  // ZG_TEST

  // Calcular os Factory Trim
  axf = (4096.0*0.34) * (pow((0.92/0.34) , (((float)ax3 - 1.0) / 30.0)));
  ayf = (4096.0*0.34) * (pow((0.92/0.34) , (((float)ay3 - 1.0) / 30.0)));
  azf = (4096.0*0.34) * (pow((0.92/0.34) , (((float)az3 - 1.0) / 30.0)));
  gxf = ( 25.0 * 131.0) * (pow( 1.046 , ((float)gx3 - 1.0) ));
  gyf = (-25.0 * 131.0) * (pow( 1.046 , ((float)gy3 - 1.0) ));
  gzf = ( 25.0 * 131.0) * (pow( 1.046 , ((float)gz3 - 1.0) ));

  // Se registrador = 0 --> Factory Trim = 0
  if (ax3 == 0) axf = 0;
  if (ay3 == 0) ayf = 0;
  if (az3 == 0) azf = 0;
  if (gx3 == 0) gxf = 0;
  if (gy3 == 0) gyf = 0;
  if (gz3 == 0) gzf = 0;

  // Calcular as Percentagens de Alteração
  axr = 100.0 * ((float)(ax2 - ax1) - axf ) / axf;
  ayr = 100.0 * ((float)(ay2 - ay1) - ayf ) / ayf;
  azr = 100.0 * ((float)(az2 - az1) - azf ) / azf;
  gxr = 100.0 * ((float)(gx2 - gx1) - gxf ) / gxf;
  gyr = 100.0 * ((float)(gy2 - gy1) - gyf ) / gyf;
  gzr = 100.0 * ((float)(gz2 - gz1) - gzf ) / gzf;

  // Remova os comentários para imprimir os resultados
  // Precisa adaptar para a Caixa Preta
  /* Serial.println("Self Test:   OFF      ON    TEST      FT      %");
  sprintf(msg,"Acel X   :  %+06d  %+06d  %+04d   ",ax1, ax2, ax3);
  Serial.print(msg);  Serial.print(axf,2);  Serial.print("  ");
  Serial.println(axr,2);
  sprintf(msg,"Acel Y   :  %+06d  %+06d  %+04d   ",ay1, ay2, ay3);
  Serial.print(msg);  Serial.print(ayf,2);  Serial.print("  ");
  Serial.println(ayr,2);
  sprintf(msg,"Acel Z   :  %+06d  %+06d  %+04d   ",az1, az2, az3);
  Serial.print(msg);  Serial.print(azf,2);  Serial.print("  ");
  Serial.println(azr,2);
  sprintf(msg,"Giro X   :  %+06d  %+06d  %+04d   ",gx1, gx2, gx3);
  Serial.print(msg);  Serial.print(gxf,2);  Serial.print("  ");
  Serial.println(gxr,2);
  sprintf(msg,"Giro Y   :  %+06d  %+06d  %+04d   ",gy1, gy2, gy3);
  Serial.print(msg);  Serial.print(gyf,2);  Serial.print("  ");
  Serial.println(gyr,2);
  sprintf(msg,"Giro Z   :  %+06d  %+06d  %+04d   ",gz1, gz2, gz3);
  Serial.print(msg);  Serial.print(gzf,2);  Serial.print("  ");
  Serial.println(gzr,2); */

  // Tolerância de +/- 14%
  if (axr<14 && ayr<14 && azr<14 && gxr<14 && gyr<14 && gzr<14)
        return 1;  //Passou no Self-Test
  else  return 0;  //Falhou no Self-Test
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


// MPU: ISR para a interrupção INT4
ISR(INT4_vect){                                
  mpu_dado_ok=TRUE;
  Scp1();
}
