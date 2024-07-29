#include "../includes/tokens.h"
#include "../includes/parser.h"
#include "../includes/pipes.h"
#include "../includes/executer.h"
#include "math.h"
// < redirection id followed by the filename
// unless it is a heredoc << it is followed by a delimiter
// hola$HOME
void pop_error(int exit_code, char *str)
{
    ft_putstr_fd(2,  str);
    exit(exit_code);
}
char *expand_quoted_word(char *str, t_lst *env_lst);
char *word_till_quotes(char *str, t_lst *env_lst);
char *qouted_word(char **str, char *start, t_lst *env_lst);
int b = 0;
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
/*$HOME ===> /users/hicham
$RANDOM ==> NULL*/
char *get_expanded_word(char *expand_word, t_lst *env_lst)
{
    // search the env and look for
    char *expanded_word = NULL;
    //if key matches return an instance of value.
    while (env_lst)
    {
       // printf("env_lst key is %s\n", env_lst->data);
        if (strcmp(env_lst->data, expand_word) == 0)
        {
            if (env_lst->value)
            {
                expanded_word = (char *)malloc(sizeof(char) * strlen(env_lst->value->data) + 1);
                expanded_word[strlen(env_lst->value->data)] = '\0';
                expanded_word = strncpy(expanded_word, env_lst->value->data, strlen(env_lst->value->data));
                return (expanded_word);
            }
            else
                return (NULL);
        }
        env_lst = env_lst->next;
    }
    return (NULL);
}

// before$USER$'g$fgfgfg' --->  HICHAM
// add tooke ns "|<>"
char *expantion(char **str, t_lst *env_lst)
{
    int i;
    char *expand_word;
    int len;
    char *expanded_word;
    char *quoted_word;
    i = 0;
    if (**str != '$')
        return (NULL);
    *str = *str + 1;
    // hahi
    if (**str == 34 || **str == 39)
    {
        // $"lol"hii --> LOLhii .. DO NOT EXPAND BUT RETURN THE WORD.
        expanded_word = make_quoted_word(str, **str, env_lst);
        /*while (quoted_word[i] != 0 && quoted_word[i] != '$' && quoted_word[i] != 34 && quoted_word[i] != 39
            && quoted_word[i] != 32 && !(quoted_word[i] >= 9 && quoted_word[i] <= 13))
                i++;*/
        return (expanded_word);
    }
    // find the end of the word dollar sign ?
    else
    {
        while (*(*str + i) != 0 && *(*str + i) != '$' && *(*str + i) != 34 && *(*str + i) != 39
            && *(*str + i) != 32 && !(*(*str + i) >= 9 && *(*str + i) <= 13))
            i++;
    }
    if (i == 0)
        return (NULL);
    len = i;
    //printf("str is %s len is %d\n", *str, len);
    expand_word = (char *)malloc(sizeof(char) * len + 1);
    expand_word[len] = '\0';
    i = 0;
    while (i < len)
    {
        expand_word[i++] =  **str;// USER
        *str = *str + 1;
    }
    expanded_word = get_expanded_word(expand_word, env_lst);//USER -->HICHAM
    return (expanded_word);
}
/*char *expand_quoted_word(char **quoted_word, t_lst *env_lst)
{
    // number of start qoutes = number of end quotes.
}*/


char *close_expanded_word(char *whole_word, char **str)
{
    int i;
    char *quotes;
    int len;

    i = 0;
    while (*(*str + i) != 0)
        i++;
    quotes =  (char *)malloc(sizeof(char) * i + 1);
    quotes[i] = '\0';
    len = i;
    i = 0;
    while (i < len)
    {
        quotes[i++] = **str;
        *str = *str + 1;
    }
    return (ft_strjoin(whole_word, quotes));
}
// '$PWD'FFFFFF'$HOME' ---> 'PWD'FFFFF'HOME'

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

/*'''''$USER'''' ----> '''''HICHAM'''''
* normalword ----> normal_word
* word$USER ---->wordhicham
*/
char *expand_quoted_word(char *str, t_lst *env_lst)
{
    //printf("word to expand is %s\n", str);
    char *whole_word;
    whole_word = NULL;
    if (!str)
        return (NULL);
    while (*str)
    {
        if (*str == '$')
        {
            whole_word = ft_strjoin(whole_word, expantion(&str, env_lst));
            continue;
        }
        else
            whole_word = ft_strcat(whole_word, *str);
        str++;
    }
    //printf("expanded word is %s\n", whole_word);
    return (whole_word);
}

// expanded the afterdollar string and joins it to whole_word
char *after_dollar_word(char **str, char *whole_word, t_lst *env_lst)
{
    char *before_word;
    char *expanded_word;
    int c;

    while (**str != 0 && **str != 32 && !(**str >= 9 && **str <= 13))
    {
        if (**str == '$')
        {
            whole_word = ft_strjoin(whole_word, expantion(str, env_lst));            
            continue;
        }
        if (**str == 34 || **str == 39)
        {
            c = **str;
            before_word = make_quoted_word(str, **str, env_lst);
            if (c == 34)
                expanded_word = expand_quoted_word(before_word, env_lst);
            else
                expanded_word = before_word;
            whole_word = ft_strjoin(whole_word, expanded_word);
            continue;
        }
        *str = *str + 1;
    }
    return (whole_word);
}

