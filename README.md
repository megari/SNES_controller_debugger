SNES controller debugger
========================

This is a little project for debugging SNES controllers. The PCB design files
and the firmware source code are included.

The hardware uses an Atmel mega32u4 microcontroller.

The firmware uses the [YAAL](https://github.com/raphendyr/yaal) library and
[yaamake](https://github.com/raphendyr/yaamake). These are included as
submodules in the firmware/vendor directory.

Compilation of the firmware
---------------------------

The avr-gcc toolchain is required in order to compile the firmware. The
repositories of your distribution probably have this available.

After the toolchain has been installed:
```sh
cd firmware
```

If you haven't built yaamake yet, do so:
```sh
cd vendor/yaamake
make
cd ../..
```

Finally, build the firmware and program the board:
```sh
nano Makefile # Check that the programmer port is correct
make
make program
```
