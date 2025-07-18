// Placeholder model weights for TinyLlama2 demo
// In production, these would be the actual quantized model weights

#include "tinyllama2.h"

// Demo weights - in real implementation these would be loaded from flash
// For now, just declare placeholder arrays to satisfy the linker

// Token embeddings (simplified)
float demo_token_embeddings[1000];

// Attention weights (simplified)
float demo_attention_weights[1000];

// FFN weights (simplified)  
float demo_ffn_weights[1000];

// Initialize demo weights with some values
void init_demo_weights(void) {
    // Initialize with small random-like values for demo
    for (int i = 0; i < 1000; i++) {
        demo_token_embeddings[i] = 0.01f * (i % 100 - 50);
        demo_attention_weights[i] = 0.001f * (i % 200 - 100);
        demo_ffn_weights[i] = 0.005f * (i % 150 - 75);
    }
}

// Model configuration constants
const int model_vocab_size = VOCAB_SIZE;
const int model_dim = DIM;
const int model_n_layers = N_LAYERS;
const int model_n_heads = N_HEADS;
