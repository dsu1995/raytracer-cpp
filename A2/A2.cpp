#include <iostream>
#include <utility>

#define GLM_SWIZZLE // enables vec3.xy()
#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>

#include "glm/ext.hpp"

#include "cs488-framework/GlErrorCheck.hpp"

#include "A2.hpp"

#include "matutils.hpp"


//----------------------------------------------------------------------------------------
// Constructor
VertexData::VertexData()
	: numVertices(0),
	  index(0)
{
	positions.resize(kMaxVertices);
	colours.resize(kMaxVertices);
}

//----------------------------------------------------------------------------------------
// Constructor
A2::A2()
: m_currentLineColour(glm::vec3(0.0f)),
	view(
		matutils::lookAt(
			glm::vec3(
				0.0f,
				0.5f * M_SQRT1_2,
				0.5f * M_SQRT1_2
			),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
		)
	),
	perspective(1),
	cubeScaleMatrix(glm::mat4()),
	cubeRotationMatrix(glm::mat4()),
	cubeTranslationMatrix(glm::mat4()),
	rotateViewHandler(view),
	translateViewHandler(view),
	perspectiveHandler(perspective),
	rotateModelHandler(cubeRotationMatrix),
	translateModelHandler(cubeTranslationMatrix, cubeRotationMatrix),
	scaleModelHandler(cubeScaleMatrix),
	inputHandlers {
		&rotateViewHandler,
		&translateViewHandler,
		&perspectiveHandler,
		&rotateModelHandler,
		&translateModelHandler,
		&scaleModelHandler
	}
{
	reset();
}

void A2::reset() {
	view.reset();
	perspective.reset();
	cubeScaleMatrix.reset();
	cubeRotationMatrix.reset();
	cubeTranslationMatrix.reset();

	curInputHandler = 3;

	isLeftMouseDragging = false;
	isMiddleMouseDragging = false;
	isRightMouseDragging = false;

	mousePrevPos = 0;
}

//----------------------------------------------------------------------------------------
// Destructor
A2::~A2() {}

//----------------------------------------------------------------------------------------
/*
 * Called once, at program start.
 */
void A2::init()
{
	// Set the background colour.
	glClearColor(0.3, 0.5, 0.7, 1.0);

	createShaderProgram();

	glGenVertexArrays(1, &m_vao);

	enableVertexAttribIndices();

	generateVertexBuffers();

	mapVboDataToVertexAttributeLocation();

	perspective = Perspective(float(m_framebufferWidth) / float(m_framebufferHeight));

	// perspectiveMatrix = matutils::perspective(
	// 	M_PI / 6,
	// 	float(m_framebufferWidth) / float(m_framebufferHeight),
	// 	1,
	// 	1000
	// );

	// std::cout << m_framebufferWidth << ' ' << m_framebufferHeight << std::endl;
	// std::cout << glm::to_string(perspectiveMatrix) << std::endl;
}

//----------------------------------------------------------------------------------------
void A2::createShaderProgram()
{
	m_shader.generateProgramObject();
	m_shader.attachVertexShader( getAssetFilePath("VertexShader.vs").c_str() );
	m_shader.attachFragmentShader( getAssetFilePath("FragmentShader.fs").c_str() );
	m_shader.link();
}

//----------------------------------------------------------------------------------------
void A2::enableVertexAttribIndices()
{
	glBindVertexArray(m_vao);

	// Enable the attribute index location for "position" when rendering.
	GLint positionAttribLocation = m_shader.getAttribLocation( "position" );
	glEnableVertexAttribArray(positionAttribLocation);

	// Enable the attribute index location for "colour" when rendering.
	GLint colourAttribLocation = m_shader.getAttribLocation( "colour" );
	glEnableVertexAttribArray(colourAttribLocation);

	// Restore defaults
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
void A2::generateVertexBuffers()
{
	// Generate a vertex buffer to store line vertex positions
	{
		glGenBuffers(1, &m_vbo_positions);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_positions);

		// Set to GL_DYNAMIC_DRAW because the data store will be modified frequently.
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * kMaxVertices, nullptr,
				GL_DYNAMIC_DRAW);


		// Unbind the target GL_ARRAY_BUFFER, now that we are finished using it.
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CHECK_GL_ERRORS;
	}

	// Generate a vertex buffer to store line colors
	{
		glGenBuffers(1, &m_vbo_colours);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_colours);

		// Set to GL_DYNAMIC_DRAW because the data store will be modified frequently.
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * kMaxVertices, nullptr,
				GL_DYNAMIC_DRAW);


		// Unbind the target GL_ARRAY_BUFFER, now that we are finished using it.
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CHECK_GL_ERRORS;
	}
}

