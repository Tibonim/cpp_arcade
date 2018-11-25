all: snake centipede

snake:
	make -C snakeDir/

centipede:
	make -C centipedeDir/

clean:
	make clean -C snakeDir/
	make clean -C centipedeDir/

fclean: clean
	make fclean -C snakeDir/
	make fclean -C centipedeDir/

re: fclean all

.PHONY: all clean fclean re
