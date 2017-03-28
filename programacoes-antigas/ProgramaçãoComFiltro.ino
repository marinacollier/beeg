/* basic.ino
 * Código para controle de robô beetleweight
 * Autores: Isabella Galvão e Marcus Vinícius
 * 14/09/2016
 */

//Inclui biblioteca para acesso a funções matemáticas avançadas
#include <math.h>
#include <Servo.h>
Servo servo1;


//Define os pinos que os motores estarão conectados
#define MOTOR_E1 11
#define MOTOR_E2 10
#define MOTOR_D1 5
#define MOTOR_D2 6
#define PORTA_RESISTENCIA 8


#define DEBUG 0
#define ESPERA 500
#define MAX_RUD 1840
#define ANGULO_MAX 30 //Define as entradas de sinal do controle
#define AILE A0
#define ELE A1
#define THR A2
#define RUD A3

//Constantes para leitura do controle (Devem ser calibradas)
#define MAX 1900
#define MIN 1200
//Intensidade máxima do sinal permitida como velocidade máxima
#define VELOCIDADE_MAXIMA 255
#define VELOCIDADE_MINIMA 10    //Mudar de acordo com o robô
#define N 3

//Vão guardar as leituras vindas do controle
int aile = 0;
int ele = 0;
int ele_potencia = 0;
int aile_potencia = 0;
int ele_filtrado,aile_filtrado;
int eleF[N],aileF[N];
int EMA_S = 0;
int EMA_S1 = 0;
float EMA_B = 2.0/(N+1);
int rud=0, thr =0;
bool acionar;




//limiar aile ele
#define LIMIAR_MAX_AILE 9
#define LIMIAR_MIN_AILE -49
#define LIMIAR_MAX_ELE  9 
#define LIMIAR_MIN_ELE  -49

void setup() {
  // put your setup code here, to run once:
  pinMode(AILE, INPUT);
  pinMode(ELE, INPUT);
  pinMode(RUD,INPUT);
  pinMode(THR,INPUT);
  pinMode(PORTA_RESISTENCIA,OUTPUT);

  pinMode(MOTOR_E1, OUTPUT);
  pinMode(MOTOR_E2, OUTPUT);
  pinMode(MOTOR_D1, OUTPUT);
  pinMode(MOTOR_D2, OUTPUT);
  servo1.attach(9);
  aile=0;
  ele=0;
  if(DEBUG){
  Serial.begin(9600);
  Serial.flush();
  }
  for(int i=0;i<N;i++){eleF[N]=0; aileF[N]=0;}
}

void loop (){
inicializacao();


Serial.print("AILE: ");
Serial.println(aile);
Serial.print("ELE : ");
Serial.println(ele);
delay(ESPERA);



if ((aile>=1489 && aile<=1560 ) && (ele>=1500 && ele<=1560)|| (aile ==0 && ele==0)){
  parado();
  Serial.println("Parado");
}

else{
 // Serial.println("funfou");
 acionamento_servo();
angulacao_servo(acionar);
acionamento_resistencia();



     ele_potencia = potenciaPwmEle(ele);
     aile_potencia = potenciaPwmAile(aile);
     ele_filtrado = filtroEle(ele_potencia);
     aile_filtrado = filtroAile(aile_potencia);
     
  
if(((aile_filtrado >= LIMIAR_MIN_AILE) && (aile_filtrado <= LIMIAR_MAX_AILE)) && ((ele_filtrado >= LIMIAR_MIN_ELE) && (ele_filtrado <= LIMIAR_MAX_ELE)))
parado();

else if(ele_filtrado > 40 && ele_filtrado <= 255){
if(aile_filtrado >=-10 && aile_filtrado <= 10){
direcoes(1);
//Serial.println("FRENTE");
}
else if((ele_filtrado > 40 && ele_filtrado <= 255)&& (aile_filtrado > 10 && aile_filtrado < 250)){
 direcoes(2); 
//Serial.println("FRENTE DIREITA");
}
else if((ele_filtrado > 40 && ele_filtrado <= 255) &&(aile_filtrado > -250 && aile_filtrado < -10)){
direcoes(12);
  //Serial.println("FRENTE ESQUERDA");
}
else if ((aile_filtrado>30 && aile_filtrado <= 255)&&(ele_filtrado>18 && ele_filtrado <255)){
direcoes(3);
//Serial.println("DIREITA CIMA");
}
else if ((aile_filtrado>=-255 && aile_filtrado <-30)&&(ele_filtrado>18 && ele_filtrado <255)){
direcoes(11);
//Serial.println("ESQUERDA CIMA");
}
}

else if(ele_filtrado >=-10 && ele_filtrado <=18 && (aile_filtrado > 29 && aile_filtrado <= 255)){
direcoes(4);
//Serial.println("DIREITA");
}
else if(ele_filtrado >=-10 && ele_filtrado <=18 && (aile_filtrado >= -255 && aile_filtrado <= -35)){
direcoes(10);
//  Serial.println("ESQUERDA");
}
else if(ele_filtrado >=-255 && ele_filtrado <-40){
if(aile_filtrado >=-10 && aile_filtrado <= 10 && (ele_filtrado >=-255 && ele_filtrado <-40)){
direcoes(7);
//  Serial.println("TRAS");
}
else if((ele_filtrado >= -255 && ele_filtrado <-40)&& (aile_filtrado > 10 && aile_filtrado < 250)){
direcoes(6);
 // Serial.println("TRAS DIREITA");
}
else if((ele_filtrado >=-255 && ele_filtrado <-40) &&(aile_filtrado > -250 && aile_filtrado < -10)){
direcoes(8);
 // Serial.println("TRAS ESQUERDA");
}
else if ((aile_filtrado>30 && aile_filtrado <= 255)&&(ele_filtrado>-255 && ele_filtrado <-40)){
direcoes(5);
//  Serial.println("DIREITA BAIXO");
}
else if ((aile_filtrado>=-255 && aile_filtrado <-30)&&(ele_filtrado>-255 && ele_filtrado <-40)){
direcoes(9);
//  Serial.println("ESQUERDA BAIXO");
}
}

}
}















