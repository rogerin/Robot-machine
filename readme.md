📌 Controle de Servos via Serial no ESP32

# Controle de Servos via Serial (ESP32 + Web Serial API)

Este projeto permite controlar **até 4 servos** conectados a um **ESP32** usando uma **interface web moderna**.  
A comunicação entre o navegador e o ESP32 é feita via **Web Serial API**, eliminando a necessidade de softwares adicionais.  

---

## 📌 **Funcionalidades**
✅ Controle de **até 4 servos** simultaneamente  
✅ Seleção **dinâmica dos pinos** para cada servo  
✅ Controle de ângulo via **sliders interativos**  
✅ **Gerenciador de posições** para criar sequências  
✅ **Execução automática de loops** com múltiplas posições  
✅ Interface web moderna, responsiva e fácil de usar  

---

## 📌 **Demonstração**
> ![Demonstração](./img/demo.gif)

---

## 📌 **Tecnologias Utilizadas**
- **ESP32** para controle dos servos
- **Arduino IDE** para programar o ESP32
- **Web Serial API** para comunicação direta via navegador
- **HTML, CSS e JavaScript** para a interface gráfica

---

## 📌 **1. Como Configurar o ESP32**
### 🔹 **Instalar a Biblioteca ESP32Servo**
Antes de rodar o código no ESP32, instale a biblioteca **ESP32Servo** no Arduino IDE:
1. Abra o **Arduino IDE**.
2. Vá para **Ferramentas** → **Gerenciador de Bibliotecas**.
3. No campo de busca, digite **ESP32Servo**.
4. Encontre **ESP32Servo by Kevin Harrington** e clique em **Instalar**.

### 🔹 **Carregar o Código no ESP32**
Copie e carregue este código no ESP32 usando o **Arduino IDE**:

```cpp
#include <ESP32Servo.h>

ESP32Servo servo1, servo2, servo3, servo4;
Servo* servos[4] = {&servo1, &servo2, &servo3, &servo4};
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
```


⸻

## 📌 2. Configurar a Interface Web

🔹 Salvar o Arquivo HTML

Crie um arquivo chamado index.html e cole o seguinte código:
```html
<!DOCTYPE html>
<html lang="pt">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Controle de Servos via Serial</title>
</head>
<body>
    <h2>Controle de 4 Servos via Serial</h2>

    <button id="connectButton">Conectar ao ESP32</button>
    
    <div id="config"></div>
    <button id="startControl" disabled>Iniciar Controle</button>
    
    <div id="servos"></div>

    <script>
        let port, writer, servoPinos = [];

        document.getElementById("connectButton").addEventListener("click", async () => {
            try {
                port = await navigator.serial.requestPort();
                await port.open({ baudRate: 115200 });
                writer = port.writable.getWriter();
                alert("Conectado ao ESP32!");
                criarConfiguracao();
            } catch (err) {
                alert("Erro ao conectar: " + err.message);
            }
        });

        function criarConfiguracao() {
            const configContainer = document.getElementById("config");
            configContainer.innerHTML = `<h3>Configurar pinos dos servos</h3>`;
            for (let i = 0; i < 4; i++) {
                let div = document.createElement("div");
                div.innerHTML = `
                    <label>Servo ${i + 1} - Escolha o pino:</label>
                    <input type="number" min="0" max="39" id="servoPino${i}" placeholder="Ex: 13">
                `;
                configContainer.appendChild(div);
            }
            document.getElementById("startControl").disabled = false;
        }

        document.getElementById("startControl").addEventListener("click", () => {
            servoPinos = [];
            for (let i = 0; i < 4; i++) {
                let pino = document.getElementById(`servoPino${i}`).value;
                if (pino === "" || isNaN(pino) || pino < 0 || pino > 39) {
                    alert(`Pino inválido para Servo ${i + 1}. Escolha um entre 0 e 39.`);
                    return;
                }
                servoPinos.push(parseInt(pino));
            }
            criarSliders();
        });
    </script>
</body>
</html>
```
⸻

## 📌 3. Como Usar
1.	Carregue o código no ESP32 via Arduino IDE.
2.	Abra o arquivo index.html no Google Chrome.
3.	Clique no botão “Conectar ao ESP32” e selecione a porta serial do ESP32.
4.	Escolha os pinos dos servos e clique em “Iniciar Controle”.
5.	Use os sliders para controlar os servos em tempo real.

⸻

### 📌 4. Conclusão

Este projeto simplifica o controle de servos usando um ESP32 e uma interface web. Ele elimina a necessidade de softwares adicionais, permitindo um controle direto via navegador.

Agora você pode escolher os pinos dinamicamente, armazenar posições e criar loops para movimentação automatizada! 🚀🔥

Caso tenha dúvidas ou melhorias, sinta-se à vontade para contribuir! 😃