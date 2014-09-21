#include	<stdio.h>
#include	<string.h>
#include	<unistd.h>
#include	"container.h"

static t_table_hash	*array = 0;

void		*malloc(size_t taille)
{
  t_list	*key;
  int		idx;
  static	int p = 0;

  //printf("-----------------\n");
  if (p == 1)
    return (0);
  p = 1;
  if ((long)taille <= 0)
    return NULL;
  taille += taille % 2;
  if (!array)
    array = create_table(array);
  taille += BLOCK;
  //printf("DEBUT MALLOC : TAILLE : %u\n", taille);
  if (!(key = search_and_destroy_in_freelist(array, taille)))
    {
      if ((idx = idx_freespace(array, taille)) == -1)
	  return NULL;
      key = insert(array, idx, taille);
    }
  //printf("FIN MALLOC\n");
  //printf("-----------------\n");
  //printf("MALLOC\n\tSBRK(0) : %p - KEY : %p - DATA : %p - LEN : %u\n", sbrk(0), (void *)key - BLOCK, key, key->len);
  //show_alloc_mem();
  p = 0;
  return key;
}

void		free(void *ptr)
{
  void		*tmp;

  //printf("-----------------\n");
  //printf("DEBUT FREE\n");
  if (array)
    {
      tmp = ptr - BLOCK;
      if (tmp > (void *)array && tmp < (void *)sbrk(0))
	{
	  if (((t_list *)tmp)->is_free == 0)
	    put_to_freelist(array, tmp);
	  else
	    printf("free(): double free or corruption (fasttop): %p\n", tmp);
	}
    }
   //printf("-----------------\n");
  //printf("p\n");
  //printf("--------FIN FREE---------\n");
}

void            *calloc(size_t nb, size_t size)
{
  size_t        *new;
  size_t        i;

  //printf("-----------------\n");
  //printf("DEBUT CALLOC\n");
  if ((new = malloc(nb * size)))
    {
      //size += size % 2;
      i = 0;
      while (i < size)
        {
          new[i] = 0;
          i++;
        }
    }
  //printf("-----------------\n");
  //printf("FIN CALLOC\n");
  return new;
}

void            *realloc(void *ptr, size_t size)
{
  void          *new;
  t_list	*tmp;

  //printf("-----------------\n");
  //printf("DEBUT REALLOC\n");
  if (!ptr)
    return (malloc(size));
  tmp = ptr - BLOCK;
  size += size % 2;
  if (tmp->len >= (size + BLOCK))
    return ptr;
  if ((new = malloc(size)))
    {
      memcpy(new, tmp, size);
      free(ptr);
    }
  //printf("-----------------\n");
  //printf("FIN REALLOC\n");
  return new;
}

void		show_alloc_mem()
{
  int		i;
  t_list	*tmp;

  printf("SHOW_ALLOC_MEM\n");
  i = 0;
  printf("Program break : %p\n", sbrk(0));
  while (array && i <= array->size)
    {
      tmp = array->list[i];
      while (tmp && tmp->len)
	{
	  if (!tmp->is_free)
	    {
	      printf("BEGIN : %p - DATA : %p - END : %p - ", tmp, tmp + BLOCK, tmp->next);
	      printf("LEN : %u - IS_FREE %d\n", tmp->len, tmp->is_free);
	    }
	  tmp = tmp->next;
	}
      printf("-------------------------\n");
      printf("Page %d : Il reste %u disponible\n", i, array->freespace[i]);
      printf("-------------------------\n");
      i++;
    }
}


