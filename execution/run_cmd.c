#include "../includes/tokens.h"
#include "../includes/executer.h"
#include "../includes/parser.h"
#include "../includes/pipes.h"
#include "../includes/executer.h"
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>

void unset(t_data *data, t_token *token);
void show_env(t_data *data);
void free_2d_str(char **str)
{
    int i;

    i = 0;
    while (str[i])
        free(str[i++]);
    free(str[i]);
    free(str);
}

int ft_strlen(char *s1)
{
     int i;

     i = 0;
     if (!s1)
          return (0);
     while (s1[i])
          i++;
     return (i);
}

int close_pipes(int **p, int index, int len)
{
     int i;

     i = 0;
     while (i < len)
     {
          if (i != 0 && i != index - 1)
               close(p[i][0]);
          if (i != index)
               close(p[i][1]);
          i++;
     }
     return (0);
}

int	fill_pipes(int ***p, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		if (pipe((*p)[i++]) != 0)
          {
               perror("pipe");
			return (-1);
          }
	}
	return (0);
}

char *slice_and_dice(t_slice slice)
{
     char *res;

     res = malloc(sizeof(char) * (slice.lenght + 1));
     if (!res)
          return (NULL);
     res[slice.lenght] = '\0';
     strncpy(res, slice.location, slice.lenght);
     return(res);
}
char **get_word_args(t_tree *head)
{
     int words_number;
     t_token *tmp;
     char **args;
     int i;

     i = 0;
     words_number = 0;
     tmp = head->token;
     while (tmp)
     {
          words_number++;
          tmp = tmp->up; 
     }
     args = (char **) malloc(sizeof(char *) * (words_number + 1));
     if (!args)
          perror("args");
     tmp = head->token;
     while (i < words_number)
     {
          args[i] = slice_and_dice(tmp->location);
          if (!args[i])
               perror("args[i]");
          i++;
          tmp = tmp->up;
     }
     args[i] = NULL;
     return  (args);
}

int close_all_pipes(int **pfx, int len)
{
     int i;

     i = 0;
     while (i < len - 1)
     {
          close(pfx[i][0]);
          close(pfx[i][1]);
          i++;
     }
     return (0);
}

/* test again with bash not zsh*/
int close_and_dup2(int **pfx, int index, int len, int flag)
{

     if (index == 0 && flag != 1)
          dup2(pfx[0][1], 1);
     else if (index == len - 1)
     {
          if (flag != 2) /*newly added but it shoulld workd*/
               dup2(pfx[index - 1][0], 0);
     }
     else if (index != 0 && index != len - 1)
     {
          if (flag != 2) /*new*/
               dup2(pfx[index - 1][0], 0);
          if (flag != 1)
               dup2(pfx[index][1], 1);
     }
     close_all_pipes(pfx, len);
     return (0);
}

// shoud pass data by refrence env_lst so the changes takes place
void built_in(int op, t_data *data, t_token *token)
{
     //printf("builtin runned\n");
     if (op ==  1)
          return (export(data, token));
     if (op == 2)
          return (unset(data, token->up));
}

//ls | cat | cat | cat
int run_cmd_main(char **args, char *cmd, t_tree *head, t_data *data)
{
     // reduce the ifs
     int i;

     i = 0;
     if (data->index != 0) // triple wc -w works with wait only
          wait(NULL);
    // while (i++ < data->words_count)
         // wait(NULL);
     //if (data->index != 0)
       //   waitpid(data->pids[data->index - 1], NULL, 0);
     if  (head->type == WORD)
     {
          if (head->token->down != NULL)
               data->flag = manage_redirections(head->token->down); //return 1 on error
     }
     else if (head->type == REIDRECTION)
          data->flag = manage_redirections(head->token); 
     if (data->words_count != 1)
          close_and_dup2(data->fdx, data->index, data->words_count, data->flag);
     if (args != NULL)
     {
          if (strcmp(args[0], "export") == 0)
               built_in(1, data, head->token);
          else
          {
               //write (1, "run exec cmd", 13);
               if (execve(cmd, args, data->envp) < 0)
               {
                    perror("");
                    ft_putstr_fd(2, cmd);
               }
          }
     }
     exit(0);
}

char *get_file_name(char *location, size_t lent)
{
     char *file_name;
     file_name = (char *)malloc(sizeof(char) * (lent + 1));
     if (!file_name)
          return (NULL);
     file_name[lent] = '\0';
     strncpy(file_name, location, lent);
     return (file_name);
}

