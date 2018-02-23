/*
 * Desenvolvido por F Alberio Lima
 * Meu canal no youtube: https://www.youtube.com/c/alberiolima
 * 
 */
#include <DS1307.h>

#define portaLuz 3

DS1307 rtc(A4, A5);

Time tempo;
byte horaAmanhecer  = 6;  //Hora do amanhecer, usei valor sem munitos para deixar o código mais simples
byte horaEntardecer = 17; //Hora do entardecer
byte minutosDuracao = 30; //Duracao em minutor

int operacao = 0; //0=Inativo,1=amanhecendo e -1=entardecendo
unsigned char intensidadeLuz = 0; //Intensidade atual da luz
unsigned long proxIncremento = 0; //Quando amanhecedo ou entardecendo,tempo do da próxima atualização da luz
unsigned int tempoIncremento = 0; //para de incremento, calculado para andarde 1 em 1 até 255 

void setup() {

  DS1307 rtc(A4, A5);
  Serial.begin( 9600 );

  pinMode( portaLuz, OUTPUT );
  
  //atualizar relógio
  //rtc.setDOW(FRIDAY);      //Define o dia da semana
  //rtc.setTime(5, 0, 0);     //Define o horario
  //rtc.setDate(1, 4, 2017);   //Define o dia, mes e ano

  rtc.setSQWRate(SQW_RATE_1);
  rtc.enableSQW(true);
  
  tempoIncremento = (int)(60000 / ( 255 / minutosDuracao )); //calculo para chegar em quanto tempo precisa para incrementar 1 na luz, ex: se for 30min o tempo fica a cada 7 segundos
}

void loop() {
  
  tempo = rtc.getTime();
  if (tempo.hour == horaAmanhecer && tempo.min == 0 && operacao==0) {
    operacao = 1;
    intensidadeLuz = 0;
    proxIncremento = 0;    
  } else if (tempo.hour == horaEntardecer && tempo.min == 0 && operacao==0) {
    operacao = -1;
    intensidadeLuz = 255;
    proxIncremento = 0;
  }

  if ((operacao != 0 )&&(millis() >= proxIncremento)) {
    proxIncremento = millis() + tempoIncremento; 
    intensidadeLuz += operacao;
    if ( intensidadeLuz >= 255 ) { //identifica final do amanhecer (acende toda a luz)
      operacao = 0;
      intensidadeLuz = 255;
    } else if ( intensidadeLuz <= 0 ) { //identifica final do entardecer (apaga toda a luz)
      operacao = 0;
      intensidadeLuz = 0;
    }
    analogWrite( portaLuz, intensidadeLuz ); //atualiza luz

    //debug
    if ( operacao == 1 ) {
      Serial.print( "Amanhecendo " );
      Serial.println( intensidadeLuz );
    } else if ( operacao == -1 ) {
      Serial.print( "Entardecendo " );
      Serial.println( intensidadeLuz );
    }
  }  
}
