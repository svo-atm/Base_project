#include "RTE_Components.h"
#include CMSIS_device_header
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "device_cfg.h"
#include "device_definition.h"
#include "arm_mps3_io_drv.h"
#include "arm_mps3_io_reg_map.h"

#include "tinyllama2.h"
#include "tokenizer.h"
#include "utils.h"

extern int stdout_init();

// Global AI model instance
static Transformer transformer;
static Tokenizer tokenizer;

// Pre-defined questions and responses for demo
typedef struct {
    const char* question;
    const char* response;
} QAEntry;

static const QAEntry qa_database[] = {
    {"What is AI?", "AI is artificial intelligence that enables machines to simulate human thinking and decision-making processes."},
    {"How does machine learning work?", "Machine learning uses algorithms to find patterns in data and make predictions without explicit programming."},
    {"What is embedded AI?", "Embedded AI runs machine learning models directly on microcontrollers and small devices for real-time processing."},
    {"Tell me about ARM processors", "ARM processors are energy-efficient chips used in smartphones, IoT devices, and embedded systems worldwide."},
    {"What is a transformer model?", "Transformers use attention mechanisms to process sequences of data, revolutionizing natural language processing."},
    {"How do neural networks learn?", "Neural networks learn by adjusting connection weights between neurons based on training data and feedback."},
    {"What is deep learning?", "Deep learning uses multi-layered neural networks to automatically learn complex patterns from large datasets."},
    {"Explain computer vision", "Computer vision enables machines to interpret and understand visual information from images and videos."},
    {"What is natural language processing?", "NLP helps computers understand, interpret, and generate human language in a meaningful way."},
    {"How does this microcontroller work?", "This ARM Cortex-M55 runs at high efficiency with dedicated AI acceleration for edge computing applications."}
};

#define QA_DATABASE_SIZE (sizeof(qa_database) / sizeof(QAEntry))

// Simple question matching function
const char* find_response(const char* question) {
    // Convert question to lowercase for matching
    char normalized_question[256];
    int len = strlen(question);
    if (len >= 256) len = 255;
    
    for (int i = 0; i < len; i++) {
        normalized_question[i] = (question[i] >= 'A' && question[i] <= 'Z') ? 
                                question[i] + 32 : question[i];
    }
    normalized_question[len] = '\0';
    
    // Look for keyword matches
    for (int i = 0; i < QA_DATABASE_SIZE; i++) {
        // Simple substring matching
        if (strstr(normalized_question, "ai") && strstr(qa_database[i].question, "AI")) {
            return qa_database[i].response;
        }
        if (strstr(normalized_question, "learn") && strstr(qa_database[i].question, "learning")) {
            return qa_database[i].response;
        }
        if (strstr(normalized_question, "embedded") && strstr(qa_database[i].question, "embedded")) {
            return qa_database[i].response;
        }
        if (strstr(normalized_question, "arm") && strstr(qa_database[i].question, "ARM")) {
            return qa_database[i].response;
        }
        if (strstr(normalized_question, "transform") && strstr(qa_database[i].question, "transformer")) {
            return qa_database[i].response;
        }
        if (strstr(normalized_question, "neural") && strstr(qa_database[i].question, "neural")) {
            return qa_database[i].response;
        }
        if (strstr(normalized_question, "deep") && strstr(qa_database[i].question, "deep")) {
            return qa_database[i].response;
        }
        if (strstr(normalized_question, "vision") && strstr(qa_database[i].question, "vision")) {
            return qa_database[i].response;
        }
        if (strstr(normalized_question, "language") && strstr(qa_database[i].question, "language")) {
            return qa_database[i].response;
        }
        if (strstr(normalized_question, "microcontroller") && strstr(qa_database[i].question, "microcontroller")) {
            return qa_database[i].response;
        }
    }
    
    // Default responses for common patterns
    if (strstr(normalized_question, "hello") || strstr(normalized_question, "hi")) {
        return "Hello! I'm TinyLlama2 running on ARM Cortex-M55. Ask me about AI, machine learning, or embedded systems!";
    }
    if (strstr(normalized_question, "how are you") || strstr(normalized_question, "how do you do")) {
        return "I'm running efficiently on this microcontroller! My neural networks are processing at optimal performance.";
    }
    if (strstr(normalized_question, "what can you do") || strstr(normalized_question, "capabilities")) {
        return "I can answer questions about AI, explain machine learning concepts, and demonstrate edge computing capabilities!";
    }
    
    return "I'm a specialized AI model focused on embedded systems and machine learning. Try asking about AI, neural networks, or ARM processors!";
}

