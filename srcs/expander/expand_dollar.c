/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynakashi <ynakashi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/18 08:18:30 by ynakashi          #+#    #+#             */
/*   Updated: 2022/06/26 09:53:31 by ynakashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"

static void	expand_exit_status(char **dollar_str, size_t dollar_loc)
{
	char	*pre_dollar_str;
	char	*post_dollar_str;
	size_t	post_size;
	char	*status;
	char	*tmp;

	post_size = ft_strlen(*dollar_str + dollar_loc + 2);
	pre_dollar_str = ft_x_substr(*dollar_str, 0, dollar_loc);
	post_dollar_str = ft_x_substr(*dollar_str, dollar_loc + 2, post_size);
	status = ft_x_itoa(g_exit_status);
	free(*dollar_str);
	tmp = ft_x_strjoin(pre_dollar_str, status);
	*dollar_str = ft_x_strjoin(tmp, post_dollar_str);
	free(pre_dollar_str);
	free(post_dollar_str);
	free(status);
	free(tmp);
}

static size_t	loc_meta_char(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (' ' <= str[i] && str[i] <= '/')
			return (i);
		else if (':' <= str[i] && str[i] <= '@')
			return (i);
		else if ('[' <= str[i] && str[i] <= '^')
			return (i);
		else if ('{' <= str[i] && str[i] <= '~')
			return (i);
		i++;
	}
	return (i);
}

static void	three_strjoin(char **str, char *value, char *pre_dl, char *post_dl)
{
	char	*env_var;
	char	*tmp;

	free(*str);
	env_var = ft_x_strdup(value);
	tmp = ft_x_strjoin(pre_dl, env_var);
	*str = ft_x_strjoin(tmp, post_dl);
	free(env_var);
	free(tmp);
	free(pre_dl);
	free(post_dl);
}

static void	expand_env_var(char **str, t_envlist *envlist, size_t dollar_loc)
{
	char	*pre_dollar;
	char	*post_dollar;
	size_t	meta_loc;
	size_t	post_size;

	pre_dollar = ft_x_substr(*str, 0, dollar_loc);
	meta_loc = loc_meta_char(*str + dollar_loc + 1);
	post_size = ft_strlen(*str + dollar_loc + meta_loc + 1);
	post_dollar = ft_x_substr(*str, dollar_loc + meta_loc + 1, post_size);
	while (envlist)
	{
		if (ft_strncmp(&((*str)[dollar_loc + 1]), envlist->key,
			ft_strlen(*str + dollar_loc) - post_size - 1) == 0)
		{
			three_strjoin(str, envlist->value, pre_dollar, post_dollar);
			return ;
		}
		envlist = envlist->next;
	}
	free(*str);
	*str = ft_x_strjoin(pre_dollar, post_dollar);
	free(pre_dollar);
	free(post_dollar);
}

void	expand_dollar(char **str, t_envlist *envlist)
{
	size_t	i;

	i = 0;
	while ((*str)[i])
	{
		if ((*str)[i] == '$')
		{
			if ((*str)[i + 1] == '?')
			{
				expand_exit_status(str, i);
				break ;
			}
			else
			{
				expand_env_var(str, envlist, i);
				break ;
			}
		}
		i++;
	}
}
