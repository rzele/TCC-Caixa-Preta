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
  lcd_str(0,0,exercicios_msg[md]);
  ser_str(exercicios_msg[md]);
  sw_qq_tecla();
  return md;
}

// 6 - exercicio
// Grava na SRAM 1000 leituras de giro dps envia p/ o monitor serial
char exercicio_6(char md){
  lcd_str(0,0,exercicios_msg[md]);
  ser_str(exercicios_msg[md]);
  sw_qq_tecla();
  return md;
}

// 7 - exercicio
// Grava na SRAM uma quantidade variavel de aceleração e giro dps envia p/ monitor serial
char exercicio_7(char md){
  lcd_str(0,0,exercicios_msg[md]);
  ser_str(exercicios_msg[md]);
  sw_qq_tecla();
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
