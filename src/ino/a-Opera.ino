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
    //modo=8;
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
      case OPERA_8:  opera_8(modo);  break;
      case OPERA_9:  opera_9(modo);  break;
      case 'T':      teste();        break;
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

// Proposta para Caixa Preta
// 6 - Caixa Preta Versão Alfa
char opera_6(char md){
  char *msg1="[6] Caixa Preta Alfa";
  char *msg2="SEL = ACIDENTE!";
  char *msg5="Qq tecla inicia";
  char *msg6="=> BATEU <=";
  char *msg7="Qq tecla termina";
  char *msg8="Gravando FLASH.";
  char msg_aux[20];
  byte tec;
  byte esc_ac,esc_gi,esc_mg;   //Guardar Escalas de acel, giro e mag
  int z;
  int aux[7];
  byte st;        //Resultado do Self Test
  int vet[18]; //Receber resultados do Self Test
  //long w;
  byte bateu;                   //Indicar se bateu (FALSE / TRUE)
  unsigned long mpu_adr;        //MPU - Contador de endereços para gravação
  unsigned long mpu_adr_bateu;  //MPU - Endereço para finalizar gravação após batida
  unsigned long gps_adr;        //GPS - Contador de endereços para gravação
  unsigned long cont_pb;        //Contador decrescente para indicar o que falta pós batida
  byte data[8];                 //Última atualização da data
  byte hora[12];                //Última atualização da hora
  
  byte vt[18];                  //Receber ax, ay, az, gx, gy, gz, hx, hy, hz ou gps_dados

  lcd_str(0,0,msg1);
  lcd_str(1,0,msg2);
  ser_str(msg1); ser_crlf(1);
  ser_str(msg2); ser_crlf(1);
  
  // Fazer toda a preparação para a operação
  // Grava SRAM, faz calibração ao ligar, inicializa MPU
  opera6_prepara();        //Fazer todos os preparativos para a operação da Caixa Preta
  lcd_str(3,0,msg5);
  ser_crlf(1);
  ser_str(msg5);
  sw_qq_tecla();    //Esperar uma tecla
  lcd_apaga_lin(3);

  // Preparar para aquisição
  bateu=FALSE;
  mpu_adr=MPU_ADR_INI;
  gps_adr=GPS_ADR_INI;
  gps_tipo=GPS_NADA;
  mpu_dado_ok=FALSE;
  gps_int();
  lcd_str(2,3,"MPU      GPS");
  
  //Laço de aquisição
  while(TRUE){
    lcd_hex32(3,0,mpu_adr);
    lcd_hex32(3,10,gps_adr);

    // MPU trabalha por interrupção
    if (mpu_dado_ok == TRUE){   //MPU a 100 Hz (10 ms)
      mpu_dado_ok=FALSE;
      mpu_rd_ac_gi_mg(vt);
      sram_wr_blk(mpu_adr, vt, MPU_PASSO); //Gravar MPU na SRAM
      mpu_adr += MPU_PASSO;
      if (mpu_adr == MPU_ADR_FIM) mpu_adr=MPU_ADR_INI;
      if (bateu == TRUE)    cont_pb -= MPU_PASSO;       //Se bateu, acionar contador decrescente
    }

    // GPS usa interrupção com dois buffers selecionados pela variável gps_msg_fase
    if (gps_msg_ok == TRUE){
      gps_msg_ok=FALSE;
      if (gps_msg_fase==0){
        ser_str(gps_msg_1);     //Imprimir msg GPS
        ser_crlf(1);            //CR LF
        gps_extrai(gps_msg_1);  //Extrair parâmetros do GPS
      }
      else{
        ser_str(gps_msg_0);     //Imprimir msg gps
        ser_crlf(1);            //CR LF
        gps_extrai(gps_msg_0);  //Extrair parâmetros do GPS
      }
    }

    // GPS - Só grava após receber GPS_GLL
    if (gps_tipo == GPS_GLL){
      gps_tipo=GPS_NADA;
      ser_crlf(1);
      str_dec32u(mpu_adr, msg_aux);                 //Gerar string do contador de endereço mpu_adr
      str_rmvz_u(msg_aux);                          //Remover zeros à esquerda
      str_copia(msg_aux,&gps_dados[GPS_ADR_SRAM]);  //Copiar para o vetor gps_dados
      sram_wr_blk(gps_adr, gps_dados, GPS_PASSO);   //Gravar gps_dados na SRAM
      gps_adr += GPS_PASSO;                         //Avançar contador de endereço gps_adr
      if (gps_adr == GPS_ADR_FIM)                   //Verificar limite do contador
        gps_adr=GPS_ADR_INI;
      str_copia(&gps_dados[GPS_HORA],hora);         //Atualizar variável hora, para o caso de batida
      str_copia(&gps_dados[GPS_DATA],data);         //Atualizar variável data, para o caso de batida
    }

    // Se bateu, verificar se já preencheu a metade do que faltava
    if (bateu == TRUE){
      lcd_dec32u(2,12,cont_pb);        //Imprimir contador pós batida
      if (mpu_adr == mpu_adr_bateu)
        break;
    }

     //Tecla? Simula batida
    if (sw_tira(&tec)==TRUE){
      if (tec==SW_SEL){
        bateu=TRUE;       //Indicar que bateu
        sram_wr_32b(OP_MPU_ADR, mpu_adr);                      //Guardar contador de endereços SRAM(MPU) no instante da batida
        sram_wr_32b(OP_GPS_ADR, gps_adr);                      //Guardar contador de endereços SRAM(GPS) no instante da batida
        sram_wr_str(OP_AC_DATA, data);                          //Guardar data da batida
        sram_wr_str(OP_AC_HORA, hora);                          //Guardar hora da batida
        sram_wr_16b(OP_BATEU,COD_SIM);                          //Marcar BATIDA
        sram_wr_16b(OP_DISP_AX, COD_SIM);                       //AX simula disparo da batida
        cont_pb=(MPU_ADR_FIM-MPU_ADR_INI)/2;                    //Inicializar contador pós batida com Metade do espaço
        mpu_adr_bateu = mpu_adr + (MPU_ADR_FIM-MPU_ADR_INI)/2;  //Calcular endereço para finalizar gravação
        if (mpu_adr_bateu>MPU_ADR_FIM)                          //Ultrapassou fim da área especificada?
          mpu_adr_bateu = mpu_adr_bateu - MPU_ADR_FIM;          //Voltar para o início (só a diferença)
        ser_str("\n==> BATEU <== mpu_adr = ");
        ser_hex32(mpu_adr);
        ser_str(" mpu_adr_bateu = ");
        ser_hex32(mpu_adr_bateu);
        ser_crlf(2);
        lcd_str(2,0,msg6);
      }
    }
  }
  
  /////////////// Marcar o final //////////////////////
  // Gravar linha com 22222 = 0x56CE
  for (z=0; z<18; z+=2){
    vt[z+0]=0x56;
    vt[z+1]=0xCE;
  }
  sram_wr_blk(mpu_adr, vt, MPU_PASSO); //Gravar linha com 22222 na SRAM

  gps_des_int();  //GPS: Desabilitar interrupção
  mpu_des_int();  //MPU: Desabilitar interrupção
  lcd_apaga_lin(2);
  lcd_apaga_lin(3);
  lcd_str(2,0,msg6);
  lcd_str(3,0,msg8);
  ser_str(msg6);
  ser_crlf(1);
  ser_str(msg8);
  ser_crlf(1);
  sram_flash();         //Copiar SRAM ==> Flash
  lcd_apaga_lin(3);
  lcd_str(3,0,msg7);
  ser_str(msg7);

  //sram_dump(CXP_ADR_INI,128);
  //flash_dump(CXP_ADR_INI,128);
  flash_dados_acid();
  sw_qq_tecla();
  return md;
}

