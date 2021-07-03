#include "pipex.h"

// FREE
void	destroy_vars(t_struct *vars)
{
	(void)vars;
}

// ERROR
void	pipex_error(char *str, t_struct *vars)
{
	perror(str);
	if (vars)
		destroy_vars(vars);
	exit(1);
}

void	exec_error(char ***cmd, char **path, char *str)
{
	ft_putstr_fd("str", 2);
	if (*cmd)
	{
		ft_putendl_fd((*cmd)[0], 2);
		ft_tabdel(cmd);
	}
	if (*path)
		ft_strdel(path);
	exit(-1);
}

char	*build_full_path(char *file, char **envp)
{
	char	*full_path;
	char	*workdir;
	char	*tmp;
	int		i;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PWD=", 4))
		i++;
	if (!envp[i] || !(envp[i] + 4))
		return (NULL);
	workdir = (char *)((ptrdiff_t)(envp[i] + 4) * (envp[i][4] != 0));
	tmp = ft_strjoin(workdir, "/");
	if (!tmp)
		pipex_error("pipex: ", NULL);
	full_path = ft_strjoin(tmp, file);
	ft_strdel(&tmp);
	if (!full_path)
		pipex_error("pipex: ", NULL);
	return (full_path);
}

char	*get_cmd_path(char **envp, char *cmd_name)
{
	char	**split;
	char	*cmd_path;
	int		i;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH", 4))
		i++;
	if (!envp[i])
		return (NULL);
	split = ft_split(envp[i], ':');
	if (!split)
		return (NULL);
	i = 0;
	while (split[i] && access(build_full_path(envp)))
		i++;
	ft_tabdel(&split);
	return (cmd_path);
}

void	child_process(char **argv, char **envp, int pipefd[2])
{
	char	**cmd;
	char	*full_path;
	int		fd;

	close(pipefd[0]);
	dup2(pipefd[1], 1);
	close(pipefd[1]);
	full_path = build_full_path(argv[1], envp);
	if (!full_path)
		pipex_error("file doesn't exist", NULL);
	fd = open(full_path, O_RDONLY, 0);
	if (fd < 0)
		pipex_error("pipex: ", NULL);
	dup2(fd, 0);
	cmd = ft_split(argv[2], ' ');
	if (execve(get_cmd_path(envp, cmd[0]), cmd, envp) == -1)
		exec_error(&cmd, full_path, "pipex: command not found: ");
	ft_tabdel(&cmd);
	ft_strdel(&full_path);
	exit(0);
}

void	parent_process(char **argv, char **envp, int pipefd[2])
{
	(void)argv;
	(void)envp;
	close(pipefd[1]);
	if (wait(NULL) == (pid_t)-1)
		pipex_error("pipex: child process failed", NULL);
}

int		main(int argc, char **argv, char **envp)
{
	int		pipefd[2];
	pid_t	pid;

	if (argc != 5)
		pipex_error("wrong number of arguments\n", NULL);
	if (pipe(pipefd) < 0)
		pipex_error("pipex: ", NULL);
	pid = fork();
	if (pid < 0)
		pipex_error("pipex: ", NULL);
	else if (pid == 0)
		child_process(argv, envp, pipefd);
	else
		parent_process(argv, envp, pipefd);
	return (0);
}
