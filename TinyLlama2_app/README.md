# TinyLlama2 ARM Cortex-M55 Demo

This project demonstrates a TinyLlama2 language model inference engine optimized for ARM Cortex-M55 processors.

## Features

- **Optimized for Embedded Systems**: Designed specifically for ARM Cortex-M55 with limited memory
- **CMSIS-DSP Integration**: Leverages ARM CMSIS-DSP library for optimized math operations
- **Memory Efficient**: Uses static memory allocation and quantized weights
- **Real-time Inference**: Demonstrates transformer-based text generation on embedded hardware

## Project Structure

```
TinyLlama2_app/
├── main.c                 # Main application entry point
├── tinyllama2.c/.h        # Core TinyLlama2 model implementation
├── transformer.c/.h       # Transformer layers and attention mechanisms
├── tokenizer.c/.h         # Simple tokenizer for text processing
├── utils.c/.h             # Utility functions and custom math
├── model_weights.c        # Model weights (placeholder for demo)
└── RTE/                   # Run-Time Environment configuration
```

## Model Configuration

- **Vocabulary Size**: 32,000 tokens
- **Model Dimension**: 288
- **Layers**: 6 transformer layers
- **Attention Heads**: 6 heads
- **Hidden Dimension**: 768
- **Max Sequence Length**: 512 tokens

## Hardware Requirements

- ARM Cortex-M55 processor with FPU
- Minimum 512KB RAM for model weights and activations
- CMSIS-DSP library support

## Building and Running

1. Open the workspace in VS Code with CMSIS extension
2. Build using the CMSIS-Toolbox build system
3. Deploy to SSE-320-FVP (Fixed Virtual Platform)
4. Use hardware switches to trigger inference demos

## Demo Features

- Interactive hardware monitoring via FPGA I/O
- LED patterns indicate AI processing status
- Console output shows inference progress
- Simplified text generation demonstration

## Optimization Features

- **CMSIS-DSP**: Uses optimized ARM math functions
- **Quantization**: Model weights can be quantized for memory efficiency  
- **Static Allocation**: No dynamic memory allocation for real-time performance
- **Custom Math**: Optimized exp() and sqrt() implementations

## Future Enhancements

- Full model weight loading from flash memory
- Advanced quantization (INT8, INT4)
- Streaming inference for longer sequences
- Integration with external memory interfaces

## Dependencies

- ARM CMSIS-CORE
- ARM CMSIS-DSP
- ARM SSE-320 BSP
- CMSIS-Compiler

This project serves as a foundation for deploying large language models on resource-constrained embedded systems.
