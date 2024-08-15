#include "../includes/tokens.h"
#include "../includes/parser.h"
#include "../includes/pipes.h"
#include "../includes/executer.h"
#include "math.h"

char *word_till_space(char **str, t_lst *env_lst)
{
    int i;
    char *word;
    char *start;
    int len;
    start = *str;
    i = 0;
    while (start[i] && !strchr(" \v\t\r", start[i]))
        i++;
    word = (char *)malloc(sizeof(char)  * (i + 1));
    word[i] = '\0';
    len = i;
    i = 0;
    while (i < len)
    {
        word[i] = start[i];
        *str = *str + 1;
        i++;
    }
    // should also be expanded
    word = expand_quoted_word(word, env_lst);
    //printf("word before dollar is %s and str points to %c\n", word, **str);
    return (word);
}
char *word_till_dollar(char **str, char *start)
{
    int i;
    char *word;

    i = 0;
    while (start[i] && start[i] != '$')
    {
        //printf("start[i] is %c\n", start[i]);
        i++;
    }
    //printf("i is %d\n", i);
    word = (char *)malloc(sizeof(char)  * (i + 1));
    if (!word)
        return (NULL);
    word[i] = '\0';
    i = 0;
    while (start[i] && start[i] != '$')
    {
        word[i] = start[i];
        i++;
    }
    if (start == *str)
        *str = *str + i;// newly addded
    //printf("word before dollar is %s and str points to %c\n", word, **str);
    return (word);
}

char *ft_strcat(char *str, char c)
{
    int len;
    int i;
    char *new_str;
    
    i = 0;
    if (str == NULL)
    {
        new_str = (char *)malloc(sizeof(char) * 2);
        new_str[0] = c;
        new_str[1] = 0;
    }
    else
    {
        len = strlen(str) + 1;
        new_str = (char *)malloc(sizeof(char) * len + 1);
        new_str[len] = '\0';
        while (i < len - 1)
        {
            new_str[i] = str[i];
            i++;
        }
        new_str[i] = c;
    }
        return (new_str);
}

char *word_till_quotes(char *str, t_lst *env_lst)
{
    int i;
    char *res;
    int j;
    int len;

    j = 0;
    i = 0;
    while (str[i])
    {
        if (str[i] == 34 || str[i] == 39)
            break;
        i++;
    }
    if (i == 0)
        return (NULL);
    res = (char *)malloc(sizeof(char) * (i + 1));
    len = i;
    i = 0;
    while (i < len)
        res[j++] = str[i++];
    res[j] = '\0';
    if (strchr(str, '$'))
        return expand_quoted_word(str, env_lst);
    return (res);
}

void pop_error(int exit_code, char *str)
{
    ft_putstr_fd(2,  str);
    exit(exit_code);
}

void free_ls(t_token **token, int direction)
{
     t_token *tmp;
     t_token *holder;

     tmp = *token;
     while (tmp)
     {
        if (direction == 1)
            holder = tmp->next;
        else if (direction == 2)
            holder = tmp->up;
        else
            holder = tmp->down;
        if (tmp->location.is_malloced)
            free(tmp->location.location);
        free(tmp);
        tmp = holder;
     }
     *token = NULL;
}

void free_tokens(t_tree *tree)
{
     if (tree->token)
     {
          free_ls(&(tree->token->up), 2);
          free_ls(&(tree->token->down), 3);
     }
}

void _free_data(t_data *data)
{
    int i;

    i = 0;
    while (data->fdx[i])
        free(data->fdx[i++]);
    free(data->fdx);
    free(data->pids);
}