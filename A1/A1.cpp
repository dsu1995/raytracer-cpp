#include <iostream>

#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "cs488-framework/GlErrorCheck.hpp"

#include "utils.hpp"

#include "A1.hpp"
#include "cube.hpp"


using namespace glm;

static const size_t DIM = 16;
const int NUM_COLOURS = 8;

//----------------------------------------------------------------------------------------
// Constructor
A1::A1()
: grid(DIM),
	activePosition {0, 0},
	rd(),
	mt(rd()),
	randFloat(0.0, 1.0),
	curColour(0),
	colours() {

	for (int i = 0; i < NUM_COLOURS; i++) {
		colours.push_back(vec3(randFloat(mt), randFloat(mt), randFloat(mt)));
	}
}

//----------------------------------------------------------------------------------------
// Destructor
A1::~A1() {}

//----------------------------------------------------------------------------------------
/*
 * Called once, at program start.
 */
void A1::init() {
	// Set the background colour.
	glClearColor(0.3, 0.5, 0.7, 1.0);

	// Build the shader
	m_shader.generateProgramObject();
	m_shader.attachVertexShader(getAssetFilePath("VertexShader.vs").c_str());
	m_shader.attachFragmentShader(getAssetFilePath("FragmentShader.fs").c_str());
	m_shader.link();

	// Set up the uniforms
	P_uni = m_shader.getUniformLocation("P");
	V_uni = m_shader.getUniformLocation("V");
	M_uni = m_shader.getUniformLocation("M");
	M2_uni = m_shader.getUniformLocation("M2");
	col_uni = m_shader.getUniformLocation("colour");

	initGrid();
	initCube();
	initIndicatorTriangle();

	CHECK_GL_ERRORS;

	// Set up initial view and projection matrices (need to do this here,
	// since it depends on the GLFW window being set up correctly).
	view = glm::lookAt(
		glm::vec3(
			0.0f,
			float(DIM) * 2.0 * M_SQRT1_2,
			float(DIM) * 2.0 * M_SQRT1_2
		),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);

	proj = glm::perspective(
		glm::radians(45.0f),
		float(m_framebufferWidth) / float(m_framebufferHeight),
		1.0f,
		1000.0f
	);
}

void A1::initCube() {
	glGenVertexArrays(1, &cube_vao);
	glBindVertexArray(cube_vao); {

		// upload vertices
		glGenBuffers(1, &cube_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, cube_vbo); {
			glBufferData(
				GL_ARRAY_BUFFER,
				utils::sizeOfVector(cube::VERTICES),
				utils::vectorToPointer(cube::VERTICES),
				GL_STATIC_DRAW
			);

			// Specify the means of extracting the position values properly.
			GLint posAttrib = m_shader.getAttribLocation("position");
			glEnableVertexAttribArray(posAttrib);
			glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		} glBindBuffer(GL_ARRAY_BUFFER, 0);

		// upload indices
		glGenBuffers(1, &cube_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_ibo); {
			glBufferData(
				GL_ELEMENT_ARRAY_BUFFER,
				utils::sizeOfVector(cube::TRIANGLES),
				utils::vectorToPointer(cube::TRIANGLES),
				GL_STATIC_DRAW
			);
		} glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	} glBindVertexArray(0);
}