void inicializacao(){
aile = pulseIn(AILE, HIGH);  
ele = pulseIn(ELE,HIGH);
} 


  

void leitura_ele_aile(){
Serial.print(" AILE : ");
Serial.print(aile_filtrado);
Serial.print(" ELE: ");
Serial.println(ele_filtrado);
//delay(ESPERA);
}
void parado(){
  digitalWrite(MOTOR_E1,LOW);
  digitalWrite(MOTOR_E2,LOW);
  digitalWrite(MOTOR_D1,LOW);
  digitalWrite(MOTOR_D2,LOW);
}


void motorEsquerdo(int pwr) {

  if (pwr > 0) {
    analogWrite(MOTOR_E1, abs(pwr));
    digitalWrite(MOTOR_E2, LOW);
  }
  else {
    analogWrite(MOTOR_E2, abs(pwr));
    digitalWrite(MOTOR_E1, LOW);
  }
}
void motorDireito(int pwr) {
  

  if (pwr > 0) {
    analogWrite(MOTOR_D1, abs(pwr));
    digitalWrite(MOTOR_D2, LOW);
  }
  else {
    analogWrite(MOTOR_D2, abs(pwr));
    digitalWrite(MOTOR_D1, LOW);
  }
}
void direcoes (int x){
      if (x==1){
      analogWrite(MOTOR_E1,VELOCIDADE_MAXIMA);
      digitalWrite(MOTOR_E2,LOW);
      analogWrite(MOTOR_D1,VELOCIDADE_MAXIMA);
      digitalWrite(MOTOR_D2,LOW);
      }
      else if(x==2){
      analogWrite(MOTOR_E1,abs(ele_filtrado));
      digitalWrite(MOTOR_E2,LOW);
      analogWrite(MOTOR_D1,abs(ele_filtrado-aile_filtrado));
      digitalWrite(MOTOR_D2,LOW);        
      }
      else if(x==3){
      analogWrite(MOTOR_E1,abs(aile_filtrado));
      digitalWrite(MOTOR_E2,LOW);
      digitalWrite(MOTOR_D1,LOW);
      analogWrite(MOTOR_D2,abs(aile_filtrado-ele_filtrado));        
      }
      else if(x==4){
      analogWrite(MOTOR_E1,VELOCIDADE_MAXIMA);
      digitalWrite(MOTOR_E2,LOW);
      digitalWrite(MOTOR_D1,LOW);
      analogWrite(MOTOR_D2,VELOCIDADE_MAXIMA);        
      }
      else if (x==5){
      digitalWrite(MOTOR_E1,LOW);
      analogWrite(MOTOR_E2,abs(aile_filtrado));
      analogWrite(MOTOR_D1,abs(aile_filtrado+ele_filtrado));
      digitalWrite(MOTOR_D2,LOW);
      }
      else if (x==6){
      digitalWrite(MOTOR_E1,LOW);
      analogWrite(MOTOR_E2,abs(ele_filtrado));
      digitalWrite(MOTOR_D1,LOW);
      analogWrite(MOTOR_D2,abs(ele_filtrado+aile_filtrado)); 
      }
      else if (x==7){
      digitalWrite(MOTOR_E1,LOW);
      analogWrite(MOTOR_E2,abs(VELOCIDADE_MAXIMA));
      digitalWrite(MOTOR_D1,LOW);
      analogWrite(MOTOR_D2,abs(VELOCIDADE_MAXIMA)); 
      }
      else if (x==8){
      digitalWrite(MOTOR_E1,LOW);
      analogWrite(MOTOR_E2,abs(-ele_filtrado+aile_filtrado));
      digitalWrite(MOTOR_D1,LOW);
      analogWrite(MOTOR_D2,abs(ele_filtrado)); 
      }
      else if (x==9){
      analogWrite(MOTOR_E1,abs(ele_filtrado-aile_filtrado));
      digitalWrite(MOTOR_E2,LOW);
      digitalWrite(MOTOR_D1,LOW);
      analogWrite(MOTOR_D2,abs(aile_filtrado)); 
      }
      else if (x==10){
      digitalWrite(MOTOR_E1,LOW);
      analogWrite(MOTOR_E2,abs(VELOCIDADE_MAXIMA));
      analogWrite(MOTOR_D1,abs(VELOCIDADE_MAXIMA));
      digitalWrite(MOTOR_D2,LOW); 
      }
    else if (x==11){
      digitalWrite(MOTOR_E1,LOW);
      analogWrite(MOTOR_E2,abs(aile_filtrado+ele_filtrado));
      analogWrite(MOTOR_D1,abs(aile_filtrado));
      digitalWrite(MOTOR_D2,LOW); 
      }
    else if(x==12){
      analogWrite(MOTOR_E1,abs(ele_filtrado+aile_filtrado));
      digitalWrite(MOTOR_E2,LOW);
      analogWrite(MOTOR_D1,abs(ele_filtrado));
      digitalWrite(MOTOR_D2,LOW);
      }
  
}