void redirect_in_out(char *file_name, char mode)
{
     int fd;
     int type;

     if (mode == 0)
          type = 0;
     else if (mode == 1)
          type = O_RDWR | O_CREAT | O_TRUNC;
     else if (mode == 2)
          type = O_RDWR | O_APPEND | O_CREAT;
     fd = open(file_name, type, 0644);
     if (fd == -1)
          perror("open");
     dup2(fd, mode - (mode % 1));
     close(fd);
}

void here_doc(t_slice *slice)
{
          char *str;
          int fd;
     
          if (access(".tmp.txt", F_OK) == 0)
               unlink(".tmp.txt");
          fd = open(".tmp.txt", O_RDWR | O_APPEND | O_CREAT, 0644);
          if (fd == -1)
               perror("open");
          slice->location[slice->lenght] = '\0';
          while (1)
          {
		     str = readline("heredoc > ");
		     if (str == NULL)
                    continue;
		     if (strcmp(str, slice->location)== 0)
			{
                    free(str);
                    break ;
               }
		     write(fd, str, ft_strlen(str));
               write(fd, "\n", 1);// should it be \n'\0' test later
		     free(str);
          }
          close(fd);
          fd = open(".tmp.txt", O_RDWR | O_CREAT, 0644);
          if (fd == -1)
               perror("open");
          dup2(fd, 0);
          close(fd);
}

int do_redirect(t_token *redirection)
{
     char *file_name;

     if (redirection == NULL)
          return (1);
     file_name = get_file_name(redirection->location.location, redirection->location.lenght);
     if (!file_name)
          perror("filename");
     if (redirection->type == IN_FILE)
          redirect_in_out(file_name, 0);
     else if (redirection->type == OUT_FILE)
     {
          redirect_in_out(file_name, 1);
          return (1);
     }
     else if (redirection->type == APPEND)
     {
          sleep(5); // append doesnt work yeet
          redirect_in_out(file_name, 2);
          return (1);
     }
     else if (redirection->type == DELIMETER)
     {
          here_doc(&(redirection->location));
          return (2);
     }
     return (0);
}

int manage_redirections(t_token *redirection)
{
     t_token *temp;
     int flag;
     flag = 0;
     temp = redirection;
     while (temp)
     {
          flag = do_redirect(temp->up);
          temp = temp->down;
     }
     return (flag);
}

int execute_cmd(t_tree *head, int index, int len, t_data *data)
{
     char **args;
     char *cmd;
     int i = 0;
     data->flag = 0;
     data->index = index;
     if (head->type == REIDRECTION)
     {
          args = NULL;
          cmd = NULL;
     }
     else
     {
          args = get_word_args(head);
          if (!args)
               perror("args");
          cmd = get_path(data->env, args[0]);
     }
     if (index == len - 1 || len == 1) /* change inside the child ? this will affect all
          //subsequent procces..  this was only for first and last command*/
     {

          return (run_cmd_main(args, cmd, head, data));
     }
     data->pids[index] = fork();
     if (data->pids[index] == 0)
     {
          //printf("running cmd %s on process[%d] and index %d\n", cmd, getpid(), index);
          if  (head->type == WORD)
          {
               if (head->token->down != NULL)
                    data->flag = manage_redirections(head->token->down);
          }
          /*what if onnly heredoc with no program name*/
          else if (head->type == REIDRECTION)
               data->flag = manage_redirections(head->token);
          if (index != 0)
          {
               while (i <= index - 1)
               waitpid(data->pids[i++], NULL, 0);
          }
          close_and_dup2(data->fdx, index, data->words_count, data->flag);
          if (args != NULL)
          {
               if (strcmp(args[0], "export") == 0)
               {
                    write(1, "multiple called builtin\n", 25);
                    built_in(1, data, head->token);
               }
               else
               {
                    //write(1, "execve run", 11);
                    if (execve(cmd, args, data->envp) < 0)
                    {
                         perror("");
                         ft_putstr_fd(2, cmd);
                    }
               }
          }
          exit(0);
     }
     else
     {
          if (args)
               free_2d_str(args);
          if (cmd)
               free(cmd);
          free_tokens(head);
          free(head);
          return (0);
     }
}

void run_cmd(t_tree **head, int index, int len, t_data *data)
{
     if (*head == NULL)
          return ;
     if ((*head)->type == WORD || (*head)->type == REIDRECTION)
          return ;
     run_cmd(&((*head)->left), index - 1, len, data);
     if ((*head)->left->type == WORD || (*head)->left->type == REIDRECTION) 
     {
          execute_cmd((*head)->left, index - 1, len, data);
          execute_cmd((*head)->right, index, len, data);
     }
     else
     {
          execute_cmd((*head)->right, index, len,data);
         // waitpid(data->pids[index], NULL, 0);
     }
}
