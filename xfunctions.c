#include	<unistd.h>

void		*xsbrk(intptr_t inc)
{
  return sbrk(inc);
}

void		*allocate(size_t len)
{
  void		*begin;

  if ((size_t)(begin = xsbrk(0)) == 0xFFFFFF)
    return ((void *)-1);
  if ((size_t)xsbrk(len) == 0xFFFFFF)
    return ((void *)-1);
  return begin;
}
