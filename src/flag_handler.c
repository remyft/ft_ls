/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flag_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 22:52:09 by rfontain          #+#    #+#             */
/*   Updated: 2018/10/01 21:47:22 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ls.h"

static void	deal_unsort(t_fg *g_fg)
{
	*g_fg |= UNSORT;
	if (*g_fg & REVERSE)
		*g_fg &= ~REVERSE;
	if (*g_fg & DATE_SORT)
		*g_fg &= ~DATE_SORT;
	if (*g_fg & SIZE_SORT)
		*g_fg &= ~SIZE_SORT;
}

static void	put_error_flag(char fl, t_fg *g_fg)
{
	if (fl == '1')
	{
		*g_fg &= ~LONG_LISTING;
		return ;
	}
	ft_putstr_fd("ft_ls: illegal option -- ", 2);
	ft_putchar_fd(fl, 2);
	ft_putchar_fd('\n', 2);
	ft_putstr_fd("usage: ./ft_ls [-ARSTaflnort1] [file ...]\n", 2);
	exit(1);
}

static void	set_flags(char fl, t_fg *g_fg)
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
	else if (fl == 'r' && !(*g_fg & UNSORT))
		*g_fg |= REVERSE;
	else if (fl == 't' && !(*g_fg & UNSORT))
		*g_fg |= DATE_SORT;
	else if (fl == 'T')
		*g_fg |= LONG_T;
	else if (fl == 'S' && !(*g_fg & UNSORT))
		*g_fg |= SIZE_SORT;
	else if (fl != '-' && fl != 'r' && fl != 't' && fl != 'S')
		put_error_flag(fl, g_fg);
	if (fl == 'f')
		deal_unsort(g_fg);
	else if (fl == 'n')
		*g_fg |= GET_ID;
}

void		get_flag(char *flags, t_fg *g_fg)
{
	int i;

	i = 1;
	if (ft_strlen(flags) == 2)
		set_flags(flags[1], g_fg);
	else
	{
		if (ft_strstr(flags + 1, "-"))
			put_error_flag('-', g_fg);
		while (flags[i])
			set_flags(flags[i++], g_fg);
	}
}
