/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/28 20:32:13 by rfontain          #+#    #+#             */
/*   Updated: 2018/09/28 20:32:44 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ls.h"

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
