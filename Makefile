
generate:
	g++ generator.cpp ponto.cpp -o generator -lGL -lGLU -lglut -L/tinyxml2/ltinyxml2
	g++ engine.cpp -o engine -lGL -lGLU -lglut -ltinyxml2


clean:
	rm -f generator
	rm -f engine

