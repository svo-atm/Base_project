# Instructions for Using Real TinyLlama2 Weights

## Prerequisites
```bash
pip install torch transformers numpy
```

## Step 1: Run Weight Extraction
```bash
cd /Users/tramyho/Work/FPTtelecom/Base_project/scripts
python extract_weights.py
```

## Step 2: Update model_weights.c
Replace the current placeholder file with the generated `real_model_weights.c`

## Step 3: Update Configuration
Replace the #define values in tinyllama2.h with values from `real_model_config.h`

## Step 4: Implement Quantized Math
Add dequantization functions to utils.c:

```c
float dequantize_weight(uint8_t quantized_val, float scale, float zero_point) {
    return scale * (quantized_val - zero_point);
}

void dequantize_array(uint8_t* quantized, float* output, int size, float scale, float zero_point) {
    for (int i = 0; i < size; i++) {
        output[i] = dequantize_weight(quantized[i], scale, zero_point);
    }
}
```

## Step 5: Update Memory Management
Modify malloc_run_state() in utils.c to handle real model dimensions

## Memory Warning ⚠️
The real model is huge! You'll need:
- External flash storage (>100MB)
- Streaming weight loading
- Heavy quantization (INT4 or lower)

## Alternative: Use a Smaller Model
Consider these instead:
- DistilBERT (66M parameters)
- TinyBERT (14M parameters) 
- Custom tiny transformer (1-10M parameters)

These are more realistic for microcontroller deployment.
