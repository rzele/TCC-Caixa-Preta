// Modos de exercicios
// CXP - Caixa Preta
// 18/04/2020

void exercicios(void){
  byte modo;    //Modo selecionado
  byte i;
  word w;
  long z;
  float f;
  while(TRUE){
    lcd_apaga();
    lcd_str(0,0,"EXERCICIOS");
    ser_str("\nModo de Exercicios\n");
    while(sw_val<0x10);  //Esperar soltar tecla SEL
    ser_str("Selecionar com LCD\n");
    modo=sel_modo(exercicios_msg, EXERCICIOS_TOT);
    //modo=10;
    lcd_apaga();
    ser_crlf(1);
    switch(modo){
      case EXERCICIO_1:  exercicio_1(modo);  break;
      case EXERCICIO_2:  exercicio_2(modo);  break;
      case EXERCICIO_3:  exercicio_3(modo);  break;
      case EXERCICIO_4:  exercicio_4(modo);  break;
      case EXERCICIO_5:  exercicio_5(modo);  break;
      case EXERCICIO_6:  exercicio_6(modo);  break;
      case EXERCICIO_7:  exercicio_7(modo);  break;
      case EXERCICIO_8:  exercicio_8(modo);  break;
    }
  }
  while(1);
}

// 1 - exercicio
// Contador binario acionado pelas chaves
// sup -> incrementa / inf -> decrementa / esq ou dir -> zera / sel -> sai
// Exibido nos led o resultado, lembrando q so tem 4 leds
char exercicio_1(char md){
  lcd_str(0,0,exercicios_msg[md]);
  ser_str(exercicios_msg[md]);
  delay(500);
  lcd_apaga();

  lcd_str(0,0, "------COMMANDS------");
  lcd_str(1,0, "         +1         ");
  lcd_str(2,0, "  zera  exit  zera  ");
  lcd_str(3,0, "         -1         ");
  leds_cont(0);

  word count = 0;
  byte key = SW_NADA;

  while (key != SW_SEL) {
    // Aguarda a leitura de uma tecla
    while( sw_tira(&key) == FALSE );

    switch (key) {
      case SW_INF:
        count--;
        break;
      case SW_DIR:
        count = 0;
        break;
      case SW_SUP:
        count++;
        break;
      case SW_ESQ:
        count = 0;
        break;
      default:
        break;
    }

    leds_cont(count);
  }

  leds_cont(0);
  return md;
}

// 2 - exercicio
// Contador binario acionado pelas chaves
// sup -> incrementa / inf -> decrementa / esq -> subtrai 10 / dir -> soma 10 / SEQ1 ou SEQ2 -> zera / sel -> sai
// Exibido o resultado no LCD
char exercicio_2(char md){
  lcd_str(0,0,exercicios_msg[md]);
  ser_str(exercicios_msg[md]);
  delay(500);
  lcd_apaga();

  lcd_str(0,0, "     +1     ");
  lcd_str(1,0, "-10 exit +10");
  lcd_str(2,0, "     -1     ");
  lcd_str(3,0, "SEQ1/2: zera");
  lcd_str(2, 14, "VALOR:");
  lcd_str(3, 14, "+00000");

  word count = 0;
  byte key = SW_NADA;

  while (key != SW_SEL) {
    // Aguarda a leitura de uma tecla
    while( sw_tira(&key) == FALSE );

    switch (key) {
      case SW_INF:
        count--;
        break;
      case SW_DIR:
        count += 10;
        break;
      case SW_SUP:
        count++;
        break;
      case SW_ESQ:
        count -= 10;
        break;
      case SW_SEQ1:
        count = 0;
        break;
      case SW_SEQ2:
        count = 0;
        break;
      default:
        break;
    }

    lcd_dec16(3, 14, count);
  }

  return md;
}

