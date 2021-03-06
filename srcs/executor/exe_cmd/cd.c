/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kahirose <kahirose@studnt.42tokyo.jp>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 19:56:29 by ynakashi          #+#    #+#             */
/*   Updated: 2022/06/18 21:33:44 by kahirose         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static char	*get_home_value(t_envlist *envlist)
{
	size_t	len;

	len = ft_strlen("HOME");
	while (envlist)
	{
		if (ft_strncmp("HOME", envlist->key, len + 1) == 0)
			return (envlist->value);
		envlist = envlist->next;
	}
	return (NULL);
}

static int	set_oldpwd(char *oldpwd, t_envlist **envlist)
{
	t_envlist	*newlist;

	newlist = (t_envlist *)malloc(sizeof(t_envlist));
	if (!newlist)
	{
		print_error("cd: malloc", NULL, errno);
		return (1);
	}
	remove_duplicate("OLDPWD", envlist);
	newlist->key = ft_x_strdup("OLDPWD");
	newlist->value = ft_x_strdup(oldpwd);
	newlist->next = NULL;
	ms_lstadd_back(envlist, newlist);
	return (0);
}

static int	set_pwd(t_envlist **envlist)
{
	char		*pwd;
	t_envlist	*newlist;

	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
	{
		ft_putstr_fd("cd: error retrieving current directory:", STDERR_FILENO);
		ft_putstr_fd(" getcwd: cannot access parent director:", STDERR_FILENO);
		ft_putstr_fd("ies: No such file or directory\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	newlist = (t_envlist *)malloc(sizeof(t_envlist));
	if (!newlist)
	{
		print_error("cd: malloc", NULL, errno);
		free(pwd);
		return (1);
	}
	remove_duplicate("PWD", envlist);
	newlist->key = ft_x_strdup("PWD");
	newlist->value = ft_x_strdup(pwd);
	newlist->next = NULL;
	ms_lstadd_back(envlist, newlist);
	free(pwd);
	return (0);
}

static int	set_cd_env(char *oldpwd, t_envlist **envlist)
{
	int	exit_status;

	exit_status = 0;
	if (oldpwd)
		exit_status = set_oldpwd(oldpwd, envlist);
	if (exit_status == 1)
		set_pwd(envlist);
	else
		exit_status = set_pwd(envlist);
	free(oldpwd);
	return (exit_status);
}

int	my_cd(char **split_ln, t_envlist **envlist)
{
	char	*oldpwd;

	oldpwd = getcwd(NULL, 0);
	if (split_ln[1] == NULL)
	{
		if (chdir(get_home_value(*envlist)) == -1)
		{
			ft_putstr_fd("my_shell: cd: HOME not set\n", STDERR_FILENO);
			free(oldpwd);
			return (1);
		}
		return (set_cd_env(oldpwd, envlist));
	}
	if (chdir(split_ln[1]) == -1)
	{
		print_error("cd", split_ln[1], errno);
		free(oldpwd);
		return (EXIT_FAILURE);
	}
	return (set_cd_env(oldpwd, envlist));
}
