#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>
#include "tokens.h"
#include "minishell.h"
typedef struct s_stack{
    t_tree *addr;
    struct s_stack *next;
}t_stack;


char **get_word_args(t_tree *head);
char *slice_and_dice(t_slice slice);
int parse_cmd(char *line, char **envp, t_data *data);
void run_multiple_commands(t_tree *root ,t_data *data);
void test_tokens(t_token *tmp);
t_tree *parser(t_token *head, t_data *data);
t_tree *parse_complete_cmd(t_token **head, t_data *data);
t_tree *parse_and_or(t_token **head, t_data *data);
t_tree *parse_pipe_line(t_token **head, t_data *data);
t_tree *parse_command(t_token **head, t_data *data);
t_tree *parse_simple_command(t_token **head, t_data *data);
t_tree *parse_redirect(t_token **head, t_data *data);
t_tree *parse_word(t_token **head, t_data *data);
t_tree *make_exec_command(t_token *word);
t_tree *make_great_command(t_tree *left, t_tree *right);
t_tree *make_or_command(t_tree *left, t_tree *right);
t_tree *make_and_new(t_tree *left, t_tree *right);
t_tree *make_pipe_cmd(t_tree *left, t_tree *right);
void bfs(t_tree **root);
t_queue *pop(t_queue **head);
t_queue *new_follower(t_queue **head, t_queue **prev, t_tree *addr);
void pre_order_traversal(t_tree **root);
t_stack *pop_stack(t_stack **head);
t_stack *new_node(t_stack **head, t_tree *addr);
void tokens_v3(t_token **tokens);
void ft_list_add_back(t_token **head, t_token *new);
void tokens_v2(t_token **tokens, t_data *data);
void merge_all_words(t_token **head);
t_token *tokens_v4(t_token *start);
void *tokens_v5(t_token **tokens);
void word_add_down(t_token **word, t_token *redir);
void swap_redir_word(t_token **redir, t_token *word);
void improve_tokens(t_token **tokens, t_data *data);
void ft_list_addback(t_token **head, t_token *new);
t_token *tokens_v6(t_token **tokens);
t_token *merge_simple_command(t_token **words_list, t_token  **redirection_list);
void merge_the_redirections(t_token **redirection_list, t_token *new_red);
void merge_the_words(t_token **words_list, t_token *new_word);
char *ft_strnchrs(char *str, int c, size_t len);
size_t make_word_with_quotes(t_token **head, char *start);
char *make_quoted_word(char **str, int c, t_lst *env_lst);
char *clean_quotes_from_word(char *res, int j, int c);
void env_to_lst(char **env, t_data *data);
void lst_addback(t_lst **head, t_lst *new_lst);
t_lst *new_list(char *str, int len);
void export(t_data *data, t_token *token);
void  add_val_to_env(t_lst *pair, t_data *data);
t_lst *export_value(t_data *data, t_token *token);
int find_value(t_slice *location, int key);
int find_key(t_slice *slice);
void env(char **env);
void echo(char *str);
bool is_empty(char *line);
char *qouted_word(char **str, char *start, t_lst *env_lst);
char *word_till_quotes(char *str, t_lst *env_lst);
char *clean_quotes_from_word(char *quoted_word, int len, int qoute);
char *make_quoted_word(char **str, int c ,t_lst *env_lst);
char *inside_quoted_word(char **str ,int c);
char *clean_quotes_from_word(char *res, int j, int c);
char *expand_quoted_word(char *str, t_lst *env_lst);
char *word_till_space(char **str, t_lst *env_lst);
void pop_error(int exit_code, char *str);
char *expantion(char **str, t_lst *env_lst);
char *close_expanded_word(char *whole_word, char **str);
char *expand_word(char **str, char *start, t_lst *env_lst
,char closing_quotes);
char *after_dollar_word(char **str, char *whole_word, t_lst *env_lst);
char *expand_quoted_word(char *str, t_lst *env_lst);
char *word_till_dollar(char **str, char *start);
char *ft_strcat(char *str, char c);
char *get_expanded_word(char *expand_word, t_lst *env_lst);
#endif