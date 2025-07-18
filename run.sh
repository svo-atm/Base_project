#!/bin/bash

# FVP Execution Script for Headless Environment
# This script runs the FVP in a headless environment without GUI dependencies

set -e

# Configuration
FVP_PATH="/mnt/hdd/stuart_data/fvp/bin/models/Linux64_GCC-9.3/FVP_Corstone_SSE-320"
# AXF_FILE="out/object-detection/AVH-SSE-320-U85/Debug/object-detection.axf"
# AXF_FILE="out/bin/ethos-u-object_detection.axf"
AXF_FILE="out/TinyLlama2_app/SSE-320-FVP/Debug/TinyLlama2_app.elf"
SIM_LIMIT="${1:-3000}"  # Increased from 300 to 3000 cycles for AI processing
OUTPUT_DIR="fvp_output"

# Create output directory
mkdir -p "$OUTPUT_DIR"

# Remove DISPLAY to prevent X11 issues
unset DISPLAY

echo "Starting FVP simulation..."
echo "Executable: $AXF_FILE"
echo "Simulation limit: $SIM_LIMIT cycles"
echo "Output directory: $OUTPUT_DIR"

# Run FVP with comprehensive headless configuration
"$FVP_PATH" \
  -a "$AXF_FILE" \
  -C mps4_board.visualisation.disable-visualisation=1 \
  -C vis_hdlcd.disable_visualisation=1 \
  -C mps4_board.uart0.shutdown_on_eot=1 \
  -C mps4_board.subsystem.ethosu.num_macs=256 \
  -C mps4_board.telnetterminal0.start_telnet=0 \
  -C mps4_board.telnetterminal1.start_telnet=0 \
  -C mps4_board.telnetterminal2.start_telnet=0 \
  -C mps4_board.telnetterminal5.start_telnet=0 \
  -C mps4_board.uart0.out_file="$OUTPUT_DIR/uart0_output.txt" \
  -C mps4_board.uart0.unbuffered_output=1 \
  --simlimit "$SIM_LIMIT" \
  --stat \
  -q > "$OUTPUT_DIR/fvp_log.txt" 2>&1 &

FVP_PID=$!
echo "FVP started with PID: $FVP_PID"

# Monitor FVP execution
TIMEOUT=120  # Increased to 120 seconds timeout for AI processing
COUNT=0
while kill -0 $FVP_PID 2>/dev/null && [ $COUNT -lt $TIMEOUT ]; do
    echo -n "."
    sleep 1
    COUNT=$((COUNT + 1))
done

echo ""

if kill -0 $FVP_PID 2>/dev/null; then
    echo "FVP still running after $TIMEOUT seconds, terminating..."
    kill $FVP_PID 2>/dev/null || true
    sleep 2
    kill -9 $FVP_PID 2>/dev/null || true
    echo "FVP terminated."
else
    echo "FVP execution completed."
fi

# Display results
echo ""
echo "=== FVP Execution Results ==="
if [ -f "$OUTPUT_DIR/uart0_output.txt" ]; then
    echo "UART0 Output:"
    cat "$OUTPUT_DIR/uart0_output.txt"
else
    echo "No UART0 output file found."
fi

if [ -f "$OUTPUT_DIR/fvp_log.txt" ]; then
    echo ""
    echo "FVP Log (last 20 lines):"
    tail -20 "$OUTPUT_DIR/fvp_log.txt"
fi

echo ""
echo "Output files saved in: $OUTPUT_DIR/"
ls -la "$OUTPUT_DIR/"
