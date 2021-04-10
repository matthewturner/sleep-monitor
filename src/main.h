#ifndef main_h
#define main_h

#include <Arduino.h>
#include <AccelStepper.h>
#include "Analyzer.h"
#include "Trigger.h"
#include "Stepper.h"
#include "HardwareEndStop.h"
#include "HardwareTrigger.h"
#include "HardwareStepper.h"
#include "HardwareMicrophone.h"
#include "Pillow.h"

#define SOUND_DETECTED LOW
#define END_STOP_TOP_PIN 12
#define END_STOP_BOTTOM_PIN 11
#define STEP_INTERFACE_TYPE 1
#define STEP_DIRECTION_PIN 7
#define STEP_STEP_PIN 6
#define STEP_ENABLE_PIN 5
#define MICROPHONE_PIN 10
#define TRIGGER_PIN 2

void printStatus(uint64_t currentTime);
void printSummary(Summary *summary);

#endif