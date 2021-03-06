/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kahirose <kahirose@studnt.42tokyo.jp>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/14 23:27:47 by kahirose          #+#    #+#             */
/*   Updated: 2022/06/07 04:25:39 by kahirose         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	free_2darray(char **darray)
{
	size_t	i;

	i = 0;
	while (darray[i])
	{
		free(darray[i]);
		i++;
	}
	free(darray);
}

char	**lst_to_arr(t_list *arglst)
{
	char	**rtn;
	size_t	i;
	t_list	*cp_arglst;
	size_t	list_cnt;
	size_t	content_len;

	cp_arglst = arglst;
	list_cnt = 0;
	while (cp_arglst)
	{
		list_cnt++;
		cp_arglst = cp_arglst->next;
	}
	rtn = x_malloc(sizeof(char *) * (list_cnt) + 1);
	i = 0;
	while (arglst)
	{
		content_len = ft_strlen(arglst->content);
		rtn[i] = (char *)x_malloc(content_len + 1);
		ft_strlcpy(rtn[i], arglst->content, content_len + 1);
		arglst = arglst->next;
		i++;
	}
	rtn[i] = NULL;
	return (rtn);
}
