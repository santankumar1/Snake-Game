SRCS = main.cpp Game.cpp GameOver.cpp AssetMan.cpp PauseGame.cpp  Snake.cpp GamePlay.cpp StateMan.cpp MainMenu.cpp
OBJS = $(SRCS:.cpp=.o)

all: compile link

compile: $(OBJS)

%.o: %.cpp
	g++	-c $< -o $@ -I"C:\Users\Santan\OneDrive\Desktop\SnakeGame\include" -DSFML_STATIC

link:
	g++ $(OBJS) -o main.exe -L"C:\Users\Santan\OneDrive\Desktop\SnakeGame\lib" -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32 -mwindows -lsfml-main -lharfbuzz

clean:
	del /f /q *.o main.exe