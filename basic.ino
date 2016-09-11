
 /* beeg.ino
 * Código para controle de robô beetleweight
 * Autores: Isabella Galvão e Marcus Vinícius
 * 05/09/2016
 */

//Inclui biblioteca para acesso a funções matemáticas avançadas
#include <math.h>
#include <Arduino.h>

//Define os pinos que os motores estarão conectados
#define MOTOR_E1 11
#define MOTOR_E2 10
#define MOTOR_D1 5
#define MOTOR_D2 6


//Define as entradas de sinal do controle
#define AILE A1
#define ELE A0

//Constantes para leitura do controle (Devem ser calibradas)
//Intensidade máxima do sinal permitida como velocidade máxima
#define MAX 1800
#define MIN 1200
#define TOLERANCIA 30
#define VELOCIDADE_MAXIMA 255
#define div  2                  //divisor de potência para diagonal,valores de teste , amanhã mudamos

//Vão guardar as leituras vindas do controle
int aile = 0;
int ele = 0;
int potencia = 0;
//limiar aile ele
#define LIMIAR_MAX_AILE 111
#define LIMIAR_MIN_AILE 101
#define LIMIAR_MAX_ELE 111  //valores de teste , amanhã mudamos
#define LIMIAR_MIN_ELE 101

void setup() {
  // put your setup code here, to run once:
  pinMode(AILE, INPUT);
  pinMode(ELE, INPUT);

  pinMode(MOTOR_E1, OUTPUT);
  pinMode(MOTOR_E2, OUTPUT);
  pinMode(MOTOR_D1, OUTPUT);
  pinMode(MOTOR_D2, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  
  //leitura dos canais do controle
   aile= pulseIn(AILE, HIGH);  
   ele= pulseIn(ELE,HIGH); 
} 
/*int MspAile (int value1) {
  value1=map(aile,MIN,MAX,-255,255);
  return value1;
}

int MapEle(int value2){
  value2=map(ele,MIN,MAX,-255,255);
  return value2;
}*/
 int mapPwm(int value) {
   return map(value, MIN, MAX, -255, 255);
}
   
//condição parado  
  if(((aile >= LIMIAR_MIN_AILE)  && (aile <= LIMIAR_MAX_AILE))  && ((ele <= LIMIAR_MIN_ELE) && (ele <= LIMIAR_MAX_ELE))){
   potencia = map(ele,MIN,MAX,-255,255);
   // if(potencia>245) {potencia=255;}
      digitalWrite(MOTOR_E1,LOW);
      digitalWrite(MOTOR_E2,LOW);
      digitalWrite(MOTOR_D1,LOW);
      digitalWrite(MOTOR_D2,LOW);
  }
  //condição para frente
  else if(((aile >= LIMIAR_MIN_AILE)  && (aile <= LIMIAR_MAX_AILE)) && (ele > LIMIAR_MAX_ELE)){
    potencia = map(ele,MIN,MAX,-255,255);  
   // if(potencia>245) {potencia=255;} 
      analogWrite(MOTOR_E1,potencia);
      digitalWrite(MOTOR_E2,LOW);
      analogWrite(MOTOR_D1,potencia);
      digitalWrite(MOTOR_D2,LOW); 
      Serial.println(potencia);
    }
     //condição para trás
  else if(((aile >= LIMIAR_MIN_AILE)  && (aile <= LIMIAR_MAX_AILE)) && (ele < LIMIAR_MIN_ELE)){
    potencia = map(ele,MIN,MAX,-255,255);
  //if(potencia>245) {potencia=255;}   
      digitalWrite(MOTOR_E1,LOW);
      analogWrite(MOTOR_E2,potencia);
      digitalWrite(MOTOR_D1,LOW);
      analogWrite(MOTOR_D2,potencia); 
    } 
    //condição para direita
  else if(((ele >= LIMIAR_MIN_ELE)  && (ele <= LIMIAR_MAX_ELE)) && (aile > LIMIAR_MAX_AILE)){
    potencia = map(aile,MIN,MAX,-255,255); 
  // if(potencia>245) {potencia=255;}  
      analogWrite(MOTOR_E1,potencia);
      digitalWrite(MOTOR_E2,LOW);
      digitalWrite(MOTOR_D1,LOW);
      digitalWrite(MOTOR_D2,LOW); 
    }
   //condição para esquerda
  else if(((ele >= LIMIAR_MIN_ELE)  && (ele <= LIMIAR_MAX_ELE)) && (aile > LIMIAR_MAX_AILE)){
    potencia = map(aile,MIN,MAX,-255,255);   
   // if(potencia>245) {potencia=255;}
      digitalWrite(MOTOR_E1,LOW);
      digitalWrite(MOTOR_E2,LOW);
      analogWrite(MOTOR_D1,potencia);
      digitalWrite(MOTOR_D2,LOW); 
    }
    //diagonal frente direita
    else if((ele > LIMIAR_MAX_ELE) && (aile > LIMIAR_MAX_AILE)){
    potencia = map(ele,MIN,MAX,-255,255);  
   // if(potencia>245) {potencia=255;} 
      analogWrite(MOTOR_E1,potencia);
      digitalWrite(MOTOR_E2,LOW);
      analogWrite(MOTOR_D1,potencia/div);
      digitalWrite(MOTOR_D2,LOW); 
    }
   //diagonal frente esquerda
    else if((ele > LIMIAR_MAX_ELE) && (aile < LIMIAR_MIN_AILE)){
    potencia = map(ele,MIN,MAX,-255,255);
   //if(potencia>245) {potencia=255;}   
      analogWrite(MOTOR_E1,potencia/div);
      digitalWrite(MOTOR_E2,LOW);
      analogWrite(MOTOR_D1,potencia);
      digitalWrite(MOTOR_D2,LOW); 
    }
   //diagonal tras direita
    else if((ele < LIMIAR_MIN_ELE) && (aile > LIMIAR_MAX_AILE)){
    potencia = map(ele,MIN,MAX,-255,255);  
    //if(potencia>245) {potencia=255;} 
      digitalWrite(MOTOR_E1,LOW);
      analogWrite(MOTOR_E2,potencia);
      digitalWrite(MOTOR_D1,LOW);
      analogWrite(MOTOR_D2,potencia/div); 
    }
   //diagonal tras esquerda
    else if((ele < LIMIAR_MIN_ELE) && (aile < LIMIAR_MIN_AILE)){
    potencia = map(ele,MIN,MAX,-255,255);
    //if(potencia>245) {potencia=255;}   
      digitalWrite(MOTOR_E1,LOW);
      analogWrite(MOTOR_E2,potencia/div);
      digitalWrite(MOTOR_D1,LOW);
      analogWrite(MOTOR_D2,potencia); 
    }
}
