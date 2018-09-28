/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 03:13:14 by rfontain          #+#    #+#             */
/*   Updated: 2018/09/28 20:33:29 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ls.h"

static t_indir	*ret_head(t_indir *lst)
{
	while (lst->prev)
		lst = lst->prev;
	return (lst);
}

void			sort_alpha(t_indir *names, int size)
{
	int		i;
	int		continuer;
	t_indir	*curr;

	continuer = 0;
	while (!continuer)
	{
		continuer = 1;
		i = 0;
		curr = ret_head(names);
		while (i < size && curr && curr->next)
		{
			if (i + 1 != size && ft_strcmp(curr->name, curr->next->name) > 0)
				continuer = str_swap(curr, curr->next);
			else
			{
				if (curr->next)
					curr = curr->next;
			}
			i++;
		}
		size--;
	}
}

void			sort_date(t_indir *names, int size)
{
	int		i;
	int		continuer;
	t_indir	*curr;

	continuer = 0;
	while (!continuer)
	{
		continuer = 1;
		i = 0;
		curr = ret_head(names);
		while (curr && curr->next && i < size)
		{
			if (i + 1 != size && curr->next && curr->itime < curr->next->itime)
			{
				continuer = str_swap(curr, curr->next);
				i++;
			}
			else
			{
				i++;
				curr = curr->next;
			}
		}
		size--;
	}
}

void			sort_size(t_indir *names, int size)
{
	int		i;
	int		continuer;
	t_indir	*curr;

	continuer = 0;
	while (!continuer)
	{
		continuer = 1;
		i = 0;
		curr = ret_head(names);
		while (curr && curr->next && i < size)
		{
			if (i + 1 != size && curr->next && curr->size < curr->next->size)
			{
				continuer = str_swap(curr, curr->next);
				i++;
			}
			else
			{
				i++;
				curr = curr->next;
			}
		}
		size--;
	}
}
