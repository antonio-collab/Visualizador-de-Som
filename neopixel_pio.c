#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "pico/time.h"
#include "ws2818b.pio.h"
#include <stdlib.h>

#define LED_COUNT 25
#define LED_PIN 7

// Definições do microfone e ADC
const uint MIC_PIN = 28;            // Pino do microfone (GPIO 28)
const float SOUND_OFFSET = 1.65f;   // Nível médio de tensão sem som
const float SOUND_THRESHOLD = 0.25f; // Limiar para detectar som
const float ADC_REF = 3.3f;         // Tensão de referência do ADC
const int ADC_RES = 4095;           // Resolução do ADC (12 bits)
const int AMOSTRA = 20;             // Número de amostras para o cálculo do RMS

typedef struct {
    uint8_t G, R, B;
} pixel_t;

pixel_t leds[LED_COUNT];
PIO np_pio;
uint sm;

// Função para inicializar o PIO e LEDs
void npInit(uint pin) {
    uint offset = pio_add_program(pio0, &ws2818b_program);
    np_pio = pio0;
    sm = pio_claim_unused_sm(np_pio, true);
    ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);

    for (uint i = 0; i < LED_COUNT; ++i)
        leds[i] = (pixel_t){0, 0, 0};  // Inicializa LEDs apagados
}

// Função para configurar a cor de um LED específico
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b) {
    if (index < LED_COUNT)
        leds[index] = (pixel_t){g, r, b};
}

// Função para atualizar os LEDs
void npWrite() {
    for (uint i = 0; i < LED_COUNT; ++i) {
        pio_sm_put_blocking(np_pio, sm, leds[i].G);
        pio_sm_put_blocking(np_pio, sm, leds[i].R);
        pio_sm_put_blocking(np_pio, sm, leds[i].B);
    }
    sleep_us(100); // Reset time
}

// Função para apagar todos os LEDs
void npClear() {
    for (uint i = 0; i < LED_COUNT; ++i)
        npSetLED(i, 0, 0, 0);
    npWrite();
}

// Função que cria o efeito de cascata
void efeito_cascata() {
    // Acende os LEDs um por um para simular uma cascata
    for (int i = 0; i < LED_COUNT; ++i) {
        npSetLED(i, 0, 200, 0); // Acende o LED em verde
        npWrite();
        sleep_ms(100); // Aguarda para o efeito
    }

    // Apaga os LEDs um por um para simular o efeito de cascata ao contrário
    for (int i = LED_COUNT - 1; i >= 0; --i) {
        npSetLED(i, 0, 0, 0); // Apaga o LED
        npWrite();
        sleep_ms(100); 
    }
}

// Função que calcula o índice do LED em uma matriz 
int getIndex(int x, int y) {
    if (y % 2 == 0)
        return 24 - (y * 5 + x);
    else
        return 24 - (y * 5 + (4 - x));
}

// Função para calcular o valor RMS das amostras de áudio
float calcularRMS() {
    float sum = 0.0f;
    
    // Coleta de AMOSTRA amostras do ADC
    for (int i = 0; i < AMOSTRA; ++i) {
        uint16_t raw_adc = adc_read(); // Leitura do ADC
        float voltage = (raw_adc * ADC_REF) / ADC_RES;
        float sound_level = voltage > SOUND_OFFSET ? (voltage - SOUND_OFFSET) : (SOUND_OFFSET - voltage);
        sum += sound_level * sound_level;  // Soma dos quadrados dos níveis de som
    }

    // Calcula o RMS
    float rms_value = sqrtf(sum / AMOSTRA);
    return rms_value;
}

int main() {
    stdio_init_all();

   
    adc_init();
    adc_gpio_init(MIC_PIN);
    adc_select_input(2); // ADC2 corresponde ao GPIO 28

    
    npInit(LED_PIN); 

    while (true) {
        // Calcula o valor RMS do som
        float rms_value = calcularRMS();
        printf("RMS do microfone: %.2f V\n", rms_value);

       
        if (rms_value > SOUND_THRESHOLD) {
            printf("Som detectado! Nível RMS: %.2f V\n", rms_value);
            efeito_cascata(); 
        } else {
            printf("Silêncio. Nível RMS: %.2f V\n", rms_value);
            npClear(); 
        }

        npWrite();
        sleep_ms(200);
    }

    return 0;
}
