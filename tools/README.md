# Rom Tools

Tools for editing old school ROM files.

## Compiling

```
$ make
```

## nes2png

**nes2png** can convert ROM files to PNG files with a given palette color.
Currently, it supports only NES and Game Boy ROMs. Usage:

```
nes2png < rom.nes > out.png
```

Options:

- `--format-nes` - NES format (default).
- `--format-gb` - Game Boy format.
- `--num-tiles-row` - The number of tiles per row of the image.
- `--palette "#ffffff #c0c0c0 #808080 #000000"` - set palette colors.
- `--palette-file colors.txt` - read palette from a file.

The palette file should contain one hexadecimal 24bit color per line:

```
#ffffff
#c0c0c0
#808080
#000000
```

If no palette is specified, a default grayscale palette is used.
