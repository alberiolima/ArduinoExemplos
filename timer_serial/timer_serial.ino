#define incrementoTimer 1000
String sTempo = "";
unsigned long iTimer = 0; 
boolean timeAtivo = false;
unsigned long proxAtualizacao = 0;

void setup() {
  Serial.begin( 9600 );
  Serial.println( "Iniciado!" );
}

void loop() {
  if ( (timeAtivo)&&(millis() >= proxAtualizacao) ) {
    proxAtualizacao = millis() + incrementoTimer;
    iTimer -= incrementoTimer;
    if ( iTimer == 0  ) {
      timeAtivo = false;
      Serial.println( "concluido" );
    }
    
  }
  if ( Serial.available() > 0 ) {
    while ( Serial.available() > 0 ) {
      char cLido = Serial.read();
      if ( cLido == '\n' ) {                                                              //012345678\n      
        if ((timeAtivo)&&( sTempo[0] == 'T' )) {
          Serial.print( "timer já ativo" );
        } else  if (( sTempo[0] == 'T' )&&(sTempo[3] == ':')&&(sTempo[6] == ':')) { //T00:00:00\n
          String sHora, sMinuto, sSegundos;
          sHora = sTempo.substring( 1, 3 );
          sMinuto = sTempo.substring( 4, 6 );
          sSegundos = sTempo.substring( 7, 9 );
          iTimer = sHora.toInt() * 3600;
          iTimer = iTimer + ( sMinuto.toInt() * 60 );
          iTimer = iTimer + sSegundos.toInt();
          iTimer = iTimer * 1000;
          timeAtivo = true;
          proxAtualizacao = 0;
        }
        sTempo = ""; 
      } else if (( cLido == 'C' )&&(timeAtivo)) {  
        timeAtivo = false;
        Serial.print( "\nTimer cancelado");
      } else if (( cLido == 'S' )) {  
        if (timeAtivo) {
          Serial.print( '\n' );
          int iHoras = 0;
          int iMinutos = 0;    
          unsigned long auxTime = iTimer / 1000;    
          if ( auxTime >= 3600 ) {
            iHoras = (int)(auxTime / 3600);
            auxTime = auxTime - ( (unsigned long)iHoras * 3600UL );
          }
          if ( auxTime >= 60 ) {
            iMinutos = (int)(auxTime / 60);
            auxTime = auxTime - ( (unsigned long)iMinutos * 60UL );
          }
          Serial.print( "A" );
          Serial.print( iHoras );
          Serial.print( ":" );
          Serial.print( iMinutos );
          Serial.print( ":" );
          Serial.print( auxTime );
        } else {
          Serial.println( "Pronto" );
        }
      } else {
        sTempo = sTempo + cLido;
      }
    }
  }
}
