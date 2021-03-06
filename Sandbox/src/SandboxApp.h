#pragma once
#include <vector>

#include "Application.h"

namespace edgl {

class Model;
class Shader;
class Light;
}

using namespace edgl;
class SandboxApp : public edgl::Application
{
public:
    SandboxApp();
    virtual void OnInit() override;
    virtual void OnLoop(float dt) override;
private:
    std::shared_ptr<InputManager> m_InputManager;
    std::shared_ptr<Camera> m_Camera;
    std::vector<std::shared_ptr<Model>> m_Models;
    glm::mat4 m_Projection;
    std::shared_ptr<Shader> m_Shader;
    std::vector<std::shared_ptr<Light>> m_Lights;
    static edgl::ApplicationSettings GetSettings();
    inline virtual const std::shared_ptr<InputManager>& GetInputManager() const override { return m_InputManager; }
};