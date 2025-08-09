#include "../includes/ft_ssl.h"

char	*md5_string(const char *input)
{
	char	*str;

	(void)input;
	str = ft_strdup("test");
	return (str);
}

int	md5_main(int argc, char **argv)
{
	t_input	input;

	input.cmd_name = "md5";
	if (parse_flags(&input, argc, argv) == -1)
		return (EXIT_FAILURE);
	return (process_input(&input, argc, argv, &md5_string));
}
