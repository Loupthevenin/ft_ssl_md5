#include "../includes/ft_ssl.h"

char	*sha256_string(const char *input)
{
	(void)input;
	return (NULL);
}

int	sha256_main(int argc, char **argv)
{
	t_input	input;

	ft_memset(&input, 0, sizeof(t_input));
	input.cmd_name = "sha256";
	if (parse_flags(&input, argc, argv) == -1)
		return (EXIT_FAILURE);
	return (process_input(&input, argc, argv, &sha256_string));
}
