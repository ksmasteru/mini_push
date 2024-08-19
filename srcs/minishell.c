#include "../includes/minishell.h"
#include "../includes/parser.h"
#include "../includes/pipes.h"
#include "../includes/tokens.h"
#include "../includes/executer.h"
#include "sys/wait.h"
#include "../includes/executer.h"
#include <stdbool.h>

// HANDLING '' "" inside expantion tokenization.c:261
// tokens arent freed  in builtns
void show_env(t_data *data, int is_export);
void built_in(int op, t_data *data, t_token *token);

bool is_empty(char *line)
{
	int i;

	i = 0;
	if (line[0] == 0)
		return (true);
	while (line[i] == 32 ||  (line[i] >= 9 && line[i] <= 13))
		i++;
	if (line[i] == 0 || line[i] == '\n')
	{
		free(line);
		return (true);
	}
	return (false);
}

/* returns null if empty line + frees it*/
char*	read_cmd()
{
	char *line;

	line = readline("\e[0;32m[minishell]$ \e[0;0m");
	if (!line)
		exit(-1);
	if (is_empty(line))
		return (NULL);
	if (access(".tmp.txt", F_OK) == 0)
		unlink(".tmp.txt");
	add_history(line);
	return (line);
}


void export_cmd(int op, char *line, t_data *data)
{
	t_token *tokens;

	tokens = lexer(line, data->env_lst);
	tokens_v2(&tokens, data);
	built_in(op, data, tokens);
}

int check_builtin2(char *line, t_data *data)
{
	t_token *tokens;

	if (ft_strlen(line) >= 5)
	{
		if (strncmp("unset", line, 5) == 0 && (line[5] == 32 || line[5] == 0))
		{
			export_cmd(2, line, data);
			free(line);
			return (1);
		}
	}
	if (ft_strlen(line) >= 3)
	{
		if (strncmp("env", line, 3) == 0 && (line[3] == 32 || line[3] == 0))
		{
			show_env(data, 0);
			free(line);
			return (1);
		}
	}
	if (strcmp(line, "exit") == 0)
	{
		free(line);
		exit(0);
	}
	if (ft_strlen(line) >= 4)
	{
		if (strncmp("echo", line, 4) == 0 && (line[4] == 32 || line[3] == 0))
		{
			ft_echo2(data, line);
			free(line);
			return (1);
		}
	}
	return (0);
}

/*frees the line after running the builting command.*/
int check_builtin(char *line, t_data *data)
{
	if (line[0] == 'c' && line[1] == 'd' && line[2] == 32)
	{
		line[ft_strlen(line)] = '\0';
		if (chdir(line + 3) < 0)
			perror("chdir");
		{
			free(line);
			return (1);
		}
	}
	if (ft_strlen(line) >= 6)
	{
		if (strncmp("export", line, 6) == 0 && (line[6] == 32 || line[6] == 0))
		{
			export_cmd(1, line, data);
			{
				free(line);
				return (1);
			}
		}
	}
	return (check_builtin2(line, data));
}

void set_data_variables(t_data *data, char **envp)
{
	data->envp = envp;
    data->words_count = 1;
    data->flag = 0;
	data->env_lst = NULL;
    env_to_lst(envp, data);
	data->env = get_envp(envp);
	data->mem_ref = NULL;
}

void free_data_variables(t_data *data)
{
	// free data->env_lst key data, value vadata
	t_lst *tmp;
	t_lst *holder;
	int i;

	i = 0;
	tmp = data->env_lst;
	while (tmp)
	{
		holder = tmp->next;
		if (tmp->data)
			free(tmp->data);
		if (tmp->value)
		{
			if (tmp->value->data)
				free(tmp->value->data);
			free(tmp->value);
		}
		free(tmp);
		tmp = holder;
	}
	while (data->env[i])
		free(data->env[i++]);
	free(data->env[i]);
	free(data->env);
}

int main(int ac, char **av, char **envp)
{
	char *line;
	int pid;

	t_data data;
	set_data_variables(&data, envp);
	while (1)
	{
		signal(SIGINT, handle_sigint1);
		signal(SIGQUIT, SIG_IGN);
		line = read_cmd();
		if (!line)
			continue;
		signal(SIGINT, handle_sigint2);
		signal(SIGQUIT, handle_sigquit); 
		if (check_builtin(line, &data))
			continue;
		pid = fork();
		if (pid == 0)
			parse_cmd(line , envp, &data);
		else
			waitpid(pid, NULL, 0);
		free(line);
	}
	free_data_variables(&data);
	return (0);
}