//----------------------------------------------------------------------------------------
void A2::mapVboDataToVertexAttributeLocation()
{
	// Bind VAO in order to record the data mapping.
	glBindVertexArray(m_vao);

	// Tell GL how to map data from the vertex buffer "m_vbo_positions" into the
	// "position" vertex attribute index for any bound shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_positions);
	GLint positionAttribLocation = m_shader.getAttribLocation( "position" );
	glVertexAttribPointer(positionAttribLocation, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Tell GL how to map data from the vertex buffer "m_vbo_colours" into the
	// "colour" vertex attribute index for any bound shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_colours);
	GLint colorAttribLocation = m_shader.getAttribLocation( "colour" );
	glVertexAttribPointer(colorAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	//-- Unbind target, and restore default values:
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}

//---------------------------------------------------------------------------------------
void A2::initLineData()
{
	m_vertexData.numVertices = 0;
	m_vertexData.index = 0;
}

//---------------------------------------------------------------------------------------
void A2::setLineColour (
		const glm::vec3 & colour
) {
	m_currentLineColour = colour;
}

//---------------------------------------------------------------------------------------
void A2::drawLine(
		const glm::vec2 & v0,   // Line Start (NDC coordinate)
		const glm::vec2 & v1    // Line End (NDC coordinate)
) {

	m_vertexData.positions[m_vertexData.index] = v0;
	m_vertexData.colours[m_vertexData.index] = m_currentLineColour;
	++m_vertexData.index;
	m_vertexData.positions[m_vertexData.index] = v1;
	m_vertexData.colours[m_vertexData.index] = m_currentLineColour;
	++m_vertexData.index;

	m_vertexData.numVertices += 2;
}


//----------------------------------------------------------------------------------------
/*
 * Called once per frame, before guiLogic().
 */
void A2::appLogic()
{
	// Place per frame, application logic here ...

	// Call at the beginning of frame, before drawing lines:
	initLineData();

	// Draw outer square:
	// setLineColour(vec3(1.0f, 0.7f, 0.8f));
	// drawLine(vec2(-0.5f, -0.5f), vec2(0.5f, -0.5f));
	// drawLine(vec2(0.5f, -0.5f), vec2(0.5f, 0.5f));
	// drawLine(vec2(0.5f, 0.5f), vec2(-0.5f, 0.5f));
	// drawLine(vec2(-0.5f, 0.5f), vec2(-0.5f, -0.5f));


	// // Draw inner square:
	// setLineColour(vec3(0.2f, 1.0f, 1.0f));
	// drawLine(vec2(-0.25f, -0.25f), vec2(0.25f, -0.25f));
	// drawLine(vec2(0.25f, -0.25f), vec2(0.25f, 0.25f));
	// drawLine(vec2(0.25f, 0.25f), vec2(-0.25f, 0.25f));
	// drawLine(vec2(-0.25f, 0.25f), vec2(-0.25f, -0.25f));

	drawWorldGnomon();

	drawCube();
}

const glm::vec3 COLOUR_WHITE(1, 1, 1);
const glm::vec3 COLOUR_RED(1, 0, 0);
const glm::vec3 COLOUR_GREEN(0, 1, 0);
const glm::vec3 COLOUR_BLUE(0, 0, 1);

const std::vector<glm::vec3> GNOMON_VERTICES = {
	{0, 0, 0},
	{1, 0, 0},
	{0, 1, 0},
	{0, 0, 1}
};

struct ColouredEdge {
	unsigned int vertex1Index;
	unsigned int vertex2Index;
	glm::vec3 colour;
};

const std::vector<ColouredEdge> GNOMON_EDGES = {
	{0, 1, COLOUR_RED},
	{0, 2, COLOUR_GREEN},
	{0, 3, COLOUR_BLUE}
};

void A2::drawWorldGnomon() {
	glm::mat4 transform = perspective.getMatrix() * view.matrix * matutils::scaleMatrix(glm::vec3(0.1));

	std::vector<glm::vec2> vertices;

	for (const glm::vec3& vertex: GNOMON_VERTICES) {
		glm::vec2 v = matutils::homogenize(transform * glm::vec4(vertex, 1));
		vertices.push_back(v);
	}

	for (const ColouredEdge& edge: GNOMON_EDGES) {
		setLineColour(edge.colour);
		drawLine(
			vertices.at(edge.vertex1Index),
			vertices.at(edge.vertex2Index)
		);
	}
}

const glm::vec3 COLOUR_YELLOW(1, 1, 0);
const glm::vec3 COLOUR_CYAN(0, 1, 1);
const glm::vec3 COLOUR_MAGENTA(1, 0, 1);

const std::vector<glm::vec3> CUBE_VERTICES = {
	{ -1, -1, -1 },
	{ -1,  1, -1 },
	{ -1,  1,  1 },
	{ -1, -1,  1 },
	{  1, -1,  1 },
	{  1, -1, -1 },
	{  1,  1, -1 },
	{  1,  1,  1 },
};

struct Edge {
	unsigned int vertex1Index;
	unsigned int vertex2Index;
};

const std::vector<Edge> CUBE_EDGES = {
	{ 0, 1 },
	{ 0, 3 },
	{ 0, 5 },
	{ 1, 2 },
	{ 1, 6 },
	{ 2, 3 },
	{ 2, 7 },
	{ 3, 4 },
	{ 4, 5 },
	{ 4, 7 },
	{ 5, 6 },
	{ 6, 7 }
};

const std::vector<ColouredEdge> CUBE_GNOMON_EDGES = {
	{0, 1, COLOUR_MAGENTA},
	{0, 2, COLOUR_CYAN},
	{0, 3, COLOUR_YELLOW}
};

void A2::drawCube() {
	glm::mat4 transform = perspective.getMatrix() *
		view.matrix *
		matutils::scaleMatrix(glm::vec3(0.1)) *
		cubeTranslationMatrix.matrix *
		cubeRotationMatrix.matrix;

	{
		std::vector<glm::vec2> vertices;

		for (const glm::vec3& vertex: GNOMON_VERTICES) {
			glm::vec2 v = matutils::homogenize(transform * glm::vec4(vertex, 1));
			vertices.push_back(v);
		}

		for (const ColouredEdge& edge: CUBE_GNOMON_EDGES) {
			setLineColour(edge.colour);
			drawLine(
				vertices.at(edge.vertex1Index),
				vertices.at(edge.vertex2Index)
			);
		}
	}

	{
		setLineColour(COLOUR_WHITE);

		glm::mat4 cubeTransform = transform * cubeScaleMatrix.matrix;

		std::vector<glm::vec2> vertices;

		for (const glm::vec3& vertex: CUBE_VERTICES) {
			glm::vec2 v = matutils::homogenize(cubeTransform * glm::vec4(vertex, 1));
			vertices.push_back(v);
		}

		for (const Edge& edge: CUBE_EDGES) {
			drawLine(
				vertices.at(edge.vertex1Index),
				vertices.at(edge.vertex2Index)
			);
		}
	}
}


//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after appLogic(), but before the draw() method.
 */
void A2::guiLogic()
{
	static bool firstRun = true;
	if (firstRun) {
		ImGui::SetNextWindowPos(ImVec2(50, 50));
		firstRun = false;
	}

	static bool showDebugWindow = true;
	ImGuiWindowFlags windowFlags(ImGuiWindowFlags_AlwaysAutoResize);
	float opacity = 0.5f;

	ImGui::Begin("Properties", &showDebugWindow, ImVec2(100,100), opacity, windowFlags);

		// Add more gui elements here here ...
		for (int i = 0; i < inputHandlers.size(); i++) {
			ImGui::PushID(i);
				ImGui::RadioButton(inputHandlers.at(i)->getName().c_str(), &curInputHandler, i);
			ImGui::PopID();
		}

		// Create Button, and check if it was clicked:
		if( ImGui::Button( "Quit Application" ) ) {
			glfwSetWindowShouldClose(m_window, GL_TRUE);
		}

		ImGui::Text( "Framerate: %.1f FPS", ImGui::GetIO().Framerate );
		ImGui::Text("FOV: %.1f degrees", perspective.getFovDegrees());
		ImGui::Text("(Near, Far): (%.1f, %.1f)", perspective.getNear(), perspective.getFar());

	ImGui::End();
}

//----------------------------------------------------------------------------------------
void A2::uploadVertexDataToVbos() {

	//-- Copy vertex position data into VBO, m_vbo_positions:
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_positions);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec2) * m_vertexData.numVertices,
				m_vertexData.positions.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CHECK_GL_ERRORS;
	}

	//-- Copy vertex colour data into VBO, m_vbo_colours:
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_colours);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * m_vertexData.numVertices,
				m_vertexData.colours.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CHECK_GL_ERRORS;
	}
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after guiLogic().
 */
