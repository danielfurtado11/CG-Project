
generate:
	g++ generator.cpp ponto.cpp transformations.cpp vetores.cpp -o  generator -g -lGLEW -lGL -lGLU -lglut -L/tinyxml2/ltinyxml2
	g++ engine.cpp ponto.cpp transformations.cpp model.cpp lights.cpp fpsCamera.cpp -lIL -o engine -g -lGLEW -lGL -lGLU -lglut -ltinyxml2


clean:
	rm -f generator
	rm -f engine





