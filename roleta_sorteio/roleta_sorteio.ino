/*
  Dispositivo para executar sorteio,tipo roleta
  Ela possui 8 LED, que podem ser 8 luzes, adicionando hardware necessário

  Então são 8 LED, cada LED representa um premio, você pode indicar quantas vezes esse premio pode ocorrer
  Quanto maiora quantidade de um premio, maior a possiblidade de ocorrência, veja exemplo:

  O premio1 tem 3 intens,
  O premio2 tem 5 itens e
  O premio3 tem 8 itens

  Assim o premio3 tem a maior possiblidade de ocorrer como funciona:
  é gerada uma sequencia assim:

  1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16
  ======================================
  1,1,1,2,2,2,2,2,3, 3, 3, 3, 3, 3, 3, 3

  No total são 3 + 5 + 8 = 16 possiblidades

  um numero aleatório é gerado entre 1 e 16
  por acaso cai o numero 12, basta saber qual o premio daquela posição, no caso na posição 12 tem o 3, então o premio sorteado foi o 3

*/

#include <SoftwareSerial.h>
#include <EEPROM.h>
#include "animacao.h"

//#define _debbug_i 1

#define quantidadeOpcoes      8 //Quantidade de LEDs, opcoes a serem sorteadas
#define tempoAnimando      8000 //tempo total da animação dos LEDs, no sorteio
#define tempoPisca         50UL //Intervalo em pisca
#define tempoCadaAnimacao 150UL //Intervalo entre itens da animação

#define pinoBotaoJoga     1 //Pino do botão de jogar
#define pinoBuzzer        3 //Buzzer 

//Variáveis públicas
byte totalAnimacao = 0;
byte posicaoAnimacao = 0;
byte quantidadesPremios[quantidadeOpcoes];
unsigned long proxAnimacao = 1;

byte pinoLED[quantidadeOpcoes] = {13, 12, 11, 10, 9, 8, 7, 6};

byte statusAnteriorBotao = HIGH;
byte dados_premios[quantidadeOpcoes];
byte dados_sorteios[quantidadeOpcoes];

SoftwareSerial Serial1( 5, 4 ); //RX, TX

void setup() {

  totalAnimacao = sizeof( animacao );
  posicaoAnimacao = totalAnimacao;
  Serial1.begin( 9600 );
#if defined(_debbug_i)
  quantidadesPremios[0] = 1;
  quantidadesPremios[1] = 1;
  quantidadesPremios[2] = 1;
  quantidadesPremios[3] = 1;
  quantidadesPremios[4] = 1;
  quantidadesPremios[5] = 1;
  quantidadesPremios[6] = 1;
  quantidadesPremios[7] = 1;
  Serial1.println( "Debbug ativado, sorteios não serão contabilizados" );
#else
  eeprom_ler_dados();
#endif

  //Define como OUTPUT os pinos dos LEDs
  for (byte i = 0; i < quantidadeOpcoes; i++) {
    pinMode( pinoLED[i], OUTPUT );
    digitalWrite( pinoLED[i], HIGH);
  }

  //botão de jogar
  pinMode( pinoBotaoJoga, INPUT );

  delay(200); //acende rapidamente os LEDs para verificar se todos estão acendendo
  desligaLEDs();
  randomSeed(analogRead(7));
}

void loop() {

  serialEvent1();
  
  //Executa animação dos LEDs
  if ((proxAnimacao > 0) && ( millis() > proxAnimacao )) {
    posicaoAnimacao++;
    if ( posicaoAnimacao >= totalAnimacao ) {
      posicaoAnimacao = 0;
    }

    proxAnimacao += tempoCadaAnimacao;
    for ( byte i = 0; i < quantidadeOpcoes; i++) {
      digitalWrite( pinoLED[i], bitRead( animacao[posicaoAnimacao], i ) );
    }
  }
  byte leituraBotao = digitalRead( pinoBotaoJoga );
  if (( leituraBotao == HIGH )&&(leituraBotao != statusAnteriorBotao)) {
    delay(10);
    leituraBotao = digitalRead( pinoBotaoJoga );
    if ( leituraBotao == HIGH ) {
      jogar();
    }
  }
  statusAnteriorBotao = leituraBotao;
}

void desligaLEDs() {
  for (byte i = 0; i < quantidadeOpcoes; i++) {
    digitalWrite( pinoLED[i], LOW );
  }
}

