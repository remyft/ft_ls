/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/08 08:54:09 by rfontain          #+#    #+#             */
/*   Updated: 2018/09/15 07:48:49 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ls.h"
#include <stdio.h>
void		free_list(t_indir *curr)
{
	t_indir *prev;

	while (curr)
	{
		prev = curr->next;
		if (g_fg & LONG_LISTING)
		{
			free(curr->right);
			free(curr->uid_user);
			free(curr->gid_user);
			free(curr->time);
		}
	//	if (!(cmp_file(curr->name)))
			free(curr->name);
		free(curr);
		curr = prev;
	}
}

t_lst		*lst_new(char *file)
{
	t_lst *new;

	new = ft_memalloc(sizeof(t_lst));
	new->name = ft_strdup(file);
	return (new);
}

int			is_long(t_indir *lst)
{
	while (lst)
	{
		if (lst->size >= 100000)
			return (1);
		lst = lst->next;
	}
	return (0);
}

static void	deal_llist(t_indir *list, int uid_size, int gid_size)
{
	char	*space;
	char	*time;

	ft_putend(list->right, "  ");
	ft_putnbr(list->nb_link);
	ft_putchar('\t');
	space = nb_space(list->uid_user, uid_size);
	ft_putend(list->uid_user, space);
	free(space);
	space = nb_space(list->gid_user, gid_size);
	ft_putend(list->gid_user, space);
	free(space);
	if (list->minor >= 0)
	{
		ft_putnbend(list->major, ",  ");
		ft_putnbend(list->minor, "  ");
	}
	else
		ft_putnbend(list->size, "\t");
	ft_putchar('\t');
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
	(void)size;
	if (nb_blk >= 0 && (list->next || list->prev) &&
			(nb_blk > 0 || g_fg & ALL_FILE || size > 2))
	{
		ft_putstr("total ");
		ft_putnbr(nb_blk);
		ft_putchar('\n');
	}
	while (list)
	{
		if (!(list->name[0] == '.' && !(g_fg & ALL_FILE)))
		{
			deal_llist(list, uid_size, gid_size);
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
//	free(file->name);
	free(tmp);
}

void		put_dlist(t_indir *list, int size, char *name)
{
	int		i;

	i = 0;
	while (i < size)
	{
		if (list->name[0] == '.' && !(g_fg & ALL_FILE))
			list = (g_fg & REVERSE) ? list->prev : list->next;
		else if ((list->type & DT_DIR ||
				(g_fg & LONG_LISTING && list->right[0] == 'd')) &&
				cmp_file(list->name) && !(list->right && list->right[0] != 'b'))
		{
			deal_dlist(list, name);
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
