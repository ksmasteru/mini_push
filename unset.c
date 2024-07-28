#include "minishell.h"
#include "tokens.h"
#include "parser.h"
#include "executer.h"

void ft_lstdelete(t_lst **head, t_lst *to_delete)
{
    t_lst *tmp;
    t_lst *prev;
    if (!to_delete)
        return (NULL);
    prev = NULL;
    tmp = *head;
    while (tmp != to_delete)
    {
        prev = tmp;
        tmp = tmp->next;
    }
    if (prev)
        prev->next = to_delete->next;
    if (to_delete->value)
        free(to_delete->value);
    free(to_delete);
}
/*
* unset a token variable form env_lst
*/
void unset(t_data *data, t_token *token)
{
    if (token == NULL)
        return ;
    t_lst *tmp;

    tmp = data->env_lst;
    while (tmp)
    {
        if (strncmp(tmp->data, token->location.location, token->location.lenght) == 0)
            return (ft_lstdelete(&(data->env_lst), tmp));
        tmp = tmp->next;       
    }
}