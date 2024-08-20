#ifndef TOKENS_H
#define TOKENS_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "minishell.h"
#include <string.h>
#include <stdbool.h>

typedef enum s_token_type{
    ERROR = 0,
    WORD = 1,
    SPECIAL = 2,
    PIPE = 3,
    AMPER = 4,
    FROM = 6,
    TO = 7,
    HEREDOC = 8,
    APPEND = 9,
    PAREN_L = 10,
    PAREN_R = 11,
    OR = 12,
    AND = 13,
    WORDS = 14,
    IN_FILE = 14,
    OUT_FILE = 16,
    DELIMETER = 17,
    REIDRECTION = 18,
}t_token_type;

typedef struct s_slice{
    char *location;
    size_t lenght;
    bool is_malloced;
}t_slice;

typedef struct s_token{
    t_token_type type;
    t_slice location;
    struct s_token *next;
    struct s_token *up;
    struct s_token *down;
}t_token;

typedef struct s_tree{
    t_token_type type;
    t_token *token;
    struct s_tree *left;
    struct s_tree *right;
}t_tree;

typedef struct s_redirection
{
    t_token_type type;
    
}t_redirection;

typedef struct s_queue{
    t_tree *addr;
    struct s_queue *next;
}t_queue;

typedef struct s_data{
    char *str;
    t_token *tokens;
    t_tree **head;
    int **fdx;
    t_lst *env_lst;
    t_lst *mem_ref;
    int index;
    int len;
    char *line;
    int flag;
    int words_count;
    int pipes_count;
    int *pids;
    char **envp;
    char **env; 
}t_data;

t_token *make_new_node(t_token_type type, char *start, size_t length);
void add_new_token(t_token **head , char **str, char *start, size_t length);
t_token  *lexer(char *str, t_lst *env_lst);
t_tree *make_pipe_cmd(t_tree *left, t_tree *right);
t_tree *make_and_cmd(t_tree *left, t_tree *right);
t_tree *make_or_command(t_tree *left, t_tree *right);
t_tree *make_great_command(t_tree *left, t_tree *right);
void make_quote_token(t_token **head, char **str);
void    tokens_v2(t_token **tokens, t_data *data);
void	*m_alloc(size_t __size, char todo);
void merge_words(t_token **current, t_token **next);
void token_overdrive(t_token **tokens);
void left_root_right(t_tree **root);
void check_syntax_error (t_token **tokens);
int syntax_error(int code, t_token **tokens);
void free_ls(t_token **token, int direction);
void free_tokens(t_tree *tree);
#endif