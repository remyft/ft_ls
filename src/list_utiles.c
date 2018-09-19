/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utiles.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 03:20:29 by rfontain          #+#    #+#             */
/*   Updated: 2018/09/19 20:16:50 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ls.h"

t_indir	*set_list(int *i, t_lst *lst, t_indir **end, t_fg *g_fg)
{
	t_indir			*begin;
	t_indir			*curr;
	t_indir			*prev;
	struct dirent	*indir;
	DIR				*dir;

	prev = NULL;
	if (!(dir = opendir(lst->name)))
		return (NULL);
//	ft_putendl(lst->name);
	while ((indir = readdir(dir)))
	{
		if (!(curr = set_indir(indir->d_name, indir->d_type, lst, g_fg)))
			return (NULL);
		curr->prev = (*end);
		if ((*end))
			(*end)->next = curr;
		else
			begin = curr;
		(*end) = curr;
		curr = curr->next;
		(*i) += 1;
	}
	if (dir)
		closedir(dir);
	return (begin);
}

void	list_swap(t_lst *curr, t_lst *next)
{
	char	*curr_name;
	int		curr_isdir;

	curr_name = curr->name;
	curr_isdir = curr->isdir;
	curr->name = next->name;
	curr->isdir = next->isdir;
	next->name = curr_name;
	next->isdir = curr_isdir;
}

t_list	*fill_such(t_list **begsuch, t_list *currsuch, char **av, int i)
{
	t_list *nosuch;

	nosuch = (t_list*)ft_memalloc(sizeof(t_list));
	if (currsuch)
		currsuch->next = nosuch;
	if (!(*begsuch))
		(*begsuch) = nosuch;
	nosuch->content = ft_strdup(av[i]);
	currsuch = nosuch;
	nosuch = nosuch->next;
	return (currsuch);
}

t_lst	*fill_list(t_lst **begin, t_lst *prev, char **av, int i)
{
	t_lst *file;

	file = lst_new(av[i]);
	if (!(*begin))
		(*begin) = file;
	if (prev)
	{
		file->prev = prev;
		prev->next = file;
	}
	prev = file;
	file = file->next;
	return (prev);
}
