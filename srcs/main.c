/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynakashi <ynakashi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 20:28:42 by ynakashi          #+#    #+#             */
/*   Updated: 2022/03/10 12:53:08 by ynakashi         ###   ########.fr       */
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

void	safe_free(char **ptr)
{
	free(*ptr);
	*ptr = NULL;
}

void	split_free(char **ptr)
{
	size_t	i;

	i = 0;
	while (ptr[i])
	{
		free(ptr[i]);
		i++;
	}
	free(ptr);
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	char	**split_ln;
	char	*path;
	pid_t pid;
	int status;
	int	abs_flg;

	while (1)
	{
		// signal処理の登録
		init_signal(SIGINT, sigint_before_rl);
		init_signal(SIGQUIT, SIG_IGN); // SIGQUITを無視
		line = readline(RL_MSG); // 入力受付
		if (!line) // ctrl+DではsignalではなくEOFが送られる,readlineはEOFを受け取ったらNULLを返す
		{
			write(STDERR_FILENO, "exit\n", 5);
			return (EXIT_SUCCESS);
		}
		init_signal(SIGINT, sigint_after_rl);
		init_signal(SIGQUIT, sigquit_after_rl); // プロセス実行時は無視できないのでこれを使う
		if (ft_strncmp(line, "exit", 5) == 0)
		{
			write(STDERR_FILENO, "exit\n", 5);
			safe_free(&line);
			return (EXIT_SUCCESS);
		}
		if (ft_strlen(line) == 0) // 改行だけの場合,空文字列がくる
		{
			safe_free(&line);
			continue ;
		}
		abs_flg = 0;
		path = NULL;
		split_ln = ft_split(line, ' ');
		if (!split_ln)
		{
			safe_free(&line);
			return (EXIT_FAILURE);
		}
		if (ft_strncmp(split_ln[0], "/bin/", 5) != 0)
		{
			path = ft_strjoin("/bin/", split_ln[0]);
			if (!path)
			{
				safe_free(&line);
				return (EXIT_FAILURE);
			}
		}
		else
		{
			abs_flg = 1;
		}
		// builtin funcの処理
		pid = fork();
		if (pid < 0)
		{
			perror("fork");
			safe_free(&line);
			return (EXIT_FAILURE);
		}
		else if (pid == 0) // 子プロセスは親プロセスのデータをコピーしただけで書き換えることはできない
		{
			if (abs_flg == 0)
			{
				if (execve(path, split_ln, envp) == -1)
				{
					printf("bash: %s: command not found\n", line);
					return (EXIT_FAILURE);
				}
			}
			else
			{
				if (execve(split_ln[0], split_ln, envp) == -1)
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
				write(STDERR_FILENO, "\n", 1);
				safe_free(&line);
				return (EXIT_FAILURE);
			}
		}
		add_history(line); // 履歴の付け足し
		safe_free(&line);
		split_free(split_ln);
		safe_free(&path);
	}
	return (EXIT_SUCCESS);
}
