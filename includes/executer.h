#ifndef EXECUTER_H
#define EXECUTER_H

#include "parser.h"
#include "tokens.h"

char    *ft_strdup2(char *s1);
char	**get_paths(int ac, char ***ultimate, char **envp);
char	*get_path(char **paths, char *cmd);
void	free_it(char ***ultimate, char **paths, int **p, int n);
void	free_it_2(int **p, int n);
int	count_words(char *cmd, char sep);
char	*make_words(char *str, char sep);
char	**ft_split(char *cmd, char sep);
void	free_splited(char ***res, int i);
char	***split_all(int ac, char **av);
char	**get_envp(char **env);
int	ft_strncmp_n(char *str, char *str2);
int	ft_strncmp(char *s1, char *s2, size_t n);
int	ft_strcmp(char *s1, char *s2);
int ft_strlen(char *str);
int	ft_putstr_fd(int fd, char *str);
char	*ft_strjoin(char *s1, char *s2);
char	*ft_strjoin2(char *s1, char *s2, char *str);
int manage_redirections(t_token *redirection);
int do_redirect(t_token *redirection);
char *get_file_name(char *location, size_t lent);
void run_multiple_commands(t_tree *root ,t_data *data);
int parse_cmd(char *line, char **envp, t_data *data);
void handle_sigint2(int sig);
void handle_sigquit(int sig);
void handle_sigint1(int sig);
void ft_echo2(t_data *data, char *line);
bool check_echo_n(t_token *tmp);
int set_exec_args(t_tree *head, t_data *data, char ***args, char **cmd);
void free_exec_args(char **args, char *cmd, t_tree *head);
#endif