/* beeg.ino
 * Código para controle de robô beetleweight
 * Autores: Gabriel Alves e Isabella Galvão
 * última alteração: 02/12/2015 - Adição de comentários
 */

//Inclui biblioteca para acesso a funções matemáticas avançadas
#include <math.h>

//Define os pinos que os motores estarão conectados
#define MOTOR_E1 5
#define MOTOR_E2 6
#define MOTOR_D1 7
#define MOTOR_D2 8

//Define as entradas de sinal do controle
#define AILE A6
#define ELE A5

//Constantes para leitura do controle (Devem ser calibradas)
//Intensidade máxima do sinal permitida como velocidade máxima
#define MAX 1800
#define MIN 1200
#define TOLERANCIA 30
#define VELOCIDADE_MAXIMA 255

//Vão guardar as leituras vindas do controle
int aile = 0;
int ele = 0;

//Define as entradas e saídas e inicia uma comunicação serial
void setup() {
	pinMode(AILE, INPUT);
	pinMode(ELE, INPUT);

	pinMode(MOTOR_E1, OUTPUT);
	pinMode(MOTOR_E2, OUTPUT);
	pinMode(MOTOR_D1, OUTPUT);
	pinMode(MOTOR_D2, OUTPUT);
	Serial.begin(9600);
}

//Execução do código
void loop() {
	//Lê do controle
	aile = pulseIn(AILE, HIGH);
	ele = pulseIn(ELE, HIGH);

	//Verifica se o controle está parado no centro
	if(estaNoCentro(TOLERANCIA)) {
		//Neste caso, o robô deve ficar parado
		aile = calculaPwmExponencial(0);
		ele = calculaPwmExponencial(0);
	}
	else {
		//Neste caso, o robô deve calcular o pwm que ele deve enviar para os atuadores
		//Existem funções diferentes que calculam o pwm para diversos fins
		aile = calculaPwmExponencial(aile);
		ele = calculaPwmExponencial(ele);
	}

	//Liga os motores com as proporções definidas.
	//O parâmetro booleano, quando verdadeiro, imprime o valor no fluxo serial
	//Quando falso, emite o sinal para o motor
	motorEsquerdo(aile + ele, true);
	motorDireito(aile - ele, true);
}

//Função que verifica se o controle está na posição central com uma certa tolerancia a erros
bool estaNoCentro(int tolerancia) {
	int center = (MAX + MIN)/2;
	return (aile > (center - tolerancia)) && (aile < (center + tolerancia))
		&& (ele > (center - tolerancia)) && (ele < (center + tolerancia));
}

//Mapeia o valor recebido para [-255,255]
int mapPwm(int value) {
	return map(value, MIN, MAX, -255, 255);
}

//Calcula o pwm numa base exponencial
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

//Calcula o pwm linearmente
int calculaPwmLinear(int value) {
	return mapPwm(value);
}

//Limita a potência em limite igual a VELOCIDADE_MAXIMA
int limitadorDePotencia(int potencia) {
	if(abs(potencia) > VELOCIDADE_MAXIMA) {
		return (potencia/abs(potencia)) * VELOCIDADE_MAXIMA;
	}
	else {
		return potencia;
	}
}

//Controla o motor esquerdo com limite de potência e opção de impressão serial
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

//Controla o motor esquerdo com limite de potência e opção de impressão serial
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
