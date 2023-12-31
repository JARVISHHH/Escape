#include "graphics.h"
#include "shapedata.h"
#include <Engine/UIKit/uiElement.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
const float near_plane = 0.1f, far_plane = 100.f;

Graphics::Graphics():
    m_uiRenderer(std::make_shared<UIRenderer>())
{

}

Graphics::~Graphics(){

}

void Graphics::initializeGLEW(){
    glewExperimental = GL_TRUE;
    std::cout<<"GLEWInit status: "<<glewInit()<<std::endl;
}

void Graphics::initialize(){
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);

    std::cout<<"initialize text renderer"<<std::endl;
    m_uiRenderer->initialize();

    std::cout<<"add shapes"<<std::endl;
    addShape("quad", quadVertexBufferData, VAOAttrib::POS | VAOAttrib::NORM | VAOAttrib::UV);
    addShape("cube", cubeVertexBufferData, VAOAttrib::POS | VAOAttrib::NORM | VAOAttrib::UV);
    addShape("sphere", sphereVertexBufferData, VAOAttrib::POS | VAOAttrib::NORM | VAOAttrib::UV);
    addShape("cylinder", cylinderVertexBufferData, VAOAttrib::POS | VAOAttrib::NORM | VAOAttrib::UV);
    addShape("cone", coneVertexBufferData, VAOAttrib::POS | VAOAttrib::NORM | VAOAttrib::UV);

    addShader("phong", {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER}, {"Resources/Shaders/phong.vert", "Resources/Shaders/phong.frag"});
    addShader("ui", {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER}, {"Resources/Shaders/ui.vert", "Resources/Shaders/ui.frag"});
    addShader("particle", {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER}, {"Resources/Shaders/particle.vert", "Resources/Shaders/particle.frag"});
    addShader("shadow", { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER }, { "Resources/Shaders/shadow.vert", "Resources/Shaders/shadow.frag" });
    addShader("pointShadow", { GL_VERTEX_SHADER, GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER }, { "Resources/Shaders/pointShadow.vert", "Resources/Shaders/pointShadow.geom", "Resources/Shaders/pointShadow.frag" });

    // Shadow
    depthMapFBOs.resize(16);
    glGenFramebuffers(16, depthMapFBOs.data());
    depthMaps.resize(16);
    depthCubeMaps.resize(16);

    std::cout << "depthMapFBOs: ";
    for (int i = 0; i < 16; i++) {
        std::cout << depthMapFBOs[i] << " ";
    }
    std::cout << std::endl;

    for (unsigned int i = 0; i < 16; i++) {
        auto& depthMapFBO = depthMapFBOs[i];
        auto& depthCubeMap = depthCubeMaps[i];
        glActiveTexture(GL_TEXTURE0 + i + 2);
        glGenTextures(1, &depthCubeMap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
        for (unsigned int j = 0; j < 6; j++) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, 0, GL_DEPTH_COMPONENT,
                SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubeMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    std::cout << "Initialized shadow FBOs and textures" << std::endl;
    
    bindShader("phong");
    
    addMaterial("default", glm::vec3(1));
    addMaterial("defaultNormal", glm::vec3(0));

    std::cout<<"add font"<<std::endl;
    addFont("opensans", "Resources/Fonts/OpenSans.ttf");
    getFont("opensans");
}

void Graphics::clearScreen(GLbitfield mask){
    glClear(mask);
}

void Graphics::setClearColor(glm::vec3 clearColor){
    glClearColor(clearColor.r, clearColor.g, clearColor.b, 1);
}

void Graphics::setCameraData(std::shared_ptr<Camera> camera){
    m_active_shader->setCamera(camera);
}

void Graphics::addShader(std::string shaderName, std::vector<GLenum> shaderTypes, std::vector<const char*> filepaths){
    m_shaders.insert({shaderName, std::make_shared<Shader>(shaderTypes, filepaths)});
}

void Graphics::removeShader(std::string shaderName){
    if(shaderName != "phong" && shaderName != "text"){
        m_shaders.erase(shaderName);
    }
}

void Graphics::bindShader(std::string shaderName){
    m_shaders.at(shaderName)->bind();
    m_active_shader = m_shaders.at(shaderName);
}

std::shared_ptr<Shape> Graphics::addShape(std::string shapeName, std::vector<float> data, VAOAttrib attribs){
    m_shapes.insert({shapeName, std::make_shared<Shape>(std::make_shared<VAO>(std::make_shared<VBO>(data), attribs))});
    return m_shapes.at(shapeName);
}
std::vector<glm::vec3> Graphics::addShape(std::string shapeName, std::string filepath, bool hasUV, int uvScale){
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    std::string dirPath = "./Resources/Meshes/";

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str(), dirPath.c_str())) {
        throw std::runtime_error(warn + err);
    }

    int numTriangles = 0;
    for(size_t s = 0; s < shapes.size(); s++){
        numTriangles += shapes[s].mesh.num_face_vertices.size();
    }

    float eachVertexSize = 17;
    std::vector<float> drawData;
    drawData.resize(numTriangles * 3 * eachVertexSize);
    std::vector<glm::vec3> collisionData;
    collisionData.resize(numTriangles * 3);

    int i = 0;
    int j = 0;
    glm::vec3 minPoint = { 10000, 10000, 10000 }, maxPoint = { -10000, -10000, -10000 };
    for(size_t s = 0; s < shapes.size(); s++) {
        size_t index_offset = 0;
        for(size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            unsigned int fv = shapes[s].mesh.num_face_vertices[f];

            for(size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

                // Add position
                drawData[i] = attrib.vertices[3*idx.vertex_index];
                drawData[i + 1] = attrib.vertices[3*idx.vertex_index + 1];
                drawData[i + 2] = attrib.vertices[3*idx.vertex_index + 2];
                // Add normal
                drawData[i + 3] = attrib.normals[3*idx.normal_index];
                drawData[i + 4] = attrib.normals[3*idx.normal_index + 1];
                drawData[i + 5] = attrib.normals[3*idx.normal_index + 2];
                // Add uv
                drawData[i + 6] = 0;
                drawData[i + 7] = 0;
                if(hasUV){
                    drawData[i + 6] = attrib.texcoords[2*idx.texcoord_index] * uvScale;
                    drawData[i + 7] = attrib.texcoords[2*idx.texcoord_index + 1] * uvScale;
                }

                // Add collision position data
                collisionData[j] = glm::vec3(attrib.vertices[3*idx.vertex_index], attrib.vertices[3*idx.vertex_index + 1], attrib.vertices[3*idx.vertex_index + 2]);
                i += eachVertexSize;
                j += 1;
            }

            index_offset += fv;
        }
    }

    for (int j = 0; j < numTriangles; j++) {
        int i = j * 3;
        glm::vec3 v0 = glm::vec3(drawData[i * eachVertexSize], drawData[i * eachVertexSize + 1], drawData[i * eachVertexSize + 2]);
        glm::vec3 v1 = glm::vec3(drawData[(i + 1) * eachVertexSize], drawData[(i + 1) * eachVertexSize + 1], drawData[(i + 1) * eachVertexSize + 2]);
        glm::vec3 v2 = glm::vec3(drawData[(i + 2) * eachVertexSize], drawData[(i + 2) * eachVertexSize + 1], drawData[(i + 2) * eachVertexSize + 2]);
    
        glm::vec2 uv0 = glm::vec2(drawData[i * eachVertexSize + 6], drawData[i * eachVertexSize + 7]);
        glm::vec2 uv1 = glm::vec2(drawData[(i + 1) * eachVertexSize + 6], drawData[(i + 1) * eachVertexSize + 7]);
        glm::vec2 uv2 = glm::vec2(drawData[(i + 2) * eachVertexSize + 6], drawData[(i + 2) * eachVertexSize + 7]);

        glm::vec3 deltaPos1 = v1 - v0;
        glm::vec3 deltaPos2 = v2 - v0;

        glm::vec2 deltaUV1 = uv1 - uv0;
        glm::vec2 deltaUV2 = uv2 - uv0;

        float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
        glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
        glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

        for (int k = 0; k < 3; k++) {
            for (int l = 0; l < 3; l++) {
                drawData[(i + k) * eachVertexSize + 11 + l] = tangent[l];
                drawData[(i + k) * eachVertexSize + 14 + l] = bitangent[l];
            }
        }
    }

    m_shapes.insert({shapeName, std::make_shared<Shape>(std::make_shared<VAO>(std::make_shared<VBO>(drawData), VAOAttrib::POS | VAOAttrib::NORM | VAOAttrib::UV | VAOAttrib::COLOR | VAOAttrib::TANGENT))});

    return collisionData;
}

