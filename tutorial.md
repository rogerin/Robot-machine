# 🚀 Controle de Servos via Serial no ESP32 - Tutorial Completo

Este tutorial irá guiá-lo **passo a passo** na criação de um **sistema de controle de servos** utilizando um **ESP32**, **Web Serial API** e uma interface **Web interativa**.

## 🎯 **Objetivo do Projeto**
✅ Criar um **sistema de controle de até 4 servos**, permitindo:
✅ **Controle de ângulo via navegador (Web Serial API)**  
✅ **Seleção dinâmica dos pinos do ESP32**  
✅ **Criação de sequências de movimentos e loops automáticos**  

---

## 📌 **1. O que Você Vai Precisar?**
### 🔹 **1.1 Hardware Necessário**
- 1x **ESP32 (DevKit V1, WROOM ou similar)**
- 1 a 4x **Servo motores** (SG90 ou MG995)
- 1x **Cabo USB** para conectar o ESP32 ao computador
- **Fios jumpers** para conexões

### 🔹 **1.2 Software Necessário**
- **Arduino IDE** ([Download Aqui](https://www.arduino.cc/en/software))
- **Drivers do ESP32** (Para Windows e Mac)  
  - [Baixar CP2102](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers)
  - [Baixar CH340](https://www.wch.cn/download/CH341SER_EXE.html)

---

## 📌 **2. Instalando o Ambiente**
### 🔹 **2.1 Instalando o Arduino IDE**
1. Acesse [https://www.arduino.cc/en/software](https://www.arduino.cc/en/software)
2. Baixe e instale o **Arduino IDE** para o seu sistema operacional.
3. Após a instalação, **abra o Arduino IDE**.

### 🔹 **2.2 Instalando o ESP32 no Arduino IDE**
1. Vá em **Arquivo** → **Preferências**.
2. No campo **URLs Adicionais para Gerenciadores de Placas**, adicione:

https://dl.espressif.com/dl/package_esp32_index.json

3. Clique em **OK** e feche essa janela.
4. Vá em **Ferramentas** → **Placa** → **Gerenciador de Placas**.
5. No campo de busca, digite **ESP32**.
6. Selecione **"ESP32 by Espressif Systems"** e clique em **Instalar**.

### 🔹 **2.3 Instalando a Biblioteca ESP32Servo**
1. No **Arduino IDE**, vá em **Ferramentas** → **Gerenciador de Bibliotecas**.
2. No campo de busca, digite **ESP32Servo**.
3. Instale a biblioteca **ESP32Servo by Kevin Harrington**.

---

## 📌 **3. Ligando o Servo Motor ao ESP32**
Os servos possuem **3 fios**:
- **Vermelho** → **3.3V ou 5V** do ESP32
- **Preto/Marrom** → **GND** do ESP32
- **Amarelo/Branco** → **Pino de sinal** (ex: GPIO13)

### 📌 **Diagrama de Ligação**

Servo SG90
-----------
|   ●   |  (Amarelo - Sinal) → GPIO13 (exemplo)
|   ●   |  (Vermelho - VCC) → 5V
|   ●   |  (Preto - GND) → GND
-----------

---

## 📌 **4. Código Básico para Controlar 1 Servo**
Agora vamos carregar um **código simples** para testar **um único servo**:

```cpp
#include <ESP32Servo.h>

Servo servo;  // Criar um objeto servo

void setup() {
    Serial.begin(115200);
    servo.attach(13);  // Conectar ao GPIO 13
}

void loop() {
    servo.write(0);   // Posição 0°
    delay(1000);
    servo.write(90);  // Posição 90°
    delay(1000);
    servo.write(180); // Posição 180°
    delay(1000);
}
```
Tste para ver se a biblioteca está funcionando.

🔹 4.1 Como Enviar o Código para o ESP32
1.	Conecte o ESP32 via USB ao computador.
2.	No Arduino IDE, vá em Ferramentas e selecione:
•	Placa: ESP32 Dev Module
•	Porta: (A porta USB que apareceu)
3.	Clique no botão Upload e aguarde o código ser enviado.

📌 Se tudo estiver certo, o servo deve se mover automaticamente!

⸻

📌 5. Código para Múltiplos Servos (4 Servos)

Agora vamos expandir para 4 servos controlados via Serial:
```cpp
#include <ESP32Servo.h>

Servo servo1, servo2, servo3, servo4;
Servo* servos[4] = {&servo1, &servo2, &servo3, &servo4};
int pinosServos[4] = {13, 15, 27, 32}; 

void setup() {
    Serial.begin(115200);
    for (int i = 0; i < 4; i++) {
        servos[i]->attach(pinosServos[i]);
    }
}

void loop() {
    if (Serial.available()) {
        int pino, anguloDesejado, velocidade;
        pino = Serial.parseInt();
        anguloDesejado = Serial.parseInt();
        velocidade = Serial.parseInt();
        Serial.read(); 

        for (int i = 0; i < 4; i++) {
            if (pinosServos[i] == pino) {
                moverServo(i, anguloDesejado, velocidade);
            }
        }
    }
}

void moverServo(int index, int anguloFinal, int velocidade) {
    for (int pos = 0; pos <= anguloFinal; pos++) {
        servos[index]->write(pos);
        delay(velocidade);
    }
}
```


⸻

📌 6. Criando a Interface Web

Agora, criaremos uma página HTML para enviar comandos via Serial.

🔹 6.1 Salvando o Arquivo HTML

Crie um arquivo index.html e cole o código abaixo:

```html
<!DOCTYPE html>
<html lang="pt">
<head>
    <meta charset="UTF-8">
    <title>Controle de Servos</title>
</head>
<body>
    <h2>Controle de Servos via Serial</h2>
    <button id="connectButton">Conectar ao ESP32</button>
    <input type="range" id="angulo" min="0" max="180">
    <button id="enviar">Enviar</button>

    <script>
        let port, writer;
        document.getElementById("connectButton").addEventListener("click", async () => {
            port = await navigator.serial.requestPort();
            await port.open({ baudRate: 115200 });
            writer = port.writable.getWriter();
        });

        document.getElementById("enviar").addEventListener("click", async () => {
            let angulo = document.getElementById("angulo").value;
            await writer.write(new TextEncoder().encode(`13 ${angulo} 10\n`));
        });
    </script>
</body>
</html>
```


⸻

📌 7. Conclusão

Agora você pode controlar servos pelo navegador, criar loops automáticos e sequências de posições!
Caso tenha dúvidas, experimente cada etapa separadamente e me avise se precisar de mais explicações! 🚀😃

---

Se precisar de mais **explicações, exemplos ou melhorias**, me avise! @rogerioalencarfilho no insta 🚀😃