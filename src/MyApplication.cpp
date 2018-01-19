#include "MyApplication.h"

using namespace Magnum;

class CDrawable: public SceneGraph::Drawable3D {
    public:
    explicit CDrawable(SceneGraph::AbstractObject3D& parent, SceneGraph::DrawableGroup3D* drawables)
            : Magnum::SceneGraph::Drawable3D{parent, drawables} {};

        /** @brief Mesh to use for this drawable and its bounding sphere radius */
        void setMesh(Mesh& mesh, Float radius) {
            _mesh = &mesh;
            _radius = radius;
        }

        void setShader(Shaders::Phong& shader) {
            _shader = &shader;
        }

        Float radius() const { return _radius; }

    void draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D& camera) override {
        _shader->setDiffuseColor(Color3(1.0f, 0.0f, 0.0f))
        .setTransformationMatrix(transformationMatrix)
        .setNormalMatrix(transformationMatrix.rotationScaling())
        .setProjectionMatrix(camera.projectionMatrix());
    //.setSpecularColor(Color3(1.0f))
    //.setShininess(20)
    //.setLightPosition({3.0f, -3.0f, 3.0f})
        _mesh->draw(*_shader);
    }

    private:
        Mesh* _mesh{};
        Shaders::Phong *_shader{};
        Float _radius;
};

MyApplication::MyApplication(const Arguments &args)
        : Platform::Application{ args, Configuration{}
    .setTitle("")
         .setWindowFlags(Configuration::WindowFlag::Resizable)
         .setCursorMode(Configuration::CursorMode::Disabled)
         .setSampleCount(8)},
_mainCameraObject{&_scene},
_mainCamera{_mainCameraObject},
_debugCameraObject{&_scene},
_debugCamera{_debugCameraObject},
_resource{"shadow-data"}
{

    Renderer::enable(Renderer::Feature::DepthTest);
    Renderer::enable(Renderer::Feature::FaceCulling);

    addModel(Primitives::Cube::solid());
    addModel(Primitives::Capsule3D::solid(1, 1, 4, 1.0f));
    addModel(Primitives::Capsule3D::solid(6, 1, 9, 1.0f));

    Object3D* ground = createSceneObject(_models[0], false, true);
    ground->setTransformation(Matrix4::scaling({100,1,100}));

    for(std::size_t i = 0; i != 200; ++i) {
        Model& model = _models[std::rand()%_models.size()];
        Object3D* object = createSceneObject(model, true, true);
        object->setTransformation(Matrix4::translation({
                    std::rand()*100.0f/RAND_MAX - 50.0f,
                        std::rand()*5.0f/RAND_MAX,
                        std::rand()*100.0f/RAND_MAX - 50.0f}));
    }


    _mainCamera.setProjectionMatrix(Matrix4::perspectiveProjection(35.0_degf,
                                                                   Vector2{defaultFramebuffer.viewport().size()}.aspectRatio(),
                                                                   MainCameraNear, MainCameraFar));
    _mainCameraObject.setTransformation(Matrix4::translation(Vector3::yAxis(3.0f)));

    _debugCamera.setProjectionMatrix(Matrix4::perspectiveProjection(35.0_degf,
                                                                    Vector2{defaultFramebuffer.viewport().size()}.aspectRatio(),
                                                                    MainCameraNear/4.0f, MainCameraFar*4.0f));
    _debugCameraObject.setTransformation(Matrix4::lookAt(
                                             {100.0f, 50.0f, 0.0f}, Vector3::zAxis(-30.0f), Vector3::yAxis()));

    _activeCamera = &_mainCamera;
    _activeCameraObject = &_mainCameraObject;

    /* Loop at 60 Hz max */
    setSwapInterval(1);
    //setMinimalLoopPeriod(16);
    _timeline.start();

    redraw();

}

MyApplication::~MyApplication()
{
}

Object3D* MyApplication::createSceneObject(Model& model, bool makeCaster, bool makeReceiver) {
    auto* object = new Object3D(&_scene);
    auto drawable = new CDrawable(*object, &_drawables);

    drawable->setShader(_shader);
    drawable->setMesh(model.mesh, model.radius);

    return object;
}

void MyApplication::drawEvent() {
    defaultFramebuffer.clear(FramebufferClear::Color|FramebufferClear::Depth);

if(!_mainCameraVelocity.isZero()) {
        Matrix4 transform = _activeCameraObject->transformation();
        transform.translation() += transform.rotation()*_mainCameraVelocity*0.3f;
        _activeCameraObject->setTransformation(transform);
        redraw();
    }

    if(!_mainCameraRotation.isZero()) {
        rotateCamera(_activeCameraObject, Vector2{_mainCameraRotation.x(), _mainCameraRotation.y()}, _mainCameraRotation.z());
    }
    Renderer::enable(Renderer::Feature::FaceCulling);
    Renderer::enable(Renderer::Feature::DepthTest);

    _activeCamera->draw(_drawables);

    swapBuffers();
}

