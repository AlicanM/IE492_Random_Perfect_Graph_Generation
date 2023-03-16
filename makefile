mckay_path = ./Graph_Generation/Brendan_McKay/showg

all: $(mckay_path).exe main.exe

$(mckay_path).exe: $(mckay_path).cpp
	gcc $(mckay_path).cpp -o $(mckay_path).exe
