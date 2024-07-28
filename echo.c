#include "minishell.h"
#include "tokens.h"
#include "parser.h"
#include "executer.h"

void ft_putstr(int fd, char *str)
{
  int i;

  i = 0;
  while(str[i])
  {
    write (fd,  str + i, 1);
    i++;
  }
}
void echo(char *str)
{
  // stt it is already parsed.
  // exec ---- > fork // check if echo in args[1]
  int i;

  i = 0;
  while (str[i])
  {
    write (1, str + i, 1);
    i++;
  }
}

void env(char **env)
{
  int i;

  i = 0;
  while (env[i])
  {
    echo(env[i++]);
    write (1, "\n", 1);
  }
}
// return the lent of key : also handles some errors
int find_key(t_slice *slice)
{
  int i;

  i = 0;
  if ((slice->location)[i] == '=')
  {
    write (1, "export: `", 10);
    i = -1;
    while (++i < slice->lenght)
      write (1, (slice->location) + i, 1);
    write(1, "': not a valid identifier", 26);
    return (-1);
  }
  else
  {
    while (i < slice->lenght)
    {
      if ((slice->location)[i] == '=')
        break;
      i++;
    }
  }
  return (i);
}

int find_value(t_slice *location, int key)
{
  //printf("key points to %d\n",(location->location)[key]);
  //printf("key points to %d\n",(location->location)[key + 1]);
  if ((location->location)[key] == '=')
    return (1);
  else
    return (-1);
}
// scans export arguments
t_lst *export_value(t_data *data, t_token *token)
{
  int key;
  int value;
  t_lst *key_val;
  t_lst *val_val;
  
  key = find_key(&(token->location));
  if (key == -1)
    return (NULL);
  value = find_value(&(token->location), key);
  if (value == -1)
    val_val = (NULL);
  else
  {
    //token->location.location = token->location.location + k
    // key  + 1 to skip =
    token->location.location[token->location.lenght] = '\0';
    val_val = new_list((token->location.location + key + 1), ft_strlen(token->location.location + key + 1));
  }
  token->location.location[key] = '\0';
  key_val = new_list(token->location.location, key);
  //printf("key is %s\n", key_val->data);
  if (val_val)
  //printf("value is %s\n", val_val->data);
  key_val->value = val_val;
  return (key_val);
}

int swap_if_key(t_lst **head, t_lst *pair)
{
  t_lst *tmp;
  char *holder;

  // leaks bdel node kaml ?
  if (!pair)
    return (-1);
  tmp = *head;
  while(tmp)
  {
    if (strcmp(tmp->data, pair->data) == 0)
    {
      if (pair->value) // only if there is value
        tmp->value = pair->value; // i changed they key
      return (1);
    }
    tmp = tmp->next;
  }
  return (-1);
}

void  add_val_to_env(t_lst *pair, t_data *data)
{
  char *new_val;
  char *holder;
  t_lst *new_lst;
  if (!pair)
    return ;
  if (swap_if_key(&(data->env_lst), pair) < 0)
    lst_addback(&(data->env_lst), pair);
}

void show_env(t_data *data, int is_export)
{
  t_lst *tmp;
  char c = 34;
  tmp = data->env_lst;
  while (tmp)
  {
    if (is_export == 1)
      ft_putstr(1, "declare -x ");
    ft_putstr(1, tmp->data);
    if (tmp->value)
      write(1, "=", 1);
    else
    {
      write (1, "\n", 1);
      tmp = tmp->next;
      continue;
    }
    write(1, &c, 1);
    if (tmp->value)
      ft_putstr(1, tmp->value->data);
    write(1, &c, 1);
    write(1, "\n", 1);
    tmp = tmp->next;
  }
}
void export(t_data *data, t_token *token)
{
  // export =value fixed
  //token : export >a->b>b->x-->NULL
  t_slice *key;
  t_slice *value;
  t_lst *pair;
  
  // pass to this token->up directly
  if (token->up == NULL)
    return (show_env(data, 1));//should changee
  else
    token = token->up;
  /*token->location.location[token->location.lenght] = '\0';
  printf("token data is %s", token->location.location);
  
  exit(0);*/
  // spaces are only copied thru '' "" 
  if (*(token->location.location) == 32 || (*(token->location.location) >= 9 && *(token->location.location) <= 13))
  {
    write (2, "export: not valid in this context:\n", 36);
    while (*(token->location.location))
    {
      if (*(token->location.location) != '=')
        write (2, (token->location.location), 1);
      (token->location.location)++;
    }
    // handle error
    return ;
  }
  while (token)
  {
    pair = export_value(data, token);
    add_val_to_env(pair, data);
    token = token->up;
  }
//show_env(data);
}
void ft_lstdelete(t_lst *head, t_lst *to_delete)
{
    t_lst *tmp;
    t_lst *prev;
    if (!to_delete)
        return ;
    prev = NULL;
    tmp = head;
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
            return (ft_lstdelete(data->env_lst, tmp));
        tmp = tmp->next;       
    }
}