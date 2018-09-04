/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/08 03:11:22 by rfontain          #+#    #+#             */
/*   Updated: 2018/04/13 22:02:58 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int		ft_countw(const char *str, char c)
{
	int i;
	int count;

	i = 0;
	count = 0;
	while (str[i])
	{
		while (str[i] == c && str[i])
			i++;
		if (str[i] != c && str[i] != '\0')
		{
			count++;
			while (str[i] != c && str[i])
				i++;
		}
	}
	return (count);
}

static	int		ft_strlensp(const char *str, char c)
{
	int i;

	i = 0;
	while (*str != c)
	{
		if (*str == '\0')
			break ;
		str++;
		i++;
	}
	return (i);
}

char			**ft_strsplit(char const *s, char c)
{
	char	**split;
	int		k;
	int		nbword;

	if (s == NULL)
		return (NULL);
	if ((split = (char**)malloc(sizeof(char*) * (ft_countw(s, c) + 1))) == NULL)
		return (NULL);
	nbword = ft_countw(s, c);
	split[nbword + 1] = 0;
	k = 0;
	while (nbword--)
	{
		while (*s == c)
			s++;
		if (*s != c && *s != '\0')
		{
			if ((split[k] = ft_strsub(s, 0, ft_strlensp(s, c))) == NULL)
				return (NULL);
			s += ft_strlensp(s, c) + 1;
			k++;
		}
	}
	split[k] = NULL;
	return (split);
}
