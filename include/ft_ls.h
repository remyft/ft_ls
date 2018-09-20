/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 19:53:05 by rfontain          #+#    #+#             */
/*   Updated: 2018/09/20 12:06:39 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H
# include "../libft/libft.h"
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <dirent.h>
# include <errno.h>
# include <pwd.h>
# include <grp.h>
# include <time.h>

# define F_ACCESS_FAIL 1 << 0
# define F_STAT_FAIL 1 << 2
# define F_IS_LINK 1 << 3
# define F_IS_CYCLE 1 << 4

typedef struct stat	t_stat;

typedef enum		e_flag
{
	RECURSIVE = 1 << 0,
	LONG_LISTING = 1 << 1,
	ALL_FILE = 1 << 2,
	REVERSE = 1 << 3,
	DATE_SORT = 1 << 4,
	LONG_T = 1 << 5
}					t_fg;

typedef struct		s_double
{
	int				x;
	int				y;
}					t_dbl;

typedef struct		s_indir
{
	char			*name;
	unsigned int	type;
	char			*right;
	int				nb_link;
	char			*uid_user;
	char			*gid_user;
	int				size;
	int				major;
	int				minor;
	char			*time;
	time_t			itime;
	struct s_indir	*next;
	struct s_indir	*prev;
}					t_indir;

typedef struct		s_lst
{
	char			*name;
	t_indir			*indir;
	int				isdir;
	int				size;
	int				nb_blk;
	t_fg			*g_fg;
	struct s_lst	*next;
	struct s_lst	*prev;
}					t_lst;

void				put_error_flag(char fl);
void				set_flags(char fl, t_fg *g_fg);
void				get_flag(char *flags, t_fg *g_fg);

void				deal_flags(t_lst *lst, t_indir *end, int size, t_fg *g_fg);
void				deal_file(t_lst *lst, t_fg *g_fg);
void				big_deal(t_lst *list, t_fg *g_fg);
void				deal_such(t_list *such);
void				big_deal_list(int i, int ac, char **av, t_fg *g_fg);

int					cmp_file(char *name);
char				*get_time(char *file, time_t itime);
void				max_size(t_indir *lst, int *uid_size, int *gid_size);
char				*nb_space(char *str, int nb, int size);
int					is_major(t_indir *lst);

void				put_nosuch(t_list *such);
void				deal_solo(int i, int ac, char **av, t_fg *g_fg);
int					str_swap(t_indir *curr);
void				put_ferror(int error, t_lst *lst);
void				put_error(int error, t_lst *lst);

void				sort_alpha(t_indir *names, int size);
void				sort_date(t_indir *names, int size);
t_lst				*sort_list(t_lst *list);
t_lst				*sort_not_dir(t_lst *list, t_fg *g_fg);
void				sort_dir(t_lst *list, t_fg *g_fg);

void				ft_putname(char *name);
int					nb_len(int nb);
int					size_len(t_indir *lst);
int					len_list(t_indir *list);
int					max_int(int nb1, int nb2);

t_indir				*set_indir(char *name, unsigned char type, t_lst *par, char *lst_name);
t_indir				*set_stat_indir(t_indir *lst, char *lst_name, t_lst *par,
		t_fg *g_fg);
char				set_type(mode_t file_stat);
char				*set_right(mode_t file_stat);

int					max_nblen(t_indir *lst, t_fg *g_fg);
int					get_rec_right(t_indir *list, char *par);
void				deal_dlist(t_indir *list, char *name, t_fg *g_fg);
void				deal_llist(t_indir *list, t_dbl ug_size, int max_len,
		t_fg *g_fg);
void				deal_slink(t_indir *list, char *par_name);

t_indir				*set_list(int *i, t_lst *lst, t_indir **end);
void				list_swap(t_lst *curr, t_lst *next);
t_list				*fill_such(t_list **begsuch, t_list *currsuch, char **av,
		int i);
t_lst				*fill_list(t_lst **begin, t_lst *prev, char **av, int i);

void				free_list(t_indir *curr, t_fg *g_fg);
t_lst				*lst_new(char *file);
void				put_list(t_indir *list, int size, t_fg *g_fg);
void				put_dlist(t_indir *list, int size, t_lst *par, t_fg *g_fg);
void				put_llist(t_indir *list, int nb_blk, t_lst *par,
		t_fg *g_fg);

#endif
