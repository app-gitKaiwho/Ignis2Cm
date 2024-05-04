/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spook <spook@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 17:08:55 by spook             #+#    #+#             */
/*   Updated: 2024/05/04 18:02:21 by spook            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gnl.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int line_len(char *line)
{
    int i;

    i = 0;
    while (line[i] != '\n' && line[i] != '\0')
        i++;
    return (i);
}

char is_number(char c)
{
    if (c >= '0' && c <= '9')
        return (c);
    return (0);
}

void edit_line(char *line, int fd2)
{
    int i;
    int size;
    char val;

    i = -1;
    val = 0;
    size = line_len(line);
    while (line[++i])
    {
        if (line[i] == ' ')
            if (size >= (i + 2) && line[i + 1] == 'x')
                val = is_number(line[i + 2]);
    }
    if (val > 0)
    {
        write(fd2, &val, 1);
        write(fd2, "x ", 2);
        write(fd2, line, (i - 5));
        write(fd2, "\n", 1);
    }
}

int main(int argc, char **argv)
{
    int fd;
    int fd2;
    char *line;

    if (argc != 2)
        return (1);
    fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        printf("Error opening file\n");
        return 1;
    }
    fd2 = open("tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    do
    {
        line = get_next_line(fd);
        if (line)
        {
            if (is_number(line[0]))
                if (line[1] == 'x')
                    if (line[2] == ' ')
                    {
                        close(fd);
                        close(fd2);
                        remove("tmp");
                        return (1);
                    }
            edit_line(line, fd2);
        }
    } while (line);
    close(fd);
    remove(argv[1]);
    close(fd2);
    rename("tmp", argv[1]);
    write(1, "File converted\n", 15);
    return (0);
}