// Fazer toda a preparação para a operação 6
void opera6_prepara(void){
  char *msg3="Self Test = ";
  char *msg4="Calib Fabr  = ";
  byte x;
  byte esc_ac,esc_gi,esc_mg;   //Guardar Escalas de acel, giro e mag
  int z;
  int aux[7];
  byte st;        //Resultado do Self Test
  int st_vet[24]; //Receber resultados do Self Test

  //Zerar a SRAM
  ser_str("Zerando SRAM...");
  lcd_str(2,0,"Zerando SRAM");
  sram_zera();
  ser_crlf(1);
  lcd_apaga_lin(2);

  // Inicializar MPU
  mpu_acorda();
  mpu_config();
  x=mpu_whoami();
  if (x != MPU9250_WHO){
    //lcd_str(2,0,"Erro: Who=0x");  //MPU respondendo
    //lcd_hex8(2,12,x);
    ser_str("Erro: Who=0x");
    ser_hex8(x);
    delay(5000);
  }

  // Marcar como Não batida
  sram_wr_16b(OP_BATEU,COD_NAO);

  //Realizar o SELF TEST
  //lcd_str(2,0,msg3);
  ser_str(msg3);
  st=mpu_self_test(st_vet,FALSE);
  if ( st == TRUE){ sram_wr_16b(OP_ST_OK,COD_SIM);  ser_str("OK");   }
  else            { sram_wr_16b(OP_ST_OK,COD_NAO);  ser_str("NOK");  }
  ser_crlf(1);

  // Existe Calibração de Fábrica ?
  //lcd_str(3,0,msg4);
  ser_str(msg4);
  z=eeprom_rd_16b(CF_OK);   //Ler se há Calibr de Fábrica
  sram_wr_16b(OP_CF_OK,z);  //Gravar código de Calibr de Fábrica
  if ( z == COD_SIM)  { ser_str("OK");   }
  else                { ser_str("NOK");  }
  ser_crlf(1);

  //Calibração Acel e Giro ao ligar o carro
  ser_str("Calibrar ao Ligar...");
  esc_ac=ACEL_FS_2G;    //Escala acel para calibrar ao ligar
  esc_gi=GIRO_FS_250;   //Escala giro para calibrar ao ligar
  mpu_calibra(aux, OP_QTD_MED_AG, esc_ac, esc_gi);  //Função de calibração
  sram_wr_16b(OPC_QTD_AG,OP_QTD_MED_AG);   //Qtd de medidas para calibração ao ligar
  sram_wr_16b(OPC_ESC_AC,esc_ac);      //Calibra sempre usa +/- 2g
  sram_wr_16b(OPC_ESC_GI,esc_gi);      //Calibra sempre usa +/- 250 gr/s
  for (x=0; x<7; x++)   sram_wr_16b(OPC_AX+(2*x),aux[x]);
  
  //Calibração Magnetômetro ao ligar o carro
  //esc_mg=0;             //Escala do Magnetômetro
  //sram_wr_16b(OPC_QTD_MG,OP_QTD_MED_MG);    //Qtd de medidas para  média do Mag calibração ao ligar
  //sram_wr_16b(OPC_ESC_MG,esc_mg);           //Escala do Mag para calibração ao ligar
  //mag_calibra();

  //Def freq e limiares de acidente
  sram_wr_32b(OP_MPU_ADR,0L);          //Endereço da SRAM onde ocorreu disparo
  sram_wr_16b(OP_FREQ_AG,OP_FREQ);      //Freq de amostragem para operação
  sram_wr_16b(OP_ESC_AC,OP_ESC_ACEL);   //Escala ACEL para operação
  sram_wr_16b(OP_ESC_GI,OP_ESC_GIRO);   //Escala GIRO para operação
  sram_wr_16b(OP_LIM_AX,LIMIAR_AX);     //AX - Limiar para disparo de acidente
  sram_wr_16b(OP_LIM_AY,LIMIAR_AY);     //AY - Limiar para disparo de acidente
  sram_wr_16b(OP_LIM_AZ,LIMIAR_AZ);     //AZ - Limiar para disparo de acidente
  sram_wr_16b(OP_LIM_GX,LIMIAR_GX);     //GX - Limiar para disparo de acidente
  sram_wr_16b(OP_LIM_GY,LIMIAR_GY);     //GY - Limiar para disparo de acidente
  sram_wr_16b(OP_LIM_GZ,LIMIAR_GZ);     //GZ - Limiar para disparo de acidente

  //Quem disparou
  for (x=0; x<6; x++)   sram_wr_16b(OP_DISP_AX+2*x,COD_NAO);

  //Data e Hora
  sram_wr_str(OP_AC_DATA,"ddmmyy");
  sram_wr_str(OP_AC_HORA,"hhmmss.sss");

  //sram_op_mostra();
}


// 7 - Calibração de Fábrica
char opera_7(char md){
  char *msg1="[7]Calibracao Fabrica";
  byte x;
  lcd_str(0,0,msg1);
  lcd_str(2,0,"Use Monitor Serial");
  while(TRUE){
    ser_str(msg1);                    ser_crlf(1);
    ser_str("SEL --> Inicia");        ser_crlf(1);
    ser_str("DIR --> Mostra dados");  ser_crlf(1);
    ser_str("ESQ --> Retorna");       ser_crlf(1);
    while( sw_tira(&x) == FALSE);
    switch(x){
      case SW_SEL:
        calibra_fab();
        break;
      case SW_DIR:
        eeprom_cf_mostra();
        break;
      case SW_ESQ:
        return md;
    }
  }
  sw_qq_tecla();
  return md;
}

