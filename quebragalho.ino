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

//Define as entradas de sinal do controle
#define AILE A0
#define ELE A1

//Constantes para leitura do controle (Devem ser calibradas)
#define MAX 1900
#define MIN 1200
//Intensidade máxima do sinal permitida como velocidade máxima
#define VELOCIDADE_MAXIMA 250
#define VELOCIDADE_MINIMA 10    //Mudar de acordo com o robô
#define MULT  0.5                  //divisor de potência para diagonal,valores de teste , amanhã mudamos

//Vão guardar as leituras vindas do controle
int aile = 0;
int ele = 0;
int ele_potencia = 0;
int aile_potencia = 0;

//limiar aile ele
#define LIMIAR_MAX_AILE 40
#define LIMIAR_MIN_AILE -40
#define LIMIAR_MAX_ELE  40 
#define LIMIAR_MIN_ELE  -40 

void setup() {
  // put your setup code here, to run once:
  pinMode(AILE, INPUT);
  pinMode(ELE, INPUT);

  pinMode(MOTOR_E1, OUTPUT);
  pinMode(MOTOR_E2, OUTPUT);
  pinMode(MOTOR_D1, OUTPUT);
  pinMode(MOTOR_D2, OUTPUT);
  Serial.begin(9600);
  Serial.flush();
}

