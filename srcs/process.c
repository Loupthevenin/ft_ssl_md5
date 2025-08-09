#include "../includes/ft_ssl.h"

static char	*read_fd(int fd)
{
	char	*line;
	char	*result;
	char	*tmp;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	while ((line = get_next_line(fd)) != NULL)
	{
		tmp = ft_strjoin(result, line);
		free(result);
		result = tmp;
		free(line);
		if (!result)
			return (NULL);
	}
	return (result);
}

static char	*read_file(const char *path)
{
	int		fd;
	char	*result;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (NULL);
	result = read_fd(fd);
	close(fd);
	return (result);
}

int	process_input(t_input *input, int argc, char **argv, t_hash_func hash)
{
	int		i;
	char	*data;
	char	*digest;

	// Si flag -p
	if (input->flags.p)
	{
		data = read_fd(STDIN_FILENO);
		if (!data)
			return (EXIT_FAILURE);
		digest = hash(data);
		ft_putstr_fd("(stdin)", 1);
		free(data);
		free(digest);
	}
	// Si flag -s, hash la string
	if (input->flags.s)
	{
		digest = hash(input->str_arg);
		print_hash(input->str_arg, digest, input);
		free(digest);
	}
	// fichiers restants
	i = input->arg_i;
	while (i < argc)
	{
		data = read_file(argv[i]);
		if (!data)
			print_file_error(input->cmd_name, argv[i]);
		else
		{
			digest = hash(data);
			print_hash(argv[i], digest, input);
			free(data);
			free(digest);
		}
		i++;
	}
	// pas de flags pas de fichiers -> stdin
	if (!input->flags.p && !input->flags.s && input->arg_i == argc)
	{
		data = read_fd(STDIN_FILENO);
		if (!data)
			return (EXIT_FAILURE);
		digest = hash(data);
		print_hash("(stdin)", digest, input);
		free(data);
		free(digest);
	}
	return (EXIT_SUCCESS);
}
