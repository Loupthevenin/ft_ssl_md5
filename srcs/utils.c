#include "../includes/ft_ssl.h"

static void	strip_newline(char *s)
{
	size_t	len;

	len = ft_strlen(s);
	if (len > 0 && s[len - 1] == '\n')
		s[len - 1] = '\0';
}

void	print_p_case(const char *data, const char *digest, t_input *input)
{
	char	*tmp;

	tmp = ft_strdup(data);
	if (!tmp)
		return ;
	strip_newline(tmp);
	if (input->flags.q)
	{
		ft_putendl_fd(tmp, STDOUT_FILENO);
		ft_putendl_fd((char *)digest, STDOUT_FILENO);
	}
	else
		ft_printf("(%s\"%s\")= %s\n", "", tmp, digest); // affichage compact
	free(tmp);
}

void	print_s_or_file(const char *label, const char *digest, t_input *input,
		int is_file)
{
	int	is_stdin;

	is_stdin = (ft_strcmp(label, "(stdin)") == 0);
	if (input->flags.q)
		ft_putendl_fd((char *)digest, STDOUT_FILENO);
	else if (input->flags.r)
	{
		ft_putstr_fd((char *)digest, STDOUT_FILENO);
		ft_putchar_fd(' ', STDOUT_FILENO);
		if (is_stdin || is_file)
			ft_putendl_fd((char *)label, STDOUT_FILENO);
		else
			ft_printf("\"%s\"\n", label);
	}
	else // format normal
	{
		if (is_stdin)
			ft_printf("(stdin)= %s\n", digest);
		else
			ft_printf("MD5 (%s%s%s) = %s\n",
						is_file ? "" : "\"",
						label,
						is_file ? "" : "\"",
						digest);
	}
}

void	print_usage(void)
{
	ft_printf("usage: ft_ssl command [flags] [file/string]\n");
	ft_printf("Commands:\n md5\n sha256\n");
	ft_printf("Flags:\n -p -q -r -s\n");
}

void	print_invalid_cmd(const char *cmd)
{
	ft_putstr_fd("ft_ssl: Error: '", 2);
	ft_putstr_fd((char *)cmd, 2);
	ft_putstr_fd("' is an invalid command.\n", 2);
}

void	print_invalid_option(char option)
{
	ft_putstr_fd("ft_ssl: illegal option '", 2);
	ft_putchar_fd(option, 2);
	ft_putstr_fd("'\n", 2);
}

void	print_file_error(const char *cmd_name, const char *filename)
{
	ft_putstr_fd("ft_ssl: ", STDERR_FILENO);
	if (cmd_name)
	{
		ft_putstr_fd((char *)cmd_name, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putstr_fd((char *)filename, STDERR_FILENO);
	ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
}

void	cleanup(t_ssl_ctx *ctx)
{
	if (ctx->commands)
	{
		free(ctx->commands);
		ctx->commands = NULL;
	}
	ctx->command_count = 0;
}
