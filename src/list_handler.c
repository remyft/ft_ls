/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/08 08:54:09 by rfontain          #+#    #+#             */
/*   Updated: 2018/09/23 01:34:09 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ls.h"

void		free_list(t_indir *curr, t_fg *g_fg)
{
	t_indir *next;

	(void)g_fg;
	while (curr)
	{
		next = curr->next;
		ft_strdel(&(curr->right));
		ft_strdel(&(curr->uid_user));
		ft_strdel(&(curr->gid_user));
		ft_strdel(&(curr->time));
		ft_strdel(&(curr->name));
		ft_memdel((void**)&curr);
		curr = next;
	}
}

t_lst		*lst_new(char *file)
{
	t_lst	*new;
	DIR		*dir;

	if (!(new = ft_memalloc(sizeof(t_lst))))
		exit(2);
	assign_char(&(new->name), file);
	if ((dir = opendir(new->name)))
		new->isdir = 1;
	if (dir)
		closedir(dir);
	return (new);
}

void		put_llist(t_indir *list, int nb_blk, t_lst *par, t_fg *g_fg)
{
	t_dbl	ug_size;
	t_dbl	size;

	ug_size.x = 0;
	ug_size.y = 0;
	max_size(list, &(ug_size.x), &(ug_size.y));
	if (nb_blk >= 0 && (list->next || list->prev) &&
			(*g_fg & ALL_FILE || len_list(list) > 0))
	{
		ft_putstr("total ");
		ft_putnbr(nb_blk);
		ft_putchar('\n');
	}
	size.x = max_nblen(list, g_fg);
	size.y = max_link_len(list, g_fg);
	while (list)
	{
		if (!(list->name[0] == '.' && !(*g_fg & ALL_FILE)))
		{
			deal_llist(list, ug_size, size, g_fg);
			deal_slink(list, par->name);
		}
		list = (*g_fg & REVERSE) ? list->prev : list->next;
	}
}

void		put_dlist(t_indir *list, int size, t_lst *par, t_fg *g_fg)
{
	int		i;

	i = 0;
	while (list && i < size)
	{
		if (list->name[0] == '.' && !(*g_fg & ALL_FILE))
			list = (*g_fg & REVERSE) ? list->prev : list->next;
		else if (cmp_file(list->name) && get_rec_right(list, par->name))
		{
			deal_dlist(list, par->name, g_fg);
			list = (*g_fg & REVERSE) ? list->prev : list->next;
		}
		else
			list = (*g_fg & REVERSE) ? list->prev : list->next;
		i++;
	}
}

void		put_list(t_indir *list, int size, t_fg *g_fg)
{
	int i;

	i = 0;
	while (i < size)
	{
		if (!(list->name[0] == '.' && !(*g_fg & ALL_FILE)))
			ft_putendl(list->name);
		list = (*g_fg & REVERSE) ? list->prev : list->next;
		i++;
	}
}
