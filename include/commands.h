#ifndef COMMANDS_H
#define COMMANDS_H

void run_ls(void);
void run_pwd(void);
void run_cd(char *path);
void run_cat(char *filename);
void run_touch(char *filename);
void run_mkdir(char *dirname);
void run_rm(char *filename);
void run_echo(char *args[], int argc);
void run_clear(void);
int run_exit(void);
void run_head(char *filename);
void run_tail(char *filename);
void run_wc(char *filename);
void run_cp(char *source, char *destination);
void run_mv(char *source, char *destination);

#endif