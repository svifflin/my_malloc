#include	<stdio.h>
#include	<string.h>
#include	<unistd.h>
#include	"container.h"

static t_table_hash	*array = 0;

void		*malloc(size_t taille)
{
  t_list	*key;
  int		idx;

  taille += (taille % 2);
  if ((long)taille <= 0)
    return NULL;
  if (!array)
    array = create_table(array);
  taille += BLOCK;
  if (!(key = search_and_destroy_in_freelist(array, taille)))
    {
      if ((idx = idx_freespace(array, taille)) == -1)
	  return NULL;
      key = insert(array, idx, taille);
    }
  return key;
}

void		free(void *ptr)
{
  t_list	*tmp;

  if (array)
    {
      tmp = (void *)ptr - BLOCK;
      if ((void *)tmp > (void *)array && tmp < (t_list *)sbrk(0))
	{
	  if (tmp->is_free == 0)
	    put_to_freelist(array, tmp);
	}
    }
}

void            *realloc(void *ptr, size_t size)
{
  void          *new;
  t_list	*tmp;

  if (!ptr)
    return (malloc(size));
  tmp = (void *)ptr - BLOCK;
  size += size % 2;
  if (tmp->len >= (size + BLOCK))
    return ptr;
  if (!tmp->is_free)
    {
      if (!(new = malloc(size)))
	return NULL;
      memcpy(new, ptr, size);
      free(ptr);
      return (new);
    }
  return ptr;
}

void		show_alloc_mem()
{
  int		i;
  t_list	*tmp;

  i = 0;
  printf("break : %p\n", sbrk(0));
  while (array && i <= array->size)
    {
      tmp = array->list[i];
      while (tmp && tmp->next && tmp->len)
	{
	  if (!tmp->is_free)
	    printf("%p - %p : %u octets \n", tmp, tmp->next, tmp->len);
	  tmp = tmp->next;
	}
      i++;
    }
}
