#pragma once
#include <AudioStream.h>
#include <AudioStream.h>
#include <synth_wavetable.h>
 
extern const AudioSynthWavetable::sample_data AnalogSaw_samples[1];
const uint8_t AnalogSaw_ranges[] = {127, };

const AudioSynthWavetable::instrument_data AnalogSaw = {1, AnalogSaw_ranges, AnalogSaw_samples };

extern const uint32_t sample_0_AnalogSaw_AnalogSaw1060L[44160];
