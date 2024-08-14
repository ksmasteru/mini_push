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

int pwd(char *cmd)
{
	int i = 0;
	while(cmd[i])
	{
		if(cmd[i] == '-')
		{
			printf("pwd: ");
			while(cmd[i] != '\0' && cmd[i] != '\n' && cmd[i] != '\t' && cmd[i] != ' ')
			{
				printf("%c", cmd[i]);
				i++;
			}
			printf(": invalid option\n");
			return 1;
		}
		i++;
	}
	printf("%s\n", getcwd(0, 0));
	return 0;
}
int cd_error(char *path);
int cd(char *path)
{
	if(path == NULL || (strlen(path) == 0) || path[0] == '~')
		chdir("/nfs/homes/aech-chi");
	else
	{
		int i = 0;
		while(path[i])
		{
			if(path[i] == ' ' && path[i + 1] != '\0')
			{
				return(write(1, "cd: too many arguments", 23), 1);
			}
			i++;
		}
		int n = chdir(path);
		if(n == -1)
			cd_error(path);
		
	}
	return 0;
}
int cd_error(char *path)
{
	int i;

	i = 0;
	write(1, "cd: ", 5);
	while(path[i])
	{
		write(1, &path[i], 1);
		i++;
	}
	write(1, ": ", 2);
	perror("");
	return 1;
}