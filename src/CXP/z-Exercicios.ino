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
  lcd_str(3,0, "SEQ1/2: zera  ");
  lcd_str(2, 14, "VALOR:");
  lcd_str(3, 14, "+00000");

  word count = 0;
  char count_str[7];
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

    str_dec16(count, count_str);
    lcd_str(3, 14, count_str);
  }

  return md;
}

// 3 - exercicio
// Lê aceleração do MPU e exibe no LCD
char exercicio_3(char md){
  lcd_str(0,0,exercicios_msg[md]);
  ser_str(exercicios_msg[md]);
  sw_qq_tecla();
  return md;
}

// 4 - exercicio
// Lê giro do MPU e exibe no LCD
char exercicio_4(char md){
  lcd_str(0,0,exercicios_msg[md]);
  ser_str(exercicios_msg[md]);
  sw_qq_tecla();
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