#pragma once

#include "cs488-framework/CS488Window.hpp"
#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/ShaderProgram.hpp"

#include <glm/glm.hpp>

#include <vector>

#include "ResettableMat4.hpp"
#include "Perspective.hpp"
#include "Camera.hpp"
#include "Viewport.hpp"

#include "InputHandler.hpp"
#include "RotateViewHandler.hpp"
#include "TranslateViewHandler.hpp"
#include "PerspectiveHandler.hpp"
#include "RotateModelHandler.hpp"
#include "TranslateModelHandler.hpp"
#include "ScaleModelHandler.hpp"
#include "ViewportHandler.hpp"


// Set a global maximum number of vertices in order to pre-allocate VBO data
// in one shot, rather than reallocating each frame.
const GLsizei kMaxVertices = 1000;


// Convenience class for storing vertex data in CPU memory.
// Data should be copied over to GPU memory via VBO storage before rendering.
class VertexData {
public:
	VertexData();

	std::vector<glm::vec2> positions;
	std::vector<glm::vec3> colours;
	GLuint index;
	GLsizei numVertices;
};

struct ColouredEdge {
	unsigned int vertex1Index;
	unsigned int vertex2Index;
	glm::vec3 colour;
};

struct ColouredEdgeVertices {
	glm::vec3 v1;
	glm::vec3 v2;
	glm::vec3 colour;
};

struct Coloured2DEdgeVertices {
	glm::vec2 v1;
	glm::vec2 v2;
	glm::vec3 colour;
};


class A2 : public CS488Window {
public:
	A2();
	virtual ~A2();

protected:
	virtual void init() override;
	virtual void appLogic() override;
	virtual void guiLogic() override;
	virtual void draw() override;
	virtual void cleanup() override;

	virtual bool cursorEnterWindowEvent(int entered) override;
	virtual bool mouseMoveEvent(double xPos, double yPos) override;
	virtual bool mouseButtonInputEvent(int button, int actions, int mods) override;
	virtual bool mouseScrollEvent(double xOffSet, double yOffSet) override;
	virtual bool windowResizeEvent(int width, int height) override;
	virtual bool keyInputEvent(int key, int action, int mods) override;

	void createShaderProgram();
	void enableVertexAttribIndices();
	void generateVertexBuffers();
	void mapVboDataToVertexAttributeLocation();
	void uploadVertexDataToVbos();

	void initLineData();

	void setLineColour(const glm::vec3 & colour);

	void drawLine (
		const glm::vec2 & v0,
		const glm::vec2 & v1
	);

	ShaderProgram m_shader;

	GLuint m_vao;            // Vertex Array Object
	GLuint m_vbo_positions;  // Vertex Buffer Object
	GLuint m_vbo_colours;    // Vertex Buffer Object

	VertexData m_vertexData;

	glm::vec3 m_currentLineColour;

private:
	glm::mat4 getTransformMatrix() const;
	// void drawWorldGnomon();
	// void drawCube();

	std::vector<ColouredEdgeVertices> getWorldGnomonEdges();
	std::vector<ColouredEdgeVertices> getCubeWorldEdges();

	void reset();

	std::vector<ColouredEdgeVertices> clipNearFar(
		const std::vector<ColouredEdgeVertices>& edges
	);

	std::vector<ColouredEdgeVertices> clipRest(
		const std::vector<ColouredEdgeVertices>& edges
	);

	std::vector<Coloured2DEdgeVertices> clip2D(
		const std::vector<Coloured2DEdgeVertices>& edges
	);

	std::vector<ColouredEdgeVertices> clip(
		const std::vector<ColouredEdgeVertices>& edges,
		const std::vector<glm::vec3>& nearPlaneNormals,
		const std::vector<glm::vec3>& nearPlanePoints
	);

	Camera camera;

	ResettableMat4 cubeScaleMatrix;
	ResettableMat4 cubeRotationMatrix;
	ResettableMat4 cubeTranslationMatrix;

	// ResettableMat4 cubeModelMatrix;
	// ResettableMat4 cubeGnomonModelMatrix;

	Perspective perspective;

	RotateViewHandler rotateViewHandler;
	TranslateViewHandler translateViewHandler;
	PerspectiveHandler perspectiveHandler;

	RotateModelHandler rotateModelHandler;
	TranslateModelHandler translateModelHandler;
	ScaleModelHandler scaleModelHandler;

	ViewportHandler viewportHandler;

	std::vector<InputHandler*> inputHandlers;
	int curInputHandler;

	bool isLeftMouseDragging;
	bool isMiddleMouseDragging;
	bool isRightMouseDragging;

	double mousePrevXPos;
	double mousePrevYPos;

	Viewport viewport;
};
