##
## EPITECH PROJECT, 2024
## B-NWP-400-BAR-4-1-myftp-renzo.maggiori
## File description:
## Makefile
##

SRC	=	$(wildcard ./*.c)\
		$(wildcard ./clients/*.c)\
		$(wildcard ./commands/*.c)\
		$(wildcard ./sockets/*.c)\

CFLAGS	=	-I./include/ -g3

TEST_SRC = $(filter-out ./main.c, $(SRC)) $(wildcard tests/*.c)

LDFLAGS = -lcriterion --coverage

NAME	=	myftp

all:
	gcc -o $(NAME) $(SRC)

clean:
	rm -rf $(NAME)

fclean: clean
	rm -rf $(NAME) unit_tests *.gcno *.gcda

re:	fclean all

tests_run:
	gcc $(CFLAGS) $(TEST_SRC) $(LDFLAGS) -o unit_tests
	./unit_tests
