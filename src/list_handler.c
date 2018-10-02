/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/08 08:54:09 by rfontain          #+#    #+#             */
/*   Updated: 2018/10/02 19:42:57 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ls.h"

void		free_list(t_indir *curr, t_fg *e_fg)
{
	t_indir *next;

	if (curr)
		while (curr->prev)
			curr = curr->prev;
	if (curr)
		while (curr)
		{
			next = curr->next;
			if (*e_fg & LONG_LISTING || *e_fg & DATE_SORT || *e_fg & SIZE_SORT
					|| *e_fg & GET_ID || *e_fg & LONG_T || *e_fg & LONGO)
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

	if (!(new = (t_lst*)ft_memalloc(sizeof(t_lst))))
		exit(2);
	assign_char(&(new->name), file);
	if ((dir = opendir(new->name)))
		new->isdir = 1;
	if (dir)
		closedir(dir);
	return (new);
}

void		put_llist(t_indir *list, int nb_blk, t_lst *par, t_fg *e_fg)
{
	t_dbl	ug_size;
	t_dbl	size;

	ug_size.x = 0;
	ug_size.y = 0;
	max_size(list, &(ug_size.x), &(ug_size.y), e_fg);
	if (nb_blk >= 0 && (list->next || list->prev)
			&& (*e_fg & ALL_FILE || len_list(list, e_fg) > 0))
	{
		ft_putstr("total ");
		ft_putnbr(nb_blk);
		ft_putchar('\n');
	}
	size.x = max_nblen(list, e_fg);
	size.y = max_link_len(list, e_fg);
	while (list)
	{
		if (!((list->name[0] == '.' && !(*e_fg & ALL_FILE)))
				|| (cmp_file(list->name) && *e_fg & HIDEN_FILE))
		{
			deal_llist(list, ug_size, size, e_fg);
			deal_slink(list, par->name);
		}
		list = (*e_fg & REVERSE) ? list->prev : list->next;
	}
}

void		put_dlist(t_indir *list, int size, t_lst *par, t_fg *e_fg)
{
	int		i;

	i = 0;
	while (list && i < size)
	{
		if (list->name[0] == '.' && !(*e_fg & ALL_FILE)
				&& !(*e_fg & HIDEN_FILE))
			list = (*e_fg & REVERSE) ? list->prev : list->next;
		else if (cmp_file(list->name) && get_rec_right(list, par->name))
		{
			deal_dlist(list, par->name, e_fg);
			list = (*e_fg & REVERSE) ? list->prev : list->next;
		}
		else
			list = (*e_fg & REVERSE) ? list->prev : list->next;
		i++;
	}
}

void		put_list(t_indir *list, int size, t_fg *e_fg)
{
	int i;

	i = 0;
	while (list && i < size)
	{
		if (!(list->name[0] == '.' && !(*e_fg & ALL_FILE))
				|| (cmp_file(list->name) && *e_fg & HIDEN_FILE))
			ft_putendl(list->name);
		list = (*e_fg & REVERSE) ? list->prev : list->next;
		i++;
	}
}
