/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/08 08:54:09 by rfontain          #+#    #+#             */
/*   Updated: 2018/09/18 03:09:05 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ls.h"

void		free_list(t_indir *curr)
{
	t_indir *next;

	while (curr)
	{
		next = curr->next;
		if (g_fg & LONG_LISTING)
		{
			ft_strdel(&(curr->right));
			ft_strdel(&(curr->uid_user));
			ft_strdel(&(curr->gid_user));
			ft_strdel(&(curr->time));
		}
		ft_strdel(&(curr->name));
		ft_memdel((void**)&curr);
		curr = next;
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