// 3 - exercicio
// Lê aceleração do MPU e exibe no LCD
char exercicio_3(char md){
  int mpu_data[7];
  float mpu_processed_data[7];
  byte key = SW_NADA, who;
  word ac_esc=2;

  lcd_str(0,0,exercicios_msg[md]);
  ser_str(exercicios_msg[md]);
  delay(500);
  lcd_apaga();

  mpu_acorda();     //Acordar MPU
  who=mpu_whoami();
  lcd_str(1,0,"Who am I = ");
  lcd_hex8(1,10,who);
  if (who == 0x73) {
    lcd_str(1,15," OK");  //MPU respondendo
  } else {
    lcd_str(1,15," NOK");  //MPU respondendo
    return 0;
  }

  mpu_inicializa();     //Inicializar
  mpu_escalas(0,0);     //+/- 2g e +/-250gr/seg
  delay(2000);
  lcd_apaga();

  lcd_str(0, 0, "ax = HHHH | +d.dddd g");
  lcd_str(1, 0, "ay = HHHH | +d.dddd g");
  lcd_str(2, 0, "az = HHHH | +d.dddd g");
  lcd_str(3, 0, " APERTE SEL P/ SAIR ");

  while (key != SW_SEL) {
    sw_tira(&key);
    mpu_rd_ac_tp_gi(mpu_data);
    mpu_processed_data[0]=ac_esc*( ((float) mpu_data[0]) / 32767 );
    mpu_processed_data[1]=ac_esc*( ((float) mpu_data[1]) / 32767 );
    mpu_processed_data[2]=ac_esc*( ((float) mpu_data[2]) / 32767 );

    lcd_hex16(0, 5, mpu_data[0]);
    lcd_hex16(1, 5, mpu_data[1]);
    lcd_hex16(2, 5, mpu_data[2]);

    lcd_float(0, 12, mpu_processed_data[0], 6);
    lcd_float(1, 12, mpu_processed_data[1], 6);
    lcd_float(2, 12, mpu_processed_data[2], 6);
  }

  return md;
}

// 4 - exercicio
// Lê giro do MPU e exibe no LCD
char exercicio_4(char md){
  int mpu_data[7];
  float mpu_processed_data[7];
  byte key = SW_NADA, who;
  word giro_esc=250;

  lcd_str(0,0,exercicios_msg[md]);
  ser_str(exercicios_msg[md]);
  delay(500);
  lcd_apaga();

  mpu_acorda();     //Acordar MPU
  who=mpu_whoami();
  lcd_str(1,0,"Who am I = ");
  lcd_hex8(1,10,who);
  if (who == 0x73) {
    lcd_str(1,15," OK");  //MPU respondendo
  } else {
    lcd_str(1,15," NOK");  //MPU respondendo
    return 0;
  }

  mpu_inicializa();     //Inicializar
  mpu_escalas(0,0);     //+/- 2g e +/-250gr/seg
  delay(2000);
  lcd_apaga();

  lcd_str(0, 0, "gx=HHHH | +d.dddd");
  lcd_str(1, 0, "gy=HHHH | +d.dddd");
  lcd_str(2, 0, "gz=HHHH | +d.dddd");
  lcd_str(3, 0, " APERTE SEL P/ SAIR ");

  while (key != SW_SEL) {
    sw_tira(&key);
    mpu_rd_ac_tp_gi(mpu_data);
    mpu_processed_data[4]=giro_esc*( ((float) mpu_data[4]) / 32767 );
    mpu_processed_data[5]=giro_esc*( ((float) mpu_data[5]) / 32767 );
    mpu_processed_data[6]=giro_esc*( ((float) mpu_data[6]) / 32767 );

    lcd_hex16(0, 3, mpu_data[4]);
    lcd_hex16(1, 3, mpu_data[5]);
    lcd_hex16(2, 3, mpu_data[6]);

    lcd_float(0, 10, mpu_processed_data[4], 6);
    lcd_float(1, 10, mpu_processed_data[5], 6);
    lcd_float(2, 10, mpu_processed_data[6], 6);
  }

  return md;
}

