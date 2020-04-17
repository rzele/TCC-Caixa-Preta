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
    //modo=sel_modo(opera_msg, OPERA_TOT);
    modo=5;
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

// 5 - Ensaio de operação
char opera_5(char md){
  char msg_aux[20];
  char *msg1="[5] Ensaio";
  char *msg2="Gravar! SEL interrompe.";
  char *msg3="Qq. tecla inicia.";
  char mpu_adr_ovf=FALSE;             //Marcar se gravação MPU deu uma volta na SRAM
  char gps_adr_ovf=FALSE;             //Marcar se gravação GPS deu uma volta na SRAM
  byte i,tec=0;
  unsigned long mpu_adr,gps_adr;          //Contador para gravação
  unsigned long adr_aux;                  //Contador para gerar saída na ordem correta
  byte vt[18];    //Receber ax, ay, az, gx, gy, gz, hx, hy, hz ou gps_dados

  lcd_str(0,0,msg1);
  lcd_str(1,0,msg2);
  lcd_str(2,0,msg3);
  ser_str(msg1); ser_crlf(1);
  ser_str(msg2); ser_crlf(1);
  ser_str(msg3); ser_crlf(1);
  sw_qq_tecla();
  ser_str("------------ Iniciou -------------------\n");
  lcd_apaga_lin(1);  
  lcd_apaga_lin(2);
  lcd_str(1,6,"Enderecos");
  lcd_str(2,3,"MPU       GPS");

  // GPS RX3: Habilitar recepção e sua interrupção
  UCSR3B = (1<<RXCIE3) | (1<<RXEN3);  //RXIE=1, RXEN=1
 
  // Habilitar interrupção MPU (Dado Pronto)
  mpu_int();

  // Laço principal
  mpu_adr=MPU_ADR_INI;
  gps_adr=GPS_ADR_INI;
  flag_10ms=FALSE;
  gps_tipo=GPS_NADA;
  while(TRUE){
    lcd_hex32(3,0,mpu_adr);
    lcd_hex32(3,10,gps_adr);
    // MPU - Gravar a cada 10 ms
    //if (flag_10ms == TRUE){  //A cada 10ms Timer1
   if (mpu_dado_ok == TRUE){   //MPU a 100 Hz (10 ms)
      mpu_dado_ok=FALSE;
      mpu_rd_ac_gi_mg(vt);
      sram_wr_blk(mpu_adr, vt, MPU_PASSO); //Gravar MPU na SRAM
      mpu_adr += MPU_PASSO;
      if (mpu_adr == MPU_ADR_FIM){
        mpu_adr=MPU_ADR_INI;    
        mpu_adr_ovf=TRUE;
      }
    }
    
    // GPS - Receber mensagens
    if (gps_msg_ok == TRUE){
      gps_msg_ok=FALSE;
      if (gps_msg_fase==0){
        ser_str(gps_msg_1);     //Imprimir msg GPS
        ser_crlf(1);            //CR LF
        gps_extrai(gps_msg_1);
      }
      else{
        ser_str(gps_msg_0);     //Imprimir msg gps
        ser_crlf(1);            //CR LF
        gps_extrai(gps_msg_0);
      }
    }

    // GPS - Gravar após receber GPS_GLL
    if (gps_tipo == GPS_GLL){
      str_dec32u(mpu_adr, msg_aux);  //Gerar string para endereço mpu_adr
      str_rmvz_u(msg_aux);           //Remover zeros à esquerda
      str_copia(msg_aux,&gps_dados[GPS_ADR_SRAM]);
      gps_tipo=GPS_NADA;
      sram_wr_blk(gps_adr, gps_dados, GPS_PASSO); //Gravar gps_dados na SRAM
      gps_adr += GPS_PASSO;
      if (gps_adr == GPS_ADR_FIM){
        gps_adr=GPS_ADR_INI;    
        gps_adr_ovf=TRUE;
      }
    }

    //Tecla?
    if (sw_tira(&tec)==TRUE){
      if (tec==SW_SEL)  break;
    }
    
  }

  // Desabilitar interrupção MPU
  mpu_des_int();

  // (GPS) RX3: Desabilitar recepção e sua interrupção
  UCSR3B &= ~((1<<RXCIE3) | (1<<RXEN3));  //RXIE=0, RXEN=0


  /////////////// Marcar o final //////////////////////
  // Gravar linha com 22222 = 0x56CE
  for (i=0; i<18; i+=2){
    vt[i+0]=0x56;
    vt[i+1]=0xCE;
  }
  sram_wr_blk(mpu_adr, vt, MPU_PASSO); //Gravar linha com 22222 na SRAM
  mpu_adr += MPU_PASSO;
  if (mpu_adr == MPU_ADR_FIM){
    mpu_adr=MPU_ADR_INI;    
    mpu_adr_ovf=TRUE;
  }

  //Imprimir dados SRAM (MPU)
  ser_str("\nax ay az gx gy gz hx hy hz\n");
  if (mpu_adr_ovf == TRUE){
    adr_aux=mpu_adr;                //mpu_adr --> topo
    while(adr_aux < MPU_ADR_FIM){
      sram_rd_blk(adr_aux, vt, MPU_PASSO); //Ler pacote de dados MPU
      ser_lin_ac_gi_mg(vt);   //Imprimir todos numa linha
      //ser_ac_gi_mg(vt);     //Imprimir um por linha
      adr_aux += MPU_PASSO;
    }
  }
  adr_aux=MPU_ADR_INI;                      //0 --> mpu_adr
  while(adr_aux < mpu_adr){
    sram_rd_blk(adr_aux, vt, MPU_PASSO); //Ler pacote de dados MPU
    ser_lin_ac_gi_mg(vt);   //Imprimir todos numa linha
    //ser_ac_gi_mg(vt);     //Imprimir um por linha
    adr_aux += MPU_PASSO;
  }

  //Imprimir dados SRAM (GPS)
  ser_str("\n  GPS \n");
  if (gps_adr_ovf == TRUE){
    adr_aux=gps_adr;                //gps_adr --> topo
    while(adr_aux < GPS_ADR_FIM){
      sram_rd_blk(adr_aux, gps_dados, GPS_PASSO); //Ler pacote de dados MPU
      ser_gps_dados(gps_dados);
      ser_crlf(1);
      adr_aux += GPS_PASSO;
    }
  }
  adr_aux=GPS_ADR_INI;            //0 --> gps_adr
  while(adr_aux < gps_adr){
    sram_rd_blk(adr_aux, gps_dados, GPS_PASSO); //Ler pacote de dados MPU
    ser_gps_dados(gps_dados);
    ser_crlf(1);
    adr_aux += GPS_PASSO;
  }

  lcd_str(1,0,"FIM !");
  ser_str("------------ FIM -------------------\n");
  delay(1000);
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