// Rotina de Calibração de Fábrica
void calibra_fab(void){
  float gp,gl;      //Ac.da gravidade
  word mpu_gp=0;    //Receber Ac.da gravidade padrão em 16 bits (+/- 2g)
  word mpu_gl=0;    //Receber Ac.da gravidade local em 16 bits (+/- 2g)
  long cont=0;
  long atg_soma[7];   //Somatório para calc media [ax-ay-az-tp-gx-gy-gz]
  int atg[7];         //Receber leituras MPU
  int atg_pri[7];     //Receber a primeira medida
  int atg_ult[7];     //Receber a última medida
  word freq;
  word x,aux=0;
  byte who;
  byte vet[40];     //Receber strings para imprimir float
  int vet_st[24];   //Receber resultados do Self test
  int st;
  lcd_str(2,0,"Use Monitor Serial");
  ser_str("\n--- Calibracao de Fabrica ---\n");
  //eeprom_wr_ff();
  //eeprom_dump(0,256);

  mpu_config();
  who=mpu_whoami();
  mpu_escalas(GIRO_FS_250,ACEL_FS_2G);   //+/- 2g e +/-250gr/seg
  freq=CF_FREQ;
  switch(freq){
    case 100:   mpu_sample_rt(SAMPLE_RT_100Hz); break;
    case 200:   mpu_sample_rt(SAMPLE_RT_200Hz); break;
    case 250:   mpu_sample_rt(SAMPLE_RT_250Hz); break;
    case 500:   mpu_sample_rt(SAMPLE_RT_500Hz); break;
  }
  mpu_int();
  mpu_dado_ok=FALSE;
  cont=0;
  aux=0;
  while (cont<CF_ESPERA){
    while (mpu_dado_ok == FALSE);   //Aguardar MPU fazer medida
      mpu_dado_ok=FALSE;
      aux++;
      if (aux==freq){
        aux=0;
        cont++;         //Passou mais um segundo
        ser_dec32unz (CF_ESPERA-cont);
        ser_crlf(1);
      }
  }

  //Gravar dados de local e data
  eeprom_wr_str(CF_DATA,CF_HOJE);         //Data - Indicado em Defines
  eeprom_wr_str(CF_LOCAL,CF_BSB);         //Local - Indicado em Defines
  gp=G_PADRAO;
  gl=G_BSB;
  str_float(gp,7,vet);
  eeprom_wr_str(CFG_PADRAO,&vet[1]);      //g padrão, pula o sinal de  +9,8...
  str_float(gl,7,vet);
  eeprom_wr_str(CFG_LOCAL,&vet[1]);       //g local, pula o sinal de +9,8...
  eeprom_wr_16b(CFG_PADRAO_BIN,0x3FF);    //1g na escala de +/- 2g
  aux=0x3FF*(gl/gp);                       
  eeprom_wr_16b(CFG_LOCAL_BIN,aux);       //g Local na escala de +/- 2g
  eeprom_wr_16b(CF_WHO,who);              //Resposta ao Who am I
  //
  aux=mpu_rd_freq();
  eeprom_wr_16b(CF_FA,aux);               //Freq Amostragem usada
  aux=mpu_rd_bw();
  eeprom_wr_16b(CF_BW,aux);               //Banda Passante
  aux=mpu_rd_esc_acel();
  eeprom_wr_16b(CF_ESC_AC,aux);           //Escala Acelerômetro
  aux=mpu_rd_esc_giro();
  eeprom_wr_16b(CF_ESC_GI,aux);           //Escala Giroscópio
  
  // Fazer a primeira medida
  mpu_dado_ok=FALSE;
  while (mpu_dado_ok == FALSE);   //Aguardar MPU fazer medida
  mpu_rd_ac_tp_gi(atg);           //Ler MPU
  for (x=0; x<7; x++){
    atg_pri[x] =atg[x];           //Guardar a primeira medida
    atg_soma[x]=atg[x];           //Inicializar Acumulador de medidas
  }
  
  //Fazer as demais medidas
  cont=0;
  while(cont<CF_QTD_MED-2){       //Primeira e última medidas feitas à parte
    while (mpu_dado_ok == FALSE);  //Aguardar MPU fazer medida
    mpu_dado_ok=FALSE;
    mpu_rd_ac_tp_gi(atg);          //Ler MPU
    for (x=0; x<7; x++)   atg_soma[x]+=atg[x];  //Acumular
    cont++;
  }

  // Fazer a última medida
  while (mpu_dado_ok == FALSE);  //Aguardar MPU fazer medida
  mpu_rd_ac_tp_gi(atg);          //Ler MPU
  for (x=0; x<7; x++){
    atg_ult[x]  =atg[x];        //Guardar a última medida
    atg_soma[x]+=atg[x];        //Acumular
  }

  // Gravar as médias, as primeiras e as últimas medidas
  eeprom_wr_16b(CF_QTD,CF_QTD_MED);
  for (x=0; x<7; x++){
    eeprom_wr_16b(CF_AX+(2*x),atg_soma[x]/CF_QTD_MED);  //Médias
    eeprom_wr_16b(CF_AX_PRI+(2*x),atg_pri[x]);          //Primeiras
    eeprom_wr_16b(CF_AX_ULT+(2*x),atg_ult[x]);          //Últimas
    eeprom_wr_32b(CF_AX_SOMA+(4*x),atg_soma[x]);        //Somatórios
  }

  st=mpu_self_test(vet_st,FALSE);
  if (st==TRUE)   eeprom_wr_16b(CF_ST_OK,COD_SIM);
  else            eeprom_wr_16b(CF_ST_OK,COD_NAO);
  
  for (x=0; x<24; x++)
    eeprom_wr_16b(CF_ST_OFF_AX+(2*x),vet_st[x]);

  eeprom_wr_16b(CF_OK,COD_SIM);
 
  eeprom_cf_mostra();
  ser_crlf(2);
  eeprom_cf_dados();
  
  ser_str("\nFim. Qq tecla para finalizar.");
  sw_qq_tecla();
  
}


