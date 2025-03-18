#include <ESP32Servo.h>  

Servo servo1, servo2, servo3, servo4;  // Declaração correta dos servos
Servo* servos[4] = {&servo1, &servo2, &servo3, &servo4};  // Array de ponteiros

int pinosServos[4] = {-1, -1, -1, -1}; 
int angulosAtuais[4] = {90, 90, 90, 90};  

void setup() {
    Serial.begin(115200);
}

void loop() {
    if (Serial.available()) {
        int pino, anguloDesejado, velocidade;

        pino = Serial.parseInt();
        anguloDesejado = Serial.parseInt();
        velocidade = Serial.parseInt();
        Serial.read();  

        if (anguloDesejado < 0 || anguloDesejado > 180 || velocidade < 1 || velocidade > 50) return;

        int indexServo = getServoIndex(pino);
        if (indexServo == -1) return;

        Serial.print("Movendo Servo no pino ");
        Serial.print(pino);
        Serial.print(" para ");
        Serial.print(anguloDesejado);
        Serial.print("° com velocidade ");
        Serial.println(velocidade);

        moverServo(indexServo, anguloDesejado, velocidade);
    }
}

int getServoIndex(int pino) {
    for (int i = 0; i < 4; i++) {
        if (pinosServos[i] == pino) return i;
    }
    for (int i = 0; i < 4; i++) {
        if (pinosServos[i] == -1) {
            pinosServos[i] = pino;
            servos[i]->attach(pino);
            return i;
        }
    }
    return -1;
}

void moverServo(int index, int anguloFinal, int velocidade) {
    if (anguloFinal > angulosAtuais[index]) {
        for (int pos = angulosAtuais[index]; pos <= anguloFinal; pos++) {
            servos[index]->write(pos);
            delay(velocidade);
        }
    } else {
        for (int pos = angulosAtuais[index]; pos >= anguloFinal; pos--) {
            servos[index]->write(pos);
            delay(velocidade);
        }
    }
    angulosAtuais[index] = anguloFinal;
}