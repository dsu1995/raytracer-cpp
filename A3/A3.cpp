#include "A3.hpp"
#include "scene_lua.hpp"
using namespace std;

#include "cs488-framework/GlErrorCheck.hpp"
#include "cs488-framework/MathUtils.hpp"

#include <imgui/imgui.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

static bool show_gui = true;

const size_t CIRCLE_PTS = 48;

//----------------------------------------------------------------------------------------
// Constructor
A3::A3(const std::string & luaSceneFile)
	: m_luaSceneFile(luaSceneFile),
	  m_positionAttribLocation(0),
	  m_normalAttribLocation(0),
	  m_vao_meshData(0),
	  m_vbo_vertexPositions(0),
	  m_vbo_vertexNormals(0),
	  m_vao_arcCircle(0),
	  m_vbo_arcCircle(0),
	  cullBackface(false),
	  cullFrontface(false),
	  enableZBuffer(true),
	  drawCircle(false),
      doPicking(false),
      mode(Mode::POSITION)
{

}

//----------------------------------------------------------------------------------------
// Destructor
A3::~A3()
{

}

//----------------------------------------------------------------------------------------
/*
 * Called once, at program start.
 */
void A3::init()
{
	// Set the background colour.
	glClearColor(0.35, 0.35, 0.35, 1.0);

	createShaderProgram();

	glGenVertexArrays(1, &m_vao_arcCircle);
	glGenVertexArrays(1, &m_vao_meshData);
	enableVertexShaderInputSlots();

	processLuaSceneFile(m_luaSceneFile);

	// Load and decode all .obj files at once here.  You may add additional .obj files to
	// this list in order to support rendering additional mesh types.  All vertex
	// positions, and normals will be extracted and stored within the MeshConsolidator
	// class.
	unique_ptr<MeshConsolidator> meshConsolidator (new MeshConsolidator{
			getAssetFilePath("cube.obj"),
			getAssetFilePath("sphere.obj"),
			getAssetFilePath("suzanne.obj")
	});


	// Acquire the BatchInfoMap from the MeshConsolidator.
	meshConsolidator->getBatchInfoMap(m_batchInfoMap);

	// Take all vertex data within the MeshConsolidator and upload it to VBOs on the GPU.
	uploadVertexDataToVbos(*meshConsolidator);

	mapVboDataToVertexShaderInputLocations();

	initPerspectiveMatrix();

	initViewMatrix();

	initLightSources();


	// Exiting the current scope calls delete automatically on meshConsolidator freeing
	// all vertex data resources.  This is fine since we already copied this data to
	// VBOs on the GPU.  We have no use for storing vertex data on the CPU side beyond
	// this point.
}

SceneNode* findNodeByName(SceneNode *node, const std::string &name) {
	if (node->m_name == name) {
		return node;
	}

	for (SceneNode* child: node->children) {
        SceneNode* result = findNodeByName(child, name);
		if (result != nullptr) {
			return result;
		}
	}
	return nullptr;
}

//----------------------------------------------------------------------------------------
void A3::processLuaSceneFile(const std::string & filename) {
	// This version of the code treats the Lua file as an Asset,
	// so that you'd launch the program with just the filename
	// of a puppet in the Assets/ directory.
	// std::string assetFilePath = getAssetFilePath(filename.c_str());
	// m_rootNode = std::shared_ptr<SceneNode>(import_lua(assetFilePath));

	// This version of the code treats the main program argument
	// as a straightforward pathname.
	m_rootNode = std::shared_ptr<SceneNode>(import_lua(filename));
	if (!m_rootNode) {
		std::cerr << "Could not open " << filename << std::endl;
	}

	neck = dynamic_cast<JointNode*>(findNodeByName(m_rootNode.get(), "neck_ud_rotate_joint"));
	headLR = dynamic_cast<JointNode*>(findNodeByName(m_rootNode.get(), "head_lr_rotate_joint"));
	headUD = dynamic_cast<JointNode*>(findNodeByName(m_rootNode.get(), "head_ud_rotate_joint"));
    head = dynamic_cast<GeometryNode*>(findNodeByName(m_rootNode.get(), "head"));

    initIdToNode(m_rootNode.get());

    findJointNodes(m_rootNode.get());

    undoStackIndex = -1;
    takeJointSnapshot();
}

