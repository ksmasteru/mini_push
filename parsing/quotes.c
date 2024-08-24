#include "../includes/tokens.h"
#include "../includes/parser.h"
#include "../includes/pipes.h"
#include "../includes/executer.h"
#include "math.h"

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
//"Hola"$PWD --> "Hola"$PWD
/*
*   "expanded_word"'Nonexpandedword'expandedword
*
*/


char* join_and_free(char *s1, char *s2)
{
    char *res;

    res = ft_strjoin(s1, s2);
    if (s1)
    {
        //printf("gonig to free %p\n", s1);
        free(s1);
    }
    if (s2)
    {
        //printf("gonig to free %p\n", s2);
        free(s2);
    }
    return res;
}

char *make_quoted_word(char **str, int c ,t_lst *env_lst)
{
    char *whole_word = NULL;
    char *inside_quotes_word;
    char *outside_quotes_word;
    char *tmp = NULL;
    //printf("str value is %d\n", **str);
    //printf("before loop str points to %s\n", *str); //"HELLO"ls'a'
    //
    while (**str == 34 || **str == 39)
    {
        c = **str;
        inside_quotes_word = inside_quoted_word(str, **str);
        //printf("inside quotes word %s\n", inside_quotes_word);
        //inside_quotes_word = clean_quotes_from_word(inside_quotes_word, ft_strlen(inside_quotes_word), c);
        if (c == 34 && strchr(inside_quotes_word, '$'))
        {
            tmp = inside_quotes_word;
            inside_quotes_word = expand_quoted_word(inside_quotes_word, env_lst);
            //printf("inside quotes word %s is\n", inside_quotes_word);
            if (tmp)
                free(tmp);
            tmp = NULL;
        }
        else
            inside_quotes_word = clean_quotes_from_word(inside_quotes_word, ft_strlen(inside_quotes_word), c);
        whole_word = join_and_free(tmp, inside_quotes_word);
        tmp = whole_word;
    }
    
    //printf("after loop whole word is %s and str points to %s\n", whole_word, *str);  // a
    
    if (strchr(" \v\t\r\n", **str) || **str == 0)
        return (whole_word);
    else if (strchr(*str, 34) || strchr(*str, 39))
        return(join_and_free(whole_word, qouted_word(str, *str, env_lst)));
    else
        return (join_and_free(whole_word, word_till_space(str, env_lst)));
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

// word"$USER"ggggg'ls' ---> wordhichamfgggggls; str doesnt always point to starting quote
char *qouted_word(char **str, char *start, t_lst *env_lst)
{
    char *word_before_qoutes;
    char *complete_word;
    char *qouted_word;

    word_before_qoutes = word_till_quotes(start, env_lst);
    while (**str)
    {
        if (**str == 34 || **str == 39)
            break;
        *str = *str + 1;
    }
    qouted_word = make_quoted_word(str, **str, env_lst);
    //printf("quoted_word is %s\n", qouted_word);
    complete_word = join_and_free(word_before_qoutes, qouted_word);
    /*complete_word = ft_strjoin(word_before_qoutes, qouted_word);
    if (word_before_qoutes)
        free(word_before_qoutes);
    if(qouted_word)
        free(qouted_word);*/
    
    return (complete_word); 
}

char *expand_quoted_word(char *str, t_lst *env_lst)
{
    //printf("word to expand is %s\n", str);
    char *whole_word;
    char *tmp;
    //$HOMEls
    whole_word = NULL;
    if (!str)
        return (NULL);
    while (*str)
    {
        if (*str == '$')
        {
            whole_word = join_and_free(whole_word, expantion(&str, env_lst));
            //printf("whole word is %s\n", whole_word);
            continue;
        }
        else
        {
            tmp = whole_word;
            whole_word = ft_strcat(whole_word, *str);
            if (tmp)
                free(tmp);
        }
        str++;
    }
    // free str if word is nnull;
    //printf("expanded word is %s\n", whole_word);
    return (whole_word);
}


