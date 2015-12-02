//TesteBeeg.ino
//Dia 9/12 2ºEE LPF
//Trabalho LPF: 2 questões:
//a. Implementação de problema em Scala
//b. Implementação de uma página HTML dinâmica em JScala
//Entrega: 10/12

#include <math.h>

#define MOTOR_E1 5
#define MOTOR_E2 6
#define MOTOR_D1 7
#define MOTOR_D2 8

#define AILE A6
#define ELE A5

#define MAX 1800
#define MIN 1200
#define TOLERANCIA 30
#define VELOCIDADE_MAXIMA 255

int aile = 0;
int ele = 0;

void setup() {
	pinMode(AILE, INPUT);
	pinMode(ELE, INPUT);

	pinMode(MOTOR_E1, OUTPUT);
	pinMode(MOTOR_E2, OUTPUT);
	pinMode(MOTOR_D1, OUTPUT);
	pinMode(MOTOR_D2, OUTPUT);
	Serial.begin(9600);
}

void loop() {
	aile = pulseIn(AILE, HIGH);
	ele = pulseIn(ELE, HIGH);

	if(estaNoCentro(TOLERANCIA)) {
		aile = calculaPwmExponencial(0);
		ele = calculaPwmExponencial(0);
	}
	else {
		aile = calculaPwmExponencial(aile);
		ele = calculaPwmExponencial(ele);
	}

	motorEsquerdo(aile + ele, true);
	motorDireito(aile - ele, true);
}

bool estaNoCentro(int tolerancia) {
	int center = (MAX + MIN)/2;
	return (aile > (center - tolerancia)) && (aile < (center + tolerancia))
		&& (ele > (center - tolerancia)) && (ele < (center + tolerancia));
}

int mapPwm(int value) {
	return map(value, MIN, MAX, -255, 255);
}

int calculaPwmExponencial(int value) {
	int x = 0;
	int k = 0;

	if(value > 0) {
		x = mapPwm(value);
		k = (8 * M_LN2)/255;
		return exp(k * x) - 1;
	}
	else if(value < 0) {
		x = mapPwm(value);
		k = ((-1) * (8 * M_LN2))/255;
		return 1 - exp(k * x);
	}
	else {
		return 0;
	}
}

int calculaPwmLinear(int value) {
	return mapPwm(value);
}

int limitadorDePotencia(int potencia) {
	if(abs(potencia) > VELOCIDADE_MAXIMA) {
		return (potencia/abs(potencia)) * VELOCIDADE_MAXIMA;
	}
	else {
		return potencia;
	}
}

void motorEsquerdo(int pwm, bool serial) {
	int potencia = limitadorDePotencia(pwm);

	if(serial) {
		Serial.print("Esquerdo: ");
		Serial.println(potencia);
	}
	else {
		if(potencia > 0) {
			analogWrite(MOTOR_E1, potencia);
			digitalWrite(MOTOR_E2, LOW);
		}
		else {
			analogWrite(MOTOR_E2, potencia);
			digitalWrite(MOTOR_E1, LOW);
		}
	}
}

void motorDireito(int pwm, bool serial) {
	int potencia = limitadorDePotencia(pwm);

	if(serial) {
		Serial.print("Direito: ");
		Serial.println(potencia);
	}
	else {
		if(potencia > 0) {
			analogWrite(MOTOR_D1, potencia);
			digitalWrite(MOTOR_D2, LOW);
		}
		else {
			analogWrite(MOTOR_D2, potencia);
			digitalWrite(MOTOR_D1, LOW);
		}
	}
}