/*
* adnnan$toExpand ----> before+EXPANDED
*   changed ft_strjoin to free  s1 and s2 
*/
char *expand_word(char **str, char *start, t_lst *env_lst
,char closing_quotes)
{
    char *before_word;
    char *expanded_word;
    char *whole_word;
    char c;

    whole_word = NULL;
    before_word = word_till_dollar(str, start);//adnan
    expanded_word = expantion(str, env_lst);//hicham
    whole_word = ft_strjoin(before_word, expanded_word);//adnanhicham
    if (before_word)
        free(before_word);
    if (expanded_word)
        free(expanded_word);
    if (**str != 0 && **str != 32 && !(**str >= 9 && **str <= 13))//tokens
        return (after_dollar_word(str, whole_word, env_lst));
    return (whole_word);//this wasnt added.
}

t_token_type decode_type(char *start)
{

    if (*start == '|')
        return (PIPE);
    else if (*start == '<' && *(start + 1) == '<') // this might SEGv
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
/*return the word insdide quotes quotes included*/
/*functionn changed for norminette*/
char *inside_quoted_word(char **str ,int c)
{
    int qoutes_counter;
    char *inside_quotes_word;
    int i;

    qoutes_counter = 1;
    i = 0;
    if (*(++*str) == '\0')
        pop_error(1, "error no closing quote found\n");
    inside_quotes_word = (char *)malloc(sizeof(char) * (ft_strlen(*str) + 2));
    inside_quotes_word[ft_strlen(*str) + 1] = '\0';
    inside_quotes_word[i++] = c;
    while (**str != '\0')
    {
    if (**str  == c)
        qoutes_counter++;
    inside_quotes_word[i++] = *(*str)++;
    if ((qoutes_counter % 2 == 0 && abs(**str - c) == 5) || (qoutes_counter % 2 == 0 && strchr(" \t\v\r", **str))) // '''echo''' 34 39
          break;
    }
    if (qoutes_counter % 2 == 1)
        pop_error(1, "quotes arent balanced\n");
    inside_quotes_word[i] = '\0';
    return (inside_quotes_word);
}

char *clean_quotes_from_word(char *res, int j, int c);
//"Hola"$PWD --> "Hola"$PWD
/*
*   "expanded_word"'Nonexpandedword'expandedword
*
*/
char *make_quoted_word(char **str, int c ,t_lst *env_lst)
{
    char *whole_word = NULL;
    char *inside_quotes_word;
    char *outside_quotes_word;
    while (**str == 34 || **str == 39)
    {
        c = **str;
        inside_quotes_word = inside_quoted_word(str, **str);
        if (c == 34 && strchr(inside_quotes_word, '$'))
            inside_quotes_word = expand_quoted_word(inside_quotes_word, env_lst);
        inside_quotes_word = clean_quotes_from_word(inside_quotes_word, ft_strlen(inside_quotes_word), c);
        whole_word = ft_strjoin(whole_word, inside_quotes_word);
    }
    if (strchr(" \v\t\r\n", **str) || **str == 0)
        return (whole_word);
    else if (strchr(*str, 34) || strchr(*str, 39))
        return(ft_strjoin(whole_word, qouted_word(str, *str, env_lst)));
    else
        return (ft_strjoin(whole_word, word_till_space(str, env_lst)));
}

char *clean_quotes_from_word(char *quoted_word, int len, int qoute)
{
  char *clean_word;
  int i;
  int x;

  i = 0;
  x = 0;
  clean_word = (char *)malloc(sizeof(char) * (len + 1));
  if (!clean_word)
      return (NULL);
  clean_word[len] = '\0';
  while (quoted_word[i] != '\0')
  {
    if (quoted_word[i] != qoute)
        clean_word[x++] = quoted_word[i++];
    else
        i++;
  }
  clean_word[x] = '\0';
  free(quoted_word);
  return (clean_word);
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

// word"$USER"ggggg'ls' ---> wordhichamfgggggls; str doesnt always point to starting quote
char *qouted_word(char **str, char *start, t_lst *env_lst)
{
    char *word_before_qoutes = word_till_quotes(start, env_lst);
    while (**str)
    {
        if (**str == 34 || **str == 39)
            break;
        *str = *str + 1;
    }
    return (ft_strjoin(word_before_qoutes, make_quoted_word(str,  **str, env_lst))); 
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
    new->location = (t_slice){start, length};
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
}
void show_env(t_data *data);
int parse_cmd(char *line, char **envp, t_data *data)
{
    t_token *tokens;
    t_tree *root;


    data->envp = envp;
    data->words_count = 1;
    data->flag = 0;
    /*if (data->env_lst == NULL)
    {
        printf("is null ()\n");
        data->env_lst = env_to_lst(envp); // you reassgin here.
    }*/
    tokens = lexer(line, data->env_lst);
	tokens_v2(&tokens, data);
    //test_tokens(tokens);
    root = parser(tokens, data);
    data->env = get_envp(data->envp);
    if (access(".tmp.txt", F_OK)) /*create it in /tmp file with a ranndom name*/
        unlink(".tmp.txt");
    if (data->words_count > 1)
        run_multiple_commands(root ,data);
    else
        execute_cmd(root, 0, 1, data);
    exit (0);
}