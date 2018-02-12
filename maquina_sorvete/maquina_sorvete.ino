/* 
 * Maquina de sorvete
 * 
 * Por Albeiro Lima (Brasil)
 * 12-02-2018
 * 
 * Funções
 * ~~~~~~~
 * 
 * setMotores => função que liga ou desliga os motores, liga a primeiro motor aguarda 5 segundos e liga o outro
 * ==========
 * 
 * postaMensagem => função que escreve a mensagem do LCD e também amensagem de debbug
 *  
 */

#define motorMisturador      8 //Misturador
#define motorCongelador      9 //Congelador
#define acionaValvula       10 //Valcula
#define botaoRetirar        A0 //Para tudo para retirada do sorvete
#define botaoTurbo          A1 //Liga misturador e congelador por um tempo

#define tempoRotinaInicial    1200000 //20 minutos
#define tempoValvulaligada      20000 //20 segundos
#define tempoAntesMotores       40000 //40 segundos
#define tempoMotoresAcionados   90000 //1,5 minutos

unsigned horaLigarMotores = 0;
unsigned horaDesligarMotores = 0;
unsigned horaDesligarValcula = 0;



void setup() {
  //Inicializa comunicação serial para debbug
  Serial.begin( 9600 );

  //Configura portas
  pinMode( motorMisturador, OUTPUT );
  pinMode( motorCongelador, OUTPUT );
  pinMode( acionaValvula, OUTPUT );

  postaMensagem( F("Boas vindas")  );
  setMotores( true ); //liga os motores
  
  //Aguarda processo inicial
  postaMensagem( F( "Aguarde processo inicial") );
  unsigned long tempoAguardar = tempoRotinaInicial;
  while ( tempoAguardar > 0 ) {
    delay(1000);    
    tempoAguardar -= 1000;
    postaMensagem( "Aguarde: " + millisToStr( tempoAguardar ) );
  }  
  setMotores( false ); //desliga os motores para dar inicio ao ciclo padrão

  //Inicia o processo padrão ligando a valvula e definindo a hora de desligar a valvula
  digitalWrite( acionaValvula, HIGH );
  horaDesligarValcula = millis() + tempoValvulaligada;
}

void loop() {
  
  //desliga valvula passado o tempo de acionamento
  if (( horaDesligarValcula > 0 ) && ( millis() >= horaDesligarValcula )) {
    horaDesligarValcula = 0;
    digitalWrite( acionaValvula, LOW );
    //define hora de liga os motores
    horaLigarMotores = millis() + tempoAntesMotores; 
  }

  //Liga motores
  if (( horaLigarMotores > 0 ) && ( millis() >= horaLigarMotores )) {
    horaLigarMotores = 0;
    setMotores( true );
    //define hora de desligar os motores
    horaDesligarMotores = millis() + tempoMotoresAcionados;
  }

  if (( horaDesligarMotores > 0 ) && ( millis() >= horaDesligarMotores )) {
    horaDesligarMotores = 0;
    //Aciona a valvula e programa hora de desligar
    digitalWrite( acionaValvula, HIGH );
    horaDesligarValcula = millis() + tempoValvulaligada;
  }
}

void postaMensagem( String mens ) {
  Serial.println( mens );
}


void setMotores( boolean ligar ) {
  if ( ligar ) {
    postaMensagem( F( "Ligando motor 1 (ON)") );
    digitalWrite( motorMisturador, HIGH ); 
    delay( 5000 );
    postaMensagem( F( "Ligando motor 2 (ON)") );
    digitalWrite( motorCongelador, HIGH );    
  } else { //desliga
    postaMensagem( F( "motores desligados (OFF)") ); 
    digitalWrite( motorMisturador, LOW );     
    digitalWrite( motorCongelador, LOW );       
  }  
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


