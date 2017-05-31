#include <iostream>
#include <utility>
#include <cassert>

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
	camera(
		glm::vec3(
			0.0f,
			0,
			10
		),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	),
	perspective(1),
	cubeScaleMatrix(glm::mat4()),
	cubeRotationMatrix(glm::mat4()),
	cubeTranslationMatrix(glm::mat4()),
	rotateViewHandler(camera),
	translateViewHandler(camera),
	perspectiveHandler(perspective),
	rotateModelHandler(cubeRotationMatrix),
	translateModelHandler(cubeTranslationMatrix, cubeRotationMatrix),
	scaleModelHandler(cubeScaleMatrix),
	viewportHandler(viewport),
	inputHandlers {
		&rotateViewHandler,
		&translateViewHandler,
		&perspectiveHandler,
		&rotateModelHandler,
		&translateModelHandler,
		&scaleModelHandler,
		&viewportHandler
	}
{
	reset();
}

void A2::reset() {
	camera.reset();
	perspective.reset();
	cubeScaleMatrix.reset();
	cubeRotationMatrix.reset();
	cubeTranslationMatrix.reset();

	viewport = Viewport{0.9, -0.9, -0.9, 0.9};

	curInputHandler = 3;

	isLeftMouseDragging = false;
	isMiddleMouseDragging = false;
	isRightMouseDragging = false;

	mousePrevXPos = 0;
	mousePrevYPos = 0;
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

const glm::vec3 COLOUR_BLACK(0, 0, 0);

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, before guiLogic().
 */
void A2::appLogic()
{
	// Place per frame, application logic here ...

	// Call at the beginning of frame, before drawing lines:
	initLineData();

	// draw viewport
	setLineColour(COLOUR_BLACK);
	drawLine(
		glm::vec2(viewport.left, viewport.top),
		glm::vec2(viewport.right, viewport.top)
	);
	drawLine(
		glm::vec2(viewport.left, viewport.bottom),
		glm::vec2(viewport.right, viewport.bottom)
	);
	drawLine(
		glm::vec2(viewport.left, viewport.top),
		glm::vec2(viewport.left, viewport.bottom)
	);
	drawLine(
		glm::vec2(viewport.right, viewport.top),
		glm::vec2(viewport.right, viewport.bottom)
	);


	// draw models
	std::vector<ColouredEdgeVertices> worldEdges = getCubeWorldEdges();
	std::vector<ColouredEdgeVertices> gnomonEdges = getWorldGnomonEdges();
	worldEdges.insert(worldEdges.end(), gnomonEdges.begin(), gnomonEdges.end());

	std::vector<ColouredEdgeVertices> cameraEdges;
	for (const ColouredEdgeVertices& edge: worldEdges) {
		const glm::vec4 v1 = camera.getMatrix() * edge.v1;
		const glm::vec4 v2 = camera.getMatrix() * edge.v2;

		cameraEdges.push_back(ColouredEdgeVertices{v1, v2, edge.colour});
	}

	std::vector<ColouredEdgeVertices> nearFarClippedEdges = clipNearFar(cameraEdges);

	std::vector<ColouredEdgeVertices> perspectiveEdges;
	for (const ColouredEdgeVertices& edge: nearFarClippedEdges) {
		glm::vec4 v1 = perspective.getMatrix() * edge.v1;
		glm::vec4 v2 = perspective.getMatrix() * edge.v2;
		v1 /= v1.w;
		v2 /= v2.w;

		perspectiveEdges.push_back(ColouredEdgeVertices{v1, v2, edge.colour});
	}

	std::vector<ColouredEdgeVertices> restClippedEdges = clipRest(perspectiveEdges);

	for (const ColouredEdgeVertices& edge: restClippedEdges) {
		setLineColour(edge.colour);

		glm::vec2 v1(
			viewport.left + (viewport.right - viewport.left) * (edge.v1.x - (-1)) / 2,
			viewport.bottom + (viewport.top - viewport.bottom) * (edge.v1.y - (-1)) / 2
		);

		glm::vec2 v2(
			viewport.left + (viewport.right - viewport.left) * (edge.v2.x - (-1)) / 2,
			viewport.bottom + (viewport.top - viewport.bottom) * (edge.v2.y - (-1)) / 2
		);

		drawLine(v1, v2);
	}
}

const glm::vec3 COLOUR_WHITE(1, 1, 1);
const glm::vec3 COLOUR_RED(1, 0, 0);
const glm::vec3 COLOUR_GREEN(0, 1, 0);
const glm::vec3 COLOUR_BLUE(0, 0, 1);

const std::vector<glm::vec4> GNOMON_VERTICES = {
	{0, 0, 0, 1},
	{1, 0, 0, 1},
	{0, 1, 0, 1},
	{0, 0, 1, 1}
};

const std::vector<ColouredEdge> GNOMON_EDGES = {
	{0, 1, COLOUR_RED},
	{0, 2, COLOUR_GREEN},
	{0, 3, COLOUR_BLUE}
};

std::vector<ColouredEdgeVertices> A2::getWorldGnomonEdges() {
	std::vector<ColouredEdgeVertices> edges;

	for (const ColouredEdge& edge: GNOMON_EDGES) {
		edges.push_back(
			ColouredEdgeVertices{
				GNOMON_VERTICES.at(edge.vertex1Index),
				GNOMON_VERTICES.at(edge.vertex2Index),
				edge.colour
			}
		);
	}

	return edges;
}

const glm::vec3 COLOUR_YELLOW(1, 1, 0);
const glm::vec3 COLOUR_CYAN(0, 1, 1);
const glm::vec3 COLOUR_MAGENTA(1, 0, 1);

const std::vector<glm::vec4> CUBE_VERTICES = {
	{ -1, -1, -1, 1 },
	{ -1,  1, -1, 1 },
	{ -1,  1,  1, 1 },
	{ -1, -1,  1, 1 },
	{  1, -1,  1, 1 },
	{  1, -1, -1, 1 },
	{  1,  1, -1, 1 },
	{  1,  1,  1, 1 },
};

const std::vector<ColouredEdge> CUBE_EDGES = {
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


std::vector<ColouredEdgeVertices> A2::getCubeWorldEdges() {
	glm::mat4 transform =
		cubeTranslationMatrix.matrix *
		cubeRotationMatrix.matrix;

	std::vector<ColouredEdgeVertices> edges;

	for (const ColouredEdge& edge: CUBE_GNOMON_EDGES) {
		edges.push_back(
			ColouredEdgeVertices{
				transform * GNOMON_VERTICES.at(edge.vertex1Index),
				transform * GNOMON_VERTICES.at(edge.vertex2Index),
				edge.colour
			}
		);
	}

	glm::mat4 cubeTransform = transform * cubeScaleMatrix.matrix;

	for (const ColouredEdge& edge: CUBE_EDGES) {
		edges.push_back(
			ColouredEdgeVertices{
				cubeTransform * CUBE_VERTICES.at(edge.vertex1Index),
				cubeTransform * CUBE_VERTICES.at(edge.vertex2Index),
				COLOUR_WHITE
			}
		);
	}

	return edges;
}

std::vector<ColouredEdgeVertices> A2::clipNearFar(
	const std::vector<ColouredEdgeVertices>& edges
) {
	std::vector<glm::vec3> normals = {
		{0, 0,  1}, // near
		// {0, 0, -1, 0}  // far
	};

	std::vector<glm::vec3> points = {
		{0, 0, perspective.getNear()},
		// perspective.getFar() *  glm::vec4(0, 0, 1, 1)
	};

	return clip(edges, normals, points);
}

std::vector<ColouredEdgeVertices> A2::clipRest(
	const std::vector<ColouredEdgeVertices>& edges
) {
	const std::vector<glm::vec3> normals = {
		// { 0,  0,  1, 0},  // near
		{ 0,  0, -1},  // far
		{ 1,  0,  0},	// left
		{-1,  0,  0},	// right
		{ 0,  1,  0},	// bottom
		{ 0, -1,  0},	// top
	};

	const std::vector<glm::vec3> points = {
		// { 0,  0, -1, 1},  // near
		{ 0,  0,  1},  // far
		{-1,  0,  0},  // left
		{ 1,  0,  0},  // right
		{ 0, -1,  0},  // bottom
		{ 0,  1,  0}   // top
	};

	return clip(edges, normals, points);
}

std::vector<ColouredEdgeVertices> A2::clip(
	const std::vector<ColouredEdgeVertices>& edges,
	const std::vector<glm::vec3>& normals,
	const std::vector<glm::vec3>& points
) {
	assert(points.size() == normals.size());

	std::vector<ColouredEdgeVertices> output;
	for (const ColouredEdgeVertices& edge: edges) {
		glm::vec3 v1 = edge.v1.xyz() / edge.v1.w;
		glm::vec3 v2 = edge.v2.xyz() / edge.v1.w;

		bool accept = true;
		for (int i = 0; i < points.size(); i++) {
			const glm::vec3& normal = normals.at(i);
			const glm::vec3& point = points.at(i);

			float wecA = glm::dot(v1 - point, normal);
			float wecB = glm::dot(v2 - point, normal);

			if (wecA < 0 && wecB < 0) {
				accept = false;
				break;
			}
			else if (wecA >= 0 && wecB >= 0) {
				continue;
			}
			else {
				float t = wecA / (wecA - wecB);

				if (wecA < 0) {
					v1 = v1 + t * (v2 - v1);
				}
				else {
					v2 = v1 + t * (v2 - v1);
				}
			}
		}
		if (accept) {
			output.push_back(ColouredEdgeVertices{glm::vec4(v1, 1), glm::vec4(v2, 1), edge.colour});
		}
	}

	return output;
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

		// Create Button, and check if it was clicked:
		if( ImGui::Button( "Reset" ) ) {
			reset();
		}

		ImGui::Text( "Framerate: %.1f FPS", ImGui::GetIO().Framerate );
		ImGui::Text("FOV: %.1f degrees", perspective.getFovDegrees());
		ImGui::Text("(Near, Far): (%.1f, %.1f)", perspective.getNear(), perspective.getFar());
		ImGui::Text("LookFrom: %s", glm::to_string(camera.getLookFrom()).c_str());
		ImGui::Text("LookAt: %s", glm::to_string(camera.getLookAt()).c_str());
		ImGui::Text("Up: %s", glm::to_string(camera.getUp()).c_str());

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
	// std::cout << xPos << ' ' << yPos << std::endl;

	bool eventHandled = false;
	if (isLeftMouseDragging) {
		inputHandlers.at(curInputHandler)->onLeftMouseDrag(mousePrevXPos, xPos);
		inputHandlers.at(curInputHandler)->onLeftMouseDragXY(
			(xPos / m_framebufferWidth) * 2 - 1,
			-((yPos / m_framebufferHeight) * 2 - 1)
		);

		eventHandled = true;
	}
	if (isMiddleMouseDragging) {
		inputHandlers.at(curInputHandler)->onMiddleMouseDrag(mousePrevXPos, xPos);

		eventHandled = true;
	}
	if (isRightMouseDragging) {
		inputHandlers.at(curInputHandler)->onRightMouseDrag(mousePrevXPos, xPos);

		eventHandled = true;
	}

	mousePrevXPos = xPos;
	mousePrevYPos = yPos;

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
			inputHandlers.at(curInputHandler)->onLeftMousePress(
				(mousePrevXPos / m_framebufferWidth) * 2 - 1,
				-((mousePrevYPos / m_framebufferHeight) * 2 - 1)
			);
			return true;
		} else if (action == GLFW_RELEASE) {
			isLeftMouseDragging = false;
			inputHandlers.at(curInputHandler)->onLeftMouseRelease(
				(mousePrevXPos / m_framebufferWidth) * 2 - 1,
				-((mousePrevYPos / m_framebufferHeight) * 2 - 1)
			);
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
		// viewportHandler
		else if (key == GLFW_KEY_V) {
			curInputHandler = 6;
			return true;
		}
	}

	return false;
}
