# Nome do compilador
CC = gcc

# Flags de compilação
CFLAGS = -Wall -g

# Nome do executável
TARGET = arvore_b

# Arquivos de origem
SRCS = main.c b_tree.c

# Arquivos objeto gerados
OBJS = $(SRCS:.c=.o)

# Regra padrão para compilar o programa
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Regra para compilar arquivos .c em .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para limpar os arquivos objeto e o executável
clean:
	rm -f $(OBJS) $(TARGET)

# Regra para rodar o programa
run: $(TARGET)
	./$(TARGET)

