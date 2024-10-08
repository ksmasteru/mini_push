#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "../includes/minishell.h"
#include "../includes/parser.h"
#include "../includes/executer.h"

t_lst *new_list(char *str, int len)
{
  int i;
  t_lst *new_node;
  i = 0;
  char *data;
  if (!str)
    return (NULL);
  //printf("---len is %d--- str is %s\n", len, str);
  //exit (0);
  new_node = malloc(sizeof(t_lst));
  new_node->data = (char *)malloc(sizeof(char) * len + 1);
  new_node->data[len] = '\0';
  new_node->data = strncpy(new_node->data, str, len); //ncpy ? len
  new_node->next = NULL;
  new_node->value = NULL;
  return (new_node);
}

void lst_addback(t_lst **head, t_lst *new_lst)
{
  t_lst *tmp;

  if (*head == NULL)
  {
    *head = new_lst;
    return ;
  }
  else
    tmp = *head;
  while (tmp->next)
    tmp = tmp->next;
  tmp->next = new_lst;
}
/*env[i] to key-value separated by =*/
t_lst *get_pair(char *str)
{
  int i;
  t_lst *pair;

  i = 0;
  while (str[i] && str[i] != '=' && str[i] != '\n')
    i++;
  pair = new_list(str, i);
  if (str[i] != '=')
    pair->value = NULL;
  else
  {
    pair->value = new_list(str + i + 1, ft_strlen(str + i + 1));
  }
  return (pair);
}
/* stores the environnement into a list of key-pair values */
void  env_to_lst(char **env, t_data *data) 
{
  int i;
  t_lst *pair;
  i = 0;
  while (env[i])
  {
    pair = get_pair(env[i]);
    add_val_to_env(pair, data);
    i++;
  }
}