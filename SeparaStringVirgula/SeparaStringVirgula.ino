/*  O foco desse exemplo é a
 *  Função pegaValor
 *  Esse função pega o primeiro valor antes da virgula e retorna esse valor
 *  a função também remove da variável "dados", passada como parâmetro esse valor
 *  ex:
 *  Se o valor passado como parâmetro através da variável "dados" for "25,496,859"
 *  o valor retornado será 25, e a variável dados ficará apenas com o restante dos caracteres: 496,859
 *  da próxima vez que for executada retornará 496 e deixará a variável passada apanas com 869
 *  da ultima vez vai retornar o ultimo valor 869 e deixará a variável "dados" vazia
*/

String sDadosRecebidos = ""; //Variável public que terá os dados recebidos

void setup() {
 Serial.begin( 9600 ); //inicialização da comunicação serial
}


String pegaValor( String& dados ) {
  String sRetorno = ""; 
  byte iPosi = dados.indexOf( "," );         //Localiza a posição da virgula dentro de dados
  if ( iPosi > -1 ) {                        //Se foi encontrada uma virgula dentro de dados
    sRetorno = dados.substring( 0, iPosi );  //Pega os caracteres desde o inicio até a posição da virgula, sem incluir a virgula
    dados = dados.substring(iPosi+1 );       //Atualiza a variável dados passada como parâmetro retirando os caracteres iniciais e também a virgula
  } else {                                   //Se não encontrar a virtula, tiver caracteres em dados, faz o ultimo retorno
    sRetorno = dados;
    dados = "";                              //deixa a variável dados vazia
  }
  return( sRetorno );
}


void loop() {
  
  if ( Serial.available() > 0 ) { 
    char byteLido = Serial.read();
    if ( byteLido == '\n' ) {
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