// 5 - exercicio
// Grava na SRAM 1000 leituras de aceleração dps envia p/ o monitor serial
char exercicio_5(char md){
  byte who;
  word count=0, n_samples=1000, blk_n_samples=10, i;
  word blk_wr_size = (blk_n_samples * 3 * 2);
  int mpu_temp_data[7];
  byte acel_blk_data[blk_wr_size];
  long sram_pt_wr=0x00000;
  long sram_pt_rd=0x00000;
  int rd_data[3];

  lcd_str(0,0,exercicios_msg[md]);
  ser_str(exercicios_msg[md]);
  delay(500);
  lcd_apaga();

  mpu_acorda();     //Acordar MPU
  who=mpu_whoami();
  lcd_str(1,0,"Who am I = ");
  lcd_hex8(1,10,who);
  if (who == 0x73) {
    lcd_str(1,15," OK");  //MPU respondendo
  } else {
    lcd_str(1,15," NOK");  //MPU respondendo
    return 0;
  }

  mpu_inicializa();     //Inicializar
  mpu_escalas(0,0);     //+/- 2g e +/-250gr/seg
  lcd_str(3,0,"Aperte p/ iniciar");
  delay(1000);
  sw_qq_tecla();
  lcd_apaga();

  lcd_str(1,0, "  Leituras feitas:  ");

  // Escreve na SRAM N medidadas da aceleração
  while (count < n_samples) {

    // Preenche o array de dados somente com a aceleração, até preencher o tamanho de 1 bloco 
    for( i=0; i < blk_n_samples; i++ ) {
      mpu_rd_ac_tp_gi( mpu_temp_data );
      // Salva ByteH depois ByteL p/ depois escrever na ram nesta ordem
      acel_blk_data[(i * 3 * 2) + 0 ] = (byte)(mpu_temp_data[0] >> 8);
      acel_blk_data[(i * 3 * 2) + 1 ] = (byte)(mpu_temp_data[0] & 0x0000FFFF);
      acel_blk_data[(i * 3 * 2) + 2 ] = (byte)(mpu_temp_data[1] >> 8);
      acel_blk_data[(i * 3 * 2) + 3 ] = (byte)(mpu_temp_data[1] & 0x0000FFFF);
      acel_blk_data[(i * 3 * 2) + 4 ] = (byte)(mpu_temp_data[2] >> 8);
      acel_blk_data[(i * 3 * 2) + 5 ] = (byte)(mpu_temp_data[2] & 0x0000FFFF);
      
      count++;
    }
    
    sram_wr_blk(sram_pt_wr, acel_blk_data, blk_wr_size );

    lcd_dec16u(2,7, count);
    sram_pt_wr += blk_wr_size;
  }

  count = 0;

  ser_str("\nFinalizado escrita dos dados\n");
  lcd_str(1,0, "  Leituras concluidas:  ");
  delay(1000);

  ser_str("Iniciando leitura\n");

  // Lê na SRAM N medidadas da aceleração uma a uma
  while (count < n_samples) {
    
    // Lê dois bytes de aceleração em cada eixo concatena e printa
    rd_data[0] = (int)( (sram_rd(sram_pt_rd + 0) << 8) | sram_rd(sram_pt_rd + 1) );
    rd_data[1] = (int)( (sram_rd(sram_pt_rd + 2) << 8) | sram_rd(sram_pt_rd + 3) );
    rd_data[2] = (int)( (sram_rd(sram_pt_rd + 4) << 8) | sram_rd(sram_pt_rd + 5) );
    ser_dec16(rd_data[0]); ser_crlf(1);
    ser_dec16(rd_data[1]); ser_crlf(1);
    ser_dec16(rd_data[2]); ser_crlf(1);

    count++;
    sram_pt_rd += 2 * 3;
  }

  return md;
}

