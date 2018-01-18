/*
 * Exemplo de como acionar o PISCA LED usando um botão touch
 * Vídeo tutorial em: 
 * Código em: 
 * 
 * Por Albério Lima (alberio.lima@gmail.com)
 * Sobral, CE, Brasil
 * http://www.youtube.com/alberiolima
 */

#define portaLED    2
#define portaBOTAO  3
#define intervalo 200

boolean LedAtivo = true;
boolean operacaoPisca = false;
byte botaoAnterior = LOW;
unsigned long proximoAcao = 0;

void setup() {
  pinMode( portaLED, OUTPUT );
  pinMode( portaBOTAO, INPUT_PULLUP );
}

void loop() {
  
  //Captura acionamento do botão
  byte leituraBotalAtudal = digitalRead( portaBOTAO );
  if (( leituraBotalAtudal == LOW )&&( botaoAnterior == HIGH )) { //botão pressionado   
    operacaoPisca = !(operacaoPisca);
    proximoAcao = millis();
    
    //Para evitar de quanto apertar o botão para desligar o LED fique aceso, 
    digitalWrite( portaLED, LOW );  //pois pode acontecer se ser apertado o botão para desativar no momento que o       
                                    //LED está aceso
  }
  botaoAnterior = leituraBotalAtudal;

  if (operacaoPisca) {
    if ( millis() > proximoAcao ){
      proximoAcao += intervalo;
      LedAtivo = !(LedAtivo);
      if (LedAtivo) {
        digitalWrite( portaLED, LOW );    
      } else {
        digitalWrite( portaLED, HIGH );    
      }
    }
  }   
}
