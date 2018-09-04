/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoinch.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 11:22:48 by dbaffier          #+#    #+#             */
/*   Updated: 2018/04/20 17:12:16 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoinch(char const *s1, char c)
{
	char		*str;
	size_t		i;
	size_t		s1_len;

	if (!s1 || !c)
		return (NULL);
	s1_len = ft_strlen(s1);
	if (!(str = ft_strnew(s1_len + 1)))
		return (NULL);
	i = -1;
	while (++i < s1_len)
		*(str + i) = *(s1 + i);
	*(str + i) = c;
	return (str);
}