void jogar() {
  Serial1.println( F( "Jogada acionada" ) );
  long quantidadeTotalPremios = 0;
  long somasPremios[quantidadeOpcoes];
  for ( byte j = 0; j < quantidadeOpcoes; j++ ) {
    quantidadeTotalPremios += (long)quantidadesPremios[j] + 1L;
    somasPremios[j] = quantidadeTotalPremios;
  }
#if defined(_debbug_i)
  Serial1.print( quantidadeTotalPremios - quantidadeOpcoes );
  Serial1.println( " premios para sortear" );
#endif
  if ( quantidadeTotalPremios == quantidadeOpcoes ) {
    return;
  }

  byte itemPremiado = 0;
  while (itemPremiado == 0) {
    long numeroSorteado = random(quantidadeTotalPremios) + 1;
    if ( numeroSorteado > somasPremios[6] ) {
      itemPremiado = 8;
    } else if ( numeroSorteado > somasPremios[5] ) {
      itemPremiado = 7;
    } else if ( numeroSorteado > somasPremios[4] ) {
      itemPremiado = 6;
    } else if ( numeroSorteado > somasPremios[3] ) {
      itemPremiado = 5;
    } else if ( numeroSorteado > somasPremios[2] ) {
      itemPremiado = 4;
    } else if ( numeroSorteado > somasPremios[1] ) {
      itemPremiado = 3;
    } else if ( numeroSorteado > somasPremios[0] ) {
      itemPremiado = 2;
    } else {
      itemPremiado = 1;
    }
    if ( quantidadesPremios[itemPremiado - 1] == 0 ) {
#if defined(_debbug_i)
      Serial1.print( "Sorteado item sem premio: " );
      Serial1.println( itemPremiado );
#endif
      itemPremiado = 0;
    }
  }

  animaSorteio(itemPremiado - 1);
  Serial1.print( "Numero premiado: " );
  Serial1.println( itemPremiado );
  quantidadesPremios[itemPremiado - 1]--;
  eeprom_grava_sorteio(itemPremiado - 1);
  
#if defined(_debbug_i)
  listaPremios();
#endif
}

#if defined(_debbug_i)
void listaPremios() {
  Serial1.println( "Lista de premios" );
  for ( byte j = 0; j < quantidadeOpcoes; j++ ) {
    Serial1.print( j + 1 );
    Serial1.print( ": " );
    Serial1.println( quantidadesPremios[j] );
  }
  Serial1.println();
}
#endif

void animaSorteio( byte itemSorteado ) {
  unsigned long tempoFinal = millis() + tempoAnimando;
  byte ledAtivo = quantidadeOpcoes;
  while (( tempoFinal > millis() ) || (ledAtivo == itemSorteado)) {
    tone(pinoBuzzer, 330, 80 );
    if ( ledAtivo >= quantidadeOpcoes ) {
      ledAtivo = 0;
    }
    digitalWrite( pinoLED[ledAtivo], HIGH );
    delay( tempoPisca  );
    digitalWrite( pinoLED[ledAtivo], LOW );
    delay( tempoPisca  );
    ledAtivo++;
  }
  for ( byte i = 0; i < 20; i++) {
    tone(pinoBuzzer, 294, 100);
    digitalWrite( pinoLED[itemSorteado], HIGH );
    delay(100);
    digitalWrite( pinoLED[itemSorteado], LOW );
    delay(100);
  }
}


void eeprom_ler_dados(){
  for (byte i = 0; i<quantidadeOpcoes;i++){
    dados_premios[i]=EEPROM.read(i);
  }
  for (byte i = 0; i<quantidadeOpcoes;i++){
    dados_sorteios[i]=EEPROM.read(quantidadeOpcoes+i);
    quantidadesPremios[i]=(dados_premios[i]-dados_sorteios[i]);
  }  
}

void eeprom_grava_sorteio(byte item){
  dados_sorteios[item]++;
  EEPROM.write(item,dados_sorteios[item]);
}


void serialEvent1() {
  String str_serial = "";
  while (Serial1.available()) {
    while (Serial1.available()) {
      str_serial += char( Serial1.read() );
    }
    delay(2); //Não alterar para microseconds
  }
  if ( str_serial.substring( 0, 6 ) == "$ajuda" ) {
    Serial1.println( F( "*** Ajudar ***" ) );
  } else if (str_serial.substring( 0, 6 ) == "$jogar") {
    #if defined(_debbug_i)
      Serial1.println( F("Executando teste de jogada...") );      
    #endif
    Serial1.println( "JOGAR");
    jogar();
  }  
}
