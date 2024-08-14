#include "../includes/minishell.h"
#include "../includes/parser.h"
#include "../includes/pipes.h"
#include "../includes/tokens.h"
#include "sys/wait.h"
#include "../includes/executer.h"
#include <stdbool.h>

// echo alone ?? freeing tokens // running with other commands
void ft_echo2(t_data *data, char *line)
{
    t_token *tokens;
    t_token *tmp;
    bool n;

    tokens = lexer(line, data->env_lst);
	tokens_v2(&tokens, data);
    tmp = tokens->up;
    if (!tmp)
        return ;
    n = check_echo_n(tmp);
    if (!n)
    {
        ft_echo(tmp, n);
        if (tmp->up)
            write(1, " ", 1);
    }
    tmp = tmp->up;
    while (tmp)
    {
        ft_echo(tmp, n);
        tmp = tmp->up;
        if (tmp)
            write (1, " ", 1);
    }
    free_ls(&(tokens), 2);
}

bool check_echo_n(t_token *tmp)
{
    int i;

    i = 1;
    if (!tmp)
        return (false);
    if (tmp->location.location[0] == '-')
    {
         while (i < tmp->location.lenght)
         {
            if (tmp->location.location[i++] != 'n')
                return (false);
         }
         return true;
    }
    else
        return (false);
}