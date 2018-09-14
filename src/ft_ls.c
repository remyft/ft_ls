/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 19:50:06 by rfontain          #+#    #+#             */
/*   Updated: 2018/09/14 06:54:27 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ls.h"

t_indir	*set_list(DIR *dir, int *i, t_lst *lst, t_indir **end)
{
	t_indir			*begin;
	t_indir			*curr;
	t_indir			*prev;
	struct dirent	*indir;

	prev = NULL;
	while ((indir = readdir(dir)))
	{
		if (!(curr = set_indir(indir->d_name, indir->d_type, lst->name)))
		{
			return (NULL);
			put_error(1 << 2, lst);
		}
		curr->prev = (*end);
		if ((*end))
			(*end)->next = curr;
		else
			begin = curr;
		(*end) = curr;
		curr = curr->next;
		(*i) += 1;
	}
	return (begin);
}

void	deal_flags(t_lst *lst, t_indir *end, int size)
{
	t_indir *curr;

	if (g_fg & DATE_SORT)
		sort_date(lst->indir, size);
	if (g_fg & LONG_LISTING || g_fg & DATE_SORT)
	{
		curr = lst->indir;
		while (curr)
		{
			if (!(curr = set_stat_indir(curr, lst->name, lst)))
				return (put_error(1 << 2, lst));
			curr = curr->next;
		}
	}
	if (g_fg & LONG_LISTING)
		(g_fg & REVERSE) ? put_llist(end, size, lst->nb_blk, lst->name) :
			put_llist(lst->indir, size, lst->nb_blk, lst->name);
	else
		(g_fg & REVERSE) ? put_list(end, size) : put_list(lst->indir, size);
	if (g_fg & RECURSIVE)
		(g_fg & REVERSE) ? put_dlist(end, size, lst->name) :
			put_dlist(lst->indir, size, lst->name);
}

void	deal_file(t_lst *lst)
{
	int				i;
	t_indir			*end;
	DIR				*dir;
	char			tmp[256];

	end = NULL;
	lst->nb_blk = 0;
	if (!(dir = opendir((char*)lst->name)))
		return (put_error(1, lst));
	if (readlink(lst->name, &tmp[0], 32) != -1)
		return (put_error(F_IS_LINK, lst));
	i = 0;
	lst->indir = set_list(dir, &i, lst, &end);
	lst->size = i;
	sort_alpha(lst->indir, i);
	deal_flags(lst, end, lst->size);
	if (dir)
		closedir(dir);
//	free_list(lst->indir);
	free(lst->name);
	free(lst);
}

int		main(int ac, char **av)
{
	int		i;
	t_lst	*file;

	i = 1;
	file = NULL;
	while (av[i] && av[i][0] == '-' && ft_strlen(av[i]) >= 2)
		get_flag(av[i++]);
	if (ac - i >= 2)
		while (i < ac)
		{
			ft_putend(av[i], ":\n");
			file = lst_new(av[i]);
			deal_file(file);
			i++;
		}
	else
	{
		file = (ac - i == 1) ? lst_new(av[i]) : lst_new(".");
		deal_file(file);
	}
	return (0);
}
