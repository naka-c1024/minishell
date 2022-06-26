/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xalloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynakashi <ynakashi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/28 03:37:49 by kahirose          #+#    #+#             */
/*   Updated: 2022/06/26 10:03:22 by ynakashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*x_malloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
	{
		perror("my_shell");
		exit(1);
	}
	return (ptr);
}

void	*ft_x_calloc(size_t count, size_t size)
{
	void	*ptr;

	ptr = ft_calloc(count, size);
	if (!ptr)
	{
		perror("my_shell");
		exit(1);
	}
	return (ptr);
}

t_list	*ft_x_lstnew(void *content)
{
	t_list	*new_element;

	new_element = ft_lstnew(content);
	if (!new_element)
	{
		perror("my_shell");
		exit(1);
	}
	return (new_element);
}