std::pair<std::vector<glm::vec3>, std::vector<glm::ivec3>> Graphics::getNavmeshData(std::string filepath){
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str())) {
        throw std::runtime_error(warn + err);
    }

    int numTriangles = 0;
    for(size_t s = 0; s < shapes.size(); s++){
        numTriangles += shapes[s].mesh.num_face_vertices.size();
    }

    std::vector<glm::vec3> positions;
    std::vector<glm::ivec3> faces;

    for(int i = 0; i<attrib.vertices.size(); i+=3){
        positions.push_back(glm::vec3(attrib.vertices[i], attrib.vertices[i+1], attrib.vertices[i+2]));
    }


    int i = 0;
    int j = 0;
    for(size_t s = 0; s < shapes.size(); s++) {
        for(size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            tinyobj::index_t idx1 = shapes[s].mesh.indices[3* f];
            tinyobj::index_t idx2 = shapes[s].mesh.indices[3* f + 1];
            tinyobj::index_t idx3 = shapes[s].mesh.indices[3* f + 2];
            faces.push_back(glm::ivec3(idx1.vertex_index, idx2.vertex_index, idx3.vertex_index));
        }
    }

    return std::make_pair(positions, faces);
}

void Graphics::removeShape(std::string shapeName){
    m_shapes.erase(shapeName);
}