void A1::initGrid() {
	size_t sz = 3 * 2 * 2 * (DIM+3);

	float *verts = new float[ sz ];
	size_t ct = 0;
	for( int idx = 0; idx < DIM+3; ++idx ) {
		// horizontal line
		// left point
		verts[ ct ] = -1;
		verts[ ct+1 ] = 0;
		verts[ ct+2 ] = idx-1;
		// right point
		verts[ ct+3 ] = DIM+1;
		verts[ ct+4 ] = 0;
		verts[ ct+5 ] = idx-1;
		ct += 6;

		// vertical line
		// top point
		verts[ ct ] = idx-1;
		verts[ ct+1 ] = 0;
		verts[ ct+2 ] = -1;
		// bottom point
		verts[ ct+3 ] = idx-1;
		verts[ ct+4 ] = 0;
		verts[ ct+5 ] = DIM+1;
		ct += 6;
	}

	// Create the vertex array to record buffer assignments.
	glGenVertexArrays( 1, &m_grid_vao );
	glBindVertexArray( m_grid_vao );

	// Create the cube vertex buffer
	glGenBuffers( 1, &m_grid_vbo );
	glBindBuffer( GL_ARRAY_BUFFER, m_grid_vbo );
	glBufferData( GL_ARRAY_BUFFER, sz*sizeof(float),
		verts, GL_STATIC_DRAW );

	// Specify the means of extracting the position values properly.
	GLint posAttrib = m_shader.getAttribLocation( "position" );
	glEnableVertexAttribArray( posAttrib );
	glVertexAttribPointer( posAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr );

	// Reset state to prevent rogue code from messing with *my* stuff!
	glBindVertexArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	// OpenGL has the buffer now, there's no need for us to keep a copy.
	delete [] verts;
}

