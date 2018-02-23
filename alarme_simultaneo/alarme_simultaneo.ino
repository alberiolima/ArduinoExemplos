#define tempo 10
#define pinoFalante 9

boolean alarmeAtivo = true;
int frequencia = 150;
char sentido = 1;

void setup() {
  pinMode( 12,OUTPUT );
  digitalWrite( 12, LOW );
}

void loop() {
  
  if ( alarmeAtivo ) {
    tone(pinoFalante, frequencia += sentido, tempo ); 
    if( ( frequencia > 1800 )|| ( frequencia < 150)){
      sentido *= -1;
    }
    delay(1); //não é necessário, porém vou deixar para não confundir...
  } 
}


/*
2o.
#define tempo 10
#define pinoFalante 9

boolean alarmeAtivo = true;
int frequencia = 150;
char sentido = 1;

void setup() {
pinMode( 12,OUTPUT );
digitalWrite( 12, LOW );
}

void loop() {

if ( alarmeAtivo ) {
tone(pinoFalante, frequencia, tempo ); 
frequencia += sentido; 
if ( frequencia > 1800 ) {
sentido= -1;
} else if ( frequencia < 150 ) {
sentido = 1;
}
delay(1); //não é necessário, porém vou deixar para não confundir...
} 
}


***** 1o.
#define tempo 10
#define pinoFalante 9

boolean alarmeAtivo = true;
int frequencia = 150;
boolean sentido = true;

void setup() {
pinMode( 12,OUTPUT );
digitalWrite( 12, LOW );
}

void loop() {

if ( alarmeAtivo ) {
tone(pinoFalante, frequencia, tempo ); 
if ( sentido ) {
frequencia++; 
if ( frequencia > 1800 ) {
sentido = !sentido;
}
} else {
frequencia--;
if ( frequencia < 150 ) {
sentido = !sentido;
}
}
delay(1); //não é necessário, porém vou deixar para não confundir...
} 
}
 */
