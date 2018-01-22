# audioIO

*still a WIP. Heavily based on [ofxAudioAnalyzer](https://github.com/leozimmerman/ofxAudioAnalyzer) and [Sonoscopio](https://github.com/leozimmerman/Sonoscopio).*

> Access audio stream from device, analyze it and send datas.

audioIO will trigger an `aio-datas` event, containing an array with datas for active channels, each with a `channel` property. Event will also contain an empty object as last item: this is a JSON hack that will be removed some day.

## Build

*I do not use xcode to work with oF.*

```shell
$ make
$ make install
```