// 6 - exercicio
// Grava na SRAM 1000 leituras de giro dps envia p/ o monitor serial
char exercicio_6(char md){
  byte who;
  word count=0, n_samples=1000, blk_n_samples=10, i;
  word blk_wr_size = (blk_n_samples * 3 * 2);
  int mpu_temp_data[7];
  byte giro_blk_data[blk_wr_size];
  long sram_pt_wr=0x00000;
  long sram_pt_rd=0x00000;
  int rd_data[3];

  lcd_str(0,0,exercicios_msg[md]);
  ser_str(exercicios_msg[md]);
  delay(500);
  lcd_apaga();

  mpu_acorda();     //Acordar MPU
  who=mpu_whoami();
  lcd_str(1,0,"Who am I = ");
  lcd_hex8(1,10,who);
  if (who == 0x73) {
    lcd_str(1,15," OK");  //MPU respondendo
  } else {
    lcd_str(1,15," NOK");  //MPU respondendo
    return 0;
  }

  mpu_inicializa();     //Inicializar
  mpu_escalas(0,0);     //+/- 2g e +/-250gr/seg
  lcd_str(3,0,"Aperte p/ iniciar");
  delay(1000);
  sw_qq_tecla();
  lcd_apaga();

  lcd_str(1,0, "  Leituras feitas:  ");

  // Escreve na SRAM N medidadas do giro
  while (count < n_samples) {

    // Preenche o array de dados somente com o giro, até preencher o tamanho de 1 bloco 
    for( i=0; i < blk_n_samples; i++ ) {
      mpu_rd_ac_tp_gi( mpu_temp_data );
      // Salva ByteH depois ByteL p/ depois escrever na ram nesta ordem
      giro_blk_data[(i * 3 * 2) + 0 ] = (byte)(mpu_temp_data[4] >> 8);
      giro_blk_data[(i * 3 * 2) + 1 ] = (byte)(mpu_temp_data[4] & 0x0000FFFF);
      giro_blk_data[(i * 3 * 2) + 2 ] = (byte)(mpu_temp_data[5] >> 8);
      giro_blk_data[(i * 3 * 2) + 3 ] = (byte)(mpu_temp_data[5] & 0x0000FFFF);
      giro_blk_data[(i * 3 * 2) + 4 ] = (byte)(mpu_temp_data[6] >> 8);
      giro_blk_data[(i * 3 * 2) + 5 ] = (byte)(mpu_temp_data[6] & 0x0000FFFF);
      
      count++;
    }
    
    sram_wr_blk(sram_pt_wr, giro_blk_data, blk_wr_size );

    lcd_dec16u(2,7, count);
    sram_pt_wr += blk_wr_size;
  }

  count = 0;

  ser_str("\nFinalizado escrita dos dados\n");
  lcd_str(1,0, "  Leituras concluidas:  ");
  delay(1000);

  ser_str("Iniciando leitura\n");

  // Lê na SRAM N medidadas do giro uma a uma
  while (count < n_samples) {
    
    // Lê dois bytes de giro em cada eixo concatena e printa
    rd_data[0] = (int)( (sram_rd(sram_pt_rd + 0) << 8) | sram_rd(sram_pt_rd + 1) );
    rd_data[1] = (int)( (sram_rd(sram_pt_rd + 2) << 8) | sram_rd(sram_pt_rd + 3) );
    rd_data[2] = (int)( (sram_rd(sram_pt_rd + 4) << 8) | sram_rd(sram_pt_rd + 5) );
    ser_dec16(rd_data[0]); ser_crlf(1);
    ser_dec16(rd_data[1]); ser_crlf(1);
    ser_dec16(rd_data[2]); ser_crlf(1);

    count++;
    sram_pt_rd += 2 * 3;
  }

  return md;
}

