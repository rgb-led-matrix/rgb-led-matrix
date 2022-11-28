TODO

This repository creates a library for top level applications and graphics. This repository will be a sub-module to that repository.

There are three sections at this directory level. Look at lower levels for more details about each section.

#### external
This is an experimental section for hardware offload modules used in rgb-led-matrix/lib/src/framebuffer.

#### firmware
This is an external universal pin-pulser. Currently there is only support for RPi PICO. This is not finished.

#### rgb-led-matrix
This is the main library source directory. The top level library includes are in rgb-led-matrix/include while the internal source is divided into rgb-led-matrix/lib which has an include and src folder. Applications do not need to worry about the internal logic, just the top level includes and the libraries.