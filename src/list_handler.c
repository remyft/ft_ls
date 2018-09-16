/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/08 08:54:09 by rfontain          #+#    #+#             */
/*   Updated: 2018/09/16 16:22:58 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ls.h"

void		free_list(t_indir *curr)
{
	t_indir *prev;

	while (curr)
	{
		prev = curr->next;
		if (g_fg & LONG_LISTING)
		{
			ft_strdel(&(curr->right));
			ft_strdel(&(curr->uid_user));
			ft_strdel(&(curr->gid_user));
			ft_strdel(&(curr->time));
		}
		ft_strdel(&(curr->name));
		ft_memdel((void**)&curr);
		curr = prev;
	}
}

t_lst		*lst_new(char *file)
{
	t_lst	*new;
	DIR		*dir;

	new = ft_memalloc(sizeof(t_lst));
	new->name = ft_strdup(file);
	if ((dir = opendir(new->name)))
		new->isdir = 1;
	if (dir)
		closedir(dir);
	return (new);
}

int			nb_len(int nb)
{
	int i;

	i = 1;
	while (nb >= 10)
	{
		nb /= 10;
		i++;
	}
	return (i);
}

int			size_len(t_indir *lst)
{
	if (lst->major < 0)
		return (nb_len(lst->size));
	else
		return (nb_len(lst->major) + nb_len(lst->minor) + 3);
}

int			max_nblen(t_indir *lst)
{
	int max;
	int size;

	max = 0;
	while (lst)
	{
		if (lst->major < 0)
			max = (size = nb_len(lst->size)) > max ? size : max;
		else
			max = (size = nb_len(lst->major) + nb_len(lst->minor) + 3) > max ?
				size : max;
		lst = (g_fg & REVERSE ? lst->prev : lst->next);
	}
	return (max);
}

static void	deal_llist(t_indir *list, int uid_size, int gid_size, int max_len)
{
	char	*space;
	char	*time;

	ft_putend(list->right, "  ");
	ft_putnbend(list->nb_link, "\t");
	space = nb_space(list->uid_user, 0, uid_size);
	ft_putend(list->uid_user, space);
	ft_strdel(&space);
	space = nb_space(list->gid_user, 0, gid_size);
	ft_putend(list->gid_user, space);
	ft_strdel(&space);
	space = nb_space(NULL, size_len(list), max_len);
	if (list->minor >= 0)
	{
		ft_putnbend(list->major, ",  ");
		ft_putnbend(list->minor, space);
	}
	else
		ft_putnbend(list->size, space);
	ft_strdel(&space);
	time = get_time(list->time, list->itime);
	ft_putend(time, "  ");
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
		tmp = ft_strjoin(par_name, "/");
		tmp = ft_strjoinfree(tmp, list->name, 1);
	}
	else
		tmp = list->name;
	if ((size = readlink(tmp, &buff[0], 256)) != -1)
	{
		buff[size] = '\0';
		ft_putend(" -> ", buff);
	}
	free(tmp);
	ft_putchar('\n');
}

void		put_llist(t_indir *list, int size, int nb_blk, char *par_name)
{
	int		uid_size;
	int		gid_size;

	uid_size = 0;
	gid_size = 0;
	max_size(list, &uid_size, &gid_size);
	if (nb_blk >= 0 && (list->next || list->prev) &&
			(g_fg & ALL_FILE || size > 2))
	{
		ft_putstr("total ");
		ft_putnbr(nb_blk);
		ft_putchar('\n');
	}
	size = max_nblen(list);
	while (list)
	{
		if (!(list->name[0] == '.' && !(g_fg & ALL_FILE)))
		{
			deal_llist(list, uid_size, gid_size, size);
			deal_slink(list, par_name);
		}
		list = (g_fg & REVERSE) ? list->prev : list->next;
	}
}

void		deal_dlist(t_indir *list, char *name)
{
	char	*tmp;
	t_lst	*file;

	tmp = ft_strdup(name);
	tmp = ft_strjoinfree(tmp, "/", 1);
	tmp = ft_strjoinfree(tmp, list->name, 1);
	ft_putchar('\n');
	ft_putend(tmp, ":\n");
	file = lst_new(tmp);
	deal_file(file);
//	if (file->name[0] && cmp_file(file->name))
//		free(file->name);
	free(tmp);
}

int			get_rec_right(t_indir *list, char *par)
{
	struct stat	file_stat;
	char		*tmp;

	tmp = ft_strdup(par);
	tmp = ft_strjoinfree(tmp, "/", 1);
	tmp = ft_strjoinfree(tmp, list->name, 1);
	if ((lstat(tmp, &file_stat)) == -1)
			return (0);
	free(tmp);
	return (S_ISDIR(file_stat.st_mode));
}

void		put_dlist(t_indir *list, int size, t_lst *par)
{
	int		i;

	i = 0;
	while (list && i < size)
	{
		if (list->name[0] == '.' && !(g_fg & ALL_FILE))
			list = (g_fg & REVERSE) ? list->prev : list->next;
		else if (cmp_file(list->name) && get_rec_right(list, par->name))
		{
			deal_dlist(list, par->name);
			list = (g_fg & REVERSE) ? list->prev : list->next;
		}
		else
			list = (g_fg & REVERSE) ? list->prev : list->next;
		i++;
	}
}

void		put_list(t_indir *list, int size)
{
	int i;

	i = 0;
	while (i < size)
	{
		if (!(list->name[0] == '.' && !(g_fg & ALL_FILE)))
			ft_putendl(list->name);
		list = (g_fg & REVERSE) ? list->prev : list->next;
		i++;
	}
}
