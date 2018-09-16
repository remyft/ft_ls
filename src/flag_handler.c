/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flag_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 22:52:09 by rfontain          #+#    #+#             */
/*   Updated: 2018/09/16 18:13:59 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ls.h"

void	get_flag(char *flags)
{
	int i;

	i = 1;
	if (ft_strlen(flags) == 2)
		set_flags(flags[1]);
	else
		while (flags[i])
			set_flags(flags[i++]);
}

void	put_error_flag(char fl)
{
	ft_putstr("ft_ls: illegal option -- ");
	ft_putchar(fl);
	ft_putchar('\n');
	ft_putstr("usage: ft_ls [-lRart] [file ...]\n");
	exit(1);
}

void	set_flags(char fl)
{
	if (fl == 'R')
		g_fg |= RECURSIVE;
	else if (fl == 'l')
		g_fg |= LONG_LISTING;
	else if (fl == 'a')
		g_fg |= ALL_FILE;
	else if (fl == 'r')
		g_fg |= REVERSE;
	else if (fl == 't')
		g_fg |= DATE_SORT;
	else if (fl == '1')
		return ;
	else if (fl == '-')
		return ;
	else
		put_error_flag(fl);
}

void	unset(int flag)
{
	if (g_fg & flag)
		g_fg &= ~flag;
}

