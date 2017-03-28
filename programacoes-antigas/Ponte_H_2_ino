#include <EEPROM.h>
int X = 13;
int Y = 12;
int CONF = 11;
int LED = 8;
int x, y, conf;
int m1a=9 ,m1b=5 ,m2a=3 ,m2b=6 ,a ,b;
int xma = 0, yma = 0, xz = 0, yz = 0, xmi = 0, ymi = 0;
int m1 = 50, m2 = 100;

void setup()
{
  pinMode(2, OUTPUT);
  pinMode(X, INPUT);
  pinMode(Y, INPUT);
  pinMode(CONF, INPUT);
  pinMode(m1a,OUTPUT);
  pinMode(m2a,OUTPUT);
  pinMode(m1b,OUTPUT);
  pinMode(m2b,OUTPUT);
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  analogWrite(2, 127);
  
  //------------------------------------------------------MODO CALIBRAGEM
  conf = pulseIn(CONF, HIGH);
  
  if(conf > 1700){
    for(int i=0; i<=3; i++){
      digitalWrite(LED, HIGH);
      delay(400);
      digitalWrite(LED, LOW);
      delay(400);
    //  Serial.println("x");
    }
    
    //MEIO
      xz = pulseIn(x, HIGH);
      yz = pulseIn(y, HIGH);
    for(int i=0; i<25; i++){
      x= pulseIn(X, HIGH);  
      y= pulseIn(Y,HIGH);
      xz = (x + xz)/2;
      yz = (y + yz)/2;
    }
    EEPROM.write(1, xz/10);
    EEPROM.write(2, yz/10);
    
    for(int i=0; i<=3; i++){
      digitalWrite(LED, HIGH);
      delay(400);
      digitalWrite(LED, LOW);
      delay(400);
    //  Serial.println("x");
    }
    
    //MAXIMO
      xma = pulseIn(x, HIGH);
      yma = pulseIn(y, HIGH);
    for(int i=0; i<25; i++){
      x= pulseIn(X, HIGH);  
      y= pulseIn(Y,HIGH);
      xma = (x + xma)/2;
      yma = (y + yma)/2;
    }
    EEPROM.write(3, xma/10);
    EEPROM.write(4, yma/10);
    
    for(int i=0; i<=3; i++){
      digitalWrite(LED, HIGH);
      delay(400);
      digitalWrite(LED, LOW);
      delay(400);
    //  Serial.println("x");
    }
    
    //MINIMO
      xmi = pulseIn(x, HIGH);
      ymi = pulseIn(y, HIGH);
    for(int i=0; i<25; i++){
      x= pulseIn(X, HIGH);  
      y= pulseIn(Y,HIGH);
      xmi = (x + xmi)/2;
      ymi = (y + ymi)/2;
    }   
    EEPROM.write(5, xmi/10);
    EEPROM.write(6, ymi/10);
    
  }
  
  xz = EEPROM.read(1);
  xz = xz*10;
  yz = EEPROM.read(2);
  yz = yz*10;
  xma = EEPROM.read(3);
  xma = xma*10;
  yma = EEPROM.read(4);
  yma = yma*10;
  xmi = EEPROM.read(5);
  xmi = xmi*10;
  ymi = EEPROM.read(6);
  ymi = ymi*10;
/*  
  Serial.print("x+: ");
  Serial.print(xma);
  Serial.print("   ");
  Serial.print("   y+: ");
  Serial.println(yma);
  
  Serial.print("x0: ");
  Serial.print(xz);
  Serial.print("   ");
  Serial.print("   y0: ");
  Serial.println(yz);
  

  Serial.print("x-: ");
  Serial.print(xmi);
  Serial.print("   ");
  Serial.print("   y-: ");
  Serial.println(ymi);
*/  
  delay(500);
  
}

