#ifndef TRANSFORMER_H
#define TRANSFORMER_H

#include "tinyllama2.h"

// Core transformer operations
void rmsnorm(float* o, float* x, float* weight, int size);
void matmul(float* xout, float* x, float* w, int n, int d);
void attention(RunState* s, TransformerWeights* w, Config* p, int layer, int pos);
void ffn(RunState* s, TransformerWeights* w, Config* p, int layer);
void transformer_forward(int token, int pos, Config* p, RunState* s, TransformerWeights* w);

// Utility functions
float* forward(Transformer* transformer, int token, int pos);

#endif // TRANSFORMER_H