// 8 - Adquirir dados
// Semelhante ao Opera_6, <bateu> inicia aquisição
// Após <bateu>, Preenche toda a memória.
char opera_8(char md){
  char *msg1="[8] Adquirir Dados";
  char *msg2="Zerando SRAM";
  char *msg3="SEL = Inic Aquisicao";
  char *msg3a="SW_INF = Interrompe";
  char *msg4=" MPUxxxxx   GPS xxxxx";
  char *msg5="Faltam xxxxx segundos";
  char *msg6="==>> ADQUIRINDO <<==";
  char *msg7="==>> TX  SERIAL <<==";
  char *msg8="FIM: Tecle para sair";
  unsigned int mpu_cont, gps_cont;    //Contar dados para animar tela
  unsigned long mpu_adr;        //MPU - Contador de endereços para gravação
  unsigned long mpu_adr_bateu;  //MPU - Endereço para finalizar gravação após batida
  unsigned long gps_adr;        //GPS - Contador de endereços para gravação
  unsigned long cont_pb;        //Contador decrescente para indicar o que falta pós batida
  int z;
  byte tec;
  byte bateu;                   //Indicar se bateu (FALSE / TRUE)
  byte flag_brk;                //Indicar que laço de aquisição foi interrompido
  word segs;                    //Segundos que faltam para finalizar
  byte vt[18];                  //Receber ax, ay, az, gx, gy, gz, hx, hy, hz ou gps_dados
  char msg_aux[20];
  byte data[10];                 //Última atualização da data
  byte hora[12];                //Última atualização da hora

  //Variáveis para enviar LAT e LONG pelo Bluetooth
  int graus,mini,fracm;          //Graus, minito e fração de minuto
  long lati,longi;              //Valor de latitude e longitude
  float flat,flong;             //Valor de latitude e longitude
  unsigned long bt_seq=0;       //Contar sequência

  lcd_apaga();
  lcd_str(0,0,msg1);
  //ser_str(msg1);  ser_crlf(1);
  opera8_prepara();

  //Laço de principal
  gps_int();
  while (TRUE){
    lcd_apaga();
    lcd_str(0,0,msg1);
    ser_crlf(1);
    ser_str(msg1);  ser_crlf(1);
    lcd_str(1,0,msg2);
    ser_str(msg2);  ser_crlf(1);
    sram_zera_mpu_gps();   //Zerar SRAM, preserva Calib ao Ligar
    mpu_cont=0;
    gps_cont=0;
    lcd_apaga_lin(1);
    lcd_str(1,0,msg3);
    lcd_str(2,0,msg3a);
    ser_str(msg3);  ser_spc(2);
    ser_str(msg3a);  ser_crlf(1);
    lcd_str(3,0,msg4);
    bateu=FALSE;
    mpu_adr=MPU_ADR_INI;
    gps_adr=GPS_ADR_INI;
    gps_tipo=GPS_NADA;
    mpu_dado_ok=FALSE;
    flag_brk=FALSE;       //Flag para indicar interrupção da aquisição
    //gps_int();

    // Laço de Aquisição
    for (z=0; z<GPS_PASSO; z++) gps_dados[z]='\0'; //***Prencher com zeros
    while(TRUE){
      lcd_dec16u(3,4,mpu_cont);
      lcd_dec16u(3,15,gps_cont);
  
      // MPU trabalha por interrupção
      if (mpu_dado_ok == TRUE){   //MPU a 100 Hz (10 ms)
        mpu_dado_ok=FALSE;
        mpu_cont++;
        mpu_rd_ac_gi_mg(vt);
        sram_wr_blk(mpu_adr, vt, MPU_PASSO); //Gravar MPU na SRAM
        mpu_adr += MPU_PASSO;
        if (mpu_adr == MPU_ADR_FIM){
          mpu_adr=MPU_ADR_INI;
          lcd_char(3,0,'*');
        }
        if (bateu == TRUE){
          //ser_str("\nmpu_adr=");  ser_hex32(mpu_adr);
          //ser_str(" cont_pb=");   ser_dec32(cont_pb);
          cont_pb -= MPU_PASSO;       //Se bateu, acionar contador decrescente
        }
      }

      // GPS usa interrupção com dois buffers selecionados pela variável gps_msg_fase
      if (gps_msg_ok == TRUE){
        gps_msg_ok=FALSE;
        if (gps_msg_fase==0){
          //ser_str(gps_msg_1);     //Imprimir msg GPS
          //ser_crlf(1);            //CR LF
          gps_extrai(gps_msg_1);  //Extrair parâmetros do GPS
        }
        else{
          //ser_str(gps_msg_0);     //Imprimir msg gps
          //ser_crlf(1);            //CR LF
          gps_extrai(gps_msg_0);  //Extrair parâmetros do GPS
        }
      }

      // GPS - Só grava após receber GPS_GLL
      if (gps_tipo == GPS_GLL){
        gps_tipo=GPS_NADA;
        gps_cont++;
        str_dec32u(mpu_adr, msg_aux);                 //Gerar string do contador de endereço mpu_adr
        str_rmvz_u(msg_aux);                          //Remover zeros à esquerda
        str_copia(msg_aux,&gps_dados[GPS_ADR_SRAM]);  //Copiar para o vetor gps_dados
        sram_wr_blk(gps_adr, gps_dados, GPS_PASSO);   //Gravar gps_dados na SRAM
        ser_gps_dados_lin(gps_dados);                 //Imprimir tudo que veio do GPS

        //Enviar LAT e LONG pelo Buetooth
        //bt_lat(gps_dados);
        //bt_long(gps_dados);
        bt_seq++;
        bt_dec32unz(bt_seq);
        bt_char(',');
        if (gps_dados[GPS_STATUS] == 'A'){
          bt_lat(&gps_dados[GPS_LAT],gps_dados[GPS_NS]);
          bt_char(',');
          bt_long(&gps_dados[GPS_LONG],gps_dados[GPS_EW]);
        }
        else{
          bt_str("Nao valido.");   
        }
        bt_crlf(1);
        
        for (z=0; z<6; z++){
          hora[z]=gps_dados[GPS_HORA+z];
          data[z]=gps_dados[GPS_DATA+z];
        }
        data[z]=hora[z]='\0';

        for (z=0; z<GPS_PASSO; z++)  gps_dados[z]='\0'; //***Prencher com zeros
        gps_adr += GPS_PASSO;                         //Avançar contador de endereço gps_adr
        if (gps_adr == GPS_ADR_FIM){                  //Verificar limite do contador
          gps_adr=GPS_ADR_INI;
          lcd_char(3,11,'*');
        }


        
      }

      // Se bateu, verificar se já preencheu toda a memória
      if (bateu == TRUE){
        segs=cont_pb/(MPU_PASSO*100);
        lcd_dec16u(2,7,segs);   //Imprimir contador segundos após batida (SEL)
        if (cont_pb == 0)   break;      //cont_pb=0 ==> FIM
      }

      //Tecla? Dá início à aquisição
      if (sw_tira(&tec)==TRUE){

        // Interromper aquisição ?
        if (bateu==TRUE && tec==SW_INF){
          flag_brk=TRUE;
          break;
        }
        
        if (tec==SW_SEL){
          bateu=TRUE;       //Indicar que bateu
          sram_wr_32b(OP_MPU_ADR, mpu_adr);                      //Guardar contador de endereços SRAM(MPU) no instante da batida
          sram_wr_32b(OP_GPS_ADR, gps_adr);                      //Guardar contador de endereços SRAM(GPS) no instante da batida
          sram_wr_str(OP_AC_DATA, data);                          //Guardar data da batida
          sram_wr_str(OP_AC_HORA, hora);                          //Guardar hora da batida
          sram_wr_16b(OP_BATEU,COD_SIM);                          //Marcar BATIDA
          sram_wr_16b(OP_DISP_AX, COD_SIM);                       //AX simula disparo da batida
          cont_pb=MPU_ADR_FIM-MPU_ADR_INI;                        //Inicializar contador pós batida
          lcd_str(1,0,msg6);
          lcd_str(2,0,msg5);
          ser_crlf(1);
          ser_str(msg6);
          ser_crlf(1);
        }
        if (tec==SW_SEQ1){      //Sair do Opera 8
          gps_des_int();  //GPS: Desabilitar interrupção
          mpu_des_int();  //MPU: Desabilitar interrupção
          lcd_apaga();
          lcd_str(0,0,msg1);
          lcd_str(2,0,msg8);
          ser_crlf(2);
          ser_str(msg8);
          sw_qq_tecla();
          return md;
        }
      }
    }

    // Marcar o final
    // Gravar linha com 22222 = 0x56CE
    for (z=0; z<18; z+=2){vt[z+0]=0x56; vt[z+1]=0xCE; }
    if (mpu_adr==0) mpu_adr=MPU_ADR_FIM-MPU_PASSO;  //Voltar ponteiro um passo
    else            mpu_adr = mpu_adr-MPU_PASSO;
    sram_wr_blk(mpu_adr, vt, MPU_PASSO);            //Gravar linha com 22222 na SRAM
    //ser_str("\nGravar 2222 em adr="); ser_hex32(mpu_adr);
    sram_wr_32b(OP_ULT_ADR,mpu_adr);    //Último endereço gravado pelo MPU
    //ser_crlf(1);

    //Gravar temperaturo do MPU no instante do acidente
    sram_wr_16b(OP_DISP_TP, mpu_rd_tp()); //Gravar Temp na SRAM (área de calibra ao ligar)

    // Marcar se aquisição foi interrompida
    if (flag_brk==TRUE){
      sram_wr_16b(OP_BRK,COD_SIM);
      ser_str("*** Interrompido ***");
    }
    else  sram_wr_16b(OP_BRK,COD_NAO);                          
    
    ser_crlf(1);

    // TX serial;
    lcd_str(1,0,msg7);
    ser_str(msg7);   
    //ser_crlf(1);
    opera8_serial();    //TX todos os dados
    //ser_crlf(5);
    //sram_op_mostra();
    //ser_crlf(3);
    //sram_op_dados();

  }
  gps_des_int();  //GPS: Desabilitar interrupção
  mpu_des_int();  //MPU: Desabilitar interrupção
  lcd_str(3,0,"--Erro--");    //Se chegou aqui, deu erro no laço
  ser_str("\n\n--Erro--");    //Se chegou aqui, deu erro no laço
  while(TRUE);
}

