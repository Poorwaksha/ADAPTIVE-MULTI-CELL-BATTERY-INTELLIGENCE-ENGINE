# Adaptive Multi-Cell Battery Intelligence Engine
## Project Overview A production-grade 4-cell Li-ion BMS built on Arduino Uno. Monitors real-time voltage, detects imbalance, and classifies battery health using a modular embedded architecture.
## Features - 4-cell individual voltage monitoring (A0–A3) - 8x oversampling ADC for noise reduction - Pack avg, imbalance %, SoC estimation - Health states: Healthy / Minor / Critical / Failure - Modular architecture (5 clean bms_* functions)
## Live Simulation [▶ Run on Wokwi](https://wokwi.com/projects/465291814391311361)
## Circuit 4x Potentiometers → Voltage divider (R1=100kΩ, R2=33kΩ) → Arduino analog pins A0–A3
## Tech Stack Platform : Arduino Uno | Simulator: Wokwi Language : C++ (Arduino) | ADC: 10-bit, 5V ref
