#ifndef _CONTAINER_H
#define _CONTAINER_H

#define PAGE	sysconf(_SC_PAGESIZE)
#define BLOCK	20
#define _XOPEN_SOURCE  500

typedef struct	s_list
{
  size_t	len;
  int		page;
  int		is_free;
  struct s_list	*next;
  struct s_list	*next_free;
}		t_list;

typedef struct	s_table_hash
{
  int		size;
  t_list	**list;
  t_list	*freelist;
  size_t	*freespace;
}		t_table_hash;

/*
** FUNCTIONS OF A HASH TABLE
*/
t_table_hash	*create_table(t_table_hash *);
t_table_hash	*init_cell_table(t_table_hash *, size_t);
void		*insert(t_table_hash *, int, size_t);

/*
** RESEARCH FUNCTIONS
*/

int		idx_freespace(t_table_hash *, size_t);
void		show_alloc_mem();

/*
** FUNCTIONS OF FREELIST
*/

void		put_to_freelist(t_table_hash *, t_list *);
void		*search_and_destroy_in_freelist(t_table_hash *, size_t);
void		print_freelist(t_table_hash *);

/*
** XFUNCTIONS
*/

void		*xsbrk(int);
void		*allocate(size_t);

#endif
