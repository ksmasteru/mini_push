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
	while (line[i] && (line[i] == 32 ||  (line[i] >= 9 && line[i] <= 13)))
		i++;
	if (line[i] == 0 || line[i] == '\n')
		return (true);
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
	{
		free(line);
		return (NULL);
	}
	if (access(".tmp.txt", F_OK) == 0)
		unlink(".tmp.txt");
	add_history(line);
	return (line);
}

// export 1 unset 2 env 3 pwd 4 cd 5 echo 6 exit 7
int built_in_code(char *line)
{
	if (ft_strlen(line) >= 6)
	{
		if (strncmp("export", line, 6) == 0 && (strchr(" \v\t\n", line[6]) || line[6] == 0))
			return (1);
	}
	if (ft_strlen(line) >= 5)
	{
		if (strncmp("unset", line, 5) == 0 && (strchr(" \v\t\n", line[5]) || line[5] == 0))
			return (2);
	}
	if (ft_strlen(line) >= 4)
	{
		if (strncmp("echo", line, 4) == 0 && (strchr(" \v\t\n", line[4]) || line[4] == 0))
			return (6);
		if (strncmp("exit", line, 4) == 0 && (strchr(" \v\t\n", line[4]) || line[4] == 0))
			return (7);
	}
	if (ft_strlen(line) >= 3)
	{
		if (strncmp("env", line, 3) == 0 && (strchr(" \v\t\n", line[3]) || line[3] == 0))
			return (3);
		if (strncmp("pwd", line, 3) == 0 && (strchr(" \v\t\n", line[3]) || line[3] == 0))
			return (4);
	}
	if (ft_strlen(line) >= 2)
	{
		if (strncmp("cd", line, 2) == 0 && (strchr(" \v\t\n", line[2]) || line[2] == 0))
			return (5);
	}
	return (0);
}

/*frees the line after running the builting command.*/
int check_builtin(char *line, t_data *data)
{
	// return tvalue of builts for exit code ?
	int n;
	
	n = built_in_code(line);
	if (n == 0)
		return (0);
	data->tokens = NULL;
	if (n < 7)
		data->tokens = lexer(line, data->env_lst);
	tokens_v2(&data->tokens, data);
	if (n == 1)
		export(data, data->tokens);//safe
	if (n == 2)
		unset(data, data->tokens); // safe
	if (n == 3)
		env(data->env); // safe
	if (n == 4)
		pwd(line); // safe
	if (n == 5)
		cd(line, data); //safe !
	if (n == 6)
		ft_echo2(data, line); // safe !
	if (n == 7)
		ft_exit(line); // leaks double free?
	free(line);
	free_all_tokens(&data->tokens);
	return (n);/*exit code*/
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

	line = NULL;
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
	if (line)
		free(line);
	free_data_variables(&data);
	return (0);
}