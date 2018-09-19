/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 03:11:04 by rfontain          #+#    #+#             */
/*   Updated: 2018/09/19 13:53:05 by rfontain         ###   ########.fr       */
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
	char	*tmp;
	int		i;

	ret = (char*)malloc(sizeof(char) * 13);
	now = time(&now);
	tmp = file;
	tmp[10] = '\0';
	ret = ft_strcpy(ret, &tmp[4]);
	ret[6] = ' ';
	i = 6;
	if (now - itime < 15778800 && now - itime >= 0)
		while (++i < 12)
			ret[i] = tmp[i + 4];
	else
	{
		while (++i < 11)
			ret[i] = tmp[i + 13];
		ret[i++] = ' ';
	}
	ret[i] = '\0';
	tmp[10] = ' ';
	tmp[24] = '\0';
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
	ret = malloc(sizeof(char) * (2 + size - stsize));
	while (i < size - stsize + 1)
		ret[i++] = ' ';
	ret[i] = '\0';
	return (ret);
}

int		is_major(t_indir *lst)
{
	int ret;

	ret = 0;
	while (lst)
	{
		if (lst->size >= 1000 && lst->major < 0 && cmp_file(lst->name))
			return (0);
		if (lst->minor >= 0)
			ret = 1;
		lst = lst->next;
	}
	return (ret);
}