void A3::initIdToNode(SceneNode* root) {
    idToNode[root->m_nodeId] = root;
    for (SceneNode* child: root->children) {
        initIdToNode(child);
    }
}

void A3::takeJointSnapshot() {
    // there is stuff on the stack after the current index
    if (undoStackIndex < undoStack.size() - 1) {
        undoStack.erase(
            undoStack.begin() + undoStackIndex + 1,
            undoStack.end()
        );
    }
    undoStack.push_back(TransformSnapshot());
    undoStackIndex++;
    TransformSnapshot& curSnapshot = undoStack.back();

    for (JointNode* node: jointNodes) {
        curSnapshot.push_back(
            {node, node->trans, node->m_joint_x.cur, node->m_joint_y.cur}
        );
    }
}

void A3::restoreSnapshot() {
    for (const JointNodeSnapshot& snapshot: undoStack.at(undoStackIndex)) {
        snapshot.node->trans = snapshot.trans;
        snapshot.node->m_joint_x.cur = snapshot.xCur;
        snapshot.node->m_joint_y.cur = snapshot.yCur;
    }
}

void A3::findJointNodes(SceneNode* root) {
    if (root->m_nodeType == NodeType::JointNode) {
        JointNode* jointNode = dynamic_cast<JointNode*>(root);
        jointNodes.push_back(jointNode);
    }
    for (SceneNode* child: root->children) {
        findJointNodes(child);
    }
}

//----------------------------------------------------------------------------------------
void A3::createShaderProgram()
{
	m_shader.generateProgramObject();
	m_shader.attachVertexShader( getAssetFilePath("VertexShader.vs").c_str() );
	m_shader.attachFragmentShader( getAssetFilePath("FragmentShader.fs").c_str() );
	m_shader.link();

	m_shader_arcCircle.generateProgramObject();
	m_shader_arcCircle.attachVertexShader( getAssetFilePath("arc_VertexShader.vs").c_str() );
	m_shader_arcCircle.attachFragmentShader( getAssetFilePath("arc_FragmentShader.fs").c_str() );
	m_shader_arcCircle.link();
}

//----------------------------------------------------------------------------------------
void A3::enableVertexShaderInputSlots()
{
	//-- Enable input slots for m_vao_meshData:
	{
		glBindVertexArray(m_vao_meshData);

		// Enable the vertex shader attribute location for "position" when rendering.
		m_positionAttribLocation = m_shader.getAttribLocation("position");
		glEnableVertexAttribArray(m_positionAttribLocation);

		// Enable the vertex shader attribute location for "normal" when rendering.
		m_normalAttribLocation = m_shader.getAttribLocation("normal");
		glEnableVertexAttribArray(m_normalAttribLocation);

		CHECK_GL_ERRORS;
	}


	//-- Enable input slots for m_vao_arcCircle:
	{
		glBindVertexArray(m_vao_arcCircle);

		// Enable the vertex shader attribute location for "position" when rendering.
		m_arc_positionAttribLocation = m_shader_arcCircle.getAttribLocation("position");
		glEnableVertexAttribArray(m_arc_positionAttribLocation);

		CHECK_GL_ERRORS;
	}

	// Restore defaults
	glBindVertexArray(0);
}

