#include "texturedobjscene.h"

// Needed for texture loading
#include "pngutils.h"

#include <iostream>
#include <QObject>
#include <QImage>
#include <QtOpenGL/QGLWidget>


/**
 * @brief TexturedObjScene::TexturedObjScene constructs the scene
 * @param _filename The filename of the obj mesh to load (handled by parent class)
 * @param _texname The filename of the texture (if empty, no texture will be mapped)
 * @param _texMode The mode of the texture (an enum in the class header)
 */
TexturedObjScene::TexturedObjScene(const std::string &_filename, const std::string &_texname, const TextureMode &_texMode)
    : ObjScene(_filename), m_texname(_texname), m_texMode(_texMode)
{
}

/**
 * @brief TexturedObjScene::~TexturedObjScene delete allocated memory (for e.g. the texture)
 */
TexturedObjScene::~TexturedObjScene()  {
    if (m_isInit && (!m_texname.empty())) {
        // Delete the memory allocated for the texture here
        glDeleteTextures(1, &m_texId);
    }
}

/**
 * @brief TexturedObjScene::init initialise the texture if one is specified.
 */
void TexturedObjScene::init() {
    // Call the parent class initialiser
    ObjScene::init();

    // Don't initialise the texture if the user didn't specify one
    if(m_texname.empty()) return;

    // Enable texturing
    glEnable(GL_TEXTURE_2D);
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

    // Load the GL texture using a custom PNG loading function.
//    GLuint width, height;
//    GLuint dataSize;
//    GLubyte *data;
//    dataSize = png_texture_size(m_texname.c_str(), width, height);
//    // If there was some problem we assume that texture won't be used
//    if (dataSize == 0) return;

//    // Allocate the memory for our image data
//    data = (GLubyte*) malloc(dataSize);

//    // Send our newly allocate data chunk over to the texture loading function
//    png_texture_load(m_texname.c_str(), data);

    QImage texImage = QImage(m_texname.c_str());
    QImage texData = QGLWidget::convertToGLFormat(texImage);


    // This texture unit would be activated by default - I'm including it to show how it's done
    glActiveTexture(GL_TEXTURE0);

    // Generate the texture ID's for this texture. The first parameter is the integer value
    // indicating how many texture ids to generate, while the second is a pointer to the first
    // element in an array to fill with texIds. This is used like a pointer, but is really a
    // table index for GL to look up the actual memory address. Giving the user the actual
    // memory address in GPU memory is a problem as the memory can be copied around by the
    // drivers for performance reasons.
    glGenTextures(1, &m_texId);

    // Bind our generated tex. This means that this texture will currently be
    // active and applied to whatever geometry will consecutively be drawn for the currently active
    // texture unit.
    glBindTexture(GL_TEXTURE_2D, m_texId);

    // Load the texture using the appropriate texture mode
    switch(m_texMode) {
    case TEXMODE_BILINEAR: // (ok?)
        glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, texData.width(), texData.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texData.bits());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        break;
    case TEXMODE_TRILINEAR: // (best?, slowest)
        gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGBA, texData.width(), texData.height(), GL_RGBA, GL_UNSIGNED_BYTE, texData.bits());
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        break;
    default: //TEXMODE_NEAREST (worst, fastest)
        glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, texData.width(), texData.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texData.bits());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        break;
    }
    // the texture wraps over at the edges (repeat)
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );    
}

/**
 * @brief TexturedObjScene::draw draws the current scene (nothing special needed for the texture - everything is bound in init())
 */
void TexturedObjScene::draw() {
    // Call the parent class draw routine
    ObjScene::draw();
}
