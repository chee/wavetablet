# wavetablet

This is mostly just a little wrapper around paul's teensy audio library synth_wavetable, updated to make sure it runs on the ATSAMD51 with adafruit's atsamd fork of Audio.

the `decoder.py` here is taken the teensy AudioSynthWaveform repo, with changes to make it output parsed soundfonts in the format expressed in the `synth_wavetable.cpp` file from the Audio repo