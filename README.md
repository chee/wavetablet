# wavetablet

This is mostly just a little wrapper around paul's teensy audio library synth_wavetable, updated to make sure it runs on the ATSAMD51 with adafruit's atsamd fork of Audio.

the `decoder.py` here is taken the teensy AudioSynthWaveform repo, with changes to make it output parsed soundfonts in the format expressed in the `synth_wavetable.cpp` file from the Audio repo

if you run decoder on a soundfont, it'll drop the headers in src/instruments. then you can edit `instrument.h` and change these lines:

```c++
#include "instruments/AnalogSaw_samples.h"
auto instrument = AnalogSaw;
```

to, for instance,

```c++
#include "instruments/lead_samples.h"
auto instrument = lead;
```

and that'll be your instrument now.
