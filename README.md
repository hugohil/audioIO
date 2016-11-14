# audioIO

*still a WIP. Heavily based on [ofxAudioAnalyzer](https://github.com/leozimmerman/ofxAudioAnalyzer) and [Sonoscopio](https://github.com/leozimmerman/Sonoscopio).*

> Access audio stream from device, analyze it and send datas.

### List of OSC addresses:

* `/audioIO/:channel-number:/rms` -> `float`
* `/audioIO/:channel-number:/power` -> `float`
* `/audioIO/:channel-number:/pitchFreq` -> `float`
* `/audioIO/:channel-number:/pitchFreqNorm` -> `float`
* `/audioIO/:channel-number:/pitchConf` -> `float`
* `/audioIO/:channel-number:/pitchSalience` -> `float`
* `/audioIO/:channel-number:/hfc` -> `float`
* `/audioIO/:channel-number:/hfcNorm` -> `float`
* `/audioIO/:channel-number:/specComp` -> `float`
* `/audioIO/:channel-number:/specCompNorm` -> `float`
* `/audioIO/:channel-number:/centroid` -> `float`
* `/audioIO/:channel-number:/centroidNorm` -> `float`
* `/audioIO/:channel-number:/inharmonicity` -> `float`
* `/audioIO/:channel-number:/dissonance` -> `float`
* `/audioIO/:channel-number:/rollOff` -> `float`
* `/audioIO/:channel-number:/rollOffNorm` -> `float`
* `/audioIO/:channel-number:/oddToEven` -> `float`
* `/audioIO/:channel-number:/oddToEvenNorm` -> `float`
* `/audioIO/:channel-number:/strongPeak` -> `float`
* `/audioIO/:channel-number:/strongPeakNorm` -> `float`
* `/audioIO/:channel-number:/strongDecay` -> `float`
* `/audioIO/:channel-number:/strongDecayNorm` -> `float`
* `/audioIO/:channel-number:/spectrum` *(not yet available)*
* `/audioIO/:channel-number:/melBands` *(not yet available)*
* `/audioIO/:channel-number:/mfcc` *(not yet available)*
* `/audioIO/:channel-number:/hpcp` *(not yet available)*
* `/audioIO/:channel-number:/tristimulus` *(not yet available)*
* `/audioIO/:channel-number:/isOnset` -> `bool`
