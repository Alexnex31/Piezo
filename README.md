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
./piezo assets/song.ogg 120 250   # song, BPM, offset of the first beat in ms
```

BPM and offset are optional (defaults: 120 BPM, 0 ms). The screen flashes on
every beat, brighter on the first beat of each bar.

No music is included: drop an `.ogg` file into `assets/`
(use freely licensed music if the project is published).

Keys: `+` / `-` shift the offset by 5 ms, `Esc` quits.

## Milestones

1. ~~OGG playback + sample-accurate song clock~~
2. ~~Visual metronome synced to the BPM~~
3. Text charts + notes scrolling down 4 columns
4. Hit judgement (event timestamps), score, combo
5. Offset calibration screen

## License

MIT, see [LICENSE](LICENSE).