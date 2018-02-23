

/* Fiz algumas modificações no código 
 *  20/02/2018
 *  
 * http://electronoobs.com/eng_circuitos_tut4.php   
 * http://www.youtube.com/c/ELECTRONOOBS
 * This is the code with EMF detection for the Arduino ESC
 * The speed control is done using a potentiometer connected on pin A0
 * You could always change it in order to have a PWM 
 * input signal to control the speed. 
 * 
 * Please, like, share and subscribe and motivate
 */

#define AA1  3
#define AA2  5
#define BB1 11
#define BB2 10
#define CC1  9
#define CC2  6
#define ESC_enable 2 //This is not used for now, The ESC is always enabled
#define emfA A0
#define emfB A1
#define emfC A2
byte seqMotor[] = { B000110, B100100, B100001, B001001, B011000, B010010 };
byte ports[] = {3, 5, 11, 10, 9, 6 };
int fase=0;
int deltaA=0;
int emA=0;
int sum=0;

int IN=A3;

static int delta= 0;
static int Lastdelta= -1;

unsigned long nextMillis = 0; 

void setup() {
  pinMode(AA1,OUTPUT);
  pinMode(AA2,OUTPUT);
  pinMode(BB1,OUTPUT);
  pinMode(BB2,OUTPUT);
  pinMode(CC1,OUTPUT);
  pinMode(CC2,OUTPUT);
 
  pinMode(ESC_enable,OUTPUT);

  pinMode(IN,INPUT);
  pinMode(emfA,INPUT);
  pinMode(emfB,INPUT);
  pinMode(emfC,INPUT);
  
  //digitalWrite(enable,HIGH);
  //previousMillis = micros();
  
}

void loop() {

  if(micros() >= nextMillis ){  
    nextMillis += map(analogRead(IN),0,1024,1,1000);

    int emA = analogRead(emfA);
    int emB = analogRead(emfB);
    int emC = analogRead(emfC);
    int sum = (emA+emB+emC)/3;  
    for(byte i = 0;i<6;i++){
      digitalWrite( ports[i], bitRead( seqMotor[fase], 6-i)?HIGH:LOW );
    }
    
     
    if ((fase==0)||(fase==3)) { 
      delta = emA-sum;
    } else if ((fase==1)||(fase==4)) { 
      delta = emC-sum;
    } else if ((fase==2 )||(fase==5)) { 
      delta = emB-sum;
    } 
    
    Lastdelta = delta; //save the last delta
    if (((Lastdelta < 0)&&(delta > 0))||((Lastdelta > 0)&&(delta < 0))){ //Zero cross from - to + and + to -
      fase++;
      if (fase > 5) {
        fase = 1;
      }
    }    
  }   
} //loop ends

