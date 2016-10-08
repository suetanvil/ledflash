/*
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <sys/stat.h>

/* Every C program is required by law to implement its own version of
 * the common Boolean type.  Here's mine: */
#undef TRUE
#undef FALSE
typedef enum {FALSE = 0, TRUE = 1} Bool;


/* This is the "file" that controls the LED device.  Currently, we
 * support Raspbian and Debian ARM on PogoPlug.  Adding others should
 * be fairly straightforward. */
#ifdef PLATFORM_POGOPLUG    /* Debian ARM on a repurposed Pogoplug. */
#   define FLASHFILE   "/sys/class/leds/plug:green:health/trigger"
#   define ON          "default-on"
#   define OFF         "none"
#endif

#ifdef PLATFORM_RASPBIAN    /* Raspberry Pi running Raspbian. */
#   define FLASHFILE   "/sys/class/leds/led0/brightness"
#   define ON          "1"
#   define OFF         "0"
#endif

/* If no platform is defined, we assume it's a debugging configuration
 * and just write to a file in /tmp. */
#if !defined(PLATFORM_RASPBIAN) && !defined(PLATFORM_POGOPLUG)
#   define FLASHFILE    "/tmp/ledcolor.txt"
#   define ON           "1"
#   define OFF          "0"
#endif



/* Other constants: */
#define INTERVAL 3
#define IDENT "ledflash"


/* Log a message and exit. */
static void
die(const char *message) {
    syslog(LOG_ERR, "%s (fatal error)", message);
    exit(1);
}/* die */


/* Write 'message' to the file (or file-like thing) that controls the
 * LED. 'message' must be one of the control strings the device
 * recognizes.  (Well, actually, it can be anything but only a valid
 * control string will have an effect.) */
static void
setflash(const char *message) {
    FILE *fh;
    int status;

    fh = fopen(FLASHFILE, "w");
    if (!fh) {
        die("Error opening file '" FLASHFILE "' for writing.");
    }/* if */

    status = fputs(message, fh);
    if (status == EOF) {
        die ("Error writing to file '" FLASHFILE "'.");
    }/* if */

    fclose (fh); 
}/* setflash*/


/* The main loop.  Toggles the LED status, then sleeps for INTERVAL
 * seconds. */
static void
flashloop() {
    Bool on = FALSE;

    for(;;) {
        setflash(on ? ON : OFF);
        on = !on;
        sleep(INTERVAL);
    }/* for*/
}/* flashloop*/


/* Fork the current process and exit, first printing the new process's
 * ID.  The new process is the actual daemon. */
static void
forkit() {
    pid_t pid;

    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Fork failed, unable to launch.\n");
        exit(1);
    }/* if */

    if (pid > 0) {
        printf("PID: %d\n", (int)pid);
        exit(0);
    }/* if */
}/* forkit*/


/* Make this program into a proper *nix daemon. */
static void
daemonize(Bool debugMode) {
    pid_t sid;

    /* First, fork off (hurr hurr) a new process unless -d is given. */
    if (!debugMode) {
        forkit();
    }/* if */

    /* Enable syslog. */
    openlog(IDENT, LOG_PID, LOG_DAEMON);
    syslog(LOG_INFO, "Starting ledflash service.");
    
    /* Now, set the umask. */
    umask(0);

    /* And quit now if we're in debug mode. */
    if (debugMode) return;

    /* Switch to root dir. */
    if (chdir("/") < 0) {
        die("Unable to chdir to root directory, bailing.\n");
    }/* if */

    /* Switch to a new session. */
    sid = setsid();
    if (sid < 0) {
        die("setsid() failed, bailing.\n");
    }/* if */

    /* Close the standard file handles. */
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}/* daemonize*/


/* The mainline routine. */
int
main(int argc, char * const argv[]) {
    Bool debugMode = FALSE;

    /* Parse args.  No need for getopt, since the only flag is going
     * to be "-d". */
    if (argc > 1) {
        if (argc == 2 && strcmp(argv[1], "-d") == 0) {
            printf("Running in debug mode.\n");
            debugMode = TRUE;
        } else {
            printf("Command line: ledflash [-d]\n");
            exit(1);
        }/* if */
    }/* if */

    daemonize(debugMode);

    flashloop();

    return 0;    /* Not reached. */
}/* main*/
