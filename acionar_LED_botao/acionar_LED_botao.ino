/*
 * Exemplo de como acionar um LED usando um botão touch
 * Vídeo tutorial em: https://youtu.be/kcG1tpctHmU
 * Código em: https://github.com/alberiolima/ArduinoExemplos/tree/master/acionar_LED_botao
 * 
 * Por Albério Lima
 * Sobral, CE, Brasil
 * http://www.youtube.com/alberiolima
 */

#define portaLED    2
#define portaBOTAO  3

boolean LedAtivo = false;
byte botaoAnterior = HIGH;

void setup() {
  pinMode( portaLED, OUTPUT );
  pinMode( portaBOTAO, INPUT_PULLUP );
}

void loop() {
  byte leituraBotalAtudal = digitalRead( portaBOTAO );
  if (( leituraBotalAtudal == LOW )&&( botaoAnterior == HIGH )) { //botão pressionado
    LedAtivo = !(LedAtivo);
  }
  botaoAnterior = leituraBotalAtudal;
  
  if (LedAtivo) {
    digitalWrite( portaLED, LOW );    
  } else {
    digitalWrite( portaLED, HIGH );    
  }  
}
