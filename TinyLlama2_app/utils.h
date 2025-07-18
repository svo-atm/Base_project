#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stdlib.h>
#include <time.h>

// Include main header for type definitions
#include "tinyllama2.h"

// Utility functions
void malloc_run_state(RunState* s, Config* p);
void free_run_state(RunState* s);
void memory_map_weights(TransformerWeights *w, Config* p, float* ptr, int shared_weights);
unsigned long long time_in_ms();
unsigned int random_u32(unsigned long long *state);
float random_f32(unsigned long long *state);
int argmax(float* probabilities, int n);

// Math utilities
float fminf_custom(float a, float b);
float fmaxf_custom(float a, float b);
float expf_custom(float x);
float sqrtf_custom(float x);

#endif // UTILS_H