// 7 - exercicio
// Grava na SRAM uma quantidade variavel de aceleração e giro dps envia p/ monitor serial
// NOTE: Selecione o tamanho do bloco de leitura (blk_n_samples) de modo que 21845‬ seja multiplo
// Uma vez que cabem 21.845‬ Amostras = 21845 * (6 * 2Bytes) = 262140 = 256KB - 4B (Não cabe mais uma amostra nesses 4B)
char exercicio_7(char md){
  byte who;
  byte stop = FALSE;
  word count=0, n_samples=0, blk_n_samples=17, i;
  word blk_wr_size = (blk_n_samples * 6 * 2);
  int mpu_temp_data[7];
  byte mpu_blk_data[blk_wr_size];
  long sram_pt_wr=0x00000;
  long sram_pt_rd=0x00000;
  int rd_data[6];

  // Cabem 21.845‬ Amostras = 21845 * (6 * 2Bytes) = 262140 = 256KB - 4B (Não cabe mais uma amostra nesses 4B)
  long sram_min=0x00000;
  long sram_max=0x3FFFC;

  // Checa se 262140B (tamanho que será escrito na ram) é multiplo de blk_wr_size
  if ( (262140 % blk_wr_size) != 0 ) {
    lcd_str(0,0, "ERRO- veja o console");
    ser_str("Tamanho do bloco de amostras não aceito, favor altere e execute novamente");
    delay(1000);
    return -1;
  }

  lcd_str(0,0,exercicios_msg[md]);
  ser_str(exercicios_msg[md]);
  delay(500);
  lcd_apaga();

  mpu_acorda();     //Acordar MPU
  who=mpu_whoami();
  lcd_str(1,0,"Who am I = ");
  lcd_hex8(1,10,who);
  if (who == 0x73) {
    lcd_str(1,15," OK");  //MPU respondendo
  } else {
    lcd_str(1,15," NOK");  //MPU respondendo
    return 0;
  }

  mpu_inicializa();     //Inicializar
  mpu_escalas(0,0);     //+/- 2g e +/-250gr/seg
  lcd_str(3,0,"Aperte p/ iniciar");
  delay(1000);
  sw_qq_tecla();
  lcd_apaga();

  lcd_str(0,0, "  Leituras feitas:  ");
  lcd_str(3,0, "  Aperte p/ parar:  ");

  // Escreve na SRAM medidade de aceleração e giro até algo ser apertado ou bater no limite da ram
  while (stop == 0 && sram_pt_wr < sram_max) {

    // Preenche o array de dados com aceleração e giro, até preencher o tamanho de 1 bloco 
    for( i=0; i < blk_n_samples; i++ ) {
      mpu_rd_ac_tp_gi( mpu_temp_data );
      // Salva ByteH depois ByteL p/ depois escrever na ram nesta ordem
      // Aceleração
      mpu_blk_data[(i * 6 * 2) + 0 ] = (byte)(mpu_temp_data[0] >> 8);
      mpu_blk_data[(i * 6 * 2) + 1 ] = (byte)(mpu_temp_data[0] & 0x0000FFFF);
      mpu_blk_data[(i * 6 * 2) + 2 ] = (byte)(mpu_temp_data[1] >> 8);
      mpu_blk_data[(i * 6 * 2) + 3 ] = (byte)(mpu_temp_data[1] & 0x0000FFFF);
      mpu_blk_data[(i * 6 * 2) + 4 ] = (byte)(mpu_temp_data[2] >> 8);
      mpu_blk_data[(i * 6 * 2) + 5 ] = (byte)(mpu_temp_data[2] & 0x0000FFFF);
      // Giro
      mpu_blk_data[(i * 6 * 2) + 6 ] = (byte)(mpu_temp_data[4] >> 8);
      mpu_blk_data[(i * 6 * 2) + 7 ] = (byte)(mpu_temp_data[4] & 0x0000FFFF);
      mpu_blk_data[(i * 6 * 2) + 8 ] = (byte)(mpu_temp_data[5] >> 8);
      mpu_blk_data[(i * 6 * 2) + 9 ] = (byte)(mpu_temp_data[5] & 0x0000FFFF);
      mpu_blk_data[(i * 6 * 2) + 10 ] = (byte)(mpu_temp_data[6] >> 8);
      mpu_blk_data[(i * 6 * 2) + 11 ] = (byte)(mpu_temp_data[6] & 0x0000FFFF);

      n_samples++;
      count++;

      if (sw_tira(&who)) {
        stop = TRUE;
      }
    }
    
    sram_wr_blk(sram_pt_wr, mpu_blk_data, blk_wr_size );

    lcd_dec16u(1,7, count);
    sram_pt_wr += blk_wr_size;
  }

  count = 0;

  ser_str("\nFinalizado escrita dos dados\n");
  lcd_str(0,0, "  Leituras concluidas:  ");
  delay(1000);

  ser_str("Iniciando leitura\n");

  // Lê na SRAM N medidadas de aceleração e giro uma a uma
  while (count < n_samples) {
    
    // Lê dois bytes de giro em cada eixo concatena e printa
    rd_data[0] = (int)( (sram_rd(sram_pt_rd + 0) << 8) | sram_rd(sram_pt_rd + 1) );
    rd_data[1] = (int)( (sram_rd(sram_pt_rd + 2) << 8) | sram_rd(sram_pt_rd + 3) );
    rd_data[2] = (int)( (sram_rd(sram_pt_rd + 4) << 8) | sram_rd(sram_pt_rd + 5) );
    rd_data[3] = (int)( (sram_rd(sram_pt_rd + 6) << 8) | sram_rd(sram_pt_rd + 7) );
    rd_data[4] = (int)( (sram_rd(sram_pt_rd + 8) << 8) | sram_rd(sram_pt_rd + 9) );
    rd_data[5] = (int)( (sram_rd(sram_pt_rd + 10) << 8) | sram_rd(sram_pt_rd + 11) );
    ser_dec16(rd_data[0]); ser_crlf(1);
    ser_dec16(rd_data[1]); ser_crlf(1);
    ser_dec16(rd_data[2]); ser_crlf(1);
    ser_dec16(rd_data[3]); ser_crlf(1);
    ser_dec16(rd_data[4]); ser_crlf(1);
    ser_dec16(rd_data[5]); ser_crlf(1);

    count++;
    sram_pt_rd += 2 * 6;
  }

  return md;
}

// 8 - exercicio
// Grava na SRAM de forma circular de aceleração e giro dps envia p/ monitor serial
char exercicio_8(char md){
  lcd_str(0,0,exercicios_msg[md]);
  ser_str(exercicios_msg[md]);
  sw_qq_tecla();
  return md;
}
