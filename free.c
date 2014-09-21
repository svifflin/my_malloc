#include	<unistd.h>
#include	<stdio.h>
#include	"container.h"

int		get_freelist_size(t_table_hash *array)
{
  int		i;
  t_list	*tmp;

  i = 0;
  tmp = array->freelist;
  while (tmp)
    {
      ++i;
      tmp = tmp->next_free;
    }
  return (i);
}

void		print_freelist(t_table_hash *array)
{
  t_list	*tmp;

  tmp = array->freelist;
  printf("FREELIST :\n");
  while (tmp)
    {
      printf("adresse : %p,  espace disponible : %d\n", tmp, tmp->len);
      tmp = tmp->next_free;
    }
}

void		put_to_freelist(t_table_hash *array, t_list *ptr)
{
  t_list	*list;
  t_list	*prev;

  list = array->freelist;
  ptr->is_free = 1;
  prev = 0;
  while (list && list->len < ptr->len)
    {
      prev = list;
      list = list->next_free;
    }
  if (prev)
    prev->next_free = ptr;
  else
    array->freelist = ptr;
  if (!list)
    ptr->next_free = NULL;
  else
    ptr->next_free = list;
}

void		*search_and_destroy_in_freelist(t_table_hash *array, size_t len)
{
  t_list        *tmp;
  t_list	*prev;

  prev = 0;
  tmp = array->freelist;
  while (tmp)
    {
      if (tmp->len >= len)
	{
	  tmp->is_free = 0;
	  if (prev)
	    prev->next_free = tmp->next_free;
	  else
	    array->freelist = tmp->next_free;
	  tmp->next_free = NULL;
	  return ((void *)tmp + BLOCK);
	}
      prev = tmp;
      tmp = tmp->next_free;
    }
  return NULL;
}
