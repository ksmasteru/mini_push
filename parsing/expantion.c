#include "../includes/tokens.h"
#include "../includes/parser.h"
#include "../includes/pipes.h"
#include "../includes/executer.h"
#include "math.h"

// 
// expanded the afterdollar string and joins it to whole_word
char *after_dollar_word(char **str, char *whole_word, t_lst *env_lst)
{
    char *before_word;
    char *expanded_word;
    int c;

    //printf("before after dollar word is %s and str poins to %s\n", whole_word, *str);
    while (**str != 0 && **str != 32 && !(**str >= 9 && **str <= 13))
    {
        if (**str == '$')
        {
            whole_word = join_and_free(whole_word, expantion(str, env_lst));
            //printf("after expation whole word is %s\n", whole_word);     
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
            whole_word = join_and_free(whole_word, expanded_word);    
            free(before_word);
            continue;
        }
        *str = *str + 1;
    }
    //printf("res from after dollar word is %s\n", whole_word);
    return (whole_word);
}

/*
* adnnan$toExpand ----> before+EXPANDED
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
    whole_word = join_and_free(before_word, expanded_word);
    //whole_word = ft_strjoin(before_word, expanded_word);//adnanhicham
    /*if (before_word)
        free(before_word);
    if (expanded_word)
        free(expanded_word);*/
    if (**str != 0 && **str != 32 && !(**str >= 9 && **str <= 13))//tokens
        return (after_dollar_word(str, whole_word, env_lst));
    return (whole_word);//this wasnt added.
}

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
    {
        return (ft_strdup2("$"));
        //return (NULL); updated :: if $ is alone it should be printed.
    }
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
    //printf("word to expand is %s\n", expand_word);
    expanded_word = get_expanded_word(expand_word, env_lst);//USER -->HICHAM
    //printf("expanded word is %s\n", expanded_word);
    return (expanded_word);
}

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
                free(expand_word);
                return (expanded_word);
            }
            else
            {
                free(expand_word);
                return (NULL);
            }
        }
        env_lst = env_lst->next;
    }
    free(expand_word);
    return (NULL);
}

