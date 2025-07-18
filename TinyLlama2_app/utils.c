#include "tinyllama2.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

void malloc_run_state(RunState* s, Config* p) {
    // Allocate memory for runtime state
    // In embedded systems, this might use static allocation instead
    
    printf("Allocating runtime state...\r\n");
    
    // For demo purposes, use static arrays to avoid malloc issues
    static float x_buffer[DIM];
    static float xb_buffer[DIM];
    static float xb2_buffer[DIM];
    static float hb_buffer[HIDDEN_DIM];
    static float hb2_buffer[HIDDEN_DIM];
    static float q_buffer[DIM];
    static float k_buffer[DIM];
    static float v_buffer[DIM];
    static float att_buffer[N_HEADS * MAX_SEQ_LEN];
    static float logits_buffer[VOCAB_SIZE];
    static float key_cache_buffer[N_LAYERS * MAX_SEQ_LEN * DIM];
    static float value_cache_buffer[N_LAYERS * MAX_SEQ_LEN * DIM];
    
    s->x = x_buffer;
    s->xb = xb_buffer;
    s->xb2 = xb2_buffer;
    s->hb = hb_buffer;
    s->hb2 = hb2_buffer;
    s->q = q_buffer;
    s->k = k_buffer;
    s->v = v_buffer;
    s->att = att_buffer;
    s->logits = logits_buffer;
    s->key_cache = key_cache_buffer;
    s->value_cache = value_cache_buffer;
    
    printf("Runtime state allocated successfully\r\n");
}

void free_run_state(RunState* s) {
    // For static allocation, nothing to free
    printf("Runtime state cleanup\r\n");
}

void memory_map_weights(TransformerWeights *w, Config* p, float* ptr, int shared_weights) {
    // Map model weights from memory
    // In real implementation, this would map from flash memory
    printf("Mapping model weights from memory...\r\n");
    
    // For demo, use placeholder pointers
    static float dummy_weights[1000]; // Placeholder
    
    w->token_embedding_table = dummy_weights;
    w->rms_att_weight = dummy_weights;
    w->rms_ffn_weight = dummy_weights;
    w->wq = dummy_weights;
    w->wk = dummy_weights;
    w->wv = dummy_weights;
    w->wo = dummy_weights;
    w->w1 = dummy_weights;
    w->w2 = dummy_weights;
    w->w3 = dummy_weights;
    w->rms_final_weight = dummy_weights;
    w->wcls = dummy_weights;
}

unsigned long long time_in_ms() {
    // Simple time function for embedded systems
    static unsigned long long counter = 0;
    return counter++;
}

unsigned int random_u32(unsigned long long *state) {
    // Simple LCG random number generator
    *state = *state * 1103515245 + 12345;
    return (unsigned int)(*state / 65536) % 32768;
}

float random_f32(unsigned long long *state) {
    return (random_u32(state) >> 5) * 0x1.0p-27f;
}

int argmax(float* probabilities, int n) {
    int max_i = 0;
    float max_p = probabilities[0];
    for (int i = 1; i < n; i++) {
        if (probabilities[i] > max_p) {
            max_i = i;
            max_p = probabilities[i];
        }
    }
    return max_i;
}

// Custom math functions for embedded systems without full math library
float fminf_custom(float a, float b) {
    return (a < b) ? a : b;
}

float fmaxf_custom(float a, float b) {
    return (a > b) ? a : b;
}

float expf_custom(float x) {
    // Simple approximation for exp(x)
    if (x > 10.0f) return 22026.4657948f; // e^10
    if (x < -10.0f) return 0.0000454f;    // e^-10
    
    // Taylor series approximation
    float result = 1.0f;
    float term = 1.0f;
    for (int i = 1; i < 10; i++) {
        term *= x / i;
        result += term;
    }
    return result;
}

float sqrtf_custom(float x) {
    if (x <= 0.0f) return 0.0f;
    
    // Newton-Raphson method
    float guess = x * 0.5f;
    for (int i = 0; i < 10; i++) {
        guess = 0.5f * (guess + x / guess);
    }
    return guess;
}
