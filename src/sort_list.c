/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/28 20:32:13 by rfontain          #+#    #+#             */
/*   Updated: 2018/10/02 19:32:41 by rfontain         ###   ########.fr       */
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

int		check_sort(t_lst *list, t_fg *e_fg, int sort)
{
	if (sort == 0)
	{
		if ((((*e_fg) & REVERSE
			&& ft_strcmp(list->name, list->next->name) < 0)
				|| (!((*e_fg) & REVERSE)
					&& ft_strcmp(list->name, list->next->name) > 0))
						&& !(*e_fg & DATE_SORT || *e_fg & SIZE_SORT))
			return (1);
	}
	else if (sort == 1)
	{
		if ((((*e_fg) & REVERSE && list->itime > list->next->itime)
				|| (!((*e_fg) & REVERSE)
					&& list->itime < list->next->itime)))
			return (1);
	}
	else
	{
		if ((((*e_fg) & REVERSE && list->size > list->next->size)
					|| (!((*e_fg) & REVERSE) && list->size < list->next->size)))
			return (1);
	}
	return (0);
}

int		decide_list(t_lst *list, t_fg *e_fg)
{
	if (!(*e_fg & DATE_SORT) && !(*e_fg & SIZE_SORT) &&
			check_sort(list, e_fg, 0))
		return (1);
	else if (*e_fg & DATE_SORT && !(*e_fg & SIZE_SORT) &&
			check_sort(list, e_fg, 1) && list->itime)
		return (1);
	else if (check_sort(list, e_fg, 2) && list->size_f)
		return (1);
	return (0);
}

t_lst	*sort_not_dir(t_lst *list, t_fg *e_fg)
{
	t_lst *begin;

	begin = list;
	while (list->next && list->next->isdir == 0)
	{
		if (decide_list(list, e_fg))
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

void	sort_dir(t_lst *list, t_fg *e_fg)
{
	t_lst *begin;

	begin = list;
	while (list && list->next)
	{
		if (decide_list(list, e_fg))
		{
			list_swap(list, list->next);
			while (list->prev && list->prev->isdir == 1)
				list = list->prev;
		}
		else
			list = list->next;
	}
}
