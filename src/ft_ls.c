/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 19:50:06 by rfontain          #+#    #+#             */
/*   Updated: 2018/09/12 07:59:37 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ls.h"

void	deal_file(t_lst *lst)
{
	int				i;
	t_indir			*prev;
	t_indir			*curr;
	int				error;
	DIR				*dir;
	struct dirent	*indir;

	curr = NULL;
	lst->nb_blk = 0;
	error = 1;
	if (!(dir = opendir((char*)lst->name)))
		return (put_error(1, lst));
	i = 0;
	if ((indir = readdir(dir)))
	{
		if (!(lst->indir = set_indir(indir->d_name, indir->d_type, lst->name)))
			return (put_error(1 << 2, lst));
		i++;
	}
	while ((indir = readdir(dir)))
	{
		if (!(curr = set_indir(indir->d_name, indir->d_type, lst->name)))
			return (put_error(1 << 2, lst));
		if (i == 1)
		{
			lst->indir->next = curr;
			curr->prev = lst->indir;
		}
		else
			curr->prev = prev;
		if (i != 1) 
			prev->next = curr;
		prev = curr;
		curr = curr->next;
		i++;
	}
	lst->size = i;
	sort_alpha(lst->indir, i);
	if (g_fg & DATE_SORT)
		sort_date(lst->indir, i);
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
	if (g_fg & LONG_LISTING && error)
		(g_fg & REVERSE) ? put_llist(prev, i, lst->nb_blk) : put_llist(lst->indir, i, lst->nb_blk);
	else
		(g_fg & REVERSE) ? put_list(prev, i) : put_list(lst->indir, i);
	if (g_fg & RECURSIVE)
		(g_fg & REVERSE) ? put_dlist(prev, i, lst->name) : put_dlist(lst->indir, i, lst->name);
	if (dir)
		closedir(dir);
	free_list(lst->indir);
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
	else if (ac - i == 1)
	{
		file = lst_new(av[i]);
		deal_file(file);
	}
	else
	{
		file = lst_new(".");
		deal_file(file);

	}
	return (0);
}
