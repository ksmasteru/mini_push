/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo (1).c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aech-chi <aech-chi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 13:57:47 by aech-chi          #+#    #+#             */
/*   Updated: 2024/08/20 14:12:47 by aech-chi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parser.h"
#include <dirent.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "../includes/executer.h"

int	pwd(char *cmd)
{
	int	i;
	char *wd;
	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '-')
		{
			printf("pwd: ");
			while (cmd[i] != '\0' && cmd[i] != '\n' && cmd[i] != '\t'
				&& cmd[i] != ' ')
			{
				printf("%c", cmd[i]);
				i++;
			}
			printf(": invalid option\n");
			return (1);
		}
		i++;
	}
	wd = getcwd(0, 0);
	printf("%s\n", wd);
	if (wd)
		free(wd);
	return (0);
}

int	cd_error(char *path, t_token *tokens);

char *get_home_path(t_data *data)
{
	char *home_path;
	t_lst *tmp = data->env_lst;
	int found = 0;
	while (tmp)
	{
		if (strcmp(tmp->data, "HOME") == 0)
		{
			found = 1;
			break;
		}
		tmp = tmp->next;
	}
	if (found == 1)
		return (tmp->value->data);
	else
	{
		printf("home path not found\n");
		return NULL;
	}
}


int ft1_strlen(char *str)
{
	int i = 0;

	if(str == NULL)
		return 0;
	while(str[i])
		i++;
	return i;
}


bool is_special(char *path)
{
	int i;
	bool found;

	found = false;
	i = 0;
	
	while (path[i] && path[i] == 32 || (path[i] >= 9 && path[i] <= 13))
		i++;
	if (path[i] == '~')
	{
		i++;
		found = true;
	}
	while (path[i] && path[i] == 32 || (path[i] >= 9 && path[i] <= 13))
		i++;
	if (found && path[i] == 0)
		return (true);
	return (false);

}
// cd
// path = line.
int cd(char *path, t_data *data)
{
	char *home;
	int n;
	int i;
	t_token *tokens = data->tokens;
	i  =0;
	home = get_home_path(data);
	if (is_empty(path + 2) || is_special(path + 2))
	{
		if (chdir(home) < 0)
			return (cd_error(home, tokens));
		//free_all_tokens(&tokens);
		return (0);
	}
	else
	{
		//if (tokens->type == WORD)
		//{
		if (tokens->up && tokens->up->up)
		{
			write(2, "cd: too many arguments\n", 24);
			//free_all_tokens(&tokens);
			return (1);
		}
		tokens->up->location.location[tokens->up->location.lenght] = 0;
		//printf("dir is %s\n",tokens->up->location.location);
		if (chdir(tokens->up->location.location) < 0)
			return (cd_error(path + i, tokens));
	}
	//}
	//free_all_tokens(&tokens);
	return 0;
}

int	cd_error(char *path, t_token *tokens)
{
	int	i;

	i = 0;
	write(1, "cd: ", 5);
	while (path[i])
	{
		write(1, &path[i], 1);
		i++;
	}
	write(1, ": ", 2);
	perror("");
	//free_all_tokens(&tokens);
	return (1);
}

int	count_args(char **status)
{
	int	i;
	int	j;
	int	n;

	n = 0;
	i = 0;
	j = 1;
	while (status[j])
	{
		while (status[j][i])
		{
			i++;
		}
		i = 0;
		n++;
		j++;
	}
	return (n);
}

int	check_argtwo(char *status)
{
	int	i;

	i = 0;
	while (status[i] == ' ' || status[i] == '\t' || status[i] == '+'
		|| status[i] == '-')
		i++;
	while (status[i] > 47 && status[i] < 57)
		i++;
	if (i == strlen(status))
		return (0);
	return (1);
}

void	exit_empty(char **status, unsigned long long int n, int i)
{
	unsigned char	c;

	if (check_argtwo(status[1]) == 1)
	{
		write(1, "minishell: exit: ", 18);
		while (status[1][i])
		{
			write(1, &status[1][i], 1);
			i++;
		}
		write(1, ": numeric argument required\n", 28);
		free_2d_str(status);
		exit(2);
	}
	else
	{
		n = ft_atoi(status[1]);
		write(1, "exit\n", 5);
		if (n < 256 && n >= 0)
		{
			free_2d_str(status);
			exit(n);
		}
		else if (n > 255)
		{
			c = n;
			free_2d_str(status);
			exit(c);
		}
	}
}

int	ft_atoi(const char *str)
{
	int						i;
	unsigned long long int	sign;
	unsigned long long int	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == ' ' || str[i] == '\n' || str[i] == '\t' || str[i] == '\v'
		|| str[i] == '\r' || str[i] == '\f')
		i++;
	if (str[i] == '+' && str[i + 1] != '-')
		i++;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10;
		result += str[i] - '0';
		i++;
	}
	return (sign * result);
}

int	ft_exit(char *line)
{
	int	i;
	char **status;
	int code;

	code = 0;
	i = 0;	
	status = ft_split(line, 32);
	if (count_args(status) == 0)
	{
		write(1, "exit\n", 5);
		code = 0;
	}
	else if (count_args(status) == 1)
	{
		free(line);
		exit_empty(status, 0, 0);
	}
	else
	{
		if (check_argtwo(status[1]) == 1)
		{
			write(1, "minishell: exit: ", 17);
			while (status[1][i])
				write(1, &status[1][i++], 1);
			write(1, ": numeric argument required\n", 28);
			code = 2;
		}
		else
			write(1, "minishell: exit: too many arguments\n", 36);
	}
	free(line);
	free_2d_str(status);
	exit(code);
}
