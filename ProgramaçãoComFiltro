/* basic.ino
 * Código para controle de robô beetleweight
 * Autores: Isabella Galvão e Marcus Vinícius
 * 14/09/2016
 */

//Inclui biblioteca para acesso a funções matemáticas avançadas
#include <math.h>


//Define os pinos que os motores estarão conectados
#define MOTOR_E1 11
#define MOTOR_E2 10
#define MOTOR_D1 5
#define MOTOR_D2 6


#define DEBUG 1
#define ESPERA 500

//Define as entradas de sinal do controle
#define AILE A0
#define ELE A1

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





//limiar aile ele
#define LIMIAR_MAX_AILE 9
#define LIMIAR_MIN_AILE -49
#define LIMIAR_MAX_ELE  9 
#define LIMIAR_MIN_ELE  -49

void setup() {
  // put your setup code here, to run once:
  pinMode(AILE, INPUT);
  pinMode(ELE, INPUT);

  pinMode(MOTOR_E1, OUTPUT);
  pinMode(MOTOR_E2, OUTPUT);
  pinMode(MOTOR_D1, OUTPUT);
  pinMode(MOTOR_D2, OUTPUT);
  if(DEBUG){
  Serial.begin(9600);
  Serial.flush();
  }
  for(int i=0;i<N;i++){eleF[N]=0; aileF[N]=0;}
}

void loop (){
inicializacao();

if(((aile_filtrado >= LIMIAR_MIN_AILE) && (aile_filtrado <= LIMIAR_MAX_AILE)) && ((ele_filtrado >= LIMIAR_MIN_ELE) && (ele_filtrado <= LIMIAR_MAX_ELE)))
  parado();

else if(ele_filtrado > 40 && ele_filtrado <= 255){
if(aile_filtrado >=-10 && aile_filtrado <= 10)
Serial.println("FRENTE");
else if((ele_filtrado > 40 && ele_filtrado <= 255)&& (aile_filtrado > 10 && aile_filtrado < 250))
Serial.println("FRENTE DIREITA");
else if((ele_filtrado > 40 && ele_filtrado <= 255) &&(aile_filtrado > -250 && aile_filtrado < -10))
Serial.println("FRENTE ESQUERDA");
else if ((aile_filtrado>30 && aile_filtrado <= 255)&&(ele_filtrado>18 && ele_filtrado <255))
Serial.println("DIREITA CIMA");
else if ((aile_filtrado>=-255 && aile_filtrado <-30)&&(ele_filtrado>18 && ele_filtrado <255))
Serial.println("ESQUERDA CIMA");
}

else if(ele_filtrado >=-10 && ele_filtrado <=18 && (aile_filtrado > 29 && aile_filtrado <= 255)){
Serial.println("DIREITA");}
else if(ele_filtrado >=-10 && ele_filtrado <=18 && (aile_filtrado >= -255 && aile_filtrado <= -35)){
Serial.println("ESQUERDA");}

else if(ele_filtrado >=-255 && ele_filtrado <-40){
if(aile_filtrado >=-10 && aile_filtrado <= 10 && (ele_filtrado >=-255 && ele_filtrado <-40))
Serial.println("TRAS");
else if((ele_filtrado >= -255 && ele_filtrado <-40)&& (aile_filtrado > 10 && aile_filtrado < 250))
Serial.println("TRAS DIREITA");
else if((ele_filtrado >=-255 && ele_filtrado <-40) &&(aile_filtrado > -250 && aile_filtrado < -10))
Serial.println("TRAS ESQUERDA");
else if ((aile_filtrado>30 && aile_filtrado <= 255)&&(ele_filtrado>-255 && ele_filtrado <-40))
Serial.println("DIREITA BAIXO");
else if ((aile_filtrado>=-255 && aile_filtrado <-30)&&(ele_filtrado>-255 && ele_filtrado <-40))
Serial.println("ESQUERDA BAIXO");
}


}












void inicializacao(){
aile = pulseIn(AILE, HIGH);  
ele = pulseIn(ELE,HIGH);
ele_potencia = potenciaPwmEle(ele);
aile_potencia = potenciaPwmAile(aile);
ele_filtrado = filtroEle(ele_potencia);
aile_filtrado = filtroAile(aile_potencia); 
}

void leitura_ele_aile(){
Serial.print(" AILE : ");
Serial.print(aile_filtrado);
Serial.print(" ELE: ");
Serial.println(ele_filtrado);
delay(ESPERA);
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
      analogWrite(MOTOR_E1,ele_filtrado);
      digitalWrite(MOTOR_E2,LOW);
      analogWrite(MOTOR_D1,ele_filtrado-aile_filtrado);
      digitalWrite(MOTOR_D2,LOW);        
      }
      else if(x==3){
      analogWrite(MOTOR_E1,VELOCIDADE_MAXIMA);
      digitalWrite(MOTOR_E2,LOW);
      digitalWrite(MOTOR_D1,LOW);
      analogWrite(MOTOR_D2,VELOCIDADE_MAXIMA);        
      }
      else if(x==4){
      digitalWrite(MOTOR_E1,LOW);
      analogWrite(MOTOR_E2,aile_filtrado);
      digitalWrite(MOTOR_D1,LOW);
      analogWrite(MOTOR_D2,abs(aile_filtrado-ele_filtrado));        
      }
      if (x==5){
      digitalWrite(MOTOR_E1,LOW);
      analogWrite(MOTOR_E2,VELOCIDADE_MAXIMA);
      digitalWrite(MOTOR_D1,LOW);
      analogWrite(MOTOR_D2,VELOCIDADE_MAXIMA);
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

 


