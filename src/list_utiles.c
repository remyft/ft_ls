/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utiles.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 03:20:29 by rfontain          #+#    #+#             */
/*   Updated: 2018/09/29 23:49:09 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ls.h"

t_indir	*set_list(int *i, t_indir **end, DIR *dir)
{
	t_indir			*begin;
	t_indir			*curr;
	t_indir			*prev;
	struct dirent	*indir;

	prev = NULL;
	while ((indir = readdir(dir)))
	{
		if (!(curr = set_indir(indir->d_name, indir->d_type)))
		{
			closedir(dir);
			return (NULL);
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
	if (dir)
		closedir(dir);
	return (begin);
}

void	list_swap(t_lst *curr, t_lst *next)
{
	t_lst *tmp_prev;
	t_lst *tmp_next;

	tmp_prev = curr->prev;
	tmp_next = next->next;
	curr->prev = next;
	curr->next = tmp_next;
	next->next = curr;
	next->prev = tmp_prev;
	if (curr->next)
		curr->next->prev = curr;
	if (next->prev)
		next->prev->next = next;
}

t_list	*fill_such(t_list **begsuch, t_list *currsuch, char **av, int i)
{
	t_list *nosuch;

	if (!(nosuch = (t_list*)ft_memalloc(sizeof(t_list))))
		exit(2);
	if (currsuch)
		currsuch->next = nosuch;
	if (!(*begsuch))
		(*begsuch) = nosuch;
	assign_char((char**)&(nosuch->content), av[i]);
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