void loop() {
  
  //leitura dos canais do controle
  aile = pulseIn(AILE, HIGH);  
  ele = pulseIn(ELE,HIGH);
  
  if (DEBUG){
  Serial.print("AILE : ");
  Serial.println(aile);
  Serial.print("ELE : ");
  Serial.println(ele);
  delay(500);
  }
 
  //CONDIÇÃO PARADO PARA CONTROLE DESLIGADO
  if((aile==0) &&(ele==0)){
    
      digitalWrite(MOTOR_E1,LOW);
      digitalWrite(MOTOR_E2,LOW);
      digitalWrite(MOTOR_D1,LOW);
      digitalWrite(MOTOR_D2,LOW);
      
      if(DEBUG){
      Serial.println("TRAVA");
      delay(500);
      }
  }
      
  else if(aile != 0 && ele !=0){
    
      ele_potencia = potenciaPwmEle(ele);
      aile_potencia = potenciaPwmAile(aile);
   
      //LER OS SINAIS MAPEADOS
      if (DEBUG){
         Serial.print(" Ele mapeado = ");
         Serial.println(ele_potencia);
         Serial.print(" Aile mapeado = ");
         Serial.println(aile_potencia);
         delay(500); 
         } 
 
    if(((aile_potencia >= LIMIAR_MIN_AILE)  && (aile_potencia <= LIMIAR_MAX_AILE))  && ((ele_potencia >= LIMIAR_MIN_ELE) && (ele_potencia <= LIMIAR_MAX_ELE))){

      digitalWrite(MOTOR_E1,LOW);
      digitalWrite(MOTOR_E2,LOW);
      digitalWrite(MOTOR_D1,LOW);
      digitalWrite(MOTOR_D2,LOW);
      
      if(DEBUG){
      Serial.println("PARADO");
      delay(500);
      }
  }
  //condição para frente
  else if(((aile_potencia >= LIMIAR_MIN_AILE)  && (aile_potencia <= LIMIAR_MAX_AILE)) && (ele_potencia > LIMIAR_MAX_ELE)){
  
      analogWrite(MOTOR_E1,ele_potencia);
      digitalWrite(MOTOR_E2,LOW);
      analogWrite(MOTOR_D1,ele_potencia); 
      digitalWrite(MOTOR_D2,LOW);
      
      if (DEBUG){
      Serial.print("potencia: "); 
      Serial.println(ele_potencia);
      Serial.println("FRENTE");
      delay(500); 
      }
      
    }
     //condição para trás
  else if(((aile_potencia >= LIMIAR_MIN_AILE)  && (aile_potencia <= LIMIAR_MAX_AILE)) && (ele_potencia < LIMIAR_MIN_ELE)){
     
      digitalWrite(MOTOR_E1,LOW);
      analogWrite(MOTOR_E2,ele_potencia);
      digitalWrite(MOTOR_D1,LOW);
      analogWrite(MOTOR_D2,ele_potencia); 
      
      if(DEBUG){
      Serial.print("potencia: ");
      Serial.println(ele_potencia);
      Serial.println("TRAS");
      delay(500);
      }
      
    } 
    //condição para direita
  else if(((ele_potencia >= LIMIAR_MIN_ELE)  && (ele_potencia <= LIMIAR_MAX_ELE)) && (aile_potencia > LIMIAR_MAX_AILE)){

      analogWrite(MOTOR_E1,aile_potencia);
      digitalWrite(MOTOR_E2,LOW);
      digitalWrite(MOTOR_D1,LOW);
      digitalWrite(MOTOR_D2,aile_potencia);
      
      if(DEBUG){
      Serial.print("potencia: ");
      Serial.println(aile_potencia);
      Serial.println("DIREITA");
      delay(500);
    }
  }
   //condição para esquerda
  else if(((ele_potencia >= LIMIAR_MIN_ELE)  && (ele_potencia <= LIMIAR_MAX_ELE)) && (aile_potencia < LIMIAR_MIN_AILE)){  

      digitalWrite(MOTOR_E1,LOW);
      digitalWrite(MOTOR_E2,aile_potencia);
      analogWrite(MOTOR_D1,aile_potencia);
      digitalWrite(MOTOR_D2,LOW);
   
      if(DEBUG){
      Serial.print("potencia: ");
      Serial.println(aile_potencia);
      Serial.println("ESQUERDA");
      delay(500); 
      }
      
    }
    //diagonal frente direita
    else if((ele_potencia > LIMIAR_MAX_ELE) && (aile_potencia > LIMIAR_MAX_AILE)){

      analogWrite(MOTOR_E1,aile_potencia*MULT);
      digitalWrite(MOTOR_E2,LOW);
      analogWrite(MOTOR_D1,aile_potencia);
      digitalWrite(MOTOR_D2,LOW);
     
     if(DEBUG){ 
     Serial.print("potencia: ");
     Serial.println(ele_potencia);
     Serial.println("DIAGONAL FRENTE DIREITA");
     delay(500);
     }
   
    }
   //diagonal frente esquerda
    else if((ele_potencia > LIMIAR_MAX_ELE) && (aile_potencia < LIMIAR_MIN_AILE)){

      analogWrite(MOTOR_E1,aile_potencia*MULT);
      digitalWrite(MOTOR_E2,LOW);
      analogWrite(MOTOR_D1,aile_potencia);
      digitalWrite(MOTOR_D2,LOW);
 
      if(DEBUG){
      Serial.print("potencia: ");
      Serial.println(ele_potencia);
      Serial.println("DIAGONAL FRENTE ESQUERDA");
      delay(500);
      }
    }
    
   //diagonal tras direita
    else if((ele_potencia < LIMIAR_MIN_ELE) && (aile_potencia > LIMIAR_MAX_AILE)){ 
      
      digitalWrite(MOTOR_E1,LOW);
      analogWrite(MOTOR_E2,aile_potencia*MULT);
      digitalWrite(MOTOR_D1,LOW);
      analogWrite(MOTOR_D2,aile_potencia); 
      if(DEBUG){
      Serial.print("potencia: ");
      Serial.println(ele_potencia);
      Serial.println("DIAGONAL TRAS DIREITA");
      delay(500);
      }
    }
   //diagonal tras esquerda
    else if((ele_potencia < LIMIAR_MIN_ELE) && (aile_potencia < LIMIAR_MIN_AILE)){
   
      if(ele <= -10 || ele >= 10){
      digitalWrite(MOTOR_E1,LOW);
      analogWrite(MOTOR_E2,250);
      digitalWrite(MOTOR_D1,LOW);
      analogWrite(MOTOR_D2,250*MULT);
      }
      else{
      digitalWrite(MOTOR_E1,LOW);
      analogWrite(MOTOR_E2,ele_potencia);
      digitalWrite(MOTOR_D1,LOW);
      analogWrite(MOTOR_D2,ele_potencia*MULT);
      }
      if(DEBUG){
     Serial.print("potencia: ");
      Serial.println(ele_potencia);
      Serial.println("DIAGONAL TRAS ESQUERDA");
      delay(500); 
      }
    }
  }
}


//Função que mapeia os valores de aile e limita a potência
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