// TX serial para opera 8
void opera8_serial(void){
  char *msg1="MPU faltam xxxxx";
  byte vet_mpu[MPU_PASSO];      //Ler bytes: ax-ay-az-gx-gy-gz
  byte vet_gps[GPS_PASSO];  //Ler mensagem GPS
  long adr;       //Ponteiro para ler os dados
  long dif,adri,adrf; //Endereço de disparo e final
  word qtd;     //Quantidade de linhas MPU ou GPS
  word cont;    //Contador do envio serial
  word aux;       

  // MPU TX
  lcd_apaga_lin(2);
  lcd_str(2,0,msg1);
  ser_str("\n\n#[m\n");                       //Marcar início TX MPU
  
  sram_rd_str(OP_AC_DATA, vet_gps, 10);       //Ler data do acidente
  ser_str(vet_gps);                           //Imprimir data do acidente
  ser_crlf(1);
  
  sram_rd_str(OP_AC_HORA, vet_gps, 10);       //Ler hora do acidente
  ser_str(vet_gps);                           //Imprimir hora do acidente
  ser_crlf(1);

  aux=sram_rd_16b(OP_DISP_TP);                //Temperatura instante acidente
  ser_dec16unz(aux);                          //Imprimir temperatura
  ser_crlf(1);
    
  adri=sram_rd_32b(OP_MPU_ADR);                //Endereço qdo SEL foi acionada
  ser_dec32unz(adri);                            //Imprimir endereço
  ser_crlf(1);
  
  adrf=sram_rd_32b(OP_ULT_ADR);                //Último Endereço usado pelo MPU
  adrf+=MPU_PASSO;                             //Avançar para o próximo (mais velho)
  if (adrf==MPU_ADR_FIM) adrf=MPU_ADR_INI;
  qtd=(MPU_ADR_FIM - MPU_ADR_INI)/MPU_PASSO;  //Qtd de linhas do MPU
  
  //Verificar se aquisição foi interrompida
  aux=sram_rd_16b(OP_BRK);
  if (aux==COD_SIM){
    dif=adrf-adri;
    if (dif<0)  dif+=(MPU_ADR_FIM - MPU_ADR_INI);
    qtd=dif/MPU_PASSO;  //Qtd de linhas do MPU até interrupção
    adr=adri;
    /*
    ser_str("\nFez calc break\n");
    ser_str("adri="); ser_hex32(adri);
    ser_str("  adrf="); ser_hex32(adrf);
    ser_str("  dif="); ser_dec16(dif);
    ser_str("  qtd="); ser_dec16(qtd);
    */
  }

  ser_dec16unz(qtd);  ser_crlf(1);    //Qtd de linhas do MPU
  
  for (cont=0; cont<qtd; cont++){
    sram_rd_blk(adr,vet_mpu,18);
    ser_dec16u(cont);
    ser_spc(1);
    //ser_dec32u(adr);
    ser_hex32(adr);
    ser_spc(1);
    ser_lin_ac_gi_mg(vet_mpu);
    adr+=MPU_PASSO;
    if (adr==MPU_ADR_FIM) adr=MPU_ADR_INI;
    lcd_dec16u(2,11,qtd-cont);
  }
  ser_str("m]#\n");    //Marcar fim TX MPU

  // GPS TX
  lcd_str(1,0,"GPS");
  ser_str("#[g\n");    //Marcar início TX GPS
  sram_rd_str(OP_AC_DATA, vet_gps, 10);       //Ler data do acidente
  ser_str(vet_gps);                           //Imprimir data do acidente
  ser_crlf(1);
  sram_rd_str(OP_AC_HORA, vet_gps, 10);       //Ler hora do acidente
  ser_str(vet_gps);                           //Imprimir hora do acidente
  ser_crlf(1);
  adr=sram_rd_32b(OP_GPS_ADR);                //Endereço qdo SEL foi acionada
  ser_dec32unz(adr);                            //Imprimir endereço
  ser_crlf(1);
  // Estimar endereço da linha mais antiga do GPS
  // qtd = ainda tem a quantidade de leituras do MPU
  cont=qtd/(1000/(OP_FREQ+1));    //Freq MPU = 1000/(OP_FREQ+1) = 1000/(9+1) = 100
  cont=cont*GPS_PASSO;
  adr = adr + cont;     //Calcular adr da linha mais antiga
  if (adr>=GPS_ADR_FIM) adr=GPS_ADR_INI+(adr-GPS_ADR_FIM);
  qtd=(GPS_ADR_FIM - GPS_ADR_INI)/GPS_PASSO;  //Qtd de linhas do GPS
  ser_dec16unz(qtd);                            //Imprimir qtd de linhas GPS
  ser_crlf(1);
  for (cont=0; cont<qtd; cont++){
    sram_rd_blk(adr,vet_gps,GPS_PASSO);
    ser_dec32u(cont);
    ser_spc(1);
    ser_dec32(adr);
    ser_spc(1);
    ser_gps_dados_lin(vet_gps);
    //ser_crlf(1);
    adr+=GPS_PASSO;
    if (adr==GPS_ADR_FIM) adr=GPS_ADR_INI;
    lcd_dec16u(2,11,qtd-cont-1);
  }
  ser_str("g]#\n");    //Marcar fim TX GPS

  ser_str("#[l\n");    //Marcar Início TX Calibração do Ligar
  sram_op_dados();
  ser_str("l]#\n");    //Marcar Fim TX Calibração do Ligar

  ser_str("#[f\n");    //Marcar Início TX Calibração do Ligar
  eeprom_cf_dados();
  ser_str("f]#\n");    //Marcar Fim TX Calibração do Ligar - Fim de tudo
}

// Fazer toda a preparação para a operação 8
void opera8_prepara(void){
  char *msg1="Erro Who am I = ";
  char *msg2="Self Test Falhou";
  byte x;
  byte st;        //Resultado do Self Test
  int st_vet[24]; //Receber resultados do Self Test
  byte esc_ac,esc_gi,esc_mg;   //Guardar Escalas de acel, giro e mag
  int aux[7];

  mpu_acorda();
  mpu_config();
  x=mpu_whoami();                     //Who am I
  if (x != MPU9250_WHO){
    lcd_str(1,0,msg1); lcd_dec16unz(1,16,x);
    ser_str(msg1);     ser_dec16unz(x);  ser_crlf(1);
    delay(1000);
  }
  sram_wr_16b(OP_BATEU,COD_NAO);        //Nao Batida

  st=mpu_self_test(st_vet,FALSE);       //Self Test
  if (st==TRUE)
    sram_wr_16b(OP_ST_OK,COD_SIM);
  else{
    sram_wr_16b(OP_ST_OK,COD_NAO);
    lcd_str(1,0,msg2);
    ser_str(msg2);  ser_crlf(1);
    delay(1000);
  }
  x=eeprom_rd_16b(CF_OK);   //Calibração de Fábrica
  sram_wr_16b(OP_CF_OK,x);  //Gravar código de Calibr de Fábrica

  //Calibração Acel e Giro ao ligar o carro
  esc_ac=ACEL_FS_2G;    //Escala acel para calibrar ao ligar
  esc_gi=GIRO_FS_250;   //Escala giro para calibrar ao ligar
  mpu_calibra(aux, OP_QTD_MED_AG, esc_ac, esc_gi);  //Função de calibração
  x=mpu_rd_freq();      sram_wr_16b(OPC_FREQ_AG,x); //Freq de amostragem
  x=mpu_rd_bw();        sram_wr_16b(OPC_BW_AG,x);   //Banda Passante
  x=mpu_rd_esc_acel();  sram_wr_16b(OPC_ESC_AC,x);  //Escala do Acelerômetro
  x=mpu_rd_esc_giro();  sram_wr_16b(OPC_ESC_GI,x);  //Escala do Giroscópio
  sram_wr_16b(OPC_QTD_AG,OP_QTD_MED_AG);            //Qtd de medidas para calibração ao ligar
  for (x=0; x<7; x++)   sram_wr_16b(OPC_AX+(2*x),aux[x]);

  //Calibração Magnetômetro ao ligar o carro
  //esc_mg=0;             //Escala do Magnetômetro
  //sram_wr_16b(OPC_QTD_MG,OP_QTD_MED_MG);    //Qtd de medidas para  média do Mag calibração ao ligar
  //sram_wr_16b(OPC_ESC_MG,esc_mg);           //Escala do Mag para calibração ao ligar
  //mag_calibra();

  //Calibração Acel e Giro para a Operação
  mpu_config();         //Novamente inicializa MPU
  esc_ac=ACEL_FS_2G;    //Escala acel para calibrar ao ligar
  esc_gi=GIRO_FS_250;   //Escala giro para calibrar ao ligar
  mpu_escalas(esc_gi,esc_ac);       //Programar escalas
  mpu_sample_rt(SAMPLE_RT_100Hz);   //Programar taxa de amostragem

  //Definir freq e limiares de acidente
  x=mpu_rd_freq();      sram_wr_16b(OP_FREQ_AG,x); //Freq de amostragem
  x=mpu_rd_bw();        sram_wr_16b(OP_BW_AG,x);   //Banda Passante
  x=mpu_rd_esc_acel();  sram_wr_16b(OP_ESC_AC,x);  //Escala do Acelerômetro
  x=mpu_rd_esc_giro();  sram_wr_16b(OP_ESC_GI,x);  //Escala do Giroscópio
  sram_wr_16b(OP_LIM_AX,LIMIAR_AX);     //AX - Limiar para disparo de acidente
  sram_wr_16b(OP_LIM_AY,LIMIAR_AY);     //AY - Limiar para disparo de acidente
  sram_wr_16b(OP_LIM_AZ,LIMIAR_AZ);     //AZ - Limiar para disparo de acidente
  sram_wr_16b(OP_LIM_GX,LIMIAR_GX);     //GX - Limiar para disparo de acidente
  sram_wr_16b(OP_LIM_GY,LIMIAR_GY);     //GY - Limiar para disparo de acidente
  sram_wr_16b(OP_LIM_GZ,LIMIAR_GZ);     //GZ - Limiar para disparo de acidente

  sram_wr_32b(OP_MPU_ADR,0L);           //Ponteiro MPU quando ocorreu disparo
  sram_wr_32b(OP_GPS_ADR,0L);           //Ponteiro GPS quando ocorreu disparo

  //Quem disparou
  for (x=0; x<6; x++)   sram_wr_16b(OP_DISP_AX+2*x,COD_NAO);

  //Data e Hora
  sram_wr_str(OP_AC_DATA,"ddmmyy");
  sram_wr_str(OP_AC_HORA,"hhmmss.sss");

  //sram_op_mostra();
  //ser_crlf(2);
  //sram_op_dados();
}


