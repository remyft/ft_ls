/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 23:14:40 by rfontain          #+#    #+#             */
/*   Updated: 2018/09/07 06:46:38 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ls.h"

t_lst	*lst_new(char *file)
{
	t_lst *new;

	new = ft_memalloc(sizeof(t_lst));
	new->name = ft_strdup(file);
	return (new);
}

void	put_list(t_indir *list, int size)
{
	int i;

	i = 0;
	while (i < size)
	{
		if (!(list->name[0] == '.' && !(g_fg & ALL_FILE)))
			ft_putendl(list->name);
		list = (g_fg & REVERSE) ? list->prev : list->next;
		i++;
	}
	ft_putchar('\n');
}


void	put_dlist(t_indir *list, int size, char *name)
{
	int		i;
	t_lst	*file;
	char	*tmp;


	i = 0;
	file = NULL;
	while (i < size)
	{
		tmp = ft_strdup(name);
		if (list->name[0] == '.' && !(g_fg & ALL_FILE))
		{
			list = (g_fg & REVERSE) ? list->prev : list->next;
			i++;
		}
		else if (list->type & DT_DIR && (ft_strcmp(list->name, ".") != 0) && (ft_strcmp(list->name, "..") != 0))
		{
			tmp  = ft_strjoinfree(tmp, "/", 1);
			tmp = ft_strjoinfree(tmp, list->name, 1);
			ft_putstr(tmp);
			ft_putstr(":\n");
			file = lst_new(tmp);
			deal_file(file);
			list = (g_fg & REVERSE) ? list->prev :  list->next;
			i++;
		}
		else
		{
			list = (g_fg & REVERSE) ? list->prev : list->next;
			i++;
		}
		free(tmp);
	}
}

int		get_mon(char *month)
{
	char	*tab[12];
	int		i;
	char	*tmp;

	tmp = month;
	tmp[3] = '\0';
	i = 0;
	tab[0] = "Jan";
	tab[1] = "Feb";
	tab[2] = "Mar";
	tab[3] = "Apr";
	tab[4] = "May";
	tab[5] = "Jun";
	tab[6] = "Jul";
	tab[7] = "Aug";
	tab[8] = "Sep";
	tab[9] = "Oct";
	tab[10] = "Nov";
	tab[11] = "Dec";
	while (i <= 11 && strcmp(tmp, tab[i]) != 0)
		i++;
	return (i + 1);
}

char	*get_time(char *file)
{
	time_t	now;
	char	*ch_now;
	char	*ret;
	char	*tmp;
	int		i;


	ret = (char*)malloc(sizeof(char) * 13);
	(void)file;
	now = time(&now);
	ch_now = ctime(&now);
	tmp = file;
	tmp[10] = '\0';
	ret = strcpy(ret, &tmp[4]);
	ret[6] = ' ';
	i = 6;
	if (get_mon(&file[4]) > 6 && get_mon(&file[4]) - get_mon(&ch_now[4]) <= 6)
		while (++i < 12)
			ret[i] = tmp[i + 4];
	else
		while(++i < 11)
			ret[i] = tmp[i + 20];
	ret[i] = '\0';
	return (ret);
}

void	put_llist(t_indir *list, int size)
{
	int		i;
	char	*time;

	i = 0;
	while (i < size)
	{
		if (!(list->name[0] == '.' && !(g_fg & ALL_FILE)))
		{
			ft_putend(list->right, "  ");
			ft_putnbr(list->nb_link);
			ft_putchar('\t');
			ft_putend(list->uid_user, "  ");
			ft_putend(list->gid_user, "  ");
			ft_putnbr(list->size);
			ft_putstr(" \t");
			time = get_time(list->time);
			ft_putend(time, "  ");
			free(time);
			ft_putendl(list->name);
		}
		list = (g_fg & REVERSE) ? list->prev : list->next;
		i++;
	}
}
