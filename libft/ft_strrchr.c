/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/04 22:58:50 by rfontain          #+#    #+#             */
/*   Updated: 2018/04/13 17:30:58 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

char	*ft_strrchr(const char *str, int c)
{
	int		i;
	char	*find;
	char	*cp;

	i = 0;
	cp = (char*)str;
	find = NULL;
	while (cp[i])
	{
		if (cp[i] == c)
			find = &cp[i];
		i++;
	}
	if (cp[i] == c)
		find = &cp[i];
	return (find);
}
