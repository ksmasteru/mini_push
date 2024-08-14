#include "../includes/executer.h"


void handle_sigint2(int sig)
{
	(void)sig;
	printf("\n");
}
void handle_sigquit(int sig)
{
	(void)sig;
	printf("Quit (core dumped)\n");
}


/*mor readline*/
void    handle_sigint1(int sig)
{
    (void)sig;
	/*9ad exit status fstruct*/
    int exit_status = 130;
    write(1, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}