void MyApplication::addModel(const Trade::MeshData3D& meshData3D) {
    _models.emplace_back();
    Model& model = _models.back();

    model.vertexBuffer.setData(MeshTools::interleave(meshData3D.positions(0), meshData3D.normals(0)),
                               BufferUsage::StaticDraw);

    Float maxMagnitudeSquared = 0.0f;
    for(Vector3 position: meshData3D.positions(0)) {
        Float magnitudeSquared = position.dot();

        if(magnitudeSquared > maxMagnitudeSquared) {
            maxMagnitudeSquared = magnitudeSquared;
        }
    }
    model.radius = std::sqrt(maxMagnitudeSquared);

    Containers::Array<char> indexData;
    Mesh::IndexType indexType;
    UnsignedInt indexStart, indexEnd;
    std::tie(indexData, indexType, indexStart, indexEnd) = MeshTools::compressIndices(meshData3D.indices());
    model.indexBuffer.setData(indexData, BufferUsage::StaticDraw);

    model.mesh.setPrimitive(meshData3D.primitive())
        .setCount(meshData3D.indices().size())
        .addVertexBuffer(model.vertexBuffer, 0, Shaders::Phong::Position{}, Shaders::Phong::Normal{})
        .setIndexBuffer(model.indexBuffer, 0, indexType, indexStart, indexEnd);
}

void MyApplication::mouseMoveEvent(MouseMoveEvent& event) {
    const Vector2 delta = 
        Vector2{event.position() - _previousMousePosition}/
        Vector2{defaultFramebuffer.viewport().size()};

        _previousMousePosition = event.position();
        rotateCamera(_activeCameraObject, delta);
        event.setAccepted();
        redraw();
}

void MyApplication::keyPressEvent(KeyEvent& event) {
    if(event.key() == KeyEvent::Key::Esc) {
        exit();
    } else if(event.key() == KeyEvent::Key::F1) {

    } else return;

    event.setAccepted();
    redraw();
}


void MyApplication::keyReleaseEvent(KeyEvent &event) {

    if(event.key() == Keys::Up || event.key() == Keys::Down) {

    } else if (event.key() == Keys::Q || event.key() == Keys::E) {

    } else if (event.key() == Keys::Right || event.key() == Keys::Left) {
    } else if (event.key() == Keys::H || event.key() == Keys::L) {
    } else if (event.key() == Keys::J || event.key() == Keys::K) {

    } else return;

    event.setAccepted();
    redraw();
}

template <class T> const T& min (const T &a, const T &b) {
    return !(b<a)?a:b;
}

Float min(const Vector2i &v) {
    return min(v.x(),v.y());
}

#define MAX_WIDTH 1920
#define MAX_HEIGHT 1200

void MyApplication::viewportEvent(const Vector2i& size) {
    const Float ratio = min(MAX_WIDTH / size.x(), MAX_HEIGHT / size.y());
    const Int newWidth = size.x()*ratio;
    const Int newHeight = size.y()*ratio;
    defaultFramebuffer.setViewport({{}, Vector2i{newWidth, newHeight}});
    Debug{} << newWidth << ", " << newHeight;
    _activeCamera->setViewport({newWidth, newHeight});
}

void MyApplication::rotateCamera(Object3D* cameraObject, const Vector2 delta, const float deltaZ) {
    constexpr const Float angleScale = 1.0f;
    const Float angleX = delta.x()*angleScale;
    const Float angleY = delta.y()*angleScale;
    const Float angleZ = deltaZ*angleScale;
    const Matrix4 transform = cameraObject->transformation();
    if(angleX != 0.0f || angleY != 0.0f) {
        cameraObject->setTransformation(Matrix4::lookAt(transform.translation(),
                                                        transform.translation() - transform.rotationScaling()*Vector3{-angleX, angleY, angleZ},
                                                        Vector3::yAxis()));
    }
}

void MyApplication::mousePressEvent(MouseEvent& event) {
    if(event.button() != MouseEvent::Button::Left) return;

    event.setAccepted();
}

void MyApplication::mouseReleaseEvent(MouseEvent& event) {

    event.setAccepted();
    redraw();
}

MAGNUM_APPLICATION_MAIN(MyApplication)