void acionamento_servo(){
rud = pulseIn(RUD, HIGH);  
thr = pulseIn(THR,HIGH);

if (rud >= MAX_RUD)
acionar = true;
else 
acionar = false;

/*Serial.print("RUD : ");
Serial.println(rud);
Serial.print("THR : ");
Serial.println(thr);
delay(500); */
  
}

void angulacao_servo(bool tipo){
  if(tipo){
  servo1.write(ANGULO_MAX);
 // Serial.println(ANGULO_MAX);
  }
  else
  servo1.write(0); 
  //Serial.println(acionar);
}
void acionamento_resistencia (){
if(thr < 1548){
digitalWrite(PORTA_RESISTENCIA,LOW);
//Serial.println("Fogo desligado boy");
//delay(ESPERA);
}
else{
digitalWrite(PORTA_RESISTENCIA,HIGH);
//Serial.println("Fogo ligado boy");
//delay(ESPERA);
}
}














int filtroEle(int sinal){
 /* for(int i= N-1;i>0;i--)eleF[i]=eleF[i-1];
  eleF[0]=sinal;
  long soma=0;
  for(int i=0;i<N;i++)soma+=eleF[i];
int filtrado = soma/N;*/
 EMA_S = (EMA_B*sinal) + ((1-EMA_B)*EMA_S);
return EMA_S;
}

int filtroAile(int sinal1){
  /*for(int i= N-1;i>0;i--)aileF[i]=aileF[i-1];
  aileF[0]=sinal1;
  long soma1=0;
  for(int i=0;i<N;i++)soma1+=aileF[i];
int filtrado1 = soma1/N;*/
 EMA_S1 = (EMA_B*sinal1) + ((1-EMA_B)*EMA_S1);
return EMA_S1;
}
int potenciaPwmAile(int sinal) {
    int potencia = map(sinal, MIN, MAX, -255, 255);               //mapeando aile
    if(abs(potencia) > VELOCIDADE_MAXIMA) {                   //limitando velocidade máxima
      return (potencia/abs(potencia)) * VELOCIDADE_MAXIMA;
    }
      else if(abs(potencia) < VELOCIDADE_MINIMA){
        return (potencia/abs(potencia)) * VELOCIDADE_MINIMA;
      }
        else{
          return potencia;
        }
  }
//Função que mapeia os valores de ele e limita a potência
int potenciaPwmEle(int sinal) {
    int potencia = map(sinal, MIN, MAX, -255, 255);             //mapeando a ele
    if(abs(potencia) > VELOCIDADE_MAXIMA) {                 //limitando a velocidade máxima 
     return (potencia/abs(potencia)) * VELOCIDADE_MAXIMA;
    }
      else if(abs(potencia) < VELOCIDADE_MINIMA){
        return (potencia/abs(potencia)) * VELOCIDADE_MINIMA;
      }
      else {
        return potencia;
    }
  }

 


