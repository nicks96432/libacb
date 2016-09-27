# libcgss

**libcgss** is a helper library for [Idolmaster Cinderella Girls Starlight Stage](http://cinderella.idolmaster.jp/sl-stage/)
(CGSS).

The project consists of a library (*libcgss*) and several other applications.

- Library
- Applications
  - *hcaenc*: HCA encoder utility.
  - *hcacc*: HCA cipher conversion utility.

This project is written in C/C++, compared to C# of its sister project [**DereTore**](https://github.com/hozuki/DereTore),
for a better interoperability with other languages or libraries. The C# version of *hcaenc* and *hcacc*
can be found in **DereTore** as applications.

## Building

Requirements:

- CMake
- MSVC, *or* Cygwin/gcc, *or* MinGW/gcc (on Windows)
- gcc (on macOS and Linux) <sup>untested</sup>

## User Manual

See [here](docs/user-manual.md).

## License

_**Modified**_ MIT License. See [LICENSE.md](LICENSE.md).

## Disclaimer

See [here](DISCLAIMER.md).

## Notes

More tech details can be found in [Behind the Scenes](docs/behind-the-scenes.md).
