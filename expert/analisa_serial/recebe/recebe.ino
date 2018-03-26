#define pinRX 3
#define quantBITS 9

unsigned long _bytes[100];
boolean b_bytes[100];
unsigned long espera;
unsigned long tempoInicial = 0;

void setup() {
  pinMode( pinRX, INPUT );
   
  Serial.begin( 115200 );
  Serial.println( "Iniciando..." );
}

void loop() {
  if((espera = pulseIn(pinRX , HIGH)) > 800000) {
    for(byte i = 0; i < quantBITS; i++) { 
      tempoInicial = micros();
      b_bytes[i] = digitalRead( pinRX );
      while (digitalRead( pinRX ) == b_bytes[i]);
      _bytes[i]=micros() - tempoInicial;      
    }
    
    Serial.println();
    Serial.print( "Intervalo=" );
    Serial.print( espera/1000 ); //em milesegundos
    Serial.print( ", " );
    for(byte i = 0; i < quantBITS; i++) {       
      Serial.print( String(i)+"]" );
      Serial.print( _bytes[i] );
      Serial.print( ' ' );
      if ( b_bytes[i] ) {
        Serial.print( "H" );
      } else {
        Serial.print( "L" );
      }
      Serial.print( '\t' ); //Tabulação entre valores
      
      //Zera variáveis para próxima consulta
      _bytes[i]=0;
      b_bytes[i]=0;      
    }    
  }
}