// 9 - Adquirir dados com Blue Tooth
// Semelhante ao Opera_8, <bateu> inicia aquisição
// Após <bateu>, Preenche toda a memória.
char opera_9(char md){
  char *msg1="[9] Adquirir com BT";
  char *msg2="Zerando SRAM";
  char *msg3="SEL = Inic Aquisicao";
  char *msg3a="SW_INF = Interrompe";
  char *msg4=" MPUxxxxx   GPS xxxxx";
  char *msg5="Faltam xxxxx segundos";
  char *msg6="==>> ADQUIRINDO <<==";
  char *msg7="==>> TX  SERIAL <<==";
  char *msg8="FIM: Tecle para sair";
  unsigned int mpu_cont, gps_cont;    //Contar dados para animar tela
  unsigned long mpu_adr;        //MPU - Contador de endereços para gravação
  unsigned long mpu_adr_bateu;  //MPU - Endereço para finalizar gravação após batida
  unsigned long gps_adr;        //GPS - Contador de endereços para gravação
  unsigned long cont_pb;        //Contador decrescente para indicar o que falta pós batida
  int z;
  byte tec;
  byte bateu;                   //Indicar se bateu (FALSE / TRUE)
  byte flag_brk;                //Indicar que laço de aquisição foi interrompido
  word segs;                    //Segundos que faltam para finalizar
  byte vt[18];                  //Receber ax, ay, az, gx, gy, gz, hx, hy, hz ou gps_dados
  char msg_aux[20];
  byte data[10];                 //Última atualização da data
  byte hora[12];                //Última atualização da hora

  //Variáveis para enviar LAT e LONG pelo Bluetooth
  int graus,mini,fracm;          //Graus, minito e fração de minuto
  long lati,longi;              //Valor de latitude e longitude
  float flat,flong;             //Valor de latitude e longitude
  unsigned long bt_seq=0;       //Contar sequência

  lcd_apaga();
  lcd_str(0,0,msg1);
  //bt_str(msg1);  bt_crlf(1);
  opera9_prepara();

  //Laço de principal
  gps_int();
  while (TRUE){
    lcd_apaga();
    lcd_str(0,0,msg1);
    bt_crlf(1);
    bt_str(msg1);  bt_crlf(1);
    lcd_str(1,0,msg2);
    bt_str(msg2);  bt_crlf(1);
    sram_zera_mpu_gps();   //Zerar SRAM, preserva Calib ao Ligar
    mpu_cont=0;
    gps_cont=0;
    lcd_apaga_lin(1);
    lcd_str(1,0,msg3);
    lcd_str(2,0,msg3a);
    bt_str(msg3);  bt_spc(2);
    bt_str(msg3a);  bt_crlf(1);
    lcd_str(3,0,msg4);
    bateu=FALSE;
    mpu_adr=MPU_ADR_INI;
    gps_adr=GPS_ADR_INI;
    gps_tipo=GPS_NADA;
    mpu_dado_ok=FALSE;
    flag_brk=FALSE;       //Flag para indicar interrupção da aquisição
    //gps_int();

    // Laço de Aquisição
    for (z=0; z<GPS_PASSO; z++) gps_dados[z]='\0'; //***Prencher com zeros
    while(TRUE){
      lcd_dec16u(3,4,mpu_cont);
      lcd_dec16u(3,15,gps_cont);
  
      // MPU trabalha por interrupção
      if (mpu_dado_ok == TRUE){   //MPU a 100 Hz (10 ms)
        mpu_dado_ok=FALSE;
        mpu_cont++;
        mpu_rd_ac_gi_mg(vt);
        sram_wr_blk(mpu_adr, vt, MPU_PASSO); //Gravar MPU na SRAM
        mpu_adr += MPU_PASSO;
        if (mpu_adr == MPU_ADR_FIM){
          mpu_adr=MPU_ADR_INI;
          lcd_char(3,0,'*');
        }
        if (bateu == TRUE){
          //bt_str("\nmpu_adr=");  bt_hex32(mpu_adr);
          //bt_str(" cont_pb=");   bt_dec32(cont_pb);
          cont_pb -= MPU_PASSO;       //Se bateu, acionar contador decrescente
        }
      }

      // GPS usa interrupção com dois buffers selecionados pela variável gps_msg_fase
      if (gps_msg_ok == TRUE){
        gps_msg_ok=FALSE;
        if (gps_msg_fase==0){
          //bt_str(gps_msg_1);     //Imprimir msg GPS
          //bt_crlf(1);            //CR LF
          gps_extrai(gps_msg_1);  //Extrair parâmetros do GPS
        }
        else{
          //bt_str(gps_msg_0);     //Imprimir msg gps
          //bt_crlf(1);            //CR LF
          gps_extrai(gps_msg_0);  //Extrair parâmetros do GPS
        }
      }

      // GPS - Só grava após receber GPS_GLL
      if (gps_tipo == GPS_GLL){
        gps_tipo=GPS_NADA;
        gps_cont++;
        str_dec32u(mpu_adr, msg_aux);                 //Gerar string do contador de endereço mpu_adr
        str_rmvz_u(msg_aux);                          //Remover zeros à esquerda
        str_copia(msg_aux,&gps_dados[GPS_ADR_SRAM]);  //Copiar para o vetor gps_dados
        sram_wr_blk(gps_adr, gps_dados, GPS_PASSO);   //Gravar gps_dados na SRAM
        //bt_gps_dados_lin(gps_dados);                 //Imprimir tudo que veio do GPS

        //Enviar LAT e LONG pelo Buetooth
        //bt_lat(gps_dados);
        //bt_long(gps_dados);
        bt_seq++;
        bt_dec32unz(bt_seq);
        bt_char(',');
        if (gps_dados[GPS_STATUS] == 'A'){
          bt_lat(&gps_dados[GPS_LAT],gps_dados[GPS_NS]);
          bt_char(',');
          bt_long(&gps_dados[GPS_LONG],gps_dados[GPS_EW]);
        }
        else{
          bt_str("Nao valido.");   
        }
        bt_crlf(1);
        
        for (z=0; z<6; z++){
          hora[z]=gps_dados[GPS_HORA+z];
          data[z]=gps_dados[GPS_DATA+z];
        }
        data[z]=hora[z]='\0';

        for (z=0; z<GPS_PASSO; z++)  gps_dados[z]='\0'; //***Prencher com zeros
        gps_adr += GPS_PASSO;                         //Avançar contador de endereço gps_adr
        if (gps_adr == GPS_ADR_FIM){                  //Verificar limite do contador
          gps_adr=GPS_ADR_INI;
          lcd_char(3,11,'*');
        }


        
      }

      // Se bateu, verificar se já preencheu toda a memória
      if (bateu == TRUE){
        segs=cont_pb/(MPU_PASSO*100);
        lcd_dec16u(2,7,segs);   //Imprimir contador segundos após batida (SEL)
        if (cont_pb == 0)   break;      //cont_pb=0 ==> FIM
      }

      //Tecla? Dá início à aquisição
      if (sw_tira(&tec)==TRUE){

        // Interromper aquisição ?
        if (bateu==TRUE && tec==SW_INF){
          flag_brk=TRUE;
          break;
        }
        
        if (tec==SW_SEL){
          bateu=TRUE;       //Indicar que bateu
          sram_wr_32b(OP_MPU_ADR, mpu_adr);                      //Guardar contador de endereços SRAM(MPU) no instante da batida
          sram_wr_32b(OP_GPS_ADR, gps_adr);                      //Guardar contador de endereços SRAM(GPS) no instante da batida
          sram_wr_str(OP_AC_DATA, data);                          //Guardar data da batida
          sram_wr_str(OP_AC_HORA, hora);                          //Guardar hora da batida
          sram_wr_16b(OP_BATEU,COD_SIM);                          //Marcar BATIDA
          sram_wr_16b(OP_DISP_AX, COD_SIM);                       //AX simula disparo da batida
          cont_pb=MPU_ADR_FIM-MPU_ADR_INI;                        //Inicializar contador pós batida
          lcd_str(1,0,msg6);
          lcd_str(2,0,msg5);
          bt_crlf(1);
          bt_str(msg6);
          bt_crlf(1);
        }
        if (tec==SW_SEQ1){      //Sair do Opera 8
          gps_des_int();  //GPS: Desabilitar interrupção
          mpu_des_int();  //MPU: Desabilitar interrupção
          lcd_apaga();
          lcd_str(0,0,msg1);
          lcd_str(2,0,msg8);
          bt_crlf(2);
          bt_str(msg8);
          sw_qq_tecla();
          return md;
        }
      }
    }

    // Marcar o final
    // Gravar linha com 22222 = 0x56CE
    for (z=0; z<18; z+=2){vt[z+0]=0x56; vt[z+1]=0xCE; }
    if (mpu_adr==0) mpu_adr=MPU_ADR_FIM-MPU_PASSO;  //Voltar ponteiro um passo
    else            mpu_adr = mpu_adr-MPU_PASSO;
    sram_wr_blk(mpu_adr, vt, MPU_PASSO);            //Gravar linha com 22222 na SRAM
    //bt_str("\nGravar 2222 em adr="); bt_hex32(mpu_adr);
    sram_wr_32b(OP_ULT_ADR,mpu_adr);    //Último endereço gravado pelo MPU
    //bt_crlf(1);

    //Gravar temperaturo do MPU no instante do acidente
    sram_wr_16b(OP_DISP_TP, mpu_rd_tp()); //Gravar Temp na SRAM (área de calibra ao ligar)

    // Marcar se aquisição foi interrompida
    if (flag_brk==TRUE){
      sram_wr_16b(OP_BRK,COD_SIM);
      bt_str("*** Interrompido ***");
    }
    else  sram_wr_16b(OP_BRK,COD_NAO);                          
    
    bt_crlf(1);

    // TX serial;
    lcd_str(1,0,msg7);
    bt_str(msg7);   
    //bt_crlf(1);
    opera9_serial();    //TX todos os dados
    //bt_crlf(5);
    //sram_op_mostra();
    //bt_crlf(3);
    //sram_op_dados();

  }
  gps_des_int();  //GPS: Desabilitar interrupção
  mpu_des_int();  //MPU: Desabilitar interrupção
  lcd_str(3,0,"--Erro--");    //Se chegou aqui, deu erro no laço
  bt_str("\n\n--Erro--");    //Se chegou aqui, deu erro no laço
  while(TRUE);
}

