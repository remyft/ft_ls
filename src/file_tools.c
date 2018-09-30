/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 03:11:04 by rfontain          #+#    #+#             */
/*   Updated: 2018/09/30 21:47:41 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ls.h"

int		cmp_file(char *name)
{
	if (ft_strcmp(name, ".") != 0 && ft_strcmp(name, "..") != 0)
		return (1);
	return (0);
}

char	*get_time(char *file, time_t itime)
{
	time_t	now;
	char	*ret;
	int		i;

	if (!(ret = (char*)malloc(sizeof(char) * 13)))
		exit(2);
	now = time(&now);
	file[10] = '\0';
	ret = ft_strcpy(ret, &file[4]);
	ret[6] = ' ';
	i = 6;
	if (now - itime < 15778800 && now - itime >= 0)
		while (++i < 12)
			ret[i] = file[i + 4];
	else
	{
		while (++i < 11)
			ret[i] = file[i + 13];
		ret[i++] = ' ';
	}
	ret[i] = '\0';
	file[10] = ' ';
	file[24] = '\0';
	return (ret);
}

void	max_size(t_indir *lst, int *uid_size, int *gid_size)
{
	while (lst->prev)
		lst = lst->prev;
	while (lst)
	{
		(*uid_size) = max_int(ft_strlen(lst->uid_user), (*uid_size));
		(*gid_size) = max_int(ft_strlen(lst->gid_user), (*gid_size));
		lst = lst->next;
	}
}

char	*nb_space(char *str, int nb, int size)
{
	int		stsize;
	int		i;
	char	*ret;

	i = 0;
	stsize = 0;
	if (str)
		stsize = ft_strlen(str);
	else
		stsize = nb;
	if (!(ret = malloc(sizeof(char) * (2 + size - stsize))))
		exit(2);
	while (i < size - stsize + 1)
		ret[i++] = ' ';
	ret[i] = '\0';
	return (ret);
}
