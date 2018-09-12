/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 19:53:05 by rfontain          #+#    #+#             */
/*   Updated: 2018/09/12 07:59:50 by rfontain         ###   ########.fr       */
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

# define F_access_fail 1 << 0
# define F_stat_fail 1 << 2

typedef struct stat	t_stat;

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
	int				itime;
	struct s_indir	*next;
	struct s_indir	*prev;
}					t_indir;

typedef struct		s_lst
{
	char			*name;
	t_indir			*indir;
	int				size;
	int				nb_blk;
}					t_lst;

typedef enum		e_flag
{
	RECURSIVE = 1 << 0,		//-R
	LONG_LISTING = 1 << 1,	//-l
	ALL_FILE = 1 << 2,		//-a
	REVERSE = 1 << 3,		//-r
	DATE_SORT = 1 << 4		//-t
}					t_fg;

t_fg				g_fg;

void				put_error_flag(char fl);
void				set_flags(char fl);
void				unset(int flag);
void				get_flag(char *flags);

void				deal_file(t_lst *lst);

int					cmp_file(char *name);
int					get_mon(char *month);
char				*get_time(char *file, time_t itime);
int					max_int(int nb1, int nb2);
void				max_size(t_indir *lst, int *uid_size, int *gid_size);
char				*nb_space(char *str, int size);
int					is_major(t_indir *lst);
int					str_swap(t_indir *curr);
void				sort_alpha(t_indir *names, int size);
void				sort_date(t_indir *names, int size);
void				ft_putname(char *name);
void				put_error(int error, t_lst *lst);

t_indir				*set_indir(char *name, unsigned char type, char *lst_name);
t_indir				*set_stat_indir(t_indir *lst, char *lst_name, t_lst *par);
char				set_type(mode_t file_stat);
char				*set_right(mode_t file_stat);

void				free_list(t_indir *curr);
t_lst				*lst_new(char *file);
void				put_list(t_indir *list, int size);
void				put_dlist(t_indir *list, int size, char *name);
void				put_llist(t_indir *list, int size, int nb_blk);

#endif
