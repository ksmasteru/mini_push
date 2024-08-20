#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../includes/parser.h"


void ft_echo(t_token *tmp, bool n)
{
	int i = 0;

	while(i < tmp->location.lenght)
	{
		write(1, tmp->location.location + i, 1);
		i++;
	}
	if (!n && !tmp->up)
		write(1, "\n", 1);
}