#include <Arduino.h>
#include <Audio.h>
#include <AudioStream.h>
#include <SPI.h>
#include <USB-MIDI.h>
#include <Wire.h>
#include <synth_wavetable.h>

#include "instrument.h"

// Audio mixers can take 4 inputs
// Let's have 8 inputs in total for an 8-finger polyphonic synth
AudioMixer4 left_hand;
AudioMixer4 right_hand;

const int len_voices = 4 * 2;

AudioSynthWavetable wavetable0, wavetable1, wavetable2, wavetable3, wavetable4,
    wavetable5, wavetable6, wavetable7;
AudioSynthWavetable *wavetables[]{&wavetable0, &wavetable1, &wavetable2,
                                  &wavetable3, &wavetable4, &wavetable5,
                                  &wavetable6, &wavetable7};

AudioConnection tl0(wavetable0, 0, left_hand, 0);
AudioConnection tl1(wavetable1, 0, left_hand, 1);
AudioConnection tl2(wavetable2, 0, left_hand, 2);
AudioConnection tl3(wavetable3, 0, left_hand, 3);
AudioConnection tr4(wavetable4, 0, right_hand, 0);
AudioConnection tr5(wavetable5, 0, right_hand, 1);
AudioConnection tr6(wavetable6, 0, right_hand, 2);
AudioConnection tr7(wavetable7, 0, right_hand, 3);

// mixers can take mixers as inputs, so let's create another to be sent to both
// speakers. pc2064v1 will be a polyphonic synth with monophonic output
AudioMixer4 mixer;
AudioConnection patch_left_hand(left_hand, 0, mixer, 0);
AudioConnection patch_right_hand(right_hand, 0, mixer, 1);

// The output is an audio stream too, it takes two inputs
AudioOutputAnalogStereo audioOut;
AudioConnection patch_left_ear(mixer, 0, audioOut, 0);
AudioConnection patch_right_ear(mixer, 0, audioOut, 1);

// some day to replace this with tinyusb again
USBMIDI_CREATE_DEFAULT_INSTANCE();

class Voice {
public:
  int id;
  byte note;
  int age;
};

Voice voices[len_voices];

int age = 0;

void play(Voice *voice, byte note, byte velocity) {
    auto wt = wavetables[voice->id];
  voice->note = note;
  voice->age = age++;
  wt->playNote(voice->note, velocity);
}

void noteOn(byte channel, byte note, byte velocity) {
  int oldest_age = infinity();
  int oldest_index = 0;

  // get a free one
  for (int i = 0; i < len_voices; i++) {
    if (wavetables[i]->getEnvState() ==
        AudioSynthWavetable::envelopeStateEnum::STATE_IDLE) {
      return play(&voices[i], note, velocity);
    }

    // ok, a releasing one then
    if (wavetables[i]->getEnvState() ==
        AudioSynthWavetable::envelopeStateEnum::STATE_RELEASE) {
      return play(&voices[i], note, velocity);
    }
    
    // or the eldest
    if (voices[i].age < oldest_age) {
      oldest_index = i;
      oldest_age = voices[i].age;
    }
  }

  play(&voices[oldest_index], note, velocity);
}

void noteOff(byte channel, byte note, byte velocity) {
  bool nothing = true;
  for (int i = 0; i < len_voices; i++) {
    if (wavetables[i]->getEnvState()) {
      nothing = false;
    }
    if (voices[i].note == note) {
      wavetables[i]->stop();
    }
  }
  if (nothing) {
    age = 0;
  }
}

void setup() {
  for (int i = 0; i < len_voices; i++) {
    voices[i].id = i;
    wavetables[i]->setInstrument(instrument);
    wavetables[i]->amplitude(1);
  }

  mixer.gain(0, 0.01);
  mixer.gain(1, 0.01);
  AudioMemory(22);
  AudioProcessorUsageMaxReset();
  AudioMemoryUsageMaxReset();
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.setHandleNoteOn(noteOn);
  MIDI.setHandleNoteOff(noteOff);
}

void loop() { MIDI.read(); }