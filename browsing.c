#include	<unistd.h>
#include	"container.h"

static int	create_page(t_table_hash *array, size_t len)
{
  int		i;

  i = len / PAGE;
  if (len % PAGE)
    ++i;
  i *= PAGE;
  if ((array = init_cell_table(array, i)) == (void *)-1)
    return (-1);
  return array->size;
}

int		idx_freespace(t_table_hash *array, size_t len)
{
  int		i;

  i = 0;
  if (array->freespace)
    while (i <= array->size)
      {
	if ((long)(array->freespace[i] - len) >= 0)
	  return i;
	++i;
      }
  return (create_page(array, len));
}
