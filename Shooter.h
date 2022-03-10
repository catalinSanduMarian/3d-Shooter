#pragma once
#include "lab_camera.h"
#include "components/simple_scene.h"
#include <vector>
namespace m1 {
    class Lab6 : public gfxc::SimpleScene
    {
    public:
        Lab6();
        ~Lab6();
        void Init() override;
        glm::mat4 projectionMatrix;
        int fov = 60;
        std::vector<std::vector<int>> mat, visited, maze;
        int cameraM = 1;
        float recoil = 1;
        glm::vec3 forwardC, upC, rightC;
        float misca = 0;
        float semn = 0.1;
        struct pozPatrat
        {
            pozPatrat()
                : Position(0)
                , Size(0)
            {}
            pozPatrat(glm::vec2 Position, glm::vec2 Size)
                : Position(Position)
                , Size(Size)
            {}
            glm::vec2 Position;
            glm::vec2 Size;
        };
        struct Projectile
        {
            Projectile()
                : time_elapsed(0)
                , TimpViata(0)
                , target(0)
                , position(0)
                , activ(0)
                , begin(0)
                , col()
            {}
            Projectile(float time_elapsed,
                float TimpViata,
                glm::vec3 target,
                glm::vec3 position,
                int activ,
                glm::vec3 begin,
                pozPatrat col)
                : time_elapsed(time_elapsed)
                , TimpViata(TimpViata)
                , target(target)
                , position(position)
                , activ(activ)
                , begin(begin)
                , col(col)
            {}
            float time_elapsed;
            float TimpViata;
            glm::vec3 target;
            glm::vec3 position;
            int activ;
            glm::vec3 begin;
            pozPatrat col;
        };
        std::vector<Projectile> gloante;
        Projectile shoot;
        struct LogicSpace
        {
            LogicSpace()
                : x(0)
                , y(0)
                , z(0)
                , width(1)
                , height(1)
                , depth(1)
            {}
            LogicSpace(float x, float y, float z, float width, float height, float depth)
                : x(x)
                , y(y)
                , z(z)
                , width(width)
                , height(height)
                , depth(depth)
            {}
            float x;
            float y;
            float z;
            float width;
            float height;
            float depth;
        };
        pozPatrat perete, pozitiejucator;
        std::vector<pozPatrat> pozitieObstacole;
        std::vector<pozPatrat> pozitieInamici;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        void RenderSimpleMesh(Mesh* mesh,
            Shader* shader,
            const glm::mat4& modelMatrix,
            const glm::vec3& color);
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void DrawPlayer(glm::mat4& Caracter_ModelMatrix);
        void DrawEnemyCOL(glm::mat4& Caracter_ModelMatrix);
        void DrawEnemyBLUE(glm::mat4& Caracter_ModelMatrix);
        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);
        void mazing(int i, int j);

    protected:
        glm::vec3 playerPosition, pos, deltapos, copiePozCam;
        GLenum polygonMode;
        bool mode = false, mode2 = false, on = false;
        float theta;
        LogicSpace bullet;
        implemented::Camera* camera;
        implemented::Camera* copie;
    };
}
