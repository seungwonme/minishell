#include "../../minishell/minishell.h"

int	process_count(t_list *lst)
{
	int	process;

	process = 1;
	while(lst != NULL)
	{
		if(ft_strncmp(lst->token, "|", 2) == 0)
			process++;
		lst = lst->next;
	}
	return (process);
}

int	cmd_size_count(t_list *lst)
{
	int	size;

	size = 1;
	while(lst != NULL && ft_strncmp(lst->token, "|", 2) != 0 )
	{
		size++;
		lst = lst->next;
	}
	 return (size);
}

int	count_cmd_count(t_list *lst)
{
	int	len;

	len = 1;
	while(lst != NULL && ft_strncmp((lst)->token, "|", 2) != 0)
	{
		if(lst->state == CMD)
			len++;
		lst = (lst)->next;
	}
	return len;
}

char	**make_cmd(t_list *lst)
{
	int		i;
	int		cmd_len;
	char	**cmd;

	i = 0;
	cmd_len = count_cmd_count(lst);
	cmd = ft_calloc(cmd_len + 1, sizeof(char *));
	while(lst != NULL && ft_strncmp((lst)->token, "|", 2) != 0)
	{
		if(lst->state == CMD)
		{
			cmd[i] = (lst)->token;
			i++;
		}
		lst = (lst)->next;
	}
	if (lst != NULL)
		lst = (lst)->next;
	return (cmd);
}

char	**make_envp(t_env *env)
{
	char	**envp;
	t_env	*tmp;
	char	*mem;
	int		i;

	tmp = env;
	i = 0;
	while (tmp != NULL)
	{
		i++;
		tmp = tmp->next;
	}
	envp = (char **)ft_calloc((i + 1), sizeof(char *));
	i = 0;
	while (env != NULL)
	{
		mem = ft_strjoin(env->key, "=");
		envp[i] = ft_strjoin(mem, env->value);
		free(mem);
		env = env->next;
		++i;
	}
	return (envp);
}

char	*path_join(char **path, char *cmd)
{
	char	*tmp;
	int		i;

	if (path == NULL)
		return (cmd);
	i = 0;
	while (path[i] != NULL)
	{
		tmp = ft_strjoin(path[i], cmd);
		if (access(tmp, X_OK) == 0)
			break ;
		free(tmp);
		++i;
	}
	if (path[i] == NULL)
		return (cmd);
	return (tmp);
}

void	free_envp(char **envp)
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
	{
		free(envp[i]);
		++i;
	}
	free(envp);
}

void	move_next_syntax(t_list **lst)
{
	int		i;
	char	**cmd;

	while(*lst != NULL && ft_strncmp((*lst)->token, "|", 2) != 0)
		*lst = (*lst)->next;
	if (*lst != NULL)
		*lst = (*lst)->next;
}

void	connect_pipe(t_vars *vars, pid_t *pid, int process, char **path)
{
	int		(*pipe_fd)[2];
	int		i;
	int		size;
	char	**cmd;
	char	**envp;
	t_list	*lst;

	lst = vars->lst;
	envp = make_envp(vars->env);
	if(process > 1)
		pipe_fd = ft_calloc(process - 1, sizeof(int [2]));
	i = 0;
	while (process > i) //cmd가 있는 경우.
	{
		cmd = make_cmd(lst);
		if(i != process - 1)
			pipe(pipe_fd[i]);
		pid[i] = fork();
		if (pid[i] == 0)
		{
			// builtin_fuc(vars);
			cmd[0] = path_join(path, cmd[0]);
			child(i, process - 1, pipe_fd, cmd, envp);//status추가.
		}
		free(cmd);
		if(i != 0)
		{
			close(pipe_fd[i - 1][0]);     //근데 close 실패하면 원래 exit이 맞나? 글고 애초에 실패할 일이 있으려나 일케 하면...?
			close(pipe_fd[i - 1][1]);
		}
		move_next_syntax(&lst);
		i++;
	}
	free_envp(envp);
	if (process > 1)
		free(pipe_fd);
}

int	builtin_fuc(t_vars *vars)
{
	t_list	*lst;

	lst = vars->lst;
	if (ft_strncmp(lst->token, "cd", 3) == 0)
		b_cd(lst);
	else if (ft_strncmp(lst->token, "pwd", 4) == 0)
		b_pwd();
	else if (ft_strncmp(lst->token, "echo", 5) == 0)
		b_echo(lst);
	else if (ft_strncmp(lst->token, "export", 7) == 0)
		b_export(lst, vars->env);
	else if (ft_strncmp(lst->token, "env", 4) == 0)
		b_env(lst, vars->env);
	// else if (ft_strncmp(lst->token, "unset", 6) == 0)
	// 	b_unset(lst, vars->env);
	else if (ft_strncmp(lst->token, "exit", 5) == 0)
		b_exit(0);
	return (1);
}

char	**parse_path(t_env *env)
{
	char	**path;

	while (env != NULL)
	{
		if (ft_strncmp(env->key, "PATH", 5) == 0)
		{
			path = ft_split(env->value, ':', '/');
			return (path);
		}
		env = env->next;
	}
	return (NULL);
}

void	free_path(char **path)
{
	int	i;

	i = 0;
	while (path[i] != NULL)
	{
		free(path[i]);
		i++;
	}
	free(path);
}

void	execute(t_vars *vars)
{
	char		**path;
	int			process;
	pid_t		*pid;
	int			stat;

	path = parse_path(vars->env);
	process = process_count(vars->lst);
	pid = ft_calloc(process, sizeof(pid_t));
	connect_pipe(vars, pid, process, path);
	use_waitpid(pid[process - 1], &stat, 0);
	while(wait(NULL) != -1);
	free_path(path);
	free(pid);
}

// 리다이렉션 여러개일 때 어떻게 할건지 토의 필요.
// cat < outfile < infile > infile 하면 infile이 빈 파일이 되는데
// cat < outfile < infile > infile2 하면 infile2 제대로 입력됨.
