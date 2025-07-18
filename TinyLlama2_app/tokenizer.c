#include "tokenizer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int build_tokenizer(Tokenizer* t, const char* tokenizer_path, int vocab_size) {
    // Initialize tokenizer with basic vocabulary
    t->vocab_size = vocab_size;
    t->max_token_length = 64;
    
    printf("Building tokenizer with vocab size: %d\r\n", vocab_size);
    
    // For embedded demo, use simplified tokenizer
    // In production, this would load from flash memory
    
    // Initialize byte pieces for single characters
    for (int i = 0; i < 256; i++) {
        t->byte_pieces[i * 2] = (unsigned char)i;
        t->byte_pieces[i * 2 + 1] = '\0';
    }
    
    printf("Tokenizer initialized successfully\r\n");
    return 0;
}

void free_tokenizer(Tokenizer* t) {
    // Cleanup tokenizer resources
    printf("Tokenizer cleanup\r\n");
}

char* decode(Tokenizer* t, int prev_token, int token) {
    // Simple character-based decoding for demo
    static char piece[3];
    
    if (token < 256) {
        piece[0] = (char)token;
        piece[1] = '\0';
        return piece;
    }
    
    // For larger tokens, return placeholder
    piece[0] = 'X';
    piece[1] = '\0';
    return piece;
}

void safe_printf(char* piece) {
    // Print token piece safely
    if (piece) {
        // Filter out control characters
        for (int i = 0; piece[i] != '\0'; i++) {
            if (piece[i] >= 32 && piece[i] <= 126) {
                printf("%c", piece[i]);
            }
        }
    }
}

int str_lookup(char* str, Tokenizer* t) {
    // Simple string to token lookup
    if (str && strlen(str) == 1) {
        return (int)str[0];
    }
    return 0; // Unknown token
}

void encode(Tokenizer* t, char* text, int bos, int eos, int* tokens, int* n_tokens) {
    // Simple character-based encoding
    *n_tokens = 0;
    
    if (bos) {
        tokens[(*n_tokens)++] = 1; // BOS token
    }
    
    // Encode each character
    for (int i = 0; text[i] != '\0' && *n_tokens < 512; i++) {
        tokens[(*n_tokens)++] = (int)text[i];
    }
    
    if (eos) {
        tokens[(*n_tokens)++] = 2; // EOS token
    }
    
    printf("Encoded %d tokens\r\n", *n_tokens);
}
