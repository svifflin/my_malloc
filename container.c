#include	<string.h>
#include	<strings.h>
#include	<unistd.h>
#include	<stdio.h>
#include	"container.h"

static void	*fill_cell(t_list *ptr, size_t len, int idx)
{
  if (ptr->len)
    {
      ptr->next = (void *)ptr + ptr->len;
      ptr = ptr->next;
    }
  ptr->len = len;
  ptr->is_free = 0;
  ptr->page = idx;
  ptr->next_free = NULL;
  ptr->next = NULL;
  return (ptr);
}

t_table_hash 	*create_table(t_table_hash *array)
{
  size_t	page;

  page = PAGE - sizeof(*array);
  if (!(array = allocate(sizeof(*array))))
    return ((void *)-1);
  if (!(array->list = allocate(page / 2)))
    return ((void *)-1);
  if (!(array->freespace = allocate(page / 2)))
    return ((void *)-1);
  array->freelist = 0;
  array->size = -1;
  return array;
}

t_table_hash	*init_cell_table(t_table_hash *array, size_t len)
{
  ++array->size;
  if (!(array->list[array->size] = allocate(len)))
    return ((void *)-1);
  array->list[array->size]->len = 0;
  array->list[array->size]->page = array->size;
  array->list[array->size]->is_free = 0;
  array->list[array->size]->next = NULL;
  array->list[array->size]->next_free = NULL;
  array->freespace[array->size] = len;
  return array;
}

void		*insert(t_table_hash *array, int idx, size_t len)
{
  t_list	*list;

  list = array->list[idx];
  while (list && list->next)
    list = list->next;
  list = fill_cell(list, len, idx);
  array->freespace[idx] -= len;
  return ((void *)list + BLOCK);
}

