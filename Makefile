
generate:
	g++ generator.cpp ponto.cpp transformations.cpp -o generator -lGLEW -lGL -lGLU -lglut -L/tinyxml2/ltinyxml2
	g++ engine.cpp transformations.cpp -o engine -lGLEW -lGL -lGLU -lglut -ltinyxml2


clean:
	rm -f generator
	rm -f engine

