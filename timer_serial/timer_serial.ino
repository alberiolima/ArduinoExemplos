#define incrementoTimer 1000

//Variáveis públicas
unsigned long iTimerAtual = 0; 
unsigned long proxAtualizacao = 0;
boolean timeAtivo = false;
String sTempo = "";

void setup() {
  Serial.begin( 9600 ); //Inicia comunicação serial
  Serial.println( "Iniciado!" );
}

void loop() {
  
  //Decrementar timer quando ativo
  if ( (timeAtivo)&&(millis() >= proxAtualizacao) ) {
    proxAtualizacao += incrementoTimer;
    iTimerAtual -= incrementoTimer;
    
    //Desativa o timer quando concluir o tempo
    if ( iTimerAtual == 0  ) {
      timeAtivo = false;
      Serial.println( "concluido" );
    }    
  }
  
  if ( Serial.available() > 0 ) {
    char cLido = Serial.read();
    if ( cLido == '\n' ) {                                                              
      if ((timeAtivo) && ( sTempo[0] == 'T' )) {
        Serial.println( "timer ja ativo" );
      } else  if (( sTempo[0] == 'T' )&&(sTempo[3] == ':')&&(sTempo[6] == ':')) { //T00:00:00
        iTimerAtual = strToMillis(sTempo.substring(1));
        //Inicia a contagem
        timeAtivo = true;
        proxAtualizacao = millis();
      }
      sTempo = ""; 
    } else if (( cLido == 'C' )&&(timeAtivo)) {  
      timeAtivo = false;
      Serial.println( "Timer cancelado");
    } else if (( cLido == 'S' )) {  
      if (timeAtivo) {        
        Serial.print( "A" );
        Serial.println( millisToStr( iTimerAtual) );
      } else {
        Serial.println( "Pronto" );
      }
    } else {
      sTempo = sTempo + cLido;
    }
  }

}

unsigned long strToMillis( String tempo ) {
  //Separa hms
  String sHora, sMinuto, sSegundos;
  sHora = tempo.substring( 0, 2 );
  sMinuto = tempo.substring( 3, 5 );
  sSegundos = tempo.substring( 6, 8 );
        
  //Converte hms para millissegundos
  unsigned long ret = 0;
  ret += sHora.toInt() * 3600;
  ret += ( sMinuto.toInt() * 60 );
  ret += sSegundos.toInt();  
  return (ret * 1000UL);  
}

String millisToStr( unsigned long ulTempo ) {
  int iHoras = 0;
  int iMinutos = 0;    
  ulTempo /= 1000;    
  if ( ulTempo >= 3600 ) {
    iHoras = (int)(ulTempo / 3600);
    ulTempo -= ( (unsigned long)iHoras * 3600UL );
  }
  if ( ulTempo >= 60 ) {
    iMinutos = (int)(ulTempo / 60);
    ulTempo -= ( (unsigned long)iMinutos * 60UL );
  }
  return ( String( iHoras) + ":" + String( iMinutos ) + ":" + String( ulTempo) );
}

