ledflash
========

*ledflash* is a simple daemon which will switch an indicator LED on a
small Linux computer (e.g. a Raspberry Pi running Raspbian) off or on
every three seconds.  This lets you immediately tell if a headless
device has crashed.  It's a small program, written in C so as to not
consume a lot of system resources.

*ledflash* will work fine on any computer that implements the
[Linux LED interface](http://www.kernel.org/doc/Documentation/leds/leds-class.txt),
provided you have defined the constant 'FLASHFILE' to point to the
correct LED control file.  It was originally written to work on a
Pogoplug that had been upgraded to run Debian.

Building and Installing
-----------------------

To build ledflash:

   1. Install gcc and the basic dev libraries.

   2. Install perl (the manual is in perldoc).

   3. Clone the source tree and cd to it.

   4. Edit the Makefile to define the correct 'PLATFORM' value.  If your
      platform isn't supported, it's pretty straightforward to add it
      in `ledflash.c`.

   5. Do a 'make && sudo make install'.

   6. Frantically review the Makefile to ensure that I didn't just
      trick you into running an exploit as root.  Revise as necessary, then
      repeat the previous step.

   7. Do a 'sudo ledflash' to start the service.

   8. Add 'ledflash' to /etc/rc.local to start the service at boot
      time.


Other Stuff
-----------

Copyright (C) 2012-2016 Chris Reuter, GPLv2, NO WARRANTY!

