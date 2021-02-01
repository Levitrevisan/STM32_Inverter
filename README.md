# STM32 Inverter
## Introduction:

University project: Simple micro-inverter using a STM32F103C8.

## Objectives

Generate 3 phase signal through SPWM with 120 degrees of phase diffence. The frequency, phase and amplitude should be controlled through digital buttons.

## Project Roadmap

*  [OK] Be able to generate 6 PWM pulses with different duty cycles
*  [OK] Be able to control duty cycles inside timmed interruption routines
*  [OK] Guarantee that interruption have the timer and counter well adjusted to required time.
*  [OK] Be able to generate 3 SPWM waves with 120 between them
*  [OK] Guarantee that SPWM generated have 120 degrees between them
*  [OK] Control SPWM Amplitude
*  [OK] Implement GPIO input interruption
*  [--] Read Zero Cross signal
*  [OK] Control phase based on Zero Cross signal

## Reference

*  https://www.infineon.com/dgdl/Infineon-IM393-S6E-DS-v02_00-EN.pdf?fileId=5546d462696dbf1201699a9d912478e4 - INFINEON CIPOS datasheet
*  https://www.infineon.com/dgdl/Infineon-AN2016-11_CIPOS_Mini_Inverter_module_reference_board_type1_for_1-shunt_resistor-AN-v01_11-EN.pdf?fileId=5546d462566bd0c7015674af2c20258a - Evaluation board manual (used for "benchmarking" propuses)
*  https://www.st.com/content/ccc/resource/technical/document/reference_manual/59/b9/ba/7f/11/af/43/d5/CD00171190.pdf/files/CD00171190.pdf/jcr:content/translations/en.CD00171190.pdf - STM32 reference manual

## How to use MatLab script to generate SPWM values

The matlab script is used to generate the senoid values that are used as reference for the SPWM generation. You should se the parameters

```
  max_pwm_value = 3550;
  min_pwm_value = 0;
  frequency = 60;
  number_of_points_in_one_cycle = 60;
```

The script will return the int array to be pasted into the MCU code like this:

```
  {1775 ,1961 ,2144 ,2324 ,2497 ,2663 ,2818 ,2963 ,3094 ,3211 ,3312 ,3397 ,3463 ,3511 ,3540 ,3550 ,3540 ,3511 ,3463 ,3397 ,3312 ,3211 ,3094 ,2963 ,2818 ,2663 ,2497 ,2324 ,2144 ,1961 ,1775 ,1589 ,1406 ,1226 ,1053 ,888 ,732 ,587 ,456 ,339 ,238 ,153 ,87 ,39 ,10 ,0 ,10 ,39 ,87 ,153 ,238 ,339 ,456 ,587 ,732 ,887 ,1053 ,1226 ,1406 ,1589 ,}
```
  
It will also return the interuption period in nanoseconds used to control the timed interruption that will update the PWM values to generate the senoid. Lastly, it will show us the stem plot of those points.