//----------------------------------------------------------------------------------------
void A3::uploadVertexDataToVbos (
		const MeshConsolidator & meshConsolidator
) {
	// Generate VBO to store all vertex position data
	{
		glGenBuffers(1, &m_vbo_vertexPositions);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexPositions);

		glBufferData(GL_ARRAY_BUFFER, meshConsolidator.getNumVertexPositionBytes(),
				meshConsolidator.getVertexPositionDataPtr(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		CHECK_GL_ERRORS;
	}

	// Generate VBO to store all vertex normal data
	{
		glGenBuffers(1, &m_vbo_vertexNormals);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexNormals);

		glBufferData(GL_ARRAY_BUFFER, meshConsolidator.getNumVertexNormalBytes(),
				meshConsolidator.getVertexNormalDataPtr(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		CHECK_GL_ERRORS;
	}

	// Generate VBO to store the trackball circle.
	{
		glGenBuffers( 1, &m_vbo_arcCircle );
		glBindBuffer( GL_ARRAY_BUFFER, m_vbo_arcCircle );

		float *pts = new float[ 2 * CIRCLE_PTS ];
		for( size_t idx = 0; idx < CIRCLE_PTS; ++idx ) {
			float ang = 2.0 * M_PI * float(idx) / CIRCLE_PTS;
			pts[2*idx] = cos( ang );
			pts[2*idx+1] = sin( ang );
		}

		glBufferData(GL_ARRAY_BUFFER, 2*CIRCLE_PTS*sizeof(float), pts, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		CHECK_GL_ERRORS;
	}
}

//----------------------------------------------------------------------------------------
void A3::mapVboDataToVertexShaderInputLocations()
{
	// Bind VAO in order to record the data mapping.
	glBindVertexArray(m_vao_meshData);

	// Tell GL how to map data from the vertex buffer "m_vbo_vertexPositions" into the
	// "position" vertex attribute location for any bound vertex shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexPositions);
	glVertexAttribPointer(m_positionAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Tell GL how to map data from the vertex buffer "m_vbo_vertexNormals" into the
	// "normal" vertex attribute location for any bound vertex shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexNormals);
	glVertexAttribPointer(m_normalAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	//-- Unbind target, and restore default values:
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	CHECK_GL_ERRORS;

	// Bind VAO in order to record the data mapping.
	glBindVertexArray(m_vao_arcCircle);

	// Tell GL how to map data from the vertex buffer "m_vbo_arcCircle" into the
	// "position" vertex attribute location for any bound vertex shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_arcCircle);
	glVertexAttribPointer(m_arc_positionAttribLocation, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	//-- Unbind target, and restore default values:
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
void A3::initPerspectiveMatrix()
{
	float aspect = ((float)m_windowWidth) / m_windowHeight;
	m_perspective = glm::perspective(degreesToRadians(60.0f), aspect, 0.1f, 100.0f);
}


//----------------------------------------------------------------------------------------
void A3::initViewMatrix() {
	m_view = glm::lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f),
			vec3(0.0f, 1.0f, 0.0f));
}

//----------------------------------------------------------------------------------------
void A3::initLightSources() {
	// World-space position
	m_light.position = vec3(-2.0f, 5.0f, 0.5f);
	m_light.rgbIntensity = vec3(0.8f); // White light
}

//----------------------------------------------------------------------------------------
void A3::uploadCommonSceneUniforms() {
	m_shader.enable();
	{
		//-- Set Perspective matrix uniform for the scene:
		GLint location = m_shader.getUniformLocation("Perspective");
		glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(m_perspective));
		CHECK_GL_ERRORS;

        location = m_shader.getUniformLocation("picking");
        glUniform1i(location, doPicking);

        if (!doPicking) {
            //-- Set LightSource uniform for the scene:
            {
                location = m_shader.getUniformLocation("light.position");
                glUniform3fv(location, 1, value_ptr(m_light.position));
                location = m_shader.getUniformLocation("light.rgbIntensity");
                glUniform3fv(location, 1, value_ptr(m_light.rgbIntensity));
                CHECK_GL_ERRORS;
            }

            //-- Set background light ambient intensity
            {
                location = m_shader.getUniformLocation("ambientIntensity");
                vec3 ambientIntensity(0.05f);
                glUniform3fv(location, 1, value_ptr(ambientIntensity));
                CHECK_GL_ERRORS;
            }
        }
	}
	m_shader.disable();
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, before guiLogic().
 */
void A3::appLogic()
{
	// Place per frame, application logic here ...

	if (cullBackface || cullFrontface) {
		glEnable(GL_CULL_FACE);
		if (cullBackface && cullFrontface) {
			glCullFace(GL_FRONT_AND_BACK);
		}
		else if (cullBackface) {
			glCullFace(GL_BACK);
		}
		else if (cullFrontface) {
			glCullFace(GL_FRONT);
		}
	}
	else {
		glDisable(GL_CULL_FACE);
	}

	uploadCommonSceneUniforms();
}

void A3::resetJoints() {
    undoStackIndex = 0;
    if (undoStack.size() > 1) {
        undoStack.erase(undoStack.begin() + 1, undoStack.end());
    }
    restoreSnapshot();
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after appLogic(), but before the draw() method.
 */
void A3::guiLogic()
{
	if (!show_gui) {
		return;
	}

    static bool firstRun = true;
    if (firstRun) {
        ImGui::SetNextWindowPos(ImVec2(50, 50));
        firstRun = false;
    }

    static bool showDebugWindow = true;
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar;
    float opacity = 0.5f;

    ImGui::Begin(
		"Properties",
		&showDebugWindow,
		ImVec2(100,100),
		opacity,
        windowFlags
	);
    {
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("Application")) {
                if (ImGui::MenuItem("Reset Joints (N)")) {
                    resetJoints();
                }

                if (ImGui::MenuItem("Quit (Q)")) {
                    glfwSetWindowShouldClose(m_window, GL_TRUE);
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Edit")) {
                if (ImGui::MenuItem("Undo (U)")) {
                    undo();
                }
                if (ImGui::MenuItem("Redo (R)")) {
                    redo();
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Options")) {
                ImGui::Checkbox("Circle (C)", &drawCircle);
                ImGui::Checkbox("Z-buffer (Z)", &enableZBuffer);
                ImGui::Checkbox("Backface culling (B)", &cullBackface);
                ImGui::Checkbox("Frontface culling (F)", &cullFrontface);

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

		ImGui::RadioButton("Position/Orientation (P)", &mode, Mode::POSITION);
		ImGui::RadioButton("Joints (J)", &mode, Mode::JOINTS);

        ImGui::Text("Framerate: %.1f FPS", ImGui::GetIO().Framerate);
        ImGui::Text("%s", statusMessage.c_str());
        ImGui::Text("Undo Stack Index: %d", undoStackIndex);
    }
    ImGui::End();
}

//----------------------------------------------------------------------------------------
// Update mesh specific shader uniforms:
void A3::updateShaderUniforms(
		const ShaderProgram& shader,
		const GeometryNode& node,
		const glm::mat4& modelView
) {

	shader.enable();
	{
		//-- Set ModelView matrix:
		GLint location = shader.getUniformLocation("ModelView");
		glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(modelView));
		CHECK_GL_ERRORS;

        if (doPicking) {
            unsigned int id = node.m_nodeId;
            float r = float(id & 0xff) / 255.f;
            float g = float((id >> 8) & 0xff) / 255.f;
            float b = float((id >> 16) & 0xff) / 255.f;

            location = m_shader.getUniformLocation("material.kd");
            glUniform3f(location, r, g, b);
            CHECK_GL_ERRORS;
        }
        else {
            //-- Set NormMatrix:
            location = shader.getUniformLocation("NormalMatrix");
            mat3 normalMatrix = glm::transpose(glm::inverse(mat3(modelView)));
            glUniformMatrix3fv(location, 1, GL_FALSE, value_ptr(normalMatrix));
            CHECK_GL_ERRORS;

            //-- Set Material values:
            location = shader.getUniformLocation("material.kd");
            vec3 kd = node.isSelected ? vec3(1, 1, 0) : node.material.kd;
            glUniform3fv(location, 1, value_ptr(kd));
            CHECK_GL_ERRORS;
            location = shader.getUniformLocation("material.ks");
            const vec3& ks = node.material.ks;
            glUniform3fv(location, 1, value_ptr(ks));
            CHECK_GL_ERRORS;
            location = shader.getUniformLocation("material.shininess");
            glUniform1f(location, node.material.shininess);
            CHECK_GL_ERRORS;
        }
	}
	shader.disable();

}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after guiLogic().
 */
void A3::draw() {
	if (enableZBuffer) {
		glEnable(GL_DEPTH_TEST);
	}
	else {
		glDisable(GL_DEPTH_TEST);
	}
	renderSceneGraph(*m_rootNode);

	if (drawCircle) {
		glDisable( GL_DEPTH_TEST );
		renderArcCircle();
	}
}

//----------------------------------------------------------------------------------------
void A3::renderSceneGraph(const SceneNode& root) {
	// Bind the VAO once here, and reuse for all GeometryNode rendering below.
	glBindVertexArray(m_vao_meshData); {

		renderSceneGraphRecursive(root, m_view);

	} glBindVertexArray(0);
	CHECK_GL_ERRORS;
}

void A3::renderSceneGraphRecursive(
	const SceneNode& root,
	const glm::mat4& transform
) {
	// This is emphatically *not* how you should be drawing the scene graph in
	// your final implementation.  This is a non-hierarchical demonstration
	// in which we assume that there is a list of GeometryNodes living directly
	// underneath the root node, and that we can draw them in a loop.  It's
	// just enough to demonstrate how to get geometry and materials out of
	// a GeometryNode and onto the screen.

	// You'll want to turn this into recursive code that walks over the tree.
	// You can do that by putting a method in SceneNode, overridden in its
	// subclasses, that renders the subtree rooted at every node.  Or you
	// could put a set of mutually recursive functions in this class, which
	// walk down the tree from nodes of different types.

	const glm::mat4 newTransform = transform * root.trans;

	if (root.m_nodeType == NodeType::GeometryNode) {
		const GeometryNode& geometryNode = dynamic_cast<const GeometryNode&>(root);

		updateShaderUniforms(m_shader, geometryNode, newTransform);

		// Get the BatchInfo corresponding to the GeometryNode's unique MeshId.
		BatchInfo batchInfo = m_batchInfoMap[geometryNode.meshId];

		//-- Now render the mesh:
		m_shader.enable(); {

			glDrawArrays(GL_TRIANGLES, batchInfo.startIndex, batchInfo.numIndices);

		} m_shader.disable();
	}

	for (SceneNode* node: root.children) {
		renderSceneGraphRecursive(*node, newTransform);
	}

	CHECK_GL_ERRORS;
}


//----------------------------------------------------------------------------------------
// Draw the trackball circle.
void A3::renderArcCircle() {
	glBindVertexArray(m_vao_arcCircle);

	m_shader_arcCircle.enable();
		GLint m_location = m_shader_arcCircle.getUniformLocation( "M" );
		float aspect = float(m_framebufferWidth)/float(m_framebufferHeight);
		glm::mat4 M;
		if( aspect > 1.0 ) {
			M = glm::scale( glm::mat4(), glm::vec3( 0.5/aspect, 0.5, 1.0 ) );
		} else {
			M = glm::scale( glm::mat4(), glm::vec3( 0.5, 0.5*aspect, 1.0 ) );
		}
		glUniformMatrix4fv( m_location, 1, GL_FALSE, value_ptr( M ) );
		glDrawArrays( GL_LINE_LOOP, 0, CIRCLE_PTS );
	m_shader_arcCircle.disable();

	glBindVertexArray(0);
	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
/*
 * Called once, after program is signaled to terminate.
 */
void A3::cleanup() {}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles cursor entering the window area events.
 */
bool A3::cursorEnterWindowEvent(int entered) {
	bool eventHandled = false;

	// Fill in with event handling code...

	return eventHandled;
}

const float ROTATE_JOINT_SENSITIVITY = 0.1;

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse cursor movement events.
 */
bool A3::mouseMoveEvent(double xPos, double yPos) {
    if (mode == Mode::JOINTS) {
        if (isMiddleMousePressed) {
            double deltaY = (yPos - curMousePos.y) * ROTATE_JOINT_SENSITIVITY;
            for (JointNode* node: selectedJoints) {
                node->rotateX(deltaY);
            }
        }
        if (isRightMousePressed && head->isSelected) {
            double deltaY = (yPos - curMousePos.y) * ROTATE_JOINT_SENSITIVITY;
            headLR->rotateY(deltaY);
        }
    }

	curMousePos = vec2(xPos, yPos);
	return true;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse button events.
 */
bool A3::mouseButtonInputEvent(int button, int action, int mods) {
    if (ImGui::IsMouseHoveringAnyWindow()) {
        return false;
    }

    bool eventHandled = false;

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            isLeftMousePressed = true;
            eventHandled = true;
        }
        else if (action == GLFW_RELEASE) {
            isLeftMousePressed = false;
            eventHandled = true;
        }
    }
    else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
        if (action == GLFW_PRESS) {
            isMiddleMousePressed = true;
            eventHandled = true;
        }
        else if (action == GLFW_RELEASE) {
            isMiddleMousePressed = false;
            eventHandled = true;
        }
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            isRightMousePressed = true;
            eventHandled = true;
        }
        else if (action == GLFW_RELEASE) {
            isRightMousePressed = false;
            eventHandled = true;
        }
    }

    if (mode == Mode::JOINTS) {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS ) {
            selectJoint();
            eventHandled = true;
        }

        if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE) {
            takeJointSnapshot();
            eventHandled = true;
        }

        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE && head->isSelected) {
            takeJointSnapshot();
            eventHandled = true;
        }
    }

	return eventHandled;
}

void A3::selectJoint() {
    doPicking = true;

    uploadCommonSceneUniforms();
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.35, 0.35, 0.35, 1.0);

    draw();

    CHECK_GL_ERRORS;

    float xPos = curMousePos.x;
    float yPos = curMousePos.y;

    xPos *= double(m_framebufferWidth) / double(m_windowWidth);

    yPos = m_windowHeight - yPos;
    yPos *= double(m_framebufferHeight) / double(m_windowHeight);

    GLubyte buffer[] = {0, 0, 0, 0};

    glReadBuffer(GL_BACK);
    glReadPixels(int(xPos), int(yPos), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    CHECK_GL_ERRORS;

    unsigned int id = buffer[0] + (buffer[1] << 8) + (buffer[2] << 16);

    // if id does not exist, do nothing
    try {
		SceneNode* node = idToNode.at(id); // may throw exception here
		SceneNode* parent = node->parent;
		if (parent != nullptr && parent->m_nodeType == NodeType::JointNode) {
			node->isSelected ^= true;

			JointNode* jointParent = dynamic_cast<JointNode*>(parent);
			jointParent->isSelected ^= true;

			// parent not in selectedJoints, add it
			if (selectedJoints.find(jointParent) == selectedJoints.end()) {
				selectedJoints.insert(jointParent);
			}
			// parent in selectedJoints, remove it
			else {
				selectedJoints.erase(jointParent);
			}
		}
    }
    catch (const std::out_of_range& e) {}

    doPicking = false;

    CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse scroll wheel events.
 */
bool A3::mouseScrollEvent(double xOffSet, double yOffSet) {
	bool eventHandled = false;

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles window resize events.
 */
bool A3::windowResizeEvent(int width, int height) {
	bool eventHandled(false);
	initPerspectiveMatrix();
	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles key input events.
 */
bool A3::keyInputEvent(int key, int action, int mods) {

	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_M) {
			show_gui = !show_gui;
			return true;
		}
		else if (key == GLFW_KEY_B) {
			cullBackface = !cullBackface;
			return true;
		}
		else if (key == GLFW_KEY_F) {
			cullFrontface = !cullFrontface;
			return true;
		}
		else if (key == GLFW_KEY_Z) {
			enableZBuffer = !enableZBuffer;
			return true;
		}
		else if (key == GLFW_KEY_C) {
			drawCircle = !drawCircle;
			return true;
		}
		else if (key == GLFW_KEY_P) {
			mode = Mode::POSITION;
			return true;
		}
		else if (key == GLFW_KEY_J) {
			mode = Mode::JOINTS;
			return true;
		}
        else if (key == GLFW_KEY_U) {
            undo();
            return true;
        }
        else if (key == GLFW_KEY_R) {
            redo();
            return true;
        }
        else if (key == GLFW_KEY_N) {
            resetJoints();
            return true;
        }
        else if (key == GLFW_KEY_Q) {
            glfwSetWindowShouldClose(m_window, GL_TRUE);
            return true;
        }
	}

	// DEBUG
//	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
//		if (key == GLFW_KEY_Q) {
//			neck->rotateX(2);
//			return true;
//		}
//		else if (key == GLFW_KEY_A) {
//			neck->rotateX(-2);
//			return true;
//		}
//		else if (key == GLFW_KEY_W) {
//			headLR->rotateY(2);
//			return true;
//		}
//		else if (key == GLFW_KEY_S) {
//			headLR->rotateY(-2);
//			return true;
//		}
//		else if (key == GLFW_KEY_E) {
//			headUD->rotateX(2);
//			return true;
//		}
//		else if (key == GLFW_KEY_D) {
//			headUD->rotateX(-2);
//			return true;
//		}
//	}

	return false;
}

void A3::undo() {
    if (undoStackIndex == 0) {
        statusMessage = "Out of Undos";
        return;
    }
    statusMessage = "";
    undoStackIndex--;

    restoreSnapshot();
}

void A3::redo() {
    if (undoStackIndex == undoStack.size() - 1) {
        statusMessage = "Out of Redos";
        return;
    }
    statusMessage = "";
    undoStackIndex++;

    restoreSnapshot();
}
