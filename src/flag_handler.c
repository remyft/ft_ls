/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flag_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 22:52:09 by rfontain          #+#    #+#             */
/*   Updated: 2018/09/28 20:41:59 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ls.h"

void	get_flag(char *flags, t_fg *g_fg)
{
	int i;

	i = 1;
	if (ft_strlen(flags) == 2)
		set_flags(flags[1], g_fg);
	else
		while (flags[i])
			set_flags(flags[i++], g_fg);
}

void	put_error_flag(char fl)
{
	ft_putstr("ft_ls: illegal option -- ");
	ft_putchar(fl);
	ft_putchar('\n');
	ft_putstr("usage: ft_ls [-ARSTafort1] [file ...]\n");
	exit(1);
}

void	set_flags(char fl, t_fg *g_fg)
{
	if (fl == 'R')
		*g_fg |= RECURSIVE;
	else if (fl == 'l')
		*g_fg |= LONG_LISTING;
	else if (fl == 'o')
		*g_fg |= LONGO;
	else if (fl == 'a' || fl == 'f')
		*g_fg |= ALL_FILE;
	else if (fl == 'A')
		*g_fg |= HIDEN_FILE;
	else if (fl == 'r')
		*g_fg |= REVERSE;
	else if (fl == 't')
		*g_fg |= DATE_SORT;
	else if (fl == '1' || fl == '-')
		return ;
	else if (fl == 'T')
		*g_fg |= LONG_T;
	else if (fl == 'S')
		*g_fg |= SIZE_SORT;
	else
		put_error_flag(fl);
	if (fl == 'f')
		*g_fg |= UNSORT;
}