void loop()
{
  //leitura dos canais do controle
  x= pulseIn(X, HIGH);  
  y= pulseIn(Y,HIGH);  
  
  //-------------------------------------------------------------DIREÇÕES
  
  //parado
  if(y>yz-m1 && y<yz+m1 && x>xz-m1 && x<xz+m1){
    digitalWrite(m1a,LOW);
    digitalWrite(m1b,LOW);
    digitalWrite(m2a,LOW);
    digitalWrite(m2b,LOW);
//    Serial.println("Parado");
//    delay(10);
  }
  
  //frente
  else if(y>yz+m1 && x>xz-m2 && x<xz+m2){
    a=map(y,yz,yma,0,255);
    if(a>245) {a=255;}
    analogWrite(m1a, a);
    digitalWrite(m2a, LOW);
    digitalWrite(m2b, LOW);
    analogWrite(m1b, a);
//    Serial.print("Frente");
//    Serial.print("   Pwm: ");
//    Serial.println(a);
//    delay(10);
  }

  //tras
  else if(y<yz-m1 && x>xz-m2 && x<xz+m2){
    a=map(y,yz,ymi,0,255);
    if(a>245) {a=255;}
    analogWrite(m2a, a);
    digitalWrite(m1b, LOW);
    digitalWrite(m1a, LOW);
    analogWrite(m2b, a);
//    Serial.print("Tras");
//    Serial.print("   Pwm: ");
//    Serial.println(a);
//    delay(10);
  } 
  
  //direita
  else if(x>xz+m1 && y>yz-m2 && y<yz+m2){
    a=map(x,xz,xma,0,255);
    if(a>245) {a=255;}
    analogWrite(m1b,a);
    digitalWrite(m1a,LOW);
    digitalWrite(m2b,LOW);
    analogWrite(m2a,a);
//    Serial.print("Direita");
//    Serial.print("   Pwm: ");
//    Serial.println(a);
//    delay(10);
  }
 
  
  //esquerda
  else if(x<xz-m1 && y>yz-m2 && y<yz+m2){
    a=map(x,xz,xmi,0,255);
    if(a>245) {a=255;}
    analogWrite(m1a,a);
    digitalWrite(m1b,LOW);
    digitalWrite(m2a,LOW);
    analogWrite(m2b,a);
//    Serial.print("Esquerda");
//    Serial.print("   Pwm: ");
//    Serial.println(a);
//    delay(10);
  }
   
  
//diagonal direita frente
  else if(x>xz+m2 && y>xz+m2){
    a=map(y,yz,yma,0,255);
    if(a>245) {a=255;}
    digitalWrite(m1a,LOW);
    analogWrite(m1b,a);
    digitalWrite(m2b,LOW);
    digitalWrite(m2a,LOW);
//    Serial.print("Diagonal Direita Frente");
//    Serial.print("   Pwm: ");
//    Serial.println(a);
//    delay(10); 
  }
 
//diagonal esquerda frente
  else if(x<xz-m2 && y>yz+m2){
    a=map(y,yz,yma,0,255);
    if(a>245) {a=255;}
    analogWrite(m1a,a);
    digitalWrite(m1b,LOW);
    digitalWrite(m2a,LOW);
    digitalWrite(m2b,LOW);
//    Serial.print("Diagonal Esquerda Frente");
//    Serial.print("   Pwm: ");
//    Serial.println(a);
//    delay(10);
  }
 
  
//diagonal direita tras
  else if(x>xz+m2 && y<yz-m2){
    a=map(y,yz,ymi,0,255);
    if(a>245) {a=255;}
    digitalWrite(m1a,LOW);
    digitalWrite(m1b,LOW);
    digitalWrite(m2a,LOW);
    analogWrite(m2b,a);
//    Serial.print("Diagonal Direita Tras");
//    Serial.print("   Pwm: ");
//    Serial.println(a);
//    delay(10);
  }
 
  
//diagonal esquerda tras
  else if(x<xz-m2 && y<yz-m2){
    a=map(y,yz,ymi,0,255);
    if(a>245) {a=255;}
    digitalWrite(m1a,LOW);
    digitalWrite(m1b,LOW);
    analogWrite(m2a,a);
    digitalWrite(m2b,LOW);
//    Serial.print("Diagonal Esquerda Tras");
//    Serial.print("   Pwm: ");
//    Serial.println(a);
//    delay(10);
  }

}


