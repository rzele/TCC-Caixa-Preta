// Timers
// CXP - Caixa Preta
// 16/01/2019

// Configurar Timer 1 para FREQ_T1 (define)
void timer1_config(void){
  cli();                                    //Int Desab geral
  TCCR1A = 0;                               //WGM11=WGM10=0 
  TCCR1B = (1<<WGM12)|(1<<CS11)|(1<<CS10);  // Modo CTC e CLK/64
  TCCR1C = 0;
  TIMSK1 = (1<<OCIE1A);                     //Int CTC (ORC1A)
  OCR1A = (25000/(FREQ_T1/10))-1;
  sei();                                    //Int: Hab geral
}

// ISR Timer 1: FREQ_T1 (50 Hz)
ISR(TIMER1_COMPA_vect){
  switch(timer1_cont){
    
    case 0: case 13: case 16: case 29:
      ADCSRA |= 1<<ADSC;     //Disparar ADC;
      break;

    case 12:
      adc_val+=ADCH;
      ADMUX = (1<<REFS1)|(1<<REFS0)|(1<<ADLAR)|(1<<MUX0);  //VCAR: Canal 1, Ref = 2,56
      sw_val=adc_val/2;
      sw_ler(sw_val);
      break;

    case 14: case 30:
      adc_val=ADCH;
      ADCSRA |= 1<<ADSC;     //Disparar ADC;
      break;

    case 15:
      vcar_val=(adc_val+ADCH)/2;      //Ler VCAR
      ADMUX = (1<<REFS0)|(1<<ADLAR);  //Progr TEC: Canal 0, Ref=AVCC
      break;

    case 28:
      adc_val+=ADCH;
      ADMUX = (1<<REFS1)|(1<<REFS0)|(1<<ADLAR)|(1<<MUX1);  //Prog VCAP: Canal 2, Ref = 2,56
      sw_val=adc_val/2;
      sw_ler(sw_val);
      break;

    case 31:
      vcap_val=(adc_val+ADCH)/2;      //Ler VCAP
      ADMUX = (1<<REFS0)|(1<<ADLAR);  //TEC: Canal 0, Ref=AVCC
      break;

    default:
      if ( (timer1_cont&1) == 1){  //1,3,5,7,9,11, 17,19, 21, 23, 25, 27
        adc_val=ADCH;       //Ler ADC
        ADCSRA |= 1<<ADSC;  //Disparar ADC;
      }
      else{                     //2, 4, 6, 8, 10,   18, 20, 22, 24, 26
        //adc_val+=ADCH;
        sw_val=(adc_val+ADCH)/2;
        sw_ler(sw_val);
      }
      break;

    /*
    // Teclado
    case 0: case 8: case 12: case 16: case 24: case 28:
      ADMUX = (1<<REFS0)|(1<<ADLAR);  //Canal 0, Ref=AVCC
      break;
    case 1: case 9: case 13: case 17: case 25: case 29:
      ADCSRA |= 1<<ADSC;     //Disparar ADC;
    case 2: case 10: case 14: case 18: case 26: case 30:
      adc_val=ADCH;          //Só o ADCH
      ADCSRA |= 1<<ADSC;     //Disparar ADC;
      break;
    case 3: case 11: case 15: case 19: case 27: case 31:
      adc_val = adc_val+ADCH;
      sw_val = adc_val/2;
      sw_ler(sw_val);
      break;

    // VCAR = Canal 1
    case 4:
      ADMUX = (1<<REFS1)|(1<<REFS0)|(1<<ADLAR)|(1<<MUX0);  //Canal 1, Ref = 2,56
      break;
    case 5:
      ADCSRA |= 1<<ADSC;     //Disparar ADC;
    case 6:
      adc_val=ADCH;          //Só o ADCH
      ADCSRA |= 1<<ADSC;     //Disparar ADC;
      break;
    case 7:
      adc_val = adc_val+ADCH;
      vcar_val = adc_val/2; //VCAR = tensão
      break;

    // VCAP = Canal 2
    case 20:
      ADMUX = (1<<REFS1)|(1<<REFS0)|(1<<ADLAR)|(1<<MUX1);  //Canal 2, Ref = 2,56
      break;
    case 21:
      ADCSRA |= 1<<ADSC;     //Disparar ADC;
    case 22:
      adc_val=ADCH;          //Só o ADCH
      ADCSRA |= 1<<ADSC;     //Disparar ADC;
      break;
    case 23:
      adc_val = adc_val+ADCH;
      vcap_val = adc_val/2; //VCAP = tensão
      break;
    default:  break;
    */
  }
  
  timer1_cont++;
  timer1_cont &= 0x1F;
} 