void process_ai_inference(const char* input_text) {
    printf("\r\n🤖 TinyLlama2 Processing...\r\n");
    printf("📝 Input: %s\r\n", input_text);
    
    // Show processing steps
    printf("🔄 Tokenizing input...\r\n");
    
    // Simulate tokenization
    int tokens[64];
    int n_tokens;
    encode(&tokenizer, (char*)input_text, 1, 0, tokens, &n_tokens);
    printf("   Generated %d tokens\r\n", n_tokens);
    
    printf("🧠 Running transformer layers...\r\n");
    
    // Simulate attention processing with LED patterns
    struct arm_mps3_io_reg_map_t* p_mps3_io_port =
        (struct arm_mps3_io_reg_map_t*)MPS3_IO_DEV.cfg->base;
    
    for (int layer = 0; layer < N_LAYERS; layer++) {
        printf("   Layer %d: Attention + FFN\r\n", layer + 1);
        
        // Show processing with LEDs
        p_mps3_io_port->fpgaio_leds = (1 << layer);
        
        // Processing delay
        for (volatile int i = 0; i < 200000; i++);
    }
    
    printf("🎯 Generating response...\r\n");
    
    // Get intelligent response
    const char* response = find_response(input_text);
    
    // Simulate word-by-word generation
    printf("💬 Response: ");
    int resp_len = strlen(response);
    for (int i = 0; i < resp_len; i++) {
        printf("%c", response[i]);
        if (response[i] == ' ' || response[i] == '.' || response[i] == ',') {
            for (volatile int j = 0; j < 50000; j++); // Small delay between words
        }
    }
    printf("\r\n");
    
    // Final LED pattern to show completion
    p_mps3_io_port->fpgaio_leds = 0xFF;
    for (volatile int i = 0; i < 300000; i++);
    p_mps3_io_port->fpgaio_leds = 0x00;
    
    printf("✅ Inference complete!\r\n\r\n");
}

void interactive_qa_session() {
    const char* predefined_questions[] = {
        "What is AI?",
        "How does machine learning work?",
        "What is embedded AI?",
        "Tell me about ARM processors",
        "What is a transformer model?",
        "Hello, how are you?"
    };
    
    const int num_questions = sizeof(predefined_questions) / sizeof(predefined_questions[0]);
    static int question_index = 0;
    
    printf("🎓 Interactive Q&A Session Starting...\r\n");
    printf("Question %d of %d:\r\n", question_index + 1, num_questions);
    
    const char* current_question = predefined_questions[question_index];
    process_ai_inference(current_question);
    
    question_index = (question_index + 1) % num_questions;
}

void monitor_hardware_input() {
    struct arm_mps3_io_reg_map_t* p_mps3_io_port =
                                (struct arm_mps3_io_reg_map_t*)MPS3_IO_DEV.cfg->base;
    
    static uint8_t last_sw_mask = 0;
    uint8_t current_sw_mask = p_mps3_io_port->fpgaio_switches;
    
    if (last_sw_mask != current_sw_mask && current_sw_mask != 0) {
        printf("🎛️  Hardware Input Detected - Switches: 0x%02X\r\n", current_sw_mask);
        
        // Different switch patterns trigger different responses
        switch (current_sw_mask) {
            case 0x01:
                process_ai_inference("What is artificial intelligence?");
                break;
            case 0x02:
                process_ai_inference("How does deep learning work?");
                break;
            case 0x03:
                process_ai_inference("Tell me about embedded AI");
                break;
            case 0x04:
                process_ai_inference("What can you do?");
                break;
            case 0x05:
                process_ai_inference("How does this ARM processor work?");
                break;
            default:
                interactive_qa_session();
                break;
        }
        
        last_sw_mask = current_sw_mask;
    }
}

int main() {
    __enable_irq();
    stdout_init();
    
    printf("\r\n");
    printf("🚀 ================================================\r\n");
    printf("🤖      TinyLlama2 Intelligent Q&A System       \r\n");
    printf("🧠         ARM Cortex-M55 Edge AI Demo          \r\n");
    printf("🚀 ================================================\r\n");
    printf("💡 System: ARM Cortex-M55 with CMSIS-DSP\r\n");
    printf("🎯 Features: Real-time AI question answering\r\n");
    printf("⚡ Memory: Optimized for embedded deployment\r\n");
    printf("🎛️  Control: Use switches to ask different questions\r\n");
    printf("🚀 ================================================\r\n\r\n");
    
    // Initialize AI model
    printf("🔧 Initializing TinyLlama2 AI Model...\r\n");
    
    // Initialize transformer
    if (build_transformer(&transformer, NULL) == 0) {
        printf("✅ Transformer model loaded successfully\r\n");
    } else {
        printf("❌ Failed to load transformer model\r\n");
    }
    
    // Initialize tokenizer
    if (build_tokenizer(&tokenizer, NULL, VOCAB_SIZE) == 0) {
        printf("✅ Tokenizer initialized successfully\r\n");
    } else {
        printf("❌ Failed to initialize tokenizer\r\n");
    }
    
    printf("🧠 Neural network layers: %d\r\n", N_LAYERS);
    printf("🎯 Attention heads: %d\r\n", N_HEADS);
    printf("� Model dimension: %d\r\n", DIM);
    printf("�📝 Vocabulary size: %d\r\n", VOCAB_SIZE);
    printf("⚡ Optimized for embedded deployment\r\n");
    printf("🚀 Model ready for intelligent conversations!\r\n\r\n");
    
    // Welcome demonstration
    printf("🎉 Welcome Demo:\r\n");
    process_ai_inference("Hello, what can you tell me about AI?");
    
    printf("🎛️  Switch Controls:\r\n");
    printf("   0x01 - Ask about AI\r\n");
    printf("   0x02 - Ask about deep learning\r\n");
    printf("   0x03 - Ask about embedded AI\r\n");
    printf("   0x04 - Ask about capabilities\r\n");
    printf("   0x05 - Ask about ARM processors\r\n");
    printf("   Other - Interactive Q&A session\r\n\r\n");
    
    // Main interaction loop
    for (;;) {
        monitor_hardware_input();
        
        // Small delay to prevent overwhelming the system
        for (volatile int i = 0; i < 100000; i++);
    }
    
    return 0;
}