void A1::initIndicatorTriangle() {
	vec3 vertices[] = {
		{0, 0, 0},
		{1, 0, 0},
		{0.5, 0, 1}
	};

	glGenVertexArrays(1, &triangle_vao);
	glBindVertexArray(triangle_vao); {
		// upload vertices
		glGenBuffers(1, &triangle_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, triangle_vbo); {
			glBufferData(
				GL_ARRAY_BUFFER,
				sizeof(vertices),
				vertices,
				GL_STATIC_DRAW
			);

			// Specify the means of extracting the position values properly.
			GLint posAttrib = m_shader.getAttribLocation("position");
			glEnableVertexAttribArray(posAttrib);
			glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		} glBindBuffer(GL_ARRAY_BUFFER, 0);
	} glBindVertexArray(0);
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, before guiLogic().
 */
void A1::appLogic() {
	// Place per frame, application logic here ...
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after appLogic(), but before the draw() method.
 */
void A1::guiLogic() {
	// We already know there's only going to be one window, so for
	// simplicity we'll store button states in static local variables.
	// If there was ever a possibility of having multiple instances of
	// A1 running simultaneously, this would break; you'd want to make
	// this into instance fields of A1.
	static bool showTestWindow(false);
	static bool showDebugWindow(true);

	ImGuiWindowFlags windowFlags(ImGuiWindowFlags_AlwaysAutoResize);
	float opacity(0.5f);

	ImGui::Begin("Debug Window", &showDebugWindow, ImVec2(100,100), opacity, windowFlags);
		if( ImGui::Button( "Quit Application" ) ) {
			glfwSetWindowShouldClose(m_window, GL_TRUE);
		}

		// Eventually you'll create multiple colour widgets with
		// radio buttons.  If you use PushID/PopID to give them all
		// unique IDs, then ImGui will be able to keep them separate.
		// This is unnecessary with a single colour selector and
		// radio button, but I'm leaving it in as an example.

		// Prefixing a widget name with "##" keeps it from being
		// displayed.

		ImGui::PushID(0);
		ImGui::ColorEdit3("##Colour", (float*)&colours.at(curColour));
		ImGui::SameLine();
		if(ImGui::RadioButton("##Col", &curColour, 0)) {
			// Select this colour.
		}
		ImGui::PopID();

/*
		// For convenience, you can uncomment this to show ImGui's massive
		// demonstration window right in your application.  Very handy for
		// browsing around to get the widget you want.  Then look in
		// shared/imgui/imgui_demo.cpp to see how it's done.
		if( ImGui::Button( "Test Window" ) ) {
			showTestWindow = !showTestWindow;
		}
*/

		ImGui::Text( "Framerate: %.1f FPS", ImGui::GetIO().Framerate );

	ImGui::End();

	if( showTestWindow ) {
		ImGui::ShowTestWindow( &showTestWindow );
	}
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after guiLogic().
 */
void A1::draw() {
	// Create a global transformation for the model (centre it).
	mat4 modelMatrix;
	modelMatrix = glm::translate(
		modelMatrix,
		vec3(
			-float(DIM) / 2.0f,
			0,
			-float(DIM) / 2.0f
		)
	);

	m_shader.enable(); {
		glEnable(GL_DEPTH_TEST);

		glUniformMatrix4fv(P_uni, 1, GL_FALSE, value_ptr(proj));
		glUniformMatrix4fv(V_uni, 1, GL_FALSE, value_ptr(view));
		glUniformMatrix4fv(M_uni, 1, GL_FALSE, value_ptr(modelMatrix));

		// Just draw the grid for now.
		drawGrid();

		// Draw the cubes
		drawCubes();

		// Highlight the active square.
		drawActiveIndicator();

	} m_shader.disable();

	CHECK_GL_ERRORS;
}

const mat4 IDENTITY_MATRIX;

void A1::drawGrid() {
	glUniformMatrix4fv(M2_uni, 1, GL_FALSE, value_ptr(IDENTITY_MATRIX));

	glBindVertexArray(m_grid_vao); {
		glUniform3f(col_uni, 1, 1, 1);
		glDrawArrays(GL_LINES, 0, (3 + DIM) * 4);
	} glBindVertexArray(0);
}

void A1::drawCubes() {
	glBindVertexArray(cube_vao); {
		for (int i = 0; i < DIM; i++) {
			for (int j = 0; j < DIM; j++) {
				int height = grid.getHeight(i, j);
				if (height > 0) {
					// set colour
					int cellColourIndex = grid.getColour(i, j);
					vec3& cellColour = colours.at(cellColourIndex);
					glUniform3fv(col_uni, 1, value_ptr(cellColour));

					// transform
					mat4 modelMatrix;
					modelMatrix = glm::scale(modelMatrix, vec3(1, height, 1));
					modelMatrix = glm::translate(modelMatrix, vec3(i, 0, j));
					glUniformMatrix4fv(M2_uni, 1, GL_FALSE, value_ptr(modelMatrix));

					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_ibo); {
						glDrawElements(
							GL_TRIANGLES,
							cube::TRIANGLES.size(),
							GL_UNSIGNED_BYTE,
							nullptr
						);
					} glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				}
			}
		}
	} glBindVertexArray(0);
}

const vec3 INDICATOR_COLOR = { 191 / 255.0f, 191 / 255.0f, 63 / 255.0f };

void A1::drawActiveIndicator() {
	glDisable(GL_DEPTH_TEST); {
		glBindVertexArray(cube_vao); {
			// set colour
			glUniform3fv(col_uni, 1, value_ptr(INDICATOR_COLOR));

			int height = grid.getHeight(activePosition.x, activePosition.y);

			// transform
			mat4 modelMatrix;
			modelMatrix = glm::translate(modelMatrix, vec3(activePosition.x, height, activePosition.y));
			glUniformMatrix4fv(M2_uni, 1, GL_FALSE, value_ptr(modelMatrix));

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_ibo); {
				glDrawElements(
					GL_TRIANGLES,
					2 * 3, // the first 2 triangles in cube::TRIANGLES form the bottom of the cube
					GL_UNSIGNED_BYTE,
					nullptr
				);
			} glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		} glBindVertexArray(0);
	} glEnable(GL_DEPTH_TEST);

	glBindVertexArray(triangle_vao); {
		// set colour
		glUniform3fv(col_uni, 1, value_ptr(INDICATOR_COLOR));

		// draw 4 indicator arrows in border
		glBindBuffer(GL_ARRAY_BUFFER, triangle_vbo); {
			// transform
			mat4 modelMatrix;
			modelMatrix = glm::translate(modelMatrix, vec3(activePosition.x, 0, -1));
			glUniformMatrix4fv(M2_uni, 1, GL_FALSE, value_ptr(modelMatrix));
			glDrawArrays(GL_TRIANGLES, 0, 3 * 3);

			modelMatrix = mat4();
			modelMatrix = glm::translate(modelMatrix, vec3(activePosition.x, 0, DIM + 1));
			modelMatrix = glm::scale(modelMatrix, vec3(1, 1, -1));
			glUniformMatrix4fv(M2_uni, 1, GL_FALSE, value_ptr(modelMatrix));
			glDrawArrays(GL_TRIANGLES, 0, 3 * 3);

			modelMatrix = mat4();
			modelMatrix = glm::translate(modelMatrix, vec3(-1, 0, activePosition.y + 1));
			modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), vec3(0, 1.0, 0));
			glUniformMatrix4fv(M2_uni, 1, GL_FALSE, value_ptr(modelMatrix));
			glDrawArrays(GL_TRIANGLES, 0, 3 * 3);

			modelMatrix = mat4();
			modelMatrix = glm::translate(modelMatrix, vec3(DIM + 1, 0, activePosition.y));
			modelMatrix = glm::rotate(modelMatrix, glm::radians(-90.0f), vec3(0, 1.0, 0));
			glUniformMatrix4fv(M2_uni, 1, GL_FALSE, value_ptr(modelMatrix));
			glDrawArrays(GL_TRIANGLES, 0, 3 * 3);

		} glBindBuffer(GL_ARRAY_BUFFER, 0);
	} glBindVertexArray(0);
}