void A2::draw()
{
	uploadVertexDataToVbos();

	glBindVertexArray(m_vao);

	m_shader.enable();
		glDrawArrays(GL_LINES, 0, m_vertexData.numVertices);
	m_shader.disable();

	// Restore defaults
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
/*
 * Called once, after program is signaled to terminate.
 */
void A2::cleanup() {}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles cursor entering the window area events.
 */
bool A2::cursorEnterWindowEvent(int entered) {
	bool eventHandled = false;

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse cursor movement events.
 */
bool A2::mouseMoveEvent(double xPos, double yPos) {
	bool eventHandled = false;
	if (isLeftMouseDragging) {
		inputHandlers.at(curInputHandler)->onLeftMouseDrag(mousePrevPos, xPos);

		eventHandled = true;
	}
	if (isMiddleMouseDragging) {
		inputHandlers.at(curInputHandler)->onMiddleMouseDrag(mousePrevPos, xPos);

		eventHandled = true;
	}
	if (isRightMouseDragging) {
		inputHandlers.at(curInputHandler)->onRightMouseDrag(mousePrevPos, xPos);

		eventHandled = true;
	}

	mousePrevPos = xPos;

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse button events.
 */
bool A2::mouseButtonInputEvent(int button, int action, int mods) {
	if (ImGui::IsMouseHoveringAnyWindow()) {
		return false;
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			isLeftMouseDragging = true;
			return true;
		} else if (action == GLFW_RELEASE) {
			isLeftMouseDragging = false;
			return true;
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
		if (action == GLFW_PRESS) {
			isMiddleMouseDragging = true;
			return true;
		} else if (action == GLFW_RELEASE) {
			isMiddleMouseDragging = false;
			return true;
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (action == GLFW_PRESS) {
			isRightMouseDragging = true;
			return true;
		} else if (action == GLFW_RELEASE) {
			isRightMouseDragging = false;
			return true;
		}
	}

	return false;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse scroll wheel events.
 */
bool A2::mouseScrollEvent(double xOffSet, double yOffSet) {
	bool eventHandled = false;

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles window resize events.
 */
bool A2::windowResizeEvent(int width, int height) {
	bool eventHandled = false;

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles key input events.
 */
bool A2::keyInputEvent(int key, int action, int mods) {
	if (action == GLFW_PRESS) {
		// quit
		if (key == GLFW_KEY_Q) {
			glfwSetWindowShouldClose(m_window, GL_TRUE);
			return true;
		}
		// reset
		else if (key == GLFW_KEY_A) {
			reset();
			return true;
		}
		// rotateViewHandler
		else if (key == GLFW_KEY_O) {
			curInputHandler = 0;
			return true;
		}
		// translateViewHandler
		else if (key == GLFW_KEY_N) {
			curInputHandler = 1;
			return true;
		}
		// perspectiveHandler
		else if (key == GLFW_KEY_P) {
			curInputHandler = 2;
			return true;
		}
		// rotateModelHandler
		else if (key == GLFW_KEY_R) {
			curInputHandler = 3;
			return true;
		}
		// translateModelHandler
		else if (key == GLFW_KEY_T) {
			curInputHandler = 4;
			return true;
		}
		// scaleModelHandler
		else if (key == GLFW_KEY_S) {
			curInputHandler = 5;
			return true;
		}
	}

	return false;
}
