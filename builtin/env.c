void	env(char *env[])
{
	int	i = 0;

	while(env[i])
	{
		printf("%s\n",env[i]);
		i++;
	}
}