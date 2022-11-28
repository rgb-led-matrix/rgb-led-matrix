# rgb-led-matrix
This repository creates a library for top level applications and graphics. This repository will be a sub-module to that repository.

## Status
Work in progress. Code is licensed under GPL 2. (I would like to use GPL 3, but the original is GPL 2.) The original code base was https://github.com/hzeller/rpi-rgb-led-matrix.

There is no warranty for this. Work is free to use according to the [GPL 2 license](COPYING). I am not responsible or liable for any damages or issues as a result of this in any way. User is responsible for all laws and regulations as required. Work is AS-IS and it is the respiblity of any user to address any shortages discovered in this code base before using it.

## Contributing, feedback, questions, etc.
If in doubt use the discussion feature. I am okay with meaningful discussion in issues however please use the discussion feature first.

Some weird conventions are used. These should be documented in LED_Matrix/README.md. This is the place for documenting technical interworkings and troubleshooting. (Outside of comments.)

## Layout
There are three sections at this directory level. Look at lower levels for more details about each section.

### external
This is an experimental section for hardware offload modules used in rgb-led-matrix/lib/src/framebuffer. 

Some framebuffers do not use offload such as BCM. External modules may have better features, however this depends on the framebuffer.

### firmware
This is an external universal pin-pulser. Currently there is only support for RPi PICO. This is not finished.

### rgb-led-matrix
This is the main library source directory. The top level library includes are in rgb-led-matrix/include while the internal source is divided into rgb-led-matrix/lib which has an include and src folder. Applications do not need to worry about the internal logic, just the top level includes and the libraries.
