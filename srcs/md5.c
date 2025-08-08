#include "../includes/ft_ssl.h"

int	md5_main(int argc, char **argv)
{
	t_input	input;

	if (parse_flags(&input, argc, argv) == -1)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