std::shared_ptr<Shape> Graphics::getShape(std::string shapeName){
    return m_shapes.at(shapeName);
}

void Graphics::drawShape(std::shared_ptr<Shape> myShape, std::shared_ptr<ModelTransform> modelTransform, std::shared_ptr<Material> material, std::shared_ptr<Material> materialNormal){
    Debug::checkGLError();
    if(material == nullptr){
        m_active_shader->setMaterial(getMaterial("default"));
    }
    else{
        m_active_shader->setMaterial(material);
    }
    Debug::checkGLError();
    if (materialNormal == nullptr) {
        m_active_shader->setNormalMap(getMaterial("defaultNormal"));
    }
    else {
        m_active_shader->setNormalMap(materialNormal);
    }
    Debug::checkGLError();
    m_active_shader->setModelTransform(modelTransform);
    Debug::checkGLError();
    myShape->draw();
    Debug::checkGLError();
}

void Graphics::drawShape(std::shared_ptr<Shape> myShape, glm::mat4 modelMatrix, std::shared_ptr<Material> material, std::shared_ptr<Material> materialNormal){
    if(material == nullptr){
        m_active_shader->setMaterial(getMaterial("default"));
    }
    else{
        m_active_shader->setMaterial(material);
    }
    if (materialNormal == nullptr) {
        m_active_shader->setNormalMap(getMaterial("defaultNormal"));
    }
    else {
        m_active_shader->setNormalMap(materialNormal);
    }
    m_active_shader->setModelTransform(modelMatrix);
    myShape->draw();
}

void Graphics::setShadow(int index)
{
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBOs[index]);
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glClear(GL_DEPTH_BUFFER_BIT);
    glm::mat4 shadowProjection = glm::perspective(glm::radians(90.0f), 1.f, near_plane, far_plane);
    glm::mat4 shadowTransforms[6] = {
    shadowProjection *
        glm::lookAt(lights[index]->getPos(), lights[index]->getPos() + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)),
    shadowProjection *
        glm::lookAt(lights[index]->getPos(), lights[index]->getPos() + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)),
    shadowProjection *
        glm::lookAt(lights[index]->getPos(), lights[index]->getPos() + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)),
    shadowProjection *
        glm::lookAt(lights[index]->getPos(), lights[index]->getPos() + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)),
    shadowProjection *
        glm::lookAt(lights[index]->getPos(), lights[index]->getPos() + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)),
    shadowProjection *
        glm::lookAt(lights[index]->getPos(), lights[index]->getPos() + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0))
    };
    glUniformMatrix4fv(glGetUniformLocation(m_active_shader->getHandle(), "shadowMatrices"), 6, GL_FALSE, glm::value_ptr(shadowTransforms[0]));
    Debug::checkGLError();
    glUniform3f(glGetUniformLocation(m_active_shader->getHandle(), "lightPos"), lights[index]->getPos().x, lights[index]->getPos().y, lights[index]->getPos().z);
    Debug::checkGLError();
    glUniform1f(glGetUniformLocation(m_active_shader->getHandle(), "farPlane"), far_plane);
    Debug::checkGLError();
}

