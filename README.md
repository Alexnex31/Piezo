# Piezo

A small 4-key rhythm game, written in C with SDL3.

## Dependencies

- SDL3 (package `sdl3` or `SDL3-devel` depending on your distro, otherwise
  build it from https://github.com/libsdl-org/SDL)
- libvorbis (package `libvorbis` or `libvorbis-devel` depending on your distro),
  used to decode the OGG songs

## Build & run

```sh
make
./piezo assets/song.chart
```

Keys: `D` `F` `J` `K` play the 4 columns (same spots on any keyboard layout),
`+` / `-` shift the global offset by 5 ms, `Up` / `Down` change the scroll
speed, `Enter` plays again from the results screen, `Esc` quits.

Timing windows: Perfect ±40 ms, Great ±80 ms, Good ±120 ms, later is a Miss.

No music is included: drop an `.ogg` file into `assets/` next to its chart
(use freely licensed music if the project is published).

## Calibration

Run this once (and again if you change headphones or speakers):

```sh
./piezo --calibrate
```

Tap any key on each click you hear, 32 times. Piezo measures how late your
taps land and saves that as the global offset in
`~/.local/share/piezo/config.txt`, used by every chart. After that, hitting
on the sound gives Perfects. `+` / `-` during a game fine-tune it, and that
is saved too.

## Chart format

A plain text file, one entry per line, `#` starts a comment:

```
song   song.ogg    # relative to the chart's folder
bpm    120
offset 250         # ms, time of the first beat
0     1            # note: time in ms, column 1 to 4
500   2
```

Note times are absolute song times; the offset only places the beat grid.

## Test track

`assets/click120.chart` goes with a 3-minute click track at exactly 120 BPM
(higher click on the first beat of each bar). Generate it with:

```sh
ffmpeg -f lavfi -i "aevalsrc='0.8*sin(2*PI*if(lt(mod(t,2),0.5),1500,1000)*t)*lt(mod(t,0.5),0.02)':s=44100:d=180" \
       -ac 2 -c:a libvorbis assets/click120.ogg
./piezo assets/click120.chart
```

## Milestones

1. ~~OGG playback + sample-accurate song clock~~
2. ~~Visual metronome synced to the BPM~~
3. ~~Text charts + notes scrolling down 4 columns~~
4. ~~Hit judgement (event timestamps), score, combo~~
5. ~~Offset calibration screen~~

## License

MIT, see [LICENSE](LICENSE).