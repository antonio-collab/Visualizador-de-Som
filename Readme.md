
# 🎤📊 Visualizador de Som com Efeito em LEDs WS2812 – Raspberry Pi Pico

Este projeto utiliza um **microfone analógico** conectado ao **Raspberry Pi Pico** para captar níveis de som ambiente. Quando o som ultrapassa um certo limiar, os **LEDs WS2812** (NeoPixels) acendem em um efeito de **cascata visual** que representa a presença de som.

---

## 🧠 Como funciona o código

### 🎯 Objetivo:

Detectar sons via um microfone conectado ao GPIO 28 (ADC2) e acionar um efeito visual nos LEDs com base na intensidade detectada.

### 🧩 Componentes principais:

* **Microfone analógico (ex: KY-038, MAX9814, etc.)**
* **Fita de LEDs WS2812 (NeoPixel)** com 25 LEDs
* **Raspberry Pi Pico**
* **Conexões**:

  * Microfone analógico → GPIO 28 (ADC2)
  * LEDs WS2812 → GPIO 7
  * Fonte 5V para LEDs (com GND em comum com o Pico)

### 🛠️ Lógica do código:

1. **Inicialização**:

   * Inicializa o ADC no GPIO 28 para leitura do microfone.
   * Inicializa o PIO para controlar os LEDs via o protocolo WS2812.

2. **Loop principal**:

   * Lê 20 amostras analógicas do microfone.
   * Calcula o **valor RMS** (Root Mean Square) do sinal para medir a intensidade do som.
   * Se o som ultrapassar o `SOUND_THRESHOLD`, executa um **efeito de cascata visual** nos LEDs.
   * Se não houver som, os LEDs permanecem apagados (`npClear()`).

3. **Efeito visual**:

   * Os LEDs se acendem um por um em **verde**, simulando uma cascata de luz.
   * Depois, se apagam no sentido inverso.

---

## 🧪 Como testar

### ✅ Pré-requisitos:

1. **Ambiente de desenvolvimento C/C++ para Raspberry Pi Pico**:

   * Instale o SDK da Raspberry Pi e configure seu `CMake`.
   * Utilize o [pico-sdk](https://github.com/raspberrypi/pico-sdk) e o [pioasm](https://github.com/raspberrypi/pico-sdk/tree/master/tools/pioasm) para compilar o arquivo `ws2818b.pio`.

2. **Arquivo adicional necessário**:

   * `ws2818b.pio.h`: Arquivo gerado do código PIO que controla os LEDs WS2812. Certifique-se de que ele esteja incluído no seu projeto.

3. **Conexões físicas**:

   * GPIO 28 (ADC2) → saída analógica do microfone
   * GPIO 7 → entrada de dados da fita de LEDs WS2812
   * Fonte 5V para LEDs (com GND comum com o Pico)

### ▶️ Passos para rodar:

1. Compile e carregue o código no seu Raspberry Pi Pico.
2. Abra um terminal serial (ex: PuTTY, minicom ou Thonny) com a saída da USB do Pico.
3. Faça um som (como bater palmas ou assoviar) próximo ao microfone.
4. Observe os LEDs:

   * Se o som for detectado, os LEDs acendem em efeito cascata.
   * Se estiver em silêncio, os LEDs permanecem apagados.

---

## ⚙️ Ajustes que você pode fazer

* **Sensibilidade**:

  * Modifique `SOUND_THRESHOLD` para ajustar a sensibilidade ao som.

* **Velocidade da animação**:

  * Edite os `sleep_ms(100)` no `efeito_cascata()` para acelerar ou desacelerar o efeito.

* **Número de LEDs**:

  * Altere o valor de `LED_COUNT` conforme a quantidade real de LEDs em sua fita.

---

