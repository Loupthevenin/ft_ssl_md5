#include "../includes/ft_ssl.h"

char	*md5_string(const char *input)
{
	(void)input;
	return (NULL);
}

int	md5_main(int argc, char **argv)
{
	t_input	input;

	if (parse_flags(&input, argc, argv) == -1)
		return (EXIT_FAILURE);
	return (process_input(&input, argc, argv, &md5_string));
}
