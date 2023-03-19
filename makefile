mckay_path = ./Graph_Generation/Brendan_McKay/showg

all: $(mckay_path) main.exe

$(mckay_path): $(mckay_path).cpp
	gcc $(mckay_path).cpp -o $(mckay_path)
