String sDadosRecebidos = "";

void setup() {
 Serial.begin( 9600 ); 
}

String pegaValor( String& dados ) {
  String sRet = ""; 
  byte iPosi = dados.indexOf( "," );
  if ( iPosi > -1 ) {
    sRet = dados.substring(0, iPosi );
    dados = dados.substring(iPosi+1 );
  } else {
    sRet = dados;
    dados = "";
  }
  return( sRet );
}


void loop() {
  
  if ( Serial.available() > 0 ) { 
    char byteLido = Serial.read();
    if ( byteLido == '\n' ) {
      Serial.print( "Valor lido: " );
      Serial.print( sDadosRecebidos );
      Serial.println();
      Serial.println( "Separado: " );
      int soma = 0;
      while ( sDadosRecebidos.length() > 0 ){
        String sVal = pegaValor( sDadosRecebidos );
        Serial.println( sVal );
        soma += sVal.toInt();
      }
      Serial.println(soma);
      
    } else {
      sDadosRecebidos += byteLido;
    }
  }
}
