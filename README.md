# rgb-led-matrix
This repository creates a library for top level applications and graphics. This repository will be a sub-module to that repository. The purpose of this code base is to allow better support for different small form factor boards regardless of hardware, operating system, etc. It also allows for support of different implementations.

## Status
Work in progress. Code is licensed under GPL 2. The original code base was https://github.com/hzeller/rpi-rgb-led-matrix. This code base has diverged from this code base signifcantly, I went another direction with it. Note my changes are also protected under GPL 2.

There is no warranty for this. Work is free to use according to the [GPL 2 license](COPYING). I am not responsible or liable for any damages or issues as a result of this in any way. User is responsible for all laws and regulations as required. Work is AS-IS and it is the respiblity of any user to address any shortages discovered in this code base before using it.

## Building
Note this is a hack for now

```bash
bash build.sh
```

## Layout
There are three sections at this directory level. Look at lower levels for more details about each section.

### firmware
This is an external universal pin-pulser. Currently there is only support for RPi PICO. This is not finished.

### rgb-led-matrix
This is the main library source directory. The top level library includes are in rgb-led-matrix/include while the internal source is divided into rgb-led-matrix/lib which has an include and src folder. Applications do not need to worry about the internal logic, just the top level includes and the libraries.

### scripts
This is a folder containing helper scripts for setup and other things. These will be referenced by other sections.