// TX serial para opera 9
void opera9_serial(void){
  char *msg1="MPU faltam xxxxx";
  byte vet_mpu[MPU_PASSO];      //Ler bytes: ax-ay-az-gx-gy-gz
  byte vet_gps[GPS_PASSO];  //Ler mensagem GPS
  long adr;       //Ponteiro para ler os dados
  long dif,adri,adrf; //Endereço de disparo e final
  word qtd;     //Quantidade de linhas MPU ou GPS
  word cont;    //Contador do envio serial
  word aux;       

  // MPU TX
  lcd_apaga_lin(2);
  lcd_str(2,0,msg1);
  bt_str("\n\n#[m\n");                       //Marcar início TX MPU
  
  sram_rd_str(OP_AC_DATA, vet_gps, 10);       //Ler data do acidente
  bt_str(vet_gps);                           //Imprimir data do acidente
  bt_crlf(1);
  
  sram_rd_str(OP_AC_HORA, vet_gps, 10);       //Ler hora do acidente
  bt_str(vet_gps);                           //Imprimir hora do acidente
  bt_crlf(1);

  aux=sram_rd_16b(OP_DISP_TP);                //Temperatura instante acidente
  bt_dec16unz(aux);                          //Imprimir temperatura
  bt_crlf(1);
    
  adri=sram_rd_32b(OP_MPU_ADR);                //Endereço qdo SEL foi acionada
  bt_dec32unz(adri);                            //Imprimir endereço
  bt_crlf(1);
  
  adrf=sram_rd_32b(OP_ULT_ADR);                //Último Endereço usado pelo MPU
  adrf+=MPU_PASSO;                             //Avançar para o próximo (mais velho)
  if (adrf==MPU_ADR_FIM) adrf=MPU_ADR_INI;
  qtd=(MPU_ADR_FIM - MPU_ADR_INI)/MPU_PASSO;  //Qtd de linhas do MPU
  
  //Verificar se aquisição foi interrompida
  aux=sram_rd_16b(OP_BRK);
  if (aux==COD_SIM){
    dif=adrf-adri;
    if (dif<0)  dif+=(MPU_ADR_FIM - MPU_ADR_INI);
    qtd=dif/MPU_PASSO;  //Qtd de linhas do MPU até interrupção
    adr=adri;
    /*
    bt_str("\nFez calc break\n");
    bt_str("adri="); bt_hex32(adri);
    bt_str("  adrf="); bt_hex32(adrf);
    bt_str("  dif="); bt_dec16(dif);
    bt_str("  qtd="); bt_dec16(qtd);
    */
  }

  bt_dec16unz(qtd);  bt_crlf(1);    //Qtd de linhas do MPU
  
  for (cont=0; cont<qtd; cont++){
    sram_rd_blk(adr,vet_mpu,18);
    bt_dec16u(cont);
    bt_spc(1);
    //bt_dec32u(adr);
    bt_hex32(adr);
    bt_spc(1);
    bt_lin_ac_gi_mg(vet_mpu);
    adr+=MPU_PASSO;
    if (adr==MPU_ADR_FIM) adr=MPU_ADR_INI;
    lcd_dec16u(2,11,qtd-cont);
  }
  bt_str("m]#\n");    //Marcar fim TX MPU

  // GPS TX
  lcd_str(1,0,"GPS");
  bt_str("#[g\n");    //Marcar início TX GPS
  sram_rd_str(OP_AC_DATA, vet_gps, 10);       //Ler data do acidente
  bt_str(vet_gps);                           //Imprimir data do acidente
  bt_crlf(1);
  sram_rd_str(OP_AC_HORA, vet_gps, 10);       //Ler hora do acidente
  bt_str(vet_gps);                           //Imprimir hora do acidente
  bt_crlf(1);
  adr=sram_rd_32b(OP_GPS_ADR);                //Endereço qdo SEL foi acionada
  bt_dec32unz(adr);                            //Imprimir endereço
  bt_crlf(1);
  // Estimar endereço da linha mais antiga do GPS
  // qtd = ainda tem a quantidade de leituras do MPU
  cont=qtd/(1000/(OP_FREQ+1));    //Freq MPU = 1000/(OP_FREQ+1) = 1000/(9+1) = 100
  cont=cont*GPS_PASSO;
  adr = adr + cont;     //Calcular adr da linha mais antiga
  if (adr>=GPS_ADR_FIM) adr=GPS_ADR_INI+(adr-GPS_ADR_FIM);
  qtd=(GPS_ADR_FIM - GPS_ADR_INI)/GPS_PASSO;  //Qtd de linhas do GPS
  bt_dec16unz(qtd);                            //Imprimir qtd de linhas GPS
  bt_crlf(1);
  for (cont=0; cont<qtd; cont++){
    sram_rd_blk(adr,vet_gps,GPS_PASSO);
    bt_dec32u(cont);
    bt_spc(1);
    bt_dec32(adr);
    bt_spc(1);
    bt_gps_dados_lin(vet_gps);
    //bt_crlf(1);
    adr+=GPS_PASSO;
    if (adr==GPS_ADR_FIM) adr=GPS_ADR_INI;
    lcd_dec16u(2,11,qtd-cont-1);
  }
  bt_str("g]#\n");    //Marcar fim TX GPS

  bt_str("#[l\n");    //Marcar Início TX Calibração do Ligar
  sram_op_dados();
  bt_str("l]#\n");    //Marcar Fim TX Calibração do Ligar

  bt_str("#[f\n");    //Marcar Início TX Calibração do Ligar
  eeprom_cf_dados();
  bt_str("f]#\n");    //Marcar Fim TX Calibração do Ligar - Fim de tudo
}

