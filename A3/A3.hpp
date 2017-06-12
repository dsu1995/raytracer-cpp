#pragma once

#include "cs488-framework/CS488Window.hpp"
#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/ShaderProgram.hpp"
#include "cs488-framework/MeshConsolidator.hpp"

#include "SceneNode.hpp"
#include "JointNode.hpp"
#include "GeometryNode.hpp"

#include <glm/glm.hpp>
#include <memory>
#include <unordered_map>
#include <unordered_set>

struct LightSource {
	glm::vec3 position;
	glm::vec3 rgbIntensity;
};


namespace Mode {
    const int POSITION = 0;
    const int JOINTS = 1;
};


class A3 : public CS488Window {
public:
	A3(const std::string & luaSceneFile);
	virtual ~A3();

protected:
	virtual void init() override;
	virtual void appLogic() override;
	virtual void guiLogic() override;
	virtual void draw() override;
	virtual void cleanup() override;

	//-- Virtual callback methods
	virtual bool cursorEnterWindowEvent(int entered) override;
	virtual bool mouseMoveEvent(double xPos, double yPos) override;
	virtual bool mouseButtonInputEvent(int button, int actions, int mods) override;
	virtual bool mouseScrollEvent(double xOffSet, double yOffSet) override;
	virtual bool windowResizeEvent(int width, int height) override;
	virtual bool keyInputEvent(int key, int action, int mods) override;

	//-- One time initialization methods:
	void processLuaSceneFile(const std::string & filename);
	void createShaderProgram();
	void enableVertexShaderInputSlots();
	void uploadVertexDataToVbos(const MeshConsolidator & meshConsolidator);
	void mapVboDataToVertexShaderInputLocations();
	void initViewMatrix();
	void initLightSources();

	void initPerspectiveMatrix();
	void uploadCommonSceneUniforms();
	void renderSceneGraph(const SceneNode &node);
	void renderSceneGraphRecursive(
		const SceneNode& node,
		const glm::mat4& transform
	);
	void renderArcCircle();

	glm::mat4 m_perspective;
	glm::mat4 m_view;

	LightSource m_light;

	//-- GL resources for mesh geometry data:
	GLuint m_vao_meshData;
	GLuint m_vbo_vertexPositions;
	GLuint m_vbo_vertexNormals;
	GLint m_positionAttribLocation;
	GLint m_normalAttribLocation;
	ShaderProgram m_shader;

	//-- GL resources for trackball circle geometry:
	GLuint m_vbo_arcCircle;
	GLuint m_vao_arcCircle;
	GLint m_arc_positionAttribLocation;
	ShaderProgram m_shader_arcCircle;

	// BatchInfoMap is an associative container that maps a unique MeshId to a BatchInfo
	// object. Each BatchInfo object contains an index offset and the number of indices
	// required to render the mesh with identifier MeshId.
	BatchInfoMap m_batchInfoMap;

	std::string m_luaSceneFile;

	std::shared_ptr<SceneNode> m_rootNode;

private:
	bool cullBackface;
	bool cullFrontface;
	bool enableZBuffer;
	bool drawCircle;

	JointNode* neck;
	JointNode* headLR;
	JointNode* headUD;
    GeometryNode* head;

	std::unordered_map<unsigned int, SceneNode*> idToNode;
    void initIdToNode(SceneNode* root);
    bool doPicking;

    void updateShaderUniforms(
        const ShaderProgram& shader,
        const GeometryNode& node,
        const glm::mat4& modelView
    );

    glm::vec2 curMousePos;
    bool isLeftMousePressed;
    bool isMiddleMousePressed;
    bool isRightMousePressed;

    int mode;

    void selectJoint();

    std::unordered_set<JointNode*> selectedJoints;

    struct JointNodeSnapshot {
        JointNode* node;
        glm::mat4 trans;
        double xCur;
        double yCur;
    };

    using TransformSnapshot = std::vector<JointNodeSnapshot>;

    std::vector<JointNode*> jointNodes;

    std::vector<TransformSnapshot> undoStack;
    int undoStackIndex;

    void findJointNodes(SceneNode* root);

    void undo();
    void redo();

    std::string statusMessage;

    void takeJointSnapshot();

    void restoreSnapshot();
};
