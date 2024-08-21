#include "executer.h"



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
		if (strncmp("pwd", line, 3) == 0 && (line[3] == 32 || line[3] == 0))
		{
			pwd(line);
			return (1);
		}
	}

	if (ft_strlen(line) >= 4)
	{
		if (strncmp("exit", line,  4) == 0  && (line[4] == 32 || line[4] == 0)) // or tab
			ft_exit(line);
	}

	if (ft_strlen(line) >= 4)
	{
		if (strncmp("echo", line, 4) == 0 && (line[4] == 32 || line[4] == 0))
		{
			ft_echo2(data, line);
			free(line);
			return (1);
		}
	}

	return (0);
}


int check_builtin(char *line, t_data *data)
{
	if (line[0] == 'c' && line[1] == 'd' && (line[2] == 32 || (line[2] >= 9 && line[2] <= 13) || line[2] == 0))
	{
		cd (line, data);// "cd home" // skip spaces at the start
		free(line);
		return (1);
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