#define tempoDuracao    35 //Duração de cada nota
#define tempoIntervalo   1 //Duração do intervalo entre as notas
#define quantOitavas     3 //Extensão 
#define oitavaInicial    2 //Oitava que será iniciado

int notasMusicais[] = { 262, 294,330, 349, 392, 440, 494 };

void setup() {
  

}

void loop() {
  for ( byte j = 0; j < quantOitavas; j++){
    for ( byte i = 0; i < 7; i++){      
      tone(9, notasMusicais[i] * j * oitavaInicial, tempoDuracao ); 
      delay(tempoDuracao + tempoIntervalo);
    }
  }
  delay(3000);
}
