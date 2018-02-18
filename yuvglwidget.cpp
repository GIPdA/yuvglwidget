#include "yuvglwidget.h"

YUVGLWidget::YUVGLWidget(QSurfaceFormat const& format, QWidget *parent) :
    QOpenGLWidget(parent)
{
    setFormat(format);
}

YUVGLWidget::~YUVGLWidget()
{
    makeCurrent();
    if (y_tex) glDeleteTextures(1, &y_tex);
    if (u_tex) glDeleteTextures(1, &u_tex);
    if (v_tex) glDeleteTextures(1, &v_tex);
    doneCurrent();
}

QSize YUVGLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize YUVGLWidget::sizeHint() const
{
    return QSize(3840, 2160); // 4k video
}

void YUVGLWidget::setFrameSize(unsigned int width, unsigned int height)
{
    m_frameWidth = width;
    m_frameHeight = height;
}

void YUVGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glDisable(GL_DEPTH_TEST);

    initializeTextures();

    // Setup shaders
    m_program.addShaderFromSourceFile(QOpenGLShader::Vertex,   ":/shaders/yuv_core.vert");
    m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/yuv_core.frag");

//    m_program.bindAttributeLocation("y_tex", 0);
//    m_program.bindAttributeLocation("u_tex", 1);
//    m_program.bindAttributeLocation("v_tex", 2);

    m_program.link();
    m_program.bind();

    m_program.setUniformValue("y_tex", 0);
    m_program.setUniformValue("u_tex", 1);
    m_program.setUniformValue("v_tex", 2);

    u_pos = m_program.uniformLocation("draw_pos");

    m_vao.create();
}

void YUVGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_vao.bind();

    QMatrix4x4 m;
    m.ortho(0, width(), height(), 0, 0.0, 100.0f);

    m_program.setUniformValue("u_pm", m);

    glUniform4f(u_pos, 0, 0, width(), height());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, y_tex);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, u_tex);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, v_tex);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}


void YUVGLWidget::setYPixels(uint8_t* pixels, int stride)
{
    bindPixelTexture(y_tex, YTexture, pixels, stride);
}

void YUVGLWidget::setUPixels(uint8_t* pixels, int stride)
{
    bindPixelTexture(u_tex, UTexture, pixels, stride);
}

void YUVGLWidget::setVPixels(uint8_t* pixels, int stride)
{
    bindPixelTexture(v_tex, VTexture, pixels, stride);
}


void YUVGLWidget::initializeTextures()
{
    //TODO: use FBO?

    // Setup textures
    glGenTextures(1, &y_tex);
    glBindTexture(GL_TEXTURE_2D, y_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, m_frameWidth, m_frameHeight, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);

    glGenTextures(1, &u_tex);
    glBindTexture(GL_TEXTURE_2D, u_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, m_frameWidth/2, m_frameHeight/2, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);

    glGenTextures(1, &v_tex);
    glBindTexture(GL_TEXTURE_2D, v_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, m_frameWidth/2, m_frameHeight/2, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);
}

void YUVGLWidget::bindPixelTexture(GLuint texture, YUVGLWidget::YUVTextureType textureType, uint8_t* pixels, int stride)
{
    if (!pixels)
        return;

    unsigned int const width = textureType == YTexture ? m_frameWidth : m_frameWidth/2;
    unsigned int const height = textureType == YTexture ? m_frameHeight : m_frameHeight/2;

    makeCurrent();
    glBindTexture(GL_TEXTURE_2D, texture);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, stride);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RED, GL_UNSIGNED_BYTE, pixels);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, m_frameWidth/2, m_frameHeight/2, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);
    doneCurrent();
}