// Fazer toda a preparação para a operação 9
void opera9_prepara(void){
  char *msg1="Erro Who am I = ";
  char *msg2="Self Test Falhou";
  byte x;
  byte st;        //Resultado do Self Test
  int st_vet[24]; //Receber resultados do Self Test
  byte esc_ac,esc_gi,esc_mg;   //Guardar Escalas de acel, giro e mag
  int aux[7];

  mpu_acorda();
  mpu_config();
  x=mpu_whoami();                     //Who am I
  if (x != MPU9250_WHO){
    lcd_str(1,0,msg1); lcd_dec16unz(1,16,x);
    bt_str(msg1);     bt_dec16unz(x);  bt_crlf(1);
    delay(1000);
  }
  sram_wr_16b(OP_BATEU,COD_NAO);        //Nao Batida

  st=mpu_self_test(st_vet,FALSE);       //Self Test
  if (st==TRUE)
    sram_wr_16b(OP_ST_OK,COD_SIM);
  else{
    sram_wr_16b(OP_ST_OK,COD_NAO);
    lcd_str(1,0,msg2);
    bt_str(msg2);  bt_crlf(1);
    delay(1000);
  }
  x=eeprom_rd_16b(CF_OK);   //Calibração de Fábrica
  sram_wr_16b(OP_CF_OK,x);  //Gravar código de Calibr de Fábrica

  //Calibração Acel e Giro ao ligar o carro
  esc_ac=ACEL_FS_2G;    //Escala acel para calibrar ao ligar
  esc_gi=GIRO_FS_250;   //Escala giro para calibrar ao ligar
  mpu_calibra(aux, OP_QTD_MED_AG, esc_ac, esc_gi);  //Função de calibração
  x=mpu_rd_freq();      sram_wr_16b(OPC_FREQ_AG,x); //Freq de amostragem
  x=mpu_rd_bw();        sram_wr_16b(OPC_BW_AG,x);   //Banda Passante
  x=mpu_rd_esc_acel();  sram_wr_16b(OPC_ESC_AC,x);  //Escala do Acelerômetro
  x=mpu_rd_esc_giro();  sram_wr_16b(OPC_ESC_GI,x);  //Escala do Giroscópio
  sram_wr_16b(OPC_QTD_AG,OP_QTD_MED_AG);            //Qtd de medidas para calibração ao ligar
  for (x=0; x<7; x++)   sram_wr_16b(OPC_AX+(2*x),aux[x]);

  //Calibração Magnetômetro ao ligar o carro
  //esc_mg=0;             //Escala do Magnetômetro
  //sram_wr_16b(OPC_QTD_MG,OP_QTD_MED_MG);    //Qtd de medidas para  média do Mag calibração ao ligar
  //sram_wr_16b(OPC_ESC_MG,esc_mg);           //Escala do Mag para calibração ao ligar
  //mag_calibra();

  //Calibração Acel e Giro para a Operação
  mpu_config();         //Novamente inicializa MPU
  esc_ac=ACEL_FS_2G;    //Escala acel para calibrar ao ligar
  esc_gi=GIRO_FS_250;   //Escala giro para calibrar ao ligar
  mpu_escalas(esc_gi,esc_ac);       //Programar escalas
  mpu_sample_rt(SAMPLE_RT_100Hz);   //Programar taxa de amostragem

  //Definir freq e limiares de acidente
  x=mpu_rd_freq();      sram_wr_16b(OP_FREQ_AG,x); //Freq de amostragem
  x=mpu_rd_bw();        sram_wr_16b(OP_BW_AG,x);   //Banda Passante
  x=mpu_rd_esc_acel();  sram_wr_16b(OP_ESC_AC,x);  //Escala do Acelerômetro
  x=mpu_rd_esc_giro();  sram_wr_16b(OP_ESC_GI,x);  //Escala do Giroscópio
  sram_wr_16b(OP_LIM_AX,LIMIAR_AX);     //AX - Limiar para disparo de acidente
  sram_wr_16b(OP_LIM_AY,LIMIAR_AY);     //AY - Limiar para disparo de acidente
  sram_wr_16b(OP_LIM_AZ,LIMIAR_AZ);     //AZ - Limiar para disparo de acidente
  sram_wr_16b(OP_LIM_GX,LIMIAR_GX);     //GX - Limiar para disparo de acidente
  sram_wr_16b(OP_LIM_GY,LIMIAR_GY);     //GY - Limiar para disparo de acidente
  sram_wr_16b(OP_LIM_GZ,LIMIAR_GZ);     //GZ - Limiar para disparo de acidente

  sram_wr_32b(OP_MPU_ADR,0L);           //Ponteiro MPU quando ocorreu disparo
  sram_wr_32b(OP_GPS_ADR,0L);           //Ponteiro GPS quando ocorreu disparo

  //Quem disparou
  for (x=0; x<6; x++)   sram_wr_16b(OP_DISP_AX+2*x,COD_NAO);

  //Data e Hora
  sram_wr_str(OP_AC_DATA,"ddmmyy");
  sram_wr_str(OP_AC_HORA,"hhmmss.sss");

  //sram_op_mostra();
  //bt_crlf(2);
  //sram_op_dados();
}
