/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 03:13:14 by rfontain          #+#    #+#             */
/*   Updated: 2018/09/19 17:32:38 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ls.h"

void	sort_alpha(t_indir *names, int size)
{
	int		i;
	int		continuer;
	t_indir	*curr;

	continuer = 0;
	while (!continuer)
	{
		continuer = 1;
		i = 0;
		curr = names;
		while (i < size)
		{
			if (i + 1 != size && ft_strcmp(curr->name, curr->next->name) > 0)
				continuer = str_swap(curr);
			else
			{
				i++;
				curr = curr->next;
			}
		}
		size--;
	}
}

void	sort_date(t_indir *names, int size)
{
	int		i;
	int		continuer;
	t_indir	*curr;

	continuer = 0;
	while (!continuer)
	{
		continuer = 1;
		i = 0;
		curr = names;
		while (i < size)
		{
			if (i + 1 != size && curr->itime < curr->next->itime)
				continuer = str_swap(curr);
			else
			{
				i++;
				curr = curr->next;
			}
		}
		size--;
	}
}

t_lst	*sort_list(t_lst *list)
{
	t_lst	*begin;

	begin = list;
	while (list->next)
	{
		if (list->isdir == 1 && list->next->isdir == 0)
		{
			list_swap(list, list->next);
			list = begin;
		}
		else
			list = list->next;
	}
	return (begin);
}

t_lst	*sort_not_dir(t_lst *list, t_fg *g_fg)
{
	t_lst *begin;

	begin = list;
	while (list->next && list->next->isdir == 0)
	{
		if (((*g_fg) & REVERSE &&
			ft_strcmp(list->name, list->next->name) < 0) ||
				(!((*g_fg) & REVERSE) &&
					ft_strcmp(list->name, list->next->name) > 0))
		{
			list_swap(list, list->next);
			list = begin;
		}
		else
			list = list->next;
	}
	if (begin->isdir == 0)
		return (list->next);
	else
		return (list);
}

void	sort_dir(t_lst *list, t_fg *g_fg)
{
	t_lst *begin;

	begin = list;
	while (list && list->next)
	{
		if (((*g_fg) & REVERSE &&
			ft_strcmp(list->name, list->next->name) < 0) ||
				(!((*g_fg) & REVERSE) &&
					ft_strcmp(list->name, list->next->name) > 0))
		{
			list_swap(list, list->next);
			list = begin;
		}
		else
			list = list->next;
	}
}
