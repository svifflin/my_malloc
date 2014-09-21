#include	<unistd.h>
#include	<stdio.h>
#include	"container.h"

static void	fusion(t_list *ptr)
{
  t_list	*begin;

  begin = ptr;
  if (ptr->next && ptr->next_free == ptr->next)
    {
      do
	{
	  ptr = ptr->next_free;
	  begin->len += ptr->len;
	}
      while (ptr && ptr->next && ptr->next_free == ptr->next);
    }
  if (begin != ptr)
    {
      begin->next = ptr->next;
      begin->next_free = ptr->next_free;
    }
}

static t_list	*split(t_list *ptr, size_t len)
{
  void		*end;
  void		*end_free;

  end = ptr->next;
  ptr->next = (void *)ptr + len;
  end_free = ptr->next_free;
  ptr->next_free = ptr->next;
  ptr->next->len = ptr->len - len;
  ptr->len = len;
  ptr->next->next = end;
  ptr->next->next_free = end_free;
  return ptr;
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
  fusion(ptr);
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
	  if (tmp->len > (len + BLOCK))
	    tmp = split(tmp, len);
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
