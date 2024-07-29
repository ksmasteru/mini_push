#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
# include <signal.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

# define ALLOC 'A'
# define FREE 'F'

/* The Struct used in the garbage collector */
typedef struct s_gc
{
	void		*ptr;
	struct s_gc	*next;
}				t_gc;

typedef struct s_lst
{
  char *data;
  struct s_lst *next;
  struct s_lst *value;
}t_lst;

#endif /*	MINISHELL_H	*/
