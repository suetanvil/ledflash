ledflash
========

*ledflash* is a simple daemon which will switch the front panel LED of
a PogoPlug off or on every three seconds.  Its purpose is to provide
immediate visual confirmation that the device is still running, at
least to some extent.  It is a small program, written in C so as to
not consume a lot of system resources.

The PogoPlug should previously have been upgraded to a more general
Linux distribution and should not be running the original proprietary
software.

Actually, *ledflash* will work fine on any computer that implements
the [Linux LED interface](http://www.kernel.org/doc/Documentation/leds/leds-class.txt),
provided you have defined the constant 'FLASHFILE' to point to the
correct LED control file.

Building and Installing
-----------------------

To build ledflash:

   1. Install gcc and the basic dev libraries.

   2. Install perl (the manual is in perldoc).

   3. Clone the source tree and cd to it.

   4. Ensure that the constant FLASHFILE in ledflash.c has the right
      filename.

   5. Do a 'make && sudo make install'.

   6. Frantically review the Makefile to ensure that I didn't just
      trick you into running an exploit as root.  Revise as necessary, then
      repeat the previous step.

   7. Do a 'sudo ledflash' to start the service.

   8. Add 'ledflash' to /etc/rc.local to start the service at boot
      time.


Other Stuff
-----------

Copyright (C) 2012 Chris Reuter, GPLv2, NO WARRANTY!

