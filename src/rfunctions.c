/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/22 11:03:08 by rfontain          #+#    #+#             */
/*   Updated: 2018/10/02 19:34:40 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ls.h"

void	assign_char(char **str, char *assign)
{
	if (!(*str = ft_strdup(assign)))
		exit(2);
}

void	join_char(char **str, char *beg, char *end, int choice)
{
	if (!choice)
	{
		if (!(*str = ft_strjoin(beg, end)))
			exit(2);
	}
	else
	{
		if (!(*str = ft_strjoinfree(beg, end, choice)))
			exit(2);
	}
}

void	double_int(int *targ1, int *targ2, int val1, int val2)
{
	*targ1 = val1;
	*targ2 = val2;
}

int		max_link_len(t_indir *list, t_fg *e_fg)
{
	int max;
	int size;

	max = 0;
	while (list)
	{
		if ((size = nb_len(list->nb_link)) > max && (*e_fg & ALL_FILE
				|| (*e_fg & HIDEN_FILE && cmp_file(list->name))
					|| list->name[0] != '.'))
			max = size;
		list = (*e_fg & REVERSE) ? list->prev : list->next;
	}
	return (max);
}

int		max_nblen(t_indir *lst, t_fg *e_fg)
{
	int max;
	int size;

	max = 0;
	while (lst)
	{
		if (lst->major < 0)
		{
			if ((size = nb_len(lst->size)) > max && (*e_fg & ALL_FILE
					|| (*e_fg & HIDEN_FILE && cmp_file(lst->name))
						|| lst->name[0] != '.'))
				max = size;
		}
		else
		{
			if ((size = nb_len(lst->major) + nb_len(lst->minor) + 2) > max
					&& (*e_fg & ALL_FILE || (*e_fg & HIDEN_FILE
						&& cmp_file(lst->name)) || lst->name[0] != '.'))
				max = size;
		}
		lst = (*e_fg & REVERSE ? lst->prev : lst->next);
	}
	return (max);
}
