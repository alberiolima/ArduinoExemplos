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
