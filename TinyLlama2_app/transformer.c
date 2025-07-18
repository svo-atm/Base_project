#include "tinyllama2.h"
#include "transformer.h"
#include "utils.h"
#include <stdio.h>
#include <math.h>

// ARM CMSIS-DSP optimized operations
#ifdef ARM_MATH_CM55
#include "arm_math.h"
#endif

void rmsnorm(float* o, float* x, float* weight, int size) {
    // Calculate sum of squares
    float ss = 0.0f;
    
#ifdef ARM_MATH_CM55
    arm_dot_prod_f32(x, x, size, &ss);
#else
    for (int j = 0; j < size; j++) {
        ss += x[j] * x[j];
    }
#endif
    
    ss /= size;
    ss += 1e-5f; // epsilon
    ss = 1.0f / sqrtf_custom(ss);
    
    // Normalize and scale
    for (int j = 0; j < size; j++) {
        o[j] = weight[j] * (ss * x[j]);
    }
}

void matmul(float* xout, float* x, float* w, int n, int d) {
    // Matrix multiplication: xout = x * w^T
    // x is (1, n), w is (d, n), xout is (1, d)
    
#ifdef ARM_MATH_CM55
    // Use CMSIS-DSP optimized matrix operations
    arm_matrix_instance_f32 mat_x, mat_w, mat_out;
    arm_mat_init_f32(&mat_x, 1, n, x);
    arm_mat_init_f32(&mat_w, d, n, w);
    arm_mat_init_f32(&mat_out, 1, d, xout);
    
    // Note: CMSIS-DSP expects w to be transposed already
    for (int i = 0; i < d; i++) {
        arm_dot_prod_f32(x, &w[i * n], n, &xout[i]);
    }
#else
    for (int i = 0; i < d; i++) {
        float val = 0.0f;
        for (int j = 0; j < n; j++) {
            val += x[j] * w[i * n + j];
        }
        xout[i] = val;
    }
#endif
}

void attention(RunState* s, TransformerWeights* w, Config* p, int layer, int pos) {
    int head_size = p->dim / p->n_heads;
    
    // Get the query, key, value vectors for this position
    matmul(s->q, s->x, w->wq + layer * p->dim * p->dim, p->dim, p->dim);
    matmul(s->k, s->x, w->wk + layer * p->dim * p->dim, p->dim, p->dim);
    matmul(s->v, s->x, w->wv + layer * p->dim * p->dim, p->dim, p->dim);
    
    // Attention computation (simplified for demo)
    for (int h = 0; h < p->n_heads; h++) {
        float* q_head = s->q + h * head_size;
        float* k_head = s->k + h * head_size;
        float* v_head = s->v + h * head_size;
        
        // Compute attention scores
        float score = 0.0f;
#ifdef ARM_MATH_CM55
        arm_dot_prod_f32(q_head, k_head, head_size, &score);
#else
        for (int i = 0; i < head_size; i++) {
            score += q_head[i] * k_head[i];
        }
#endif
        score /= sqrtf_custom((float)head_size);
        
        // Apply softmax (simplified)
        s->att[h] = expf_custom(score);
    }
    
    // Normalize attention weights
    float sum = 0.0f;
    for (int h = 0; h < p->n_heads; h++) {
        sum += s->att[h];
    }
    for (int h = 0; h < p->n_heads; h++) {
        s->att[h] /= sum;
    }
    
    // Output projection
    matmul(s->xb, s->v, w->wo + layer * p->dim * p->dim, p->dim, p->dim);
}

void ffn(RunState* s, TransformerWeights* w, Config* p, int layer) {
    // Feed-forward network
    matmul(s->hb, s->x, w->w1 + layer * p->dim * p->hidden_dim, p->dim, p->hidden_dim);
    matmul(s->hb2, s->x, w->w3 + layer * p->dim * p->hidden_dim, p->dim, p->hidden_dim);
    
    // Apply SiLU activation: x * sigmoid(x)
    for (int i = 0; i < p->hidden_dim; i++) {
        float val = s->hb[i];
        val *= (1.0f / (1.0f + expf_custom(-val))); // SiLU
        s->hb[i] = val * s->hb2[i]; // Gated
    }
    
    // Output projection
    matmul(s->xb, s->hb, w->w2 + layer * p->hidden_dim * p->dim, p->hidden_dim, p->dim);
}

void transformer_forward(int token, int pos, Config* p, RunState* s, TransformerWeights* w) {
    // Token embedding
    float* content_row = w->token_embedding_table + token * p->dim;
    for (int i = 0; i < p->dim; i++) {
        s->x[i] = content_row[i];
    }
    
    // Forward through layers
    for (int l = 0; l < p->n_layers; l++) {
        // Attention block
        rmsnorm(s->xb, s->x, w->rms_att_weight + l * p->dim, p->dim);
        attention(s, w, p, l, pos);
        
        // Residual connection
        for (int i = 0; i < p->dim; i++) {
            s->x[i] += s->xb[i];
        }
        
        // FFN block
        rmsnorm(s->xb, s->x, w->rms_ffn_weight + l * p->dim, p->dim);
        ffn(s, w, p, l);
        
        // Residual connection
        for (int i = 0; i < p->dim; i++) {
            s->x[i] += s->xb[i];
        }
    }
    
    // Final norm
    rmsnorm(s->x, s->x, w->rms_final_weight, p->dim);
    
    // Classifier
    matmul(s->logits, s->x, w->wcls, p->dim, p->vocab_size);
}

float* forward(Transformer* transformer, int token, int pos) {
    transformer_forward(token, pos, &transformer->config, &transformer->state, &transformer->weights);
    return transformer->state.logits;
}
