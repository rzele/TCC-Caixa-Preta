// Modos de operação
// CXP - Caixa Preta
// 06/04/2020

// Modo Operação 
void opera(void){
  byte modo;    //Modo selecionado
  byte i;
  word w;
  long z;
  float f;
  while(TRUE){
    lcd_apaga();
    lcd_str(0,0,"OPERA");
    ser_str("\nModo Opera\n");
    lcd_str(0,5," - Selecionar");
    ser_str("Selecionar com LCD\n");
    modo=sel_modo(opera_msg, OPERA_TOT);
    //modo=teste_sel();
    //modo=12;
    lcd_apaga();
    ser_crlf(1);
    switch(modo){
      case OPERA_1:  opera_1(modo);  break;
      case OPERA_2:  opera_2(modo);  break;
      case OPERA_3:  opera_3(modo);  break;
      case OPERA_4:  opera_4(modo);  break;
      case OPERA_5:  opera_5(modo);  break;
      case OPERA_6:  opera_6(modo);  break;
      case OPERA_7:  opera_7(modo);  break;
    }
  }
  while(1);
}


// 1 - Vazio
char opera_1(char md){
  lcd_str(0,0,opera_msg[md]);
  ser_str(opera_msg[md]);
  sw_qq_tecla();
  return md;
}

// 2 - Vazio
char opera_2(char md){
  lcd_str(0,0,opera_msg[md]);
  ser_str(opera_msg[md]);
  sw_qq_tecla();
  return md;
}

// 3 - Vazio
char opera_3(char md){
  lcd_str(0,0,opera_msg[md]);
  ser_str(opera_msg[md]);
  sw_qq_tecla();
  return md;
}
// 4 - Vazio
char opera_4(char md){
  lcd_str(0,0,opera_msg[md]);
  ser_str(opera_msg[md]);
  sw_qq_tecla();
  return md;
}

// 5 - Vazio
char opera_5(char md){
  lcd_str(0,0,opera_msg[md]);
  ser_str(opera_msg[md]);
  sw_qq_tecla();
  return md;
}

// 6 - Vazio
char opera_6(char md){
  lcd_str(0,0,opera_msg[md]);
  ser_str(opera_msg[md]);
  sw_qq_tecla();
  return md;
}

// 7 - Vazio
char opera_7(char md){
  lcd_str(0,0,opera_msg[md]);
  ser_str(opera_msg[md]);
  sw_qq_tecla();
  return md;
}
