/*




*/

#define _debbug_i 1

#define tempoAnimando  8000 //tempo total da animação dos LEDs
#define tempoPisca       50 //Intervalo em pisca

#define quantidadeOpcoes  8
#define pinoBotaoJoga    A0

unsigned int quantidadeJogadas = 200;
byte pinoLED[quantidadeOpcoes] = {2, 3, 4, 6, 7, 8, 9, 10};
byte quantidadesPremios[quantidadeOpcoes];

void setup() {
#if defined(_debbug_i)
  Serial.begin( 9600 );
  quantidadesPremios[0] = 10;
  quantidadesPremios[1] = 20;
  quantidadesPremios[2] = 30;
  quantidadesPremios[3] = 40;
  quantidadesPremios[4] = 50;
  quantidadesPremios[5] = 60;
  quantidadesPremios[6] = 70;
  quantidadesPremios[7] = 80;
  Serial.println( "Rotina de sorteio inicia,aperte x para jogar" );
#endif


  //Define como OUTPUT os pinos dos LEDs
  for (byte i = 0; i < quantidadeOpcoes; i++) {
    pinMode( pinoLED[i], OUTPUT );
    digitalWrite( pinoLED[i], HIGH);
  }

  //botão de jogar
  pinMode( pinoBotaoJoga, INPUT_PULLUP );

  delay(200); //acende rapidamente os LEDs para verificar se todos estão acendendo
  desligaLEDs();
  randomSeed(analogRead(1));
}

void loop() {
#if defined(_debbug_i)
  if (Serial.available() > 0) {
    if ( Serial.read() == 'x' ) {
      jogar();
    }
  }
#endif

}

void desligaLEDs() {
  for (byte i = 0; i < quantidadeOpcoes; i++) {
    digitalWrite( pinoLED[i], LOW );
  }
}


void jogar() {
  long quantidadeTotalPremios = 0;
  long somasPremios[quantidadeOpcoes];
  for ( byte j = 0; j < quantidadeOpcoes; j++ ) {
    quantidadeTotalPremios += (long)quantidadesPremios[j];
    somasPremios[j] = quantidadeTotalPremios;
  }
  Serial.print( quantidadeTotalPremios );
  Serial.println( " premios para sortear" );

  long numeroSorteado = random(quantidadeTotalPremios) + 1;
  Serial.print( "Numero sorteado: " );
  Serial.println( numeroSorteado );
  byte itemPremiado = 0;
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
  animaSorteio();
  Serial.print( "Numero premiado: " );
  Serial.println( itemPremiado );
  quantidadesPremios[itemPremiado - 1]--;

  listaPremios();
}

void listaPremios() {
  Serial.println( "Lista de premios" );
  for ( byte j = 0; j < quantidadeOpcoes; j++ ) {
    Serial.print( j + 1 );
    Serial.print( ": " );
    Serial.println( quantidadesPremios[j] );
  }
  Serial.println();
}

void animaSorteio(){
  unsigned long tempoFinal = millis() + tempoAnimando;
  byte ledAtivo = quantidadeOpcoes;
  while ( tempoFinal > millis() ) {
    if ( ledAtivo >= quantidadeOpcoes ) {
      ledAtivo = 0;
    }
    digitalWrite( pinoLED[ledAtivo], HIGH );
    delay( tempoPisca  );
    digitalWrite( pinoLED[ledAtivo], LOW );
    delay( tempoPisca  );
    ledAtivo++;
  }
  
}

