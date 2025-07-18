#!/usr/bin/env python3
"""
TinyLlama2 Weight Extraction Script
Converts PyTorch model weights to C arrays for embedded deployment
"""

import torch
import numpy as np
from transformers import LlamaForCausalLM, LlamaTokenizer
import struct

def extract_weights(model_path="TinyLlama/TinyLlama-1.1B-Chat-v1.0"):
    """Extract weights from TinyLlama2 model"""
    print("Loading TinyLlama2 model...")
    model = LlamaForCausalLM.from_pretrained(model_path, torch_dtype=torch.float32)
    
    # Extract key weight matrices
    weights = {}
    
    # Token embeddings
    weights['token_embedding_table'] = model.model.embed_tokens.weight.detach().numpy()
    
    # Layer weights (for each transformer layer)
    weights['layers'] = []
    for i, layer in enumerate(model.model.layers):
        layer_weights = {
            'attention_norm': layer.input_layernorm.weight.detach().numpy(),
            'ffn_norm': layer.post_attention_layernorm.weight.detach().numpy(),
            'wq': layer.self_attn.q_proj.weight.detach().numpy(),
            'wk': layer.self_attn.k_proj.weight.detach().numpy(), 
            'wv': layer.self_attn.v_proj.weight.detach().numpy(),
            'wo': layer.self_attn.o_proj.weight.detach().numpy(),
            'w1': layer.mlp.gate_proj.weight.detach().numpy(),
            'w2': layer.mlp.down_proj.weight.detach().numpy(),
            'w3': layer.mlp.up_proj.weight.detach().numpy(),
        }
        weights['layers'].append(layer_weights)
    
    # Final layer norm and output projection
    weights['norm_final'] = model.model.norm.weight.detach().numpy()
    weights['output_proj'] = model.lm_head.weight.detach().numpy()
    
    return weights

def quantize_weights(weights, bits=8):
    """Quantize weights to reduce memory usage"""
    quantized = {}
    
    for key, value in weights.items():
        if isinstance(value, np.ndarray):
            # Simple linear quantization
            min_val, max_val = value.min(), value.max()
            scale = (max_val - min_val) / (2**bits - 1)
            zero_point = -min_val / scale
            
            quantized_data = np.round(value / scale + zero_point).astype(np.uint8)
            quantized[key] = {
                'data': quantized_data,
                'scale': scale,
                'zero_point': zero_point,
                'shape': value.shape
            }
        elif isinstance(value, list):
            quantized[key] = [quantize_weights({'layer': layer}, bits)['layer'] for layer in value]
    
    return quantized

def generate_c_array(name, data, data_type="float"):
    """Generate C array declaration"""
    if data_type == "float":
        flat_data = data.flatten()
        array_str = "const float " + name + "[] = {\n"
        for i, val in enumerate(flat_data):
            if i % 8 == 0:
                array_str += "    "
            array_str += f"{val:.6f}f"
            if i < len(flat_data) - 1:
                array_str += ", "
            if (i + 1) % 8 == 0:
                array_str += "\n"
        if len(flat_data) % 8 != 0:
            array_str += "\n"
        array_str += "};\n\n"
    else:  # uint8 quantized
        flat_data = data.flatten()
        array_str = "const uint8_t " + name + "[] = {\n"
        for i, val in enumerate(flat_data):
            if i % 16 == 0:
                array_str += "    "
            array_str += f"{val}"
            if i < len(flat_data) - 1:
                array_str += ", "
            if (i + 1) % 16 == 0:
                array_str += "\n"
        if len(flat_data) % 16 != 0:
            array_str += "\n"
        array_str += "};\n\n"
    
    return array_str

def generate_weight_file(weights, output_file="real_model_weights.c", use_quantization=True):
    """Generate C file with all model weights"""
    
    with open(output_file, 'w') as f:
        f.write("// Real TinyLlama2 Model Weights\n")
        f.write("// Generated automatically from PyTorch model\n\n")
        f.write("#include \"tinyllama2.h\"\n")
        f.write("#include <stdint.h>\n\n")
        
        if use_quantization:
            # Generate quantized weights
            quantized = quantize_weights(weights)
            
            # Token embeddings
            token_data = quantized['token_embedding_table']
            f.write(generate_c_array("token_embedding_data", token_data['data'], "uint8"))
            f.write(f"const float token_embedding_scale = {token_data['scale']:.8f}f;\n")
            f.write(f"const float token_embedding_zero_point = {token_data['zero_point']:.8f}f;\n\n")
            
            # Layer weights (simplified - just first layer as example)
            layer0 = quantized['layers'][0]
            for weight_name, weight_data in layer0.items():
                if isinstance(weight_data, dict):
                    f.write(generate_c_array(f"layer0_{weight_name}_data", weight_data['data'], "uint8"))
                    f.write(f"const float layer0_{weight_name}_scale = {weight_data['scale']:.8f}f;\n")
                    f.write(f"const float layer0_{weight_name}_zero_point = {weight_data['zero_point']:.8f}f;\n\n")
        
        else:
            # Generate full precision weights (memory intensive!)
            f.write(generate_c_array("token_embedding_table", weights['token_embedding_table']))
            
            # Just first layer for demo (full model would be too large)
            layer0 = weights['layers'][0]
            for weight_name, weight_data in layer0.items():
                f.write(generate_c_array(f"layer0_{weight_name}", weight_data))
        
        # Generate weight mapping functions
        f.write("// Weight loading functions\n")
        f.write("void load_real_weights(TransformerWeights* w) {\n")
        if use_quantization:
            f.write("    // Load quantized weights and dequantize\n")
            f.write("    // TODO: Implement dequantization\n")
        else:
            f.write("    w->token_embedding_table = (float*)token_embedding_table;\n")
            f.write("    // TODO: Load remaining weights\n")
        f.write("}\n\n")

def main():
    print("TinyLlama2 Weight Extraction Tool")
    print("=================================")
    
    # Extract weights
    weights = extract_weights()
    print(f"Extracted weights from {len(weights['layers'])} layers")
    
    # Print weight sizes
    token_emb_size = weights['token_embedding_table'].nbytes / (1024*1024)
    print(f"Token embedding size: {token_emb_size:.2f} MB")
    
    # Generate C file
    generate_weight_file(weights, use_quantization=True)
    print("Generated real_model_weights.c with quantized weights")
    
    # Generate header with dimensions
    with open("real_model_config.h", 'w') as f:
        f.write("// Real model configuration\n")
        f.write(f"#define REAL_VOCAB_SIZE {weights['token_embedding_table'].shape[0]}\n")
        f.write(f"#define REAL_MODEL_DIM {weights['token_embedding_table'].shape[1]}\n")
        f.write(f"#define REAL_N_LAYERS {len(weights['layers'])}\n")
    
    print("Generated real_model_config.h with actual dimensions")

if __name__ == "__main__":
    main()
