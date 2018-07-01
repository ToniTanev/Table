#include "functionality.h"


int main()
{
	GLFWwindow* window;
	int shaderProgram;

	init();
	createWindow(OUT window);
	createShaderProgram(OUT shaderProgram);
	render(window, shaderProgram);
	end();
	
	return 0;
}