void Graphics::bindShadow()
{
    // Matrices
    //glm::mat4 lightProjection = glm::perspective(1.f, 1.f, near_plane, far_plane);
    //glm::mat4 lightSpaceMatrix[16];
    //for (int i = 0; i < 16; i++) lightSpaceMatrix[i] = glm::mat4(1.0f);
    //for (int i = 0; i < lights.size(); i++) {
    //    glm::mat4 lightView = glm::lookAt(lights[i]->getPos(),
    //                                      lights[i]->getPos() + glm::vec3(0.0f, -1.0f, 0.0f),
    //                                      glm::vec3(-1.0f, 0.0f, 0.0f));
    //    lightSpaceMatrix[i] = lightProjection * lightView;
    //}
    //Debug::checkGLError();
    //auto lightSpaceMatrixLocation = glGetUniformLocation(m_active_shader->getHandle(), "lightSpaceMatrix");
    //Debug::checkGLError();
    //glUniformMatrix4fv(lightSpaceMatrixLocation, 16, GL_FALSE, glm::value_ptr(lightSpaceMatrix[0]));
    //Debug::checkGLError();

    // Texture
    auto samplerLocation = glGetUniformLocation(m_active_shader->getHandle(), "depthCubeMaps");
    Debug::checkGLError();
    GLint samplers[16];
    for (int i = 0; i < 16; i++) {
        samplers[i] = i + 2;
        glActiveTexture(GL_TEXTURE0 + i + 2);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMaps[i]);
    }
    glUniform1iv(samplerLocation, 16, samplers);
    Debug::checkGLError();

    // Others
    glUniform1f(glGetUniformLocation(m_active_shader->getHandle(), "farPlane"), far_plane);
}

std::vector<float> Graphics::getObjData(std::string filepath){
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str())) {
        throw std::runtime_error(warn + err);
    }

    std::vector<glm::vec3> faces;
    std::vector<glm::vec3> vertices;

    for(size_t s = 0; s < shapes.size(); s++) {
        size_t index_offset = 0;
        for(size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            unsigned int fv = shapes[s].mesh.num_face_vertices[f];

            glm::vec3 face;
            for(size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

                face[v] = idx.vertex_index;

            }
            faces.push_back(face);

            index_offset += fv;
        }
    }
    for (size_t i = 0; i < attrib.vertices.size(); i += 3) {
        vertices.emplace_back(attrib.vertices[i], attrib.vertices[i + 1], attrib.vertices[i + 2]);
    }

    std::vector<float> data;
    data.resize(18 * faces.size());
    for(int i = 0; i<faces.size(); i++){
        glm::vec3 v0 = vertices[static_cast<int>(faces[i].x)];
        glm::vec3 v1 = vertices[static_cast<int>(faces[i].y)];
        glm::vec3 v2 = vertices[static_cast<int>(faces[i].z)];
        glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

        data[18*i] = v0.x;
        data[18*i+1] = v0.y;
        data[18*i+2] = v0.z;
        data[18*i+3] = normal.x;
        data[18*i+4] = normal.y;
        data[18*i+5] = normal.z;
        data[18*i+6] = v1.x;
        data[18*i+7] = v1.y;
        data[18*i+8] = v1.z;
        data[18*i+9] = normal.x;
        data[18*i+10] = normal.y;
        data[18*i+11] = normal.z;
        data[18*i+12] = v2.x;
        data[18*i+13] = v2.y;
        data[18*i+14] = v2.z;
        data[18*i+15] = normal.x;
        data[18*i+16] = normal.y;
        data[18*i+17] = normal.z;
    }

    return data;
}

std::shared_ptr<Material> Graphics::addMaterial(std::string materialName, glm::vec3 color, float shininess){
    std::shared_ptr<Material> newMaterial = std::make_shared<Material>(color, shininess);
    m_materials.insert({materialName, newMaterial});
    std::cout << "Added new material: " << materialName << std::endl;
    return m_materials.at(materialName);
}

