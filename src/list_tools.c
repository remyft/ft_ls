/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 03:00:44 by rfontain          #+#    #+#             */
/*   Updated: 2018/09/29 19:18:48 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ls.h"

int			get_rec_right(t_indir *list, char *par)
{
	struct stat	file_stat;
	char		*tmp;

	assign_char(&tmp, par);
	join_char(&tmp, tmp, "/", 1);
	join_char(&tmp, tmp, list->name, 1);
	if ((lstat(tmp, &file_stat)) == -1)
	{
		free(tmp);
		return (0);
	}
	free(tmp);
	return (S_ISDIR(file_stat.st_mode));
}

void		deal_dlist(t_indir *list, char *name, t_fg *g_fg)
{
	char	*tmp;
	t_lst	*file;

	assign_char(&tmp, name);
	join_char(&tmp, tmp, "/", 1);
	join_char(&tmp, tmp, list->name, 1);
	ft_putchar('\n');
	if (name[0] == '/' && name[1] == '/')
		ft_putend(tmp + 1, ":\n");
	else
		ft_putend(tmp, ":\n");
	file = lst_new(tmp);
	deal_file(file, g_fg);
	free(file->name);
	free(file);
	free(tmp);
}

static void	deal_link(t_indir *list, int max)
{
	char *space;

	space = nb_space(NULL, nb_len(list->nb_link), max);
	ft_putstr(space);
	ft_putnbend(list->nb_link, " ");
	free(space);
}

void		deal_llist(t_indir *list, t_dbl ug_size, t_dbl max_len, t_fg *g_fg)
{
	char	*space;
	char	*time;

	ft_putend(list->right, " ");
	deal_link(list, max_len.y);
	space = nb_space(list->uid_user, 0, ug_size.x);
	ft_putend(list->uid_user, space);
	ft_strdel(&space);
	space = nb_space(list->gid_user, 0, ug_size.y);
	!(*g_fg & LONGO) ? ft_putend(list->gid_user, space) : ft_putstr("");
	ft_strdel(&space);
	space = nb_space(NULL, size_len(list), max_len.x);
	if (list->minor >= 0)
	{
		ft_putendnb(space, list->major);
		ft_putendnb(", ", list->minor);
	}
	else
		ft_putendnb(space, list->size);
	ft_putchar(' ');
	ft_strdel(&space);
	time = get_time(list->time, list->itime);
	(*g_fg) & LONG_T ? ft_putend(&(list->time[4]), " ") : ft_putend(time, " ");
	free(time);
	ft_putstr(list->name);
}

void		deal_slink(t_indir *list, char *par_name)
{
	char	*tmp;
	char	buff[256];
	int		size;

	if (list->name[0] != '/')
	{
		join_char(&tmp, par_name, "/", 0);
		join_char(&tmp, tmp, list->name, 1);
	}
	else
		assign_char(&tmp, list->name);
	if ((size = readlink(tmp, &buff[0], 256)) != -1)
	{
		buff[size] = '\0';
		ft_putend(" -> ", buff);
	}
	free(tmp);
	ft_putchar('\n');
}
