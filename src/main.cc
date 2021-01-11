#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <AudioStream.h>
#include <Audio.h>
#include <USB-MIDI.h>
#include <synth_wavetable.h>
#include <AnalogSaw_samples.h>
#include <Flute_100kbyte_samples.h>
#include <lead_samples.h>

// Audio mixers can take 4 inputs
// Let's have 8 inputs in total for an 8-finger polyphonic synth
AudioMixer4 left_hand;
AudioMixer4 right_hand;

AudioSynthWavetable synth1;
AudioSynthKarplusStrong string1;
AudioConnection patch1(synth1, 0, left_hand, 0);
AudioConnection patch2(string1, 0, left_hand, 1);

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

void noteOn(byte channel, byte note, byte velocity) {
  Serial.printf("c: %d, note: %d, v: %d", channel, note, velocity);
  AudioNoInterrupts();
  synth1.playNote(note, 100);
  // string1.noteOn(synth1.noteToFreq(note), 0.9);
  AudioInterrupts();
}

void noteOff(byte channel, byte note, byte velocity) {
  Serial.printf("c: %d, note: %d, v: %d", channel, note, velocity);
  synth1.stop(); 
}

void setup() {
  synth1.setInstrument(lead);
  synth1.amplitude(1);
  Serial.println("hello, jerg");
  left_hand.gain(0, 1.0);
  right_hand.gain(0, 1.0);
  mixer.gain(0, 0.5);
  mixer.gain(1, 0.5);
  AudioMemory(22);
  AudioProcessorUsageMaxReset();
  AudioMemoryUsageMaxReset();
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.setHandleNoteOn(noteOn);
  MIDI.setHandleNoteOff(noteOff);
}

void loop() {
  MIDI.read();
}