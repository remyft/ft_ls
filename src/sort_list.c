/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/28 20:32:13 by rfontain          #+#    #+#             */
/*   Updated: 2018/09/30 22:30:00 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ls.h"

t_lst	*sort_list(t_lst *list)
{
	t_lst	*begin;

	begin = list;
	while (list->next)
	{
		if ((list->isdir == 1 && list->next->isdir == 0)
				|| (((list->isdir == 0 && list->next->isdir == 0)
					|| (list->isdir == 1 && list->next->isdir == 1))
						&& ft_strcmp(list->name, list->next->name) > 0))
		{
			list_swap(list, list->next);
			list = ret_list(list);
		}
		else
			list = list->next;
	}
	while (begin->prev)
		begin = begin->prev;
	return (begin);
}

int		check_sort(t_lst *list, t_fg *g_fg, int sort)
{
	if (sort == 0)
	{
		if ((((*g_fg) & REVERSE
			&& ft_strcmp(list->name, list->next->name) < 0)
				|| (!((*g_fg) & REVERSE)
					&& ft_strcmp(list->name, list->next->name) > 0))
						&& !(*g_fg & DATE_SORT || *g_fg & SIZE_SORT))
			return (1);
	}
	else if (sort == 1)
	{
		if ((((*g_fg) & REVERSE && list->itime > list->next->itime)
				|| (!((*g_fg) & REVERSE)
					&& list->itime < list->next->itime)))
			return (1);
	}
	else
	{
		if ((((*g_fg) & REVERSE && list->size > list->next->size)
					|| (!((*g_fg) & REVERSE) && list->size < list->next->size)))
			return (1);
	}
	return (0);
}

int		decide_list(t_lst *list, t_fg *g_fg)
{
	if (!(*g_fg & DATE_SORT) && !(*g_fg & SIZE_SORT) &&
			check_sort(list, g_fg, 0))
		return (1);
	else if (*g_fg & DATE_SORT && !(*g_fg & SIZE_SORT) &&
			check_sort(list, g_fg, 1) && list->itime)
		return (1);
	else if (check_sort(list, g_fg, 2) && list->size_f)
		return (1);
	return (0);
}

t_lst	*sort_not_dir(t_lst *list, t_fg *g_fg)
{
	t_lst *begin;

	begin = list;
	while (list->next && list->next->isdir == 0)
	{
		if (decide_list(list, g_fg))
		{
			list_swap(list, list->next);
			list = ret_list(list);
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
		if (decide_list(list, g_fg))
		{
			list_swap(list, list->next);
			while (list->prev && list->prev->isdir == 1)
				list = list->prev;
		}
		else
			list = list->next;
	}
}
