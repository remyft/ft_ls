/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 19:53:05 by rfontain          #+#    #+#             */
/*   Updated: 2018/09/05 11:49:47 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H
# include "../libft/libft.h"
# include <sys/types.h>
# include <sys/stat.h>
# include <dirent.h>

typedef struct		s_indir
{
	char			*name;
	unsigned int	type;
	struct s_indir	*next;
	struct s_indir	*prev;
}					t_indir;

typedef struct		s_lst
{
	char			*name;
	t_indir			*indir;
	int				size;
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

void				put_list(t_indir *list, int size);
void				put_dlist(t_indir *list, int size, char *name);

t_lst				*lst_new(char *file);

void				deal_file(t_lst *lst);
#endif
