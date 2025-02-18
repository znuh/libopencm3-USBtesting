#include "platform.h"
#include "console.h"

#include <string.h>
#include <stdio.h>
#include <unistd.h>

/* these are some example console commands */

CONSOLE_COMMAND_DEF(ver, "show firmware info/version");
static void ver_command_handler(void) {
	puts("FW_BASE: "GIT_VERSION"\n"__FILE__" "__DATE__" "__TIME__);
}

CONSOLE_COMMAND_DEF(erase_vt, "erase flash page 0 to reenable DFU bootloader");
static void erase_vt_command_handler(void) {
	const char *yes = "yes\n";
	puts("WARNING! This will erase a part of the firmware (flash page 0) to reenable");
	puts("the BootROM DFU bootloader. The firmware will no longer work after this!");
	puts("A powercycle may be needed to access the bootloader.");
	fputs("Continue? (Enter yes): ", stdout);
	fflush(stdout);
	ACM_waitfor_txdone();
	for(SIGINT=0;*yes;yes++) {
		int v = ACM_readbyte();
		if((*yes ^ v) || SIGINT)
			goto abort;
		write(1,&v,1);
	}
	if((!(*yes)) && (!SIGINT)) {
		puts("Erasing page 0 - USB will disconnect now.");
		fflush(stdout);
		ACM_waitfor_txdone();
		usb_shutdown();
		erase_page0(0xAA55);
	}
	return;
abort:
	puts("\nuser abort");
}

CONSOLE_COMMAND_DEF(anim, "nonsense command to demonstrate SIGINT & WFI sleeping");
static void anim_command_handler(void) {
	const char seq[] = "\r.\ro\rO\ro";
	int idx=0;
	puts("demo loop - abort with Ctrl+C");
	// SIGINT is set in USB rx handler ISR - must be cleared by user
	for(SIGINT=0;!SIGINT;idx+=2) {
		write(1,seq+(idx&7),2);
		sleep_ms(100); // this will not abort on SIGINT - use SLEEP_UNTIL(timeout_check(...) || SIGINT) for interruptible sleeping
	}
	puts("");
}

/* TODO:
 * - optional heartbeat LED
 * - make STDIO optional?
 */

/* list of console commands */
static const console_command_def_t * const console_commands[] = {
	ver, erase_vt, anim,
	NULL
};

/* write function for console */
static void console_write(const char *s) {
	int len=strlen(s);
	write(1,s,len);
}

int main(void) {
	const console_init_t init_console = {.write_function = console_write};
	const console_command_def_t * const *cmd;

	hw_init(); // see ../common-code/platform.c

	/* init console & register all commands */
	console_init(&init_console);
	for(cmd=console_commands;*cmd;cmd++)
		console_command_register(*cmd);

	/* main loop */
	while(1) {
		SLEEP_UNTIL(ACM_rx_fill);
		if(ACM_rx_fill)
			ACM_to_console();
	}
	return 0;
}
