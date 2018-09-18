/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 19:50:06 by rfontain          #+#    #+#             */
/*   Updated: 2018/09/18 03:21:21 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ls.h"

int		main(int ac, char **av)
{
	int		i;

	i = 1;
	while (av[i] && av[i][0] == '-' && ft_strlen(av[i]) >= 2)
		get_flag(av[i++]);
	if (ac - i >= 2)
		big_deal_list(i, ac, av);
	else
		deal_solo(i, ac, av);
	return (0);
}
