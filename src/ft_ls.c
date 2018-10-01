/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 19:50:06 by rfontain          #+#    #+#             */
/*   Updated: 2018/10/01 21:34:40 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ls.h"

int		main(int ac, char **av)
{
	int		i;
	t_fg	g_fg;

	i = 1;
	g_fg = 0;
	while (av[i] && av[i][0] == '-' && ft_strlen(av[i]) >= 2)
	{
		if (ft_strcmp(av[i], "--") == 0)
		{
			i++;
			break ;
		}
		get_flag(av[i++], &g_fg);
	}
	if (ac - i >= 2)
		big_deal_list(i, ac, av, &g_fg);
	else
		deal_solo(i, ac, av, &g_fg);
	return (0);
}
