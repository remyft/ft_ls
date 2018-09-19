/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deal.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 03:15:30 by rfontain          #+#    #+#             */
/*   Updated: 2018/09/19 20:17:29 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ls.h"

void	deal_flags(t_lst *lst, t_indir *end, int size, t_fg *g_fg)
{
//	t_indir *curr;
//	t_indir *test;

	if (*g_fg & DATE_SORT)
		sort_date(lst->indir, size);
	/*if (*g_fg & LONG_LISTING || *g_fg & DATE_SORT)
	{
		curr = lst->indir;
		test = lst->indir;
		while (curr)
		{
			if (!(curr = set_stat_indir(curr, lst->name, lst, g_fg)))
			{
				//ft_putend(test->name, " : bug\n");
				return(put_error(F_STAT_FAIL, lst, g_fg));
			}
			test = test->next;
			curr = curr->next;
		}
	}*/
	if (*g_fg & LONG_LISTING)
		(*g_fg & REVERSE) ? put_llist(end, lst->nb_blk, lst, g_fg) :
			put_llist(lst->indir, lst->nb_blk, lst, g_fg);
	else
		(*g_fg & REVERSE) ? put_list(end, size, g_fg) :
			put_list(lst->indir, size, g_fg);
	if (*g_fg & RECURSIVE)
		(*g_fg & REVERSE) ? put_dlist(end, size, lst, g_fg) :
			put_dlist(lst->indir, size, lst, g_fg);
}

void	deal_file(t_lst *lst, t_fg *g_fg)
{
	int				i;
	t_indir			*end;
	DIR				*dir;
	char			tmp[256];

	end = NULL;
	lst->nb_blk = 0;
	if (!(dir = opendir((char*)lst->name)))
		return (put_error(1, lst, g_fg));
	if (dir)
		closedir(dir);
	if (readlink(lst->name, &tmp[0], 256) != -1)
		return (put_error(F_IS_LINK, lst, g_fg));
	i = 0;
	if (!(lst->indir = set_list(&i, lst, &end, g_fg)))
		return (put_error(1 << 2, lst, g_fg));
	lst->size = i;
	sort_alpha(lst->indir, i);
	deal_flags(lst, end, lst->size, g_fg);
	while (lst->indir->prev)
		lst->indir = lst->indir->prev;
	free_list(lst->indir, g_fg);
}

void	big_deal(t_lst *list, t_fg *g_fg)
{
	t_lst	*begin;
	t_lst	*mid;

	begin = list;
	list = begin;
	list = sort_list(list);
	mid = sort_not_dir(list, g_fg);
	sort_dir(mid, g_fg);
	while (begin)
	{
		if (begin->prev && begin->isdir == 1)
			ft_putchar('\n');
		if (list->size != 1 && begin->isdir == 1)
			ft_putend(begin->name, ":\n");
		mid = begin->next;
		deal_file(begin, g_fg);
		begin = mid;
	}
	while (list)
	{
		mid = list->next;
		free(list->name);
		free(list);
		list = mid;
	}
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

void	big_deal_list(int i, int ac, char **av, t_fg *g_fg)
{
	t_list	*begsuch;
	t_list	*currsuch;
	DIR		*dir;
	t_lst	*begin;
	t_lst	*curr;

	curr = NULL;
	begin = NULL;
	begsuch = NULL;
	currsuch = NULL;
	while (i < ac)
	{
		if (!(dir = opendir(av[i])) && errno == ENOENT)
			currsuch = fill_such(&begsuch, currsuch, av, i);
		else
			curr = fill_list(&begin, curr, av, i);
		if (dir)
			closedir(dir);
		i++;
	}
	if (begsuch)
		deal_such(begsuch);
	if (begin)
		big_deal(begin, g_fg);
}
