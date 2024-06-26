TODO

The current structure of the code is divided into three sections.

## Warning
This builds multiple flavors. The code is designed to be modular and structured. I created CMake libraries for each of the modules then link them together. I use interface for these rather than static or dynamic libraries because I want to centralize the compiler management. I could do flavor builds for each module however this means I have to keep track of many configurations. 

Currently the compilation is likely faster using interface libraries, but in the future it may be faster with static or dynamic libraries using flavors. This is currently done in IO for FTDI D2XX driver but this is also an interface library.

It should be noted this was done to keep changes localized and the layout clean. This should allow parallel work with fewer merge conflicts. Top level API should be set then implementations follow this. Changing the top level APIs should be done with care. Currently the top level APIs are mostly independent from each other and this should be maintained if possible. This is done with design patterns like singletons and factories.

#### Framebuffer
At the bottom you have the interfaces and controllers these interact with the actual LED panels. This code base is only allowed to use memory, GPIO and networking as the interface. Anything requiring tight timing cannot be handled in one of those interfaces must use external logic. The software interface for these is Framebuffer. 

Different external framebuffers have been conceptualized for other projects.
- GPIO Bit-bang (Raspberry Pi, BeagleBone Black, STM32MP1, etc.)
- Medium to large FPGA via bit-bang (Memory can be internal or external)
- Zynq (Series 7 FPGA soft cores, etc.)
- RP2040 as hardware module
- Microcontroller and small FPGA
- Using receiver card (I am still checking to see if this is allowed.)

#### Port
The middle layer is the hardware platform. These allow porting the code to Linux, Windows IoT, etc. It also allows supporting Raspberry Pi, BeagleBone Black, Rock Pi S, STM32MP1, Zynq, etc.

No bare metal support is planned, however in some cases this could be possible. User space logic is preferred. No support for RTLinux, Kernel Thread/Modules, etc. is planned. (No /dev/mem black magic.) Currently this should be mostly standard C++ logic.

These will be compiled into libraries containing the required interface implementations. These will be used by the application logic to interface with the hardware. There will be one per hardware port.

#### Application
The top layer is the applications and graphics. These use the libraries created from the lower two layers. This repository will be a sub-module to that repository. 

Note pixel mapping is likely handled by an auxiliary layer. Framebuffer is responsible for multiplex mapping of the panels. This has not really been considered yet.
