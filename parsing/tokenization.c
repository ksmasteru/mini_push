#include "../includes/tokens.h"
#include "../includes/parser.h"
#include "../includes/pipes.h"
#include "../includes/executer.h"
#include "math.h"

t_token_type decode_type(char *start)
{

    if (*start == '|')
        return (PIPE);
    else if (*start == '<' && *(start + 1) == '<')
        return (HEREDOC);
    else if (*start == '<')
        return (FROM);
    else if (*start == '>' && *(start + 1) == '>')
        return (APPEND);
    else if (*start == '>')
        return (TO);
    else if (*start == '(')
        return (PAREN_L);
    else if (*start == ')')
        return (PAREN_R);
    else
        return (WORD);
}

void set_alloc_flag(t_token **head)
{
    t_token *tmp;
    t_token *holder;

    tmp = *head;
    while (tmp->next)
        tmp = tmp->next;
    tmp->location.is_malloced = true;
}

void words_lexer(t_token **head, char **str, char *start, t_lst *env_lst)
{
    size_t length;
    int word;
    char *new_word; 

    word = 0;
    length = 0;
    while (**str && !(strchr(" \t\v\r|><)(", **str)))
    {
        if (**str == 34 || **str == 39 || **str == '$')
        {
            word = 0;
            if (**str == '$')
                new_word = expand_word(str, start, env_lst, 0);
            else
                new_word = qouted_word(str, start, env_lst);
            add_new_token(head, str, new_word, strlen(new_word));
            if (new_word)
            {
                printf("allocated %s [%p]\n", new_word, new_word);
                set_alloc_flag(head);
            }
            continue;
        }
        word = 1;
        *str = *str + 1;
        length++;
    }
    if (word == 1)
        add_new_token(head, str, start, length);
}

t_token  *lexer(char *str, t_lst *env_lst)
{
    char *start;
    t_token *head;

    start = str;
    head = NULL;
    while (*str)
    {
        while (*str && (strchr(" \t\v\r", *str)))
            str = str + 1;
        start = str;
        if(*str && strchr("|<>()", *str))
        {
            add_new_token(&head, &str, start, 1);
            start = str;
        }
        words_lexer(&head, &str, start, env_lst);
    }
    return (head);
}

t_token *make_new_node(t_token_type type, char *start, size_t length)
{
    t_token *new;

    new = malloc(sizeof(t_token));
    if (!new)
        return (NULL);
    new->type = type;
    new->location = (t_slice){start, length, false};
    new->next = NULL;
	new->up = NULL;
    new->down = NULL;
    return (new);
}

void add_new_token(t_token **head , char **str, char *start, size_t length)
{
    t_token *tmp;
    t_token *new;

    if (start == NULL || str == NULL)
        return ;
    tmp = *head;
    t_token_type type = decode_type(start);
    if (type == HEREDOC || type == APPEND)
        *str = *str + 2;
    else if (type != WORD)
        *str = *str + 1;
    if (*head == NULL)
    {
        *head = make_new_node(type, start, length);
        return ;
    }
    new = make_new_node(type, start, length);
    if (!new)
        return ;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new;
}

void test_tokens(t_token *tmp)
{
    t_token *holder;
    while (tmp)
    {
        holder = tmp;
        printf(" parent type %d\n", tmp->type);
        if (tmp->location.is_malloced)
            printf("yes");
        if (tmp->down)
        {
            printf("  down is %d\n", tmp->down->type);
            if (tmp->down->up)
                printf("redirection type is %d\n", tmp->down->up->type);
        }
        while (holder->up)
        {
            printf("child up is %d\n", holder->up->type);
            printf("child value is %s\n", holder->up->location.location);
            holder = holder->up;
        }
        tmp = tmp->next;
    }
    exit(0);
}
