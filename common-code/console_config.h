#pragma once

// See the README for documentation on these options.

#ifndef CONSOLE_MAX_COMMANDS
#define CONSOLE_MAX_COMMANDS 32
#endif

#ifndef CONSOLE_MAX_LINE_LENGTH
#define CONSOLE_MAX_LINE_LENGTH 64
#endif

#ifndef CONSOLE_PROMPT
#define CONSOLE_PROMPT "> "
#endif

#ifndef CONSOLE_RETURN_KEY
#define CONSOLE_RETURN_KEY '\n'
#endif

#ifndef CONSOLE_NEWLINE
#define CONSOLE_NEWLINE "\n"
#endif

#ifndef CONSOLE_BUFFER_ATTRIBUTES
#define CONSOLE_BUFFER_ATTRIBUTES
#endif

#ifndef CONSOLE_HELP_COMMAND
#define CONSOLE_HELP_COMMAND 1
#endif

#ifndef CONSOLE_FULL_CONTROL
#define CONSOLE_FULL_CONTROL 1
#endif

#ifndef CONSOLE_TAB_COMPLETE
#define CONSOLE_TAB_COMPLETE 1
#endif

#if CONSOLE_TAB_COMPLETE && !CONSOLE_FULL_CONTROL
#error "CONSOLE_TAB_COMPLETE requires CONSOLE_FULL_CONTROL to be enabled"
#endif

#ifndef CONSOLE_HISTORY
#define CONSOLE_HISTORY 16
#endif

#if CONSOLE_HISTORY && !CONSOLE_FULL_CONTROL
#error "CONSOLE_HISTORY requires CONSOLE_FULL_CONTROL to be enabled"
#endif
