CC=gcc
CFLAGS= -std=c99 -Wall -Wextra -lm


OBJFILES = main.o
NAME=IMS


main: $(NAME)
	
$(NAME) : $(OBJFILES)	
	$(CC) $(CFLAGS) -o $(NAME) $(OBJFILES) 

run:
	./$(NAME) -a

run_price:
	./$(NAME) -A

run_natural:
	./$(NAME) -b

run_natural_price:
	./$(NAME) -B

run_coal:
	./$(NAME) -c

run_coal_price:
	./$(NAME) -C	

clean:
	rm -f $(OBJFILES) $(NAME) *~

