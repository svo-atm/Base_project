#ifndef TOKENIZER_H
#define TOKENIZER_H

// Simple tokenizer for demo purposes
typedef struct {
    char** vocab;
    float* vocab_scores;
    int vocab_size;
    unsigned int max_token_length;
    unsigned char byte_pieces[256*2]; // stores all single-byte strings
} Tokenizer;

// Tokenizer functions
int build_tokenizer(Tokenizer* t, const char* tokenizer_path, int vocab_size);
void free_tokenizer(Tokenizer* t);
char* decode(Tokenizer* t, int prev_token, int token);
void safe_printf(char* piece);
int str_lookup(char* str, Tokenizer* t);
void encode(Tokenizer* t, char* text, int bos, int eos, int* tokens, int* n_tokens);

#endif // TOKENIZER_H
