#include "../includes/tokens.h"
#include "../includes/parser.h"
#include "../includes/pipes.h"
#include "../includes/executer.h"
#include "math.h"

void run_multiple_commands(t_tree *root ,t_data *data)
{
    int *pids;
    int **pfd;
    int i;

    i = 0;
    pids = (int *)malloc(sizeof(int) * (data->words_count - 1));
    pfd = (int **)malloc(sizeof(int *) * data->words_count - 1);
    while (i < data->words_count - 1)
        pfd[i++] = (int *)malloc(sizeof(int) * 2);
    fill_pipes(&pfd, data->words_count - 1);
    data->fdx = pfd;
    data->pids = pids;
    run_cmd(&root, data->words_count - 1, data->words_count, data);
    _free_data(data);
}

int parse_cmd(char *line, char **envp, t_data *data)
{
    t_token *tokens;
    t_tree *root;

    data->envp = envp;
    data->words_count = 1;
    data->flag = 0;
    tokens = lexer(line, data->env_lst);
	tokens_v2(&tokens, data);
    test_tokens(tokens);
    root = parser(tokens, data);
    if (access(".tmp.txt", F_OK))
        unlink(".tmp.txt");
    if (data->words_count > 1)
        run_multiple_commands(root ,data);
    else
        execute_cmd(root, 0, 1, data);
    free_ls(&tokens, 1);
    exit (0);
}