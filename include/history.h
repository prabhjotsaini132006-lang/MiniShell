#ifndef HISTORY_H
#define HISTORY_H

void add_history_command(const char *command);
void show_history(void);
void free_history(void);

#endif