//----------------------------------------------------------------------------------------
/*
 * Called once, after program is signaled to terminate.
 */
void A1::cleanup() {}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles cursor entering the window area events.
 */
bool A1::cursorEnterWindowEvent(int entered) {
	bool eventHandled = false;

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse cursor movement events.
 */
bool A1::mouseMoveEvent(double xPos, double yPos) {
	bool eventHandled = false;

	if (!ImGui::IsMouseHoveringAnyWindow()) {
		// Put some code here to handle rotations.  Probably need to
		// check whether we're *dragging*, not just moving the mouse.
		// Probably need some instance variables to track the current
		// rotation amount, and maybe the previous X position (so
		// that you can rotate relative to the *change* in X.
	}

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse button events.
 */
bool A1::mouseButtonInputEvent(int button, int actions, int mods) {
	bool eventHandled = false;

	if (!ImGui::IsMouseHoveringAnyWindow()) {
		// The user clicked in the window.  If it's the left
		// mouse button, initiate a rotation.
	}

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse scroll wheel events.
 */
bool A1::mouseScrollEvent(double xOffSet, double yOffSet) {
	bool eventHandled = false;

	// Zoom in or out.

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles window resize events.
 */
bool A1::windowResizeEvent(int width, int height) {
	bool eventHandled = false;

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles key input events.
 */
bool A1::keyInputEvent(int key, int action, int mods) {
	// Fill in with event handling code...
	if(action == GLFW_PRESS || action == GLFW_REPEAT) {
		// Respond to some key events.

		if (key == GLFW_KEY_Q) {
			glfwSetWindowShouldClose(m_window, GL_TRUE);
			return true;
		} else if (key == GLFW_KEY_SPACE) {
			if (grid.getHeight(activePosition.x, activePosition.y) == 0) {
				grid.setColour(activePosition.x, activePosition.y, curColour);
			}
			grid.incHeight(activePosition.x, activePosition.y);
			return true;
		} else if (key == GLFW_KEY_BACKSPACE) {
			grid.decHeight(activePosition.x, activePosition.y);
			return true;
		} else if (key == GLFW_KEY_UP || key == GLFW_KEY_DOWN || key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT) {
			int oldX = activePosition.x;
			int oldY = activePosition.y;

			if (key == GLFW_KEY_UP) {
				activePosition.y = std::max(0, oldY - 1);
			} else if (key == GLFW_KEY_DOWN) {
				activePosition.y = std::min((int)DIM - 1, activePosition.y + 1);
			} else if (key == GLFW_KEY_LEFT) {
				activePosition.x = std::max(0, activePosition.x - 1);
			} else if (key == GLFW_KEY_RIGHT) {
				activePosition.x = std::min((int)DIM - 1, activePosition.x + 1);
			}

			if (mods & GLFW_MOD_SHIFT) {
				grid.setColour(
					activePosition.x, activePosition.y,
					grid.getColour(oldX, oldY)
				);
				grid.setHeight(
					activePosition.x, activePosition.y,
					grid.getHeight(oldX, oldY)
				);
			}
			return true;
		}
	}

	return false;
}
