#include "Shooter.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;
using namespace m1;
void
Lab6::mazing(int i, int j)
{
    int ok = 0;
    int locatie[2][4];
    int a;
    int randommm;
    maze[i][j] = 1;
    randommm = rand() % 100;
    if (randommm > 80)
        maze[i][j] = 3;
    visited[i][j] = 1;
    if (i < 9 && visited[i + 1][j] == 0) {
        locatie[0][ok] = i + 1;
        locatie[1][ok] = j;
        ok++;
        visited[i + 1][j] = 1;
        a = rand();
        if (a % 100 > 75)
            mazing(i + 1, j);
    }
    if (i > 0 && visited[i - 1][j] == 0) {
        locatie[0][ok] = i - 1;
        locatie[1][ok] = j;
        ok++;
        visited[i - 1][j] = 1;
    }
    if (j > 0 && visited[i][j - 1] == 0) {
        locatie[0][ok] = i;
        locatie[1][ok] = j - 1;
        ok++;
        visited[i][j - 1] = 1;
    }
    if (j < 9 && visited[i][j + 1] == 0) {
        locatie[0][ok] = i;
        locatie[1][ok] = j + 1;
        ok++;
        visited[i][j + 1] = 1;
        a = rand();
        if (a % 100 > 75)
            mazing(i, j + 1);
    }
    if (ok == 0) {
        return;
    }
    a = rand();
    a = a % ok;
    mazing(locatie[0][a], locatie[1][a]);
}
bool
Itcolides(Lab6::pozPatrat rect1, Lab6::pozPatrat rect2)
{
    return (rect1.Position.x < rect2.Position.x + rect2.Size.x &&
        rect1.Position.x + rect1.Size.x > rect2.Position.x &&
        rect1.Position.y < rect2.Position.y + rect2.Size.y &&
        rect1.Position.y + rect1.Size.y > rect2.Position.y);
}
Lab6::Lab6() {}
Lab6::~Lab6() {}
void
Lab6::Init()
{
    camera = new implemented::Camera();
    for (int i = 0; i < 10; i++) {
        std::vector<int> vec;
        for (int j = 0; j < 10; j++) {
            vec.push_back(0);
        }
        visited.push_back(vec);
    }
    for (int i = 0; i < 10; i++) {
        std::vector<int> vec;
        for (int j = 0; j < 10; j++) {
            vec.push_back(0);
        }
        maze.push_back(vec);
    }
    mazing(0, 0);
    maze[0][0] = 1;
    for (int i = 0; i < 10; i++) {
        std::vector<int> vec;
        for (int j = 0; j < 10; j++) {
            cout << maze[i][j] << " ";
        }
        cout << "\n";
    }
    pozitiejucator = pozPatrat(glm::vec2(0, 0), glm::vec2(0.2, 0.05));
    camera->Set(glm::vec3(-0.5, 1, -1.5), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
    playerPosition = camera->GetTargetPosition();
    playerPosition[1] = 0.1;
    projectionMatrix =
        glm::perspective(RADIANS(fov), window->props.aspectRatio, 0.01f, 200.0f);
    for (int i = 0; i < 10; i++) {
        std::vector<int> vec;
        for (int j = 0; j < 10; j++) {
            if (maze[i][j] == 0) {
                perete = pozPatrat(glm::vec2(i, j), glm::vec2(1, 1));
                pozitieObstacole.push_back(perete);
            }
            if (maze[i][j] >= 2) {
                perete = pozPatrat(glm::vec2(i, j), glm::vec2(0.7, 0.7));
                pozitieInamici.push_back(perete);
            }
        }
    }
    for (int j = -1; j < 10; j++) {
        perete = pozPatrat(glm::vec2(-1, j), glm::vec2(1, 1));
        pozitieObstacole.push_back(perete);
    }
    for (int i = -1; i < 10; i++) {
        perete = pozPatrat(glm::vec2(i, -1), glm::vec2(1, 1));
        pozitieObstacole.push_back(perete);
    }
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(
            PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"),
            "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(
            PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"),
            "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    glEnable(GL_CULL_FACE);
    window->HidePointer();
    window->DisablePointer();
}
void
Lab6::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}
void
Lab6::Update(float deltaTimeSeconds)
{
    int nrglont = gloante.size();
    int nrInamici = pozitieInamici.size();
    for (int i = 0; i < nrglont; i++) {
        if (gloante[i].activ == 1) {
            gloante[i].time_elapsed += deltaTimeSeconds;
            gloante[i].position =
                gloante[i].begin + (gloante[i].target - gloante[i].begin) *
                (gloante[i].time_elapsed / gloante[i].TimpViata);
            if (gloante[i].time_elapsed >= gloante[i].TimpViata ||
                gloante[i].position[1] >= 1 || gloante[i].position[1] <= 0) {
                gloante[i].activ = 0;
                gloante.erase(gloante.begin() + i);
                nrglont--;
            }
            else {
                gloante[i].col.Position.x = gloante[i].position[0];
                gloante[i].col.Position.y = gloante[i].position[2];
                int ok = 1;
                for (pozPatrat obstacol : pozitieObstacole) {
                    if (Itcolides(gloante[i].col, obstacol)) {
                        gloante[i].activ = 0;
                        gloante.erase(gloante.begin() + i);
                        nrglont--;
                        ok = 0;
                        break;
                    }
                }
                if (ok)
                    for (int k = 0; k < nrInamici; k++) {
                        if (Itcolides(gloante[i].col, pozitieInamici[k])) {
                            maze[pozitieInamici[k].Position.x]
                                [pozitieInamici[k].Position.y] -= 1;
                            gloante[i].activ = 0;
                            gloante.erase(gloante.begin() + i);
                            nrglont--;
                            if (maze[pozitieInamici[k].Position.x]
                                [pozitieInamici[k].Position.y] == 1) {
                                pozitieInamici.erase(pozitieInamici.begin() + k);
                                nrInamici--;
                            }
                            ok = 0;
                            break;
                        }
                    }
                if (ok) {
                    glm::mat4 modelMatrix =
                        glm::translate(glm::mat4(1), gloante[i].position) *
                        glm::scale(glm::mat4(1), glm::vec3(0.05, 0.05, 0.05));
                    ;
                    RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);
                }
            }
        }
    }
    recoil += deltaTimeSeconds;
    // peretii.
    for (size_t i = 0; i < 10; i++) {
        for (size_t j = 0; j < 10; j++) {
            if (maze[i][j] == 0) {
                glm::mat4 modelMatrix =
                    glm::translate(glm::mat4(1), glm::vec3(i + 0.5, 0.5, j + 0.5));
                RenderMesh(meshes["box"], shaders["Color"], modelMatrix);
                modelMatrix =
                    glm::translate(glm::mat4(1), glm::vec3(i + 0.5, -0.5, j + 0.5));
                RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
            }
            else {
                glm::mat4 modelMatrix =
                    glm::translate(glm::mat4(1), glm::vec3(i + 0.5, -0.5, j + 0.5));
                RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
                // inamicii
                if (maze[i][j] == 3) {
                    if (misca >= 0.2) {
                        misca = 0.2;
                        semn = -0.1;
                    }
                    if (misca <= -0.2) {
                        semn = 0.1;
                        misca = -0.2;
                    }
                    misca = deltaTimeSeconds * semn + misca;
                    glm::mat4 modelMatrix =
                        glm::translate(glm::mat4(1),
                            glm::vec3(i + 0.5 + misca, 0, j + 0.5)) *
                        glm::scale(glm::mat4(1), glm::vec3(0.3, 0.3, 0.3));
                    DrawEnemyCOL(modelMatrix);
                }
                if (maze[i][j] == 2) {
                    glm::mat4 modelMatrix =
                        glm::translate(glm::mat4(1), glm::vec3(i + 0.5, 0, j + 0.5)) *
                        glm::scale(glm::mat4(1), glm::vec3(0.3, 0.3, 0.3));
                    DrawEnemyBLUE(modelMatrix);
                }
            }
        }
    }
    for (int j = -1; j < 10; j++) {
        glm::mat4 modelMatrix =
            glm::translate(glm::mat4(1), glm::vec3(-1 + 0.5, 0.5, j + 0.5));
        RenderMesh(meshes["box"], shaders["Color"], modelMatrix);
        modelMatrix = glm::translate(glm::mat4(1), glm::vec3(-1 + 0.5, -0.5, j + 0.5));
        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }
    for (int i = -1; i < 10; i++) {
        glm::mat4 modelMatrix =
            glm::translate(glm::mat4(1), glm::vec3(i + 0.5, 0.5, -0.5));
        RenderMesh(meshes["box"], shaders["Color"], modelMatrix);
        modelMatrix = glm::translate(glm::mat4(1), glm::vec3(i + 0.5, -0.5, -0.5));
        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }
    glm::mat4 Caracter_ModelMatrix = glm::translate(glm::mat4(1), playerPosition) *
        glm::scale(glm::mat4(1), glm::vec3(0.1, 0.1, 0.1));
    DrawPlayer(Caracter_ModelMatrix);
}
void
Lab6::DrawEnemyCOL(glm::mat4& Caracter_ModelMatrix)
{
    {
        glm::mat4 aux = glm::mat4(1);
        glm::mat4 modelMatrix =
            Caracter_ModelMatrix * aux * glm::translate(aux, glm::vec3(0, 1.15f, 0)) *
            glm::translate(aux, glm::vec3(0, 0.2, 0)) * glm::scale(aux, glm::vec3(2, 2, 2));
        RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);
    }
}
void
Lab6::DrawEnemyBLUE(glm::mat4& Caracter_ModelMatrix)
{
    {
        glm::mat4 aux = glm::mat4(1);
        glm::mat4 modelMatrix =
            Caracter_ModelMatrix * aux * glm::translate(aux, glm::vec3(0, 1.15f, 0)) *
            glm::translate(aux, glm::vec3(0, 0.2, 0)) * glm::scale(aux, glm::vec3(2, 2, 2));
        RenderMesh(meshes["sphere"], shaders["Color"], modelMatrix);
    }
}
void
Lab6::DrawPlayer(glm::mat4& Caracter_ModelMatrix)
{
    { // Cap
        glm::mat4 aux = glm::mat4(1);
        glm::mat4 modelMatrix = Caracter_ModelMatrix * aux *
            glm::translate(aux, glm::vec3(0, 1.15f, 0)) *
            glm::translate(aux, glm::vec3(0, 1.2f, 0)) *
            glm::scale(aux, glm::vec3(0.5, 0.5, 0.3));
        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }
    { // trunchi
        glm::mat4 modelMatrix = Caracter_ModelMatrix *
            glm::translate(glm::mat4(1), glm::vec3(0, 0.25f, 0)) *
            glm::translate(glm::mat4(1), glm::vec3(0.0f, 1.0f, 0)) *
            glm::scale(glm::mat4(1), glm::vec3(1.2, 1.3, 0.6));
        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }
    { // mana 1
        glm::mat4 modelMatrix = Caracter_ModelMatrix *
            glm::translate(glm::mat4(1), glm::vec3(-0.8f, 0.5f, 0)) *
            glm::translate(glm::mat4(1), glm::vec3(0, 0.8f, 0)) *
            glm::scale(glm::mat4(1), glm::vec3(0.3, 1.5f, 0.3));
        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }
    { // mana 2
        glm::mat4 modelMatrix = Caracter_ModelMatrix *
            glm::translate(glm::mat4(1), glm::vec3(-0.8f, 0.5f, 0)) *
            glm::translate(glm::mat4(1), glm::vec3(1.6, 0.8f, 0)) *
            glm::scale(glm::mat4(1), glm::vec3(0.3, 1.5f, 0.3));
        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }
    { // Picior 1
        glm::mat4 modelMatrix = Caracter_ModelMatrix *
            glm::translate(glm::mat4(1), glm::vec3(-0.8f, 0.5f, 0)) *
            glm::translate(glm::mat4(1), glm::vec3(0.5, -0.6f, 0)) *
            glm::scale(glm::mat4(1), glm::vec3(0.5, 2, 0.4));
        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }
    { // Picior 2
        glm::mat4 modelMatrix = Caracter_ModelMatrix *
            glm::translate(glm::mat4(1), glm::vec3(-0.8f, 0.5f, 0)) *
            glm::translate(glm::mat4(1), glm::vec3(1.1, -0.6f, 0)) *
            glm::scale(glm::mat4(1), glm::vec3(0.5, 2, 0.4));
        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }
}
void
Lab6::FrameEnd()
{
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}
void
Lab6::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh) {
        cout << "nimic";
        return;
    }
    if (!shader) {
        cout << "shd";
        return;
    }
    if (!shader->program) {
        cout << "yikes";
        return;
    }
    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(
        shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(
        shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(
        shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    mesh->Render();
}
void
Lab6::OnInputUpdate(float deltaTime, int mods)
{
    if (cameraM) {
        pos = camera->position;
        if (window->KeyHold(GLFW_KEY_W)) {
            camera->MoveForward(deltaTime * 2.f);
            playerPosition = camera->GetTargetPosition();
            playerPosition[1] = 0.1;
            pozitiejucator.Position.x = playerPosition[0] - 0.1;
            pozitiejucator.Position.y = playerPosition[2] - 0.025;
            for (pozPatrat obstacol : pozitieObstacole) {
                if (Itcolides(obstacol, pozitiejucator)) {
                    camera->MoveForward(-deltaTime * 2.f);
                    playerPosition = camera->GetTargetPosition();
                    playerPosition[1] = 0.1;
                    break;
                }
            }
            for (pozPatrat obstacol : pozitieInamici) {
                if (Itcolides(obstacol, pozitiejucator)) {
                    // ar trb sa piarda viata
                    camera->MoveForward(-deltaTime * 2.f);
                    playerPosition = camera->GetTargetPosition();
                    playerPosition[1] = 0.1;
                    break;
                }
            }
        }
        else if (window->KeyHold(GLFW_KEY_S)) {
            camera->MoveForward(-1 * deltaTime * 2.f);
            playerPosition = camera->GetTargetPosition();
            playerPosition[1] = 0.1;
            pozitiejucator.Position.x = playerPosition[0] - 0.1;
            pozitiejucator.Position.y = playerPosition[2] - 0.025;
            for (pozPatrat obstacol : pozitieObstacole) {
                if (Itcolides(obstacol, pozitiejucator)) {
                    // ar trb sa piarda viata
                    camera->MoveForward(deltaTime * 2.f);
                    playerPosition = camera->GetTargetPosition();
                    playerPosition[1] = 0.1;
                    break;
                }
            }
            for (pozPatrat obstacol : pozitieInamici) {
                if (Itcolides(obstacol, pozitiejucator)) {
                    camera->MoveForward(deltaTime * 2.f);
                    playerPosition = camera->GetTargetPosition();
                    playerPosition[1] = 0.1;
                    break;
                }
            }
        }
        if (window->KeyHold(GLFW_KEY_D)) {
            camera->TranslateRight(deltaTime * 2.f);
            playerPosition = camera->GetTargetPosition();
            playerPosition[1] = 0.1;
            pozitiejucator.Position.x = playerPosition[0] - 0.1;
            pozitiejucator.Position.y = playerPosition[2] - 0.025;
            for (pozPatrat obstacol : pozitieObstacole) {
                if (Itcolides(obstacol, pozitiejucator)) {
                    camera->TranslateRight(-deltaTime * 2.f);
                    playerPosition = camera->GetTargetPosition();
                    playerPosition[1] = 0.1;
                    break;
                }
            }
            for (pozPatrat obstacol : pozitieInamici) {
                if (Itcolides(obstacol, pozitiejucator)) {
                    // ar trb sa piarda viata
                    camera->TranslateRight(-deltaTime * 2.f);
                    playerPosition = camera->GetTargetPosition();
                    playerPosition[1] = 0.1;
                    break;
                }
            }
        }
        else if (window->KeyHold(GLFW_KEY_A)) {
            camera->TranslateRight(-1 * deltaTime * 2.f);
            playerPosition = camera->GetTargetPosition();
            playerPosition[1] = 0.1;
            pozitiejucator.Position.x = playerPosition[0] - 0.1;
            pozitiejucator.Position.y = playerPosition[2] - 0.025;
            for (pozPatrat obstacol : pozitieObstacole) {
                if (Itcolides(obstacol, pozitiejucator)) {
                    camera->TranslateRight(deltaTime * 2.f);
                    playerPosition = camera->GetTargetPosition();
                    playerPosition[1] = 0.1;
                    break;
                }
            }
            for (pozPatrat obstacol : pozitieInamici) {
                if (Itcolides(obstacol, pozitiejucator)) {
                    // ar trb sa piarda viata
                    camera->TranslateRight(deltaTime * 2.f);
                    playerPosition = camera->GetTargetPosition();
                    playerPosition[1] = 0.1;
                    break;
                }
            }
        }
        if (playerPosition[0] >= 10 || playerPosition[2] >= 10) {
            cout << "\n\nAI CASTIGAT\n\n";
            exit(0);
        }
    }
}
void
Lab6::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_SPACE && cameraM == 1) {
        copiePozCam = camera->position;
        forwardC = camera->forward;
        upC = camera->up;
        rightC = camera->right;
        camera->Set(glm::vec3(playerPosition[0], 0.7, playerPosition[2]),
            glm::vec3(0, 1, 0),
            glm::vec3(0, 1, 0));
        camera->distanceToTarget = -2;
        cameraM = 0;
    }
    if (key == GLFW_KEY_G && cameraM == 0) {
        camera->position = copiePozCam;
        camera->forward = forwardC;
        camera->up = upC;
        camera->right = rightC;
        camera->distanceToTarget = 2.2;
        cameraM = 1;
    }
}
void
Lab6::OnKeyRelease(int key, int mods)
{}
void
Lab6::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    {
        float sensivityOX = 0.005f;
        float sensivityOY = 0.005f;
        if (cameraM)
            camera->RotateThirdPerson_OY(-deltaX * sensivityOX);
        else {
            camera->RotateFirstPerson_OY(-deltaX * sensivityOX);
            camera->RotateFirstPerson_OX(-deltaY * sensivityOY);
        }
    }
}
void
Lab6::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (cameraM == 0) {
        if (recoil > 0.53) {

            recoil = 0;
            pozPatrat colizune = pozPatrat(
                glm::vec2(playerPosition[0], playerPosition[2]), glm::vec2(0.015, 0.015));
            shoot = Projectile(
                0, 1.5, camera->GetGUD(), playerPosition, 1, playerPosition, colizune);
            gloante.push_back(shoot);
        }
    }
}
void
Lab6::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{}
void
Lab6::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{}
void
Lab6::OnWindowResize(int width, int height)
{}
