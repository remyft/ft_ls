/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flag_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 22:52:09 by rfontain          #+#    #+#             */
/*   Updated: 2018/09/29 18:01:42 by rfontain         ###   ########.fr       */
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
	ft_putstr_fd("ft_ls: illegal option -- ", 2);
	ft_putchar_fd(fl, 2);
	ft_putchar_fd('\n', 2);
	ft_putstr_fd("usage: ft_ls [-ARSTafnort1] [file ...]\n", 2);
	exit(1);
}

void	set_flags(char fl, t_fg *g_fg)
{
	if (fl == 'R')
		*g_fg |= RECURSIVE;
	else if (fl == 'l' || fl == 'n')
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
	else if (fl == 'T')
		*g_fg |= LONG_T;
	else if (fl == 'S')
		*g_fg |= SIZE_SORT;
	else if (fl != '1' && fl != '-')
		put_error_flag(fl);
	if (fl == 'f')
		*g_fg |= UNSORT;
	else if (fl == 'n')
		*g_fg |= GET_ID;
}
