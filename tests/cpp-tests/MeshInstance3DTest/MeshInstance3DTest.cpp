#include "MeshInstance3DTest.h"
#include <ocf/3d/MeshInstance3D.h>
#include <ocf/base/Camera.h>
#include <ocf/base/View.h>
#include <ocf/math/vec3.h>
#include <ocf/math/geometric.h>

using namespace ocf;
using namespace ocf::math;

MeshInstance3DTest::MeshInstance3DTest()
{
}

MeshInstance3DTest::~MeshInstance3DTest()
{
}

void MeshInstance3DTest::onEnter()
{
    Camera* camera = Camera::createPerspective(math::radians(60.0f), 920.0f / 720.0f, 0.1f, 100.0f);
    camera->setPosition(vec3(8.0f, 5.0f, 8.0f));
    camera->setCenter(vec3(0.0f, 0.0f, 0.0f));

    View* view = new View();
    view->setCamera(camera);
    addNode(view);

    m_meshInstance = MeshInstance3D::create("models/teapot.obj");
    view->addChild(m_meshInstance);
}

void MeshInstance3DTest::onExit()
{
}

void MeshInstance3DTest::process(float deltaTime)
{
    auto rotateY = m_meshInstance->getRotation().y;
    m_meshInstance->setRotation(vec3(0.0f, rotateY + 20.0f * deltaTime, 0.0f));
}
