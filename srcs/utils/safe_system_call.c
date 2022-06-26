/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_system_call.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynakashi <ynakashi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/05 16:10:16 by kahirose          #+#    #+#             */
/*   Updated: 2022/06/26 09:59:17 by ynakashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	safe_func(int result_status)
{
	if (result_status == -1)
	{
		perror("my_shell");
		exit(1);
	}
	return (result_status);
}

int	safe_func_with_file(int result_status, char *file_name)
{
	if (result_status == -1)
	{
		safe_func(write(1, "my_shell: ", 11));
		perror(file_name);
		exit (1);
	}
	return (result_status);
}
