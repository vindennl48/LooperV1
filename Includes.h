#ifndef INCLUDES_H
#define INCLUDES_H

/* ::OUTSIDE LIBRARIES:: */
#include <Arduino.h>
#include <Bounce2.h>
#include "AudioStream.h"
#include "Audio.h"
#include "SdFat.h"
#include "BALibrary.h"
using namespace BALibrary;

/* ::FIRST LEVEL:: */
#define INCLUDE_STANDARD_DEFINITIONS
#include "Standard_Definitions.h"
#define INCLUDE_STANDARD_NAV
#include "Standard_Nav.h"
//#define INCLUDE_STANDARD_RING
//#include "Standard_Ring.h"
#define INCLUDE_STANDARD_BIBUFFER
#include "Standard_BiBuffer.h"
//#define INCLUDE_HARDWARE_L_ADC
//#include "Hardware_L_ADC.h"
#define INCLUDE_HARDWARE_BTN
#include "Hardware_BTN.h"
//#define INCLUDE_HARDWARE_SWITCH
//#include "Hardware_Switch.h"
//#define INCLUDE_HARDWARE_POT
//#include "Hardware_Pot.h"

/* ::SECOND LEVEL:: */
#define INCLUDE_HARDWARE_STORAGE
#include "Hardware_Storage.h"
#define INCLUDE_HARDWARE_HARDWARE
#include "Hardware_Hardware.h"
#define INCLUDE_AUDIO_AUDIOFILE
#include "Audio_AudioFile.h"
//#define INCLUDE_AUDIO_BUFFERS
//#include "Audio_Buffers.h"
#define INCLUDE_AUDIO_MYBLOCKS
#include "Audio_MyBlocks.h"
#define INCLUDE_AUDIO_ENGINE
#include "Audio_Engine.h"

#endif
