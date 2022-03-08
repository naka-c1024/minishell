/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynakashi <ynakashi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 20:28:42 by ynakashi          #+#    #+#             */
/*   Updated: 2022/03/08 10:53:02 by ynakashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// ctrl+C
void	sigint_before_rl(int not_use)
{
	(void)not_use;

	write(STDOUT_FILENO, "\n", 1);
	// rl_replace_line("", 0); // 入力されたものをclear, m1macだとコンパイルできないためコメントアウトしている
	rl_on_new_line(); // 次の行に移動
	rl_redisplay(); // -再表示して以上の操作を画面上に反映
}

void	sigint_after_rl(int not_use)
{
	(void)not_use;

	exit(EXIT_FAILURE);
}
// "ctrl+\"
void	sigquit_after_rl(int not_use)
{
	(void)not_use;
	exit(EXIT_FAILURE);
}

void	init_signal(int sig_num, void (*func)(int not_use))
{
	struct sigaction	act;

	ft_bzero(&act, sizeof(struct sigaction));
	act.sa_handler = func;
	if (sigaction(sig_num, &act, NULL) == -1)
		exit (EXIT_FAILURE);
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	char	**split_ln;
	char	*path;
	pid_t pid;
	int status;

	while (1)
	{
		// signal処理の登録
		init_signal(SIGINT, sigint_before_rl);
		init_signal(SIGQUIT, SIG_IGN); // SIGQUITを無視
		// 入力受付
		line = readline(RL_MSG);
		// ctrl+Dの処理
		if (!line)
		{
			write(STDERR_FILENO, "exit\n", 5);
			return (EXIT_SUCCESS);
		}
		init_signal(SIGINT, sigint_after_rl);
		init_signal(SIGQUIT, sigquit_after_rl); // プロセス実行時は無視できないのでこれを使う
		if (ft_strlen(line) > 0)
		{
			split_ln = ft_split(line, ' ');
			if (ft_strncmp(line, "exit", 5) == 0)
			{
				write(STDERR_FILENO, "exit\n", 5);
				free(line);
				return (EXIT_SUCCESS);
			}
			path = NULL;
			// if (ft_strncmp(split_ln[0], "/bin/", 5))
			// {
			// 	path = ft_strjoin("/bin/ls", split_ln[0]);
			// }
			// else
			// {
			// 	path = split_ln[0];
			// }
			// builtin funcの処理
			pid = fork();
			if (pid < 0)
			{
				perror("fork");
				free(line);
				return (EXIT_FAILURE);
			}
			else if (pid == 0) // 子プロセスは親プロセスのデータをコピーしただけで書き換えることはできない
			{
				if (execve(split_ln[0], split_ln, envp) == -1) // 絶対pathでの実行
				{
					path = ft_strjoin("/bin/", split_ln[0]);
					if (execve(path, split_ln, envp) == -1) // /bin/を付け足した絶対pathの実行
					{
						printf("bash: %s: command not found\n", line);
						return (EXIT_FAILURE);
					}
				}
			}
			else
			{
				if (wait(&status) < 0)
				{
					free(path);
					write(STDERR_FILENO, "\n", 1);
					free(line);
					return (EXIT_FAILURE);
				}
				else
				{
					free(path);
				}
			}
			// 履歴の付け足し
			add_history(line);
		}
		free(line);
	}
	return (EXIT_SUCCESS);
}
