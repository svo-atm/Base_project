#ifndef TINYLLAMA2_H
#define TINYLLAMA2_H

#include <stdint.h>
#include <stddef.h>

// Model configuration (reduced for embedded demo)
#define VOCAB_SIZE 1000        // Reduced from 32000
#define MAX_SEQ_LEN 64         // Reduced from 512
#define DIM 64                 // Reduced from 288
#define N_LAYERS 3             // Reduced from 6
#define N_HEADS 4              // Reduced from 6
#define N_KV_HEADS 4           // Reduced from 6
#define HEAD_SIZE (DIM / N_HEADS)
#define HIDDEN_DIM 128         // Reduced from 768

// Model weights structure
typedef struct {
    float* token_embedding_table;    // (vocab_size, dim)
    float* rms_att_weight;          // (layer, dim) rmsnorm weights
    float* rms_ffn_weight;          // (layer, dim)
    float* wq;                      // (layer, dim, n_heads * head_size)
    float* wk;                      // (layer, dim, n_kv_heads * head_size)
    float* wv;                      // (layer, dim, n_kv_heads * head_size)
    float* wo;                      // (layer, n_heads * head_size, dim)
    float* w1;                      // (layer, hidden_dim, dim)
    float* w2;                      // (layer, dim, hidden_dim)
    float* w3;                      // (layer, hidden_dim, dim)
    float* rms_final_weight;        // (dim,)
    float* wcls;                    // (vocab_size, dim)
} TransformerWeights;

// Model configuration structure
typedef struct {
    int dim;
    int hidden_dim;
    int n_layers;
    int n_heads;
    int n_kv_heads;
    int vocab_size;
    int seq_len;
} Config;

// Runtime state
typedef struct {
    float* x;      // activation at current time stamp (dim,)
    float* xb;     // same, but inside a residual branch (dim,)
    float* xb2;    // an additional buffer just for convenience (dim,)
    float* hb;     // buffer for hidden dimension in the ffn (hidden_dim,)
    float* hb2;    // buffer for hidden dimension in the ffn (hidden_dim,)
    float* q;      // query (dim,)
    float* k;      // key (dim,)
    float* v;      // value (dim,)
    float* att;    // buffer for scores/attention values (n_heads, seq_len)
    float* logits; // output logits
    float* key_cache;   // (layer, seq_len, dim)
    float* value_cache; // (layer, seq_len, dim)
} RunState;

// Main transformer structure
typedef struct {
    Config config;
    TransformerWeights weights;
    RunState state;
    int fd;  // file descriptor for memory mapped weights
    float* data; // memory mapped data pointer
    size_t file_size;
} Transformer;

// Function declarations
int build_transformer(Transformer* t, const char* checkpoint_path);
void free_transformer(Transformer* t);
void generate(Transformer* transformer, int* prompt, int steps, float temperature);
int sample(float* probabilities, int n);
void softmax(float* x, int size);

#endif // TINYLLAMA2_H
