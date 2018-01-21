#if !defined(MYAPPLICATION_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Joaqim Planstedt $
   ======================================================================== */

#define MYAPPLICATION_H

#include <Magnum/DefaultFramebuffer.h>
#include <Magnum/Platform/GlfwApplication.h>
#include <Magnum/Shaders/Phong.h>
#include <Magnum/Timeline.h>

#include <Corrade/PluginManager/Manager.h>
#include <Corrade/Utility/Resource.h>
#include <Corrade/Utility/Debug.h>
#include <Corrade/Utility/Arguments.h>
#include <Corrade/Utility/Assert.h>

#include <Magnum/SceneGraph/SceneGraph.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/SceneGraph/AbstractObject.h>
#include <Magnum/SceneGraph/AbstractFeature.h>
#include <Magnum/SceneGraph/Object.h>

#include <Magnum/Shapes/Box.h>
#include <Magnum/Shapes/Shape.h>
#include <Magnum/Shapes/ShapeGroup.h>

#include <Magnum/Math/Constants.h>

#include <Magnum/DebugTools/ShapeRenderer.h>
#include <Magnum/DebugTools/ResourceManager.h>

#pragma warning (push)
#pragma warning (disable : 4127)
#include <Magnum/MeshTools/Interleave.h>
#pragma warning (pop)
#include <Magnum/MeshTools/CompressIndices.h>
#include <Magnum/MeshTools/Compile.h>


#include <Magnum/Trade/PhongMaterialData.h>
#include <Magnum/Trade/MeshData3D.h>
#include <Magnum/Trade/SceneData.h>

#include <Magnum/Primitives/Cube.h>
#include <Magnum/Primitives/Capsule.h>
#include <Magnum/Primitives/Plane.h>

#include <Magnum/Renderer.h>
#include <Magnum/Buffer.h>
#include <Magnum/DefaultFramebuffer.h>
#include <Magnum/Mesh.h>
#include <Magnum/PixelFormat.h>
#include <Magnum/ResourceManager.h>
#include <Magnum/Texture.h>
#include <Magnum/TextureFormat.h>

#include <Meta.h>

#include "configure.h"
#include "Types.hpp"

using namespace Magnum;
using namespace Math::Literals;

constexpr const float MainCameraNear = 0.01f;
constexpr const float MainCameraFar = 100.0f * 2.f;

typedef ResourceManager<Buffer, Mesh, Texture2D, Shaders::Phong, Trade::PhongMaterialData> ViewerResourceManager;

class MyApplication: public Platform::Application {
public:
    explicit MyApplication(const Arguments& args);
    ~MyApplication();
private:

    struct Keys {
        static constexpr auto Up    = KeyEvent::Key::W;
        static constexpr auto Down  = KeyEvent::Key::S;
        static constexpr auto Left  = KeyEvent::Key::A;
        static constexpr auto Right = KeyEvent::Key::D;

        static constexpr auto Q     = KeyEvent::Key::Q;
        static constexpr auto E     = KeyEvent::Key::E;

        static constexpr auto H     = KeyEvent::Key::J;
        static constexpr auto J     = KeyEvent::Key::C;
        static constexpr auto K     = KeyEvent::Key::V;
        static constexpr auto L     = KeyEvent::Key::P;
    };

    void drawEvent() override;
    void mousePressEvent(MouseEvent& event) override;
    void mouseReleaseEvent(MouseEvent& event) override;
    void mouseMoveEvent(MouseMoveEvent& event) override;
    void keyPressEvent(KeyEvent &event) override;
    void keyReleaseEvent(KeyEvent &event) override;
    void viewportEvent(const Vector2i &size) override;

    void textInputEvent(const Platform::Application::TextInputEvent &event);

void rotateCamera(Object3D* cameraObject, const Vector2 delta, float deltaZ=1.0f);
    void addModel(const Trade::MeshData3D& meshData3D);
Object3D* createSceneObject(Model& model, bool makeCaster, bool makeReceiver);
private:
    Scene3D _scene;

    Object3D _mainCameraObject;
    SceneGraph::Camera3D _mainCamera;
    Object3D _debugCameraObject;
    SceneGraph::Camera3D _debugCamera;

    Object3D* _activeCameraObject;
    SceneGraph::Camera3D* _activeCamera;

    Utility::Resource _resource;

    std::vector<Model> _models;
    SceneGraph::DrawableGroup3D _drawables;
    Shaders::Phong _shader;
    ViewerResourceManager _resourceManager;
    Object3D *_root;

    Vector3 _mainCameraVelocity;
    Vector3 _mainCameraRotation{0.0f, 0.0f, 1.0f};

    Vector2i _previousMousePosition;
    Timeline _timeline;
};

class CDrawable: public SceneGraph::Drawable3D {
    public:
    explicit CDrawable(SceneGraph::AbstractObject3D& parent, SceneGraph::DrawableGroup3D* drawables);

    void setMesh(Mesh& mesh, Float radius);

    void setShader(Shaders::Phong& shader);

    Float radius() const;

    void draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D& camera) override;

public:
    Float value;
    private:
        Mesh* _mesh{};
        Shaders::Phong *_shader{};
        Float _radius;
};

namespace metas {

template<>
inline auto registerMembers<CDrawable>()
{
    return members(
        member("value", &CDrawable::value)
                   );
}

}


#endif
