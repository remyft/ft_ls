/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utiles.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 03:06:59 by rfontain          #+#    #+#             */
/*   Updated: 2018/10/01 21:53:49 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ls.h"

int		nb_len(int nb)
{
	int i;

	i = 1;
	while (nb >= 10)
	{
		nb /= 10;
		i++;
	}
	return (i);
}

void	ft_putname(char *name)
{
	int		i;
	int		j;

	i = 0;
	while (name[i])
		i++;
	j = 0;
	while (i - j && name[i - j] != '/')
		j++;
	j--;
	ft_putstr_fd(&name[i - j], 2);
}

int		size_len(t_indir *lst)
{
	if (lst->major < 0)
		return (nb_len(lst->size));
	else
		return (nb_len(lst->major) + nb_len(lst->minor) + 2);
}

int		max_int(int nb1, int nb2)
{
	return ((nb1 > nb2) ? nb1 : nb2);
}

int		len_list(t_indir *list, t_fg *g_fg)
{
	t_indir	*curr;
	int		i;

	i = 0;
	curr = list;
	while (curr)
	{
		if (curr->name[0] != '.' || (cmp_file(curr->name)
					&& *g_fg & HIDEN_FILE))
			i++;
		curr = curr->next;
	}
	return (i);
}
