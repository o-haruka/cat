#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void	do_cat(const char *path);
static void	die(const char *s);

int	main(int argc, char *argv[])
{
	int	i;

	if (argc < 2)
	{
		fprintf(stderr, "%s file name not found!\n", argv[0]);
		exit(1);
	}
	i = 1;
	while (i < argc)
	{
		do_cat(argv[i]);
		i++;
	}
	exit(0);
}

static void	do_cat(const char *path)
{
	int				fd;
	unsigned char	bf[10];
	int				n;

	fd = open(path, O_RDONLY);
	printf("fdの中身・・・%d", fd);
	if (fd < 0)
		die(path);
	while (1)
	{
		n = read(fd, bf, sizeof(bf));
		if (n < 0)
			die(path);
		if (n == 0)
			break ;
		if (write(STDOUT_FILENO, bf, n) < 0)
			die(path);
	}
}

static void	die(const char *s)
{
	perror(s);
	exit(1);
}