/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deal.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 03:15:30 by rfontain          #+#    #+#             */
/*   Updated: 2018/10/11 05:19:18 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ls.h"

void	deal_flags(t_lst *lst, t_indir *end, int size, t_fg *e_fg)
{
	if (*e_fg & DATE_SORT || *e_fg & LONG_LISTING || *e_fg & LONGO
			|| *e_fg & SIZE_SORT)
		if (!(lst->indir = set_stat_indir(&(lst->indir), lst->indir,
			lst, lst->name)))
			return ;
	if (*e_fg & DATE_SORT && !(*e_fg & UNSORT) && !(*e_fg & SIZE_SORT))
		sort_date(lst->indir, size);
	if (*e_fg & SIZE_SORT && !(*e_fg & UNSORT))
		sort_size(lst->indir, size);
	while (lst->indir->prev)
		lst->indir = lst->indir->prev;
	end = lst->indir;
	while (end->next)
		end = end->next;
	if (*e_fg & LONG_LISTING || *e_fg & LONGO)
		(*e_fg & REVERSE) ? put_llist(end, lst->nb_blk, lst, e_fg)
			: put_llist(lst->indir, lst->nb_blk, lst, e_fg);
	else
		(*e_fg & REVERSE) ? put_list(end, size, e_fg)
			: put_list(lst->indir, size, e_fg);
	if (*e_fg & RECURSIVE)
		(*e_fg & REVERSE) ? put_dlist(end, size, lst, e_fg)
			: put_dlist(lst->indir, size, lst, e_fg);
}

void	deal_file(t_lst *lst, t_fg *e_fg)
{
	int				i;
	t_indir			*end;
	DIR				*dir;
	char			tmp[256];

	end = NULL;
	lst->nb_blk = 0;
	lst->e_fg = e_fg;
	dir = NULL;
	if (readlink(lst->name, &tmp[0], 256) != -1)
		return (put_error(F_IS_LINK, lst, dir));
	if (!(dir = opendir((char*)lst->name)))
		return (put_error(1, lst, dir));
	i = 0;
	if (!(lst->indir = set_list(&i, &end, dir)))
		return (put_error(1 << 2, lst, dir));
	lst->size = i;
	if (!(*e_fg & UNSORT))
		sort_alpha(lst->indir, i);
	while (lst->indir->prev)
		lst->indir = lst->indir->prev;
	deal_flags(lst, end, lst->size, e_fg);
	free_list(lst->indir, e_fg);
}

void	big_deal(t_lst *list, t_fg *e_fg)
{
	t_lst	*begin;
	t_lst	*mid;

	begin = list;
	list = set_stat_list(list);
	list = sort_list(list);
	mid = sort_not_dir(list, e_fg);
	sort_dir(mid, e_fg);
	while (begin->prev)
		begin = begin->prev;
	list = begin;
	while (begin)
	{
		if (begin->prev && begin->isdir == 1 && begin->prev->stat)
			ft_putchar('\n');
		if (list->size != 1 && begin->isdir == 1)
			ft_putend(begin->name, ":\n");
		mid = begin->next;
		deal_file(begin, e_fg);
		begin = mid;
	}
	big_free(list);
}

void	deal_such(t_list *such)
{
	t_list	*begin;
	char	*tmp;

	begin = such;
	while (such->next)
	{
		if (ft_strcmp((char*)(such->content), (char*)(such->next->content)) > 0)
		{
			tmp = such->content;
			such->content = such->next->content;
			such->next->content = tmp;
			such = begin;
		}
		else
			such = such->next;
	}
	put_nosuch(begin);
}

void	big_deal_list(int i, int ac, char **av, t_fg *e_fg)
{
	t_list	*begsuch;
	t_list	*currsuch;
	t_stat	stat;
	t_lst	*begin;
	t_lst	*curr;

	curr = NULL;
	begin = NULL;
	begsuch = NULL;
	currsuch = NULL;
	while (i < ac)
	{
		if ((lstat(av[i], &stat) == -1) && errno == ENOENT)
			currsuch = fill_such(&begsuch, currsuch, av, i);
		else
			curr = fill_list(&begin, curr, av, i);
		i++;
	}
	if (begsuch)
		deal_such(begsuch);
	if (begin)
		big_deal(begin, e_fg);
}