std::shared_ptr<Material> Graphics::addMaterial(std::string materialName, float shininess){
    std::shared_ptr<Material> newMaterial = std::make_shared<Material>(shininess);
    m_materials.insert({materialName, newMaterial});
    return m_materials.at(materialName);
}

std::shared_ptr<Material> Graphics::addMaterial(std::string materialName, std::string filePath, float shininess){
    std::shared_ptr<Texture> newTexture = std::make_shared<Texture>(filePath);
    std::shared_ptr<Material> newMaterial = std::make_shared<Material>(newTexture, shininess);
    m_materials.insert({materialName, newMaterial});
    return m_materials.at(materialName);
}

std::shared_ptr<Material> Graphics::addMaterial(std::string materialName, std::string filePath, GLenum texUnit, float shininess)
{
    std::shared_ptr<Texture> newTexture = std::make_shared<Texture>(filePath, texUnit);
    std::shared_ptr<Material> newMaterial = std::make_shared<Material>(newTexture, shininess);
    m_materials.insert({ materialName, newMaterial });
    return m_materials.at(materialName);
}

void Graphics::removeMaterial(std::string materialName){
    if(materialName != "default"){
        m_materials.erase(materialName);
    }
}

std::shared_ptr<Material> Graphics::getMaterial(std::string materialName){
    //std::cout << "get material: " << materialName << " at: " << m_materials.at(materialName) << std::endl;
    return m_materials.at(materialName);
}

std::shared_ptr<Font> Graphics::addFont(std::string fontName, std::string filepath){
    std::shared_ptr<Font> newFont = std::make_shared<Font>(filepath);
    m_fonts.insert({fontName, newFont});
    return m_fonts.at(fontName);
}

void Graphics::removeFont(std::string fontName){
    m_fonts.erase(fontName);
}

std::shared_ptr<Font> Graphics::getFont(std::string fontName){
    return m_fonts.at(fontName);
}

void Graphics::drawUIText(std::shared_ptr<Font> font, std::string text, glm::vec2 anchorPosition, AnchorPoint anchorPoint, float textBoxWidth, float fontSize, float lineSpacing, glm::vec3 textColor){
    m_active_shader->setUIUniforms(m_windowSize.x, m_windowSize.y, textColor);
    m_uiRenderer->renderUIText(font, text, anchorPosition, anchorPoint, textBoxWidth, fontSize, lineSpacing, textColor);
}

void Graphics::drawUI(std::shared_ptr<UIElement> uiElement)
{
    m_active_shader->setUIUniforms(m_windowSize.x, m_windowSize.y, uiElement);
    auto material = getMaterial(uiElement->getMaterialName());
    //std::cout << "material: " << uiElement->getMaterialName() << std::endl;
    material->getTexture()->bind();
    glUniform1i(glGetUniformLocation(m_active_shader->getHandle(), "sprite"), material->getTexture()->getTexUnitUint());
    m_uiRenderer->renderUI(uiElement);
}

void Graphics::setGlobalData(glm::vec3 globalCoeffs){
    m_active_shader->setGlobalCoeffs(globalCoeffs);
}

void Graphics::setLights(std::vector<std::shared_ptr<Light>> lights){
    m_active_shader->setLights(lights);
    this->lights = lights;
}

std::vector<std::shared_ptr<Light>>& Graphics::getLights()
{
    return lights;
}

void Graphics::clearLights(){
    m_active_shader->clearLights();
    lights.clear();
}

void Graphics::setUniform4fv(const GLchar* uniform, glm::vec4 value)
{
    m_active_shader->setUniform4fv(uniform, value);
}

void Graphics::setUniformFloat(const GLchar* uniform, float value)
{
    m_active_shader->setUniformFloat(uniform, value);
}

void Graphics::setWindowSize(glm::ivec2 windowSize){
    m_windowSize = windowSize;
}

glm::ivec2 Graphics::getWindowSize(){
    return m_windowSize;
}

void Graphics::setFramebufferSize(glm::ivec2 framebufferSize){
    m_framebufferSize = framebufferSize;
    glViewport(0, 0, m_framebufferSize.x, m_framebufferSize.y);
}

glm::ivec2 Graphics::getFramebufferSize(){
    return m_framebufferSize;
}
