#if !defined(TYPES_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Joaqim Planstedt $
   ======================================================================== */

#define TYPES_H

#include <Magnum/SceneGraph/SceneGraph.h>
#include <Magnum/Buffer.h>
#include <Magnum/Mesh.h>

using namespace Magnum;

typedef SceneGraph::Object<SceneGraph::MatrixTransformation3D> Object3D;
typedef SceneGraph::Scene<SceneGraph::MatrixTransformation3D> Scene3D;


struct Model {
    Buffer indexBuffer, vertexBuffer;
    Mesh mesh;
    Float radius;
};

#endif
