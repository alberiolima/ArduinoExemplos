#define PIN_LEITURA 2
#define PIN_GRAVAR  3

#define sinalLer    HIGH
#define sinalGravar HIGH

unsigned long tempoIniciou = 0;
unsigned long tempoFinalizar = 0;

void setup() {
  if ( sinalLer == LOW ) {
    pinMode( PIN_LEITURA, INPUT_PULLUP );
  } else {
    pinMode( PIN_LEITURA, INPUT );
  }
  pinMode( PIN_GRAVAR, OUTPUT );
  digitalWrite( PIN_GRAVAR, !sinalGravar);

  Serial.begin(9600);
}

void loop() {
  //Faz leitura do sinal
  if ((digitalRead( PIN_LEITURA ) == sinalLer) && (tempoIniciou == 0))
  {
    delay(5);//debounce
    if (digitalRead( PIN_LEITURA ) == sinalLer)
    {
      tempoIniciou = millis(); //Pega o tempo inicial do recebimento do sinal de entrada
      Serial.println();
      Serial.print( "Recebendo sinal de entrada, " );
    }
  }
  else if ((digitalRead( PIN_LEITURA ) != sinalLer) && (tempoIniciou > 0))
  {
    delay(5);//debounce
    if (digitalRead( PIN_LEITURA ) != sinalLer)
    {
      if (tempoFinalizar == 0)
      {
        digitalWrite( PIN_GRAVAR, sinalGravar); //Aciona sinal de saída
        unsigned long tempoDecorrido = ( millis() - tempoIniciou );
        tempoFinalizar = millis() + tempoDecorrido; //Agenda fim do sinal de saída
        Serial.print( " tempo decorrido: " );
        Serial.print( tempoDecorrido );
        Serial.println( "ms" );
        Serial.println( "Repetindo sinal na saida" );
      }
      tempoIniciou = 0;//Inicia a variável para a próxima leitura
    }
  }

  //Finaliza o sinal da saída no tempo agendado
  if ((tempoFinalizar > 0) && (millis() > tempoFinalizar))
  {
    tempoFinalizar = 0;
    digitalWrite( PIN_GRAVAR, !sinalGravar);
    Serial.println( "Sinal de saida finalizado" );
  }
}
