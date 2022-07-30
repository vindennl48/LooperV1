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
#include "Standard_Definitions.h"
#include "Standard_Nav.h"
#include "Standard_Ring.h"
#include "Hardware_BTN.h"
#include "Hardware_Switch.h"
#include "Hardware_Pot.h"

/* ::SECOND LEVEL:: */
#include "Hardware_Hardware.h"
#include "Hardware_Storage.h"
#include "Audio_Buffers.h"
#include "Audio_MyBlocks.h"

#endif
