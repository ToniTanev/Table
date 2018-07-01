#include "functionality.h"


const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"   gl_Position = projection*view*model*vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.87f, 0.72f, 0.53f, 1.0f);\n"
"}\n\0";

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void init()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void createWindow(OUT GLFWwindow*& window)
{
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Table", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}
}

void createShaderProgram(OUT int& shaderProgram)
{
	// vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// link shaders
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void configureBuffersAndDraw(float* _vertices, size_t verticesSize, unsigned int* _indices, size_t indicesSize)
{
	float vertices[400];
	for (int i = 0; i < verticesSize; i++)
	{
		vertices[i] = _vertices[i];
	}

	unsigned int indices[400];
	for (int i = 0; i < indicesSize; i++)
	{
		indices[i] = _indices[i];
	}

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void drawTetragon(Point p1, Point p2, Point p3, Point p4) 
{
	float vertices[] = {
		p1.x, p1.y, p1.z,
		p2.x, p2.y, p2.z,
		p3.x, p3.y, p3.z,
		p4.x, p4.y, p4.z
	};

	unsigned int indices[] = {
		0, 1, 2,
		1, 2, 3
	};

	configureBuffersAndDraw(vertices, 12, indices, 6);

}

std::vector<Point> drawParallelepiped(float width, float length, float height, Point center)
{
	std::vector<Point> result;
	float x = width / 2, y = length / 2, z = height / 2;
	float vertices[] = {
		-x, -y, -z,
		x, -y, -z,
		-x, y, -z,
		x, y, -z,
		-x, -y, z,
		x, -y, z,
		-x, y, z,
		x, y, z
	};
	for (int i = 0; i < 24; i += 3)
	{
		vertices[i] += center.x;
		vertices[i + 1] += center.y;
		vertices[i + 2] += center.z;
	}
	for (int i = 0; i < 24; i += 3)
	{
		result.push_back(Point(vertices[i], vertices[i + 1], vertices[i + 2]));
	}
	unsigned int indices[] = {
		0, 1, 2, //predna stena
		1, 2, 3,
		4, 5, 6, //zadna stena
		5, 6, 7,
		0, 1, 4, //dolna stena
		1, 4, 5,
		2, 3, 6, //gorna stena
		3, 6, 7,
		0, 2, 4, //lqva stena
		2, 4, 6,
		1, 3, 5, //dqsna stena
		3, 5, 7
	};

	configureBuffersAndDraw(vertices, 24, indices, 36);

	return result;
}

std::vector<Point> drawPartialCircle(float r, Point center, float drawAngle, float startAngle) 
{
	std::vector<Point> result;
	float vertices[303];
	vertices[0] = vertices[1] = vertices[2] = 0.0;
	vertices[3] = r; vertices[4] = vertices[5] = 0.0;
	//rotirame do jelaniq nachalen ugul
	vertices[3] = cos(startAngle)*r;
	vertices[4] = sin(startAngle)*r;

	for (int i = 6; i < 303; i += 3)
	{
		vertices[i] = cos(drawAngle / 100.0)*vertices[i - 3] - sin(drawAngle / 100.0)*vertices[i - 2];
		vertices[i + 1] = sin(drawAngle / 100.0)*vertices[i - 3] + cos(drawAngle / 100.0)*vertices[i - 2];
		vertices[i + 2] = 0.0;
	}

	for (int i = 0; i < 303; i += 3)
	{
		vertices[i] += center.x;
		vertices[i + 1] += center.y;
		vertices[i + 2] += center.z;
	}

	for (int i = 0; i < 303; i += 3)
	{
		result.push_back(Point(vertices[i], vertices[i + 1], vertices[i + 2]));
	}

	unsigned int indices[300];
	for (int i = 0, j = 1; i < 297; i += 3)
	{
		indices[i] = 0;
		indices[i + 1] = j++;
		indices[i + 2] = j;
	}
	indices[297] = 0;
	indices[298] = 100;
	indices[299] = 1;

	configureBuffersAndDraw(vertices, 303, indices, 300);

	return result;
}

std::vector<Point> drawOval(float width, float length, Point center)
{
	std::vector<Point> result, res1, res2, res3, res4;
	if (width > 1.3*length) 
		width = 1.3*length;
	float R = length / 2, r = R / 2, a = width - R - r;
	res1 = drawPartialCircle(R, center);
	res2 = drawPartialCircle(r, Point(a + center.x, center.y, center.z));
	//first
	float firstCenterY = (pow(a, 2) - pow((R - r), 2)) / (2 * (R - r));
	float firstRadius = (pow(R, 2) - pow(r, 2) + pow(a, 2)) / (2 * (R - r));
	float firstDrawAngle = atan(a / firstCenterY);
	float firstStartAngle = atan(firstCenterY / a);
	res3 = drawPartialCircle(firstRadius, Point(center.x, -firstCenterY + center.y, center.z), firstDrawAngle, firstStartAngle);
	//second 
	float secondCenterY = firstCenterY;
	float secondRadius = firstRadius;
	float secondDrawAngle = -firstDrawAngle;
	float secondStartAngle = -firstStartAngle;
	res4 = drawPartialCircle(secondRadius, Point(center.x, secondCenterY + center.y, center.z), secondDrawAngle, secondStartAngle);

	for (int i = 0; i < res1.size(); i++)
	{
		result.push_back(res1[i]);
	}
	for (int i = 0; i < res2.size(); i++)
	{
		result.push_back(res2[i]);
	}
	for (int i = 0; i < res3.size(); i++)
	{
		result.push_back(res3[i]);
	}
	for (int i = 0; i < res4.size(); i++)
	{
		result.push_back(res4[i]);
	}

	return result;
}

std::vector<Point> drawOvalPlot(float width, float length, float height, Point center)
{
	std::vector<Point> result, res1, res2;

	res1 = drawOval(width, length, Point(center.x, center.y, center.z + height / 2));
	res2 = drawOval(width, length, Point(center.x, center.y, center.z - height / 2));

	for (int i = 0; i < res1.size() - 1; i++)
	{
		drawTetragon(res1[i], res1[i + 1], res2[i], res2[i + 1]);
	}
	drawTetragon(res1[res1.size() - 1], res1[0], res2[res2.size() - 1], res2[0]);

	for (int i = 0; i < res1.size(); i++)
	{
		result.push_back(res1[i]);
	}
	for (int i = 0; i < res2.size(); i++)
	{
		result.push_back(res2[i]);
	}

	return result;
}

std::vector<Point> drawCylinder(float radius, float height, Point center)
{
	std::vector<Point> result, res1, res2;

	res1 = drawPartialCircle(radius, Point(center.x, center.y, center.z + height / 2));
	res2 = drawPartialCircle(radius, Point(center.x, center.y, center.z - height / 2));
	for (int i = 0; i < res1.size() - 1; i++)
	{
		drawTetragon(res1[i], res1[i + 1], res2[i], res2[i + 1]);
	}

	for (int i = 0; i < res1.size(); i++)
	{
		result.push_back(res1[i]);
	}
	for (int i = 0; i < res2.size(); i++)
	{
		result.push_back(res2[i]);
	}
	return result;
}

istream& operator >> (istream& is, Shape& shape)
{
	std::string str;
	is >> str;
	if (str == "rectangle" || str == "RECTANGLE" || str == "Rectangle" || str == "rect" || str == "Rect" || str == "0")
		shape = RECTANGLE;
	if (str == "oval" || str == "OVAL" || str == "Oval" || str == "1")
		shape = OVAL;
	if (str == "circle" || str == "CIRCLE" || str == "Circle" || str == "2")
		shape = CIRCLE;
	if (str == "triangle" || str == "TRIANGLE" || str == "Triangle" || str == "3")
		shape = TRIANGLE;
	if (str == "square" || str == "SQUARE" || str == "Square" || str == "4")
		shape = SQUARE;

	return is;
}

ostream& operator << (ostream& os, Shape& shape)
{
	switch (shape)
	{
	case RECTANGLE: {os << "RECTANGLE"; break; }
	case OVAL: {os << "OVAL"; break; }
	case CIRCLE: {os << "CIRCLE"; break; }
	case TRIANGLE: {os << "TRIANGLE"; break; }
	case SQUARE: {os << "SQUARE"; break; }
	}

	return os;
}

void drawTable(PlotShape& plot, LegShape& leg)
{
	float offset = 5.0f + leg.maxDist(); //50 mm offset + offset for center point
	if (plot.getShape() == RECTANGLE)
	{
		plot.draw();

		leg.setCenter(Point(plot.getWidth() / 2 - offset, plot.getLength() / 2 - offset, -plot.getHeight() / 2 - leg.getHeight() / 2));
		leg.draw();
		leg.setCenter(Point(-plot.getWidth() / 2 + offset, plot.getLength() / 2 - offset, -plot.getHeight() / 2 - leg.getHeight() / 2));
		leg.draw();
		leg.setCenter(Point(plot.getWidth() / 2 - offset, -plot.getLength() / 2 + offset, -plot.getHeight() / 2 - leg.getHeight() / 2));
		leg.draw();
		leg.setCenter(Point(-plot.getWidth() / 2 + offset, -plot.getLength() / 2 + offset, -plot.getHeight() / 2 - leg.getHeight() / 2));
		leg.draw();
	}
	if (plot.getShape() == OVAL)
	{
		plot.draw();

		leg.setCenter(Point(plot.getWidth() - plot.getLength() / 2 - offset, 0.0, -plot.getHeight() / 2 - leg.getHeight() / 2));
		leg.draw();
		leg.setCenter(Point(0.0, plot.getLength() / 2 - offset, -plot.getHeight() / 2 - leg.getHeight() / 2));
		leg.draw();
		leg.setCenter(Point(0.0, -plot.getLength() / 2 + offset, -plot.getHeight() / 2 - leg.getHeight() / 2));
		leg.draw();
	}
}

void input(OUT PlotShape*& plot, OUT LegShape*& leg)
{
	//1 cm = 1
	float plotWidth, plotLength, plotHeight = 3.0f; // plotHeight = 30 mm
	Shape plotShape, legShape;
	std::cout << "Insert plot shape(valid options are: rectangle and oval): ";
	std::cin >> plotShape;
	while (plotShape != RECTANGLE && plotShape != OVAL)
	{
		std::cout << "Incorrect plot shape. Insert new plot shape(valid options are: rectangle and oval): ";
		std::cin >> plotShape;
	}
	std::cout << "Insert plot width and length: ";
	std::cin >> plotWidth >> plotLength;
	if (plotShape == RECTANGLE)
	{
		plot = new RectPlot(plotWidth, plotLength, plotHeight);
		float legHeight; //must be between 25 and 90 cm
		std::cout << "Insert legs height: ";
		std::cin >> legHeight;
		while (legHeight < 25 || legHeight > 90)
		{
			std::cout << "Legs height must be between 25 and 90 cm. Insert new height: ";
			std::cin >> legHeight;
		}
		std::cout << "Insert legs shape(valid options are: square, rectangle and circle): ";
		std::cin >> legShape;
		while (legShape != RECTANGLE && legShape != CIRCLE && legShape != SQUARE)
		{
			std::cout << "Incorrect legs shape. Insert new legs shape(valid options are: square, rectangle and circle): ";
			std::cin >> legShape;
		}
		if (legShape == SQUARE)
		{
			float legWidth;
			std::cout << "Insert square width: ";
			std::cin >> legWidth;
			leg = new RectLeg(legWidth, legWidth, legHeight);
		}
		if (legShape == RECTANGLE)
		{
			float legWidth, legLength;
			std::cout << "Insert rectangle width and length: ";
			std::cin >> legWidth >> legLength;
			leg = new RectLeg(legWidth, legLength, legHeight);
		}
		if (legShape == CIRCLE)
		{
			float radius;
			std::cout << "Insert circle radius: ";
			std::cin >> radius;
			leg = new CircleLeg(radius, legHeight);
		}
	}
	if (plotShape == OVAL)
	{

		plot = new OvalPlot(plotWidth, plotLength, plotHeight);
		float legHeight; //must be between 25 and 90 cm
		std::cout << "Insert legs height: ";
		std::cin >> legHeight;
		while (legHeight < 25 || legHeight > 90)
		{
			std::cout << "Legs height must be between 25 and 90 cm. Insert new height: ";
			std::cin >> legHeight;
		}
		std::cout << "Insert legs shape(valid options are: square, rectangle and circle): ";
		std::cin >> legShape;
		while (legShape != RECTANGLE && legShape != CIRCLE && legShape != SQUARE)
		{
			std::cout << "Incorrect legs shape. Insert new legs shape(valid options are: square, rectangle and circle): ";
			std::cin >> legShape;
		}
		if (legShape == SQUARE)
		{
			float legWidth;
			std::cout << "Insert square width: ";
			std::cin >> legWidth;
			leg = new RectLeg(legWidth, legWidth, legHeight);
		}
		if (legShape == RECTANGLE)
		{
			float legWidth, legLength;
			std::cout << "Insert rectangle width and length: ";
			std::cin >> legWidth >> legLength;
			leg = new RectLeg(legWidth, legLength, legHeight);
		}
		if (legShape == CIRCLE)
		{
			float radius;
			std::cout << "Insert circle radius: ";
			std::cin >> radius;
			leg = new CircleLeg(radius, legHeight);
		}
	}
}

void render(GLFWwindow* window, int shaderProgram)
{
	glEnable(GL_DEPTH_TEST);

	PlotShape* plot = nullptr;
	LegShape* leg = nullptr;
	input(OUT plot, OUT leg);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

		glUseProgram(shaderProgram);

		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;

		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		view = glm::translate(view, glm::vec3(0.0f, -20.0f, -200.0f));
		projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);

		unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
		unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
		unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);

		drawTable(*plot, *leg);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	delete plot;
	delete leg;
}

void end()
{
	glfwTerminate();
}