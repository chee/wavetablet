#pragma once
#include <AudioStream.h>
#include <synth_wavetable.h>

extern const AudioSynthWavetable::sample_data lead_samples[2];
const uint8_t lead_ranges[] = {71, 127, };

const AudioSynthWavetable::instrument_data lead = {2, lead_ranges, lead_samples };


extern const uint32_t sample_0_lead_lead1L[68992];

extern const uint32_t sample_1_lead_lead1hiR[34560];
