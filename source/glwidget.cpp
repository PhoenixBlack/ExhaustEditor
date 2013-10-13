////////////////////////////////////////////////////////////////////////////////
/// @file
////////////////////////////////////////////////////////////////////////////////
/// Copyright (C) 2012-2013, Black Phoenix
/// All rights reserved.
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions are met:
///   - Redistributions of source code must retain the above copyright
///     notice, this list of conditions and the following disclaimer.
///   - Redistributions in binary form must reproduce the above copyright
///     notice, this list of conditions and the following disclaimer in the
///     documentation and/or other materials provided with the distribution.
///   - Neither the name of the author nor the names of the contributors may
///     be used to endorse or promote products derived from this software without
///     specific prior written permission.
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
/// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
/// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
/// DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS BE LIABLE FOR ANY
/// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
/// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
/// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
/// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
/// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
/// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
////////////////////////////////////////////////////////////////////////////////
#include <QWidget>
#include <QtOpenGL>
#include <QMessageBox>
#include <math.h>

#include "main.h"
#include "glwidget.h"


////////////////////////////////////////////////////////////////////////////////
/// @brief
////////////////////////////////////////////////////////////////////////////////
GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent) {
	current_quaternion = QQuaternion();
	current_zoom = 2.0;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief
////////////////////////////////////////////////////////////////////////////////
GLWidget::~GLWidget() {
}


////////////////////////////////////////////////////////////////////////////////
/// @brief
////////////////////////////////////////////////////////////////////////////////
/*QGLShaderProgram* GLWidget::compileShader(const QString& name) {
	if (!QGLShaderProgram::hasOpenGLShaderPrograms(context())) {
		return 0;
	}

	QGLShader fragment(QGLShader::Fragment);
	QGLShader vertex(QGLShader::Vertex);

	//if (!fragment.compileSourceFile("../source/foxworks_editor/resources/" + name + ".frag")) {
	if (!fragment.compileSourceFile(":/" + name + ".frag")) {
		QMessageBox::warning(0, tr("EVDS Editor"),tr("Shader error in [%1.frag]:\n%2.").arg(name).arg(fragment.log()));
		return 0;
	}
	//if (!vertex.compileSourceFile("../source/foxworks_editor/resources/" + name + ".vert")) {
	if (!vertex.compileSourceFile(":/" + name + ".vert")) {
		QMessageBox::warning(0, tr("EVDS Editor"),tr("Shader error in [%1.vert]:\n%2.").arg(name).arg(fragment.log()));
		return 0;
	}

	QGLShaderProgram* shader = new QGLShaderProgram(context());
	shader->addShader(&fragment);
	shader->addShader(&vertex);
	shader->link();
	return shader;	
}*/


////////////////////////////////////////////////////////////////////////////////
/// @brief
////////////////////////////////////////////////////////////////////////////////
void GLWidget::initializeGL() {
	//Load shaders
	//reloadShaders();
	
	//Create framebuffer
	//fbo_outline = 0;
	//fbo_selected_outline = 0;
	//fbo_window = 0;

	// For VSYNC problem under Mac OS X
	#if defined(Q_OS_MAC)
	const GLint swapInterval = 1;
	CGLSetParameter(CGLGetCurrentContext(), kCGLCPSwapInterval, &swapInterval);
	#endif

	//m_GlView.initGl();
	//m_GlView.reframe(m_Collection.boundingBox());
}


////////////////////////////////////////////////////////////////////////////////
/// @brief
////////////////////////////////////////////////////////////////////////////////
/*void GLWidget::reloadShaders() {
	shader_object = compileShader("shader_object");
	shader_outline_object = compileShader("shader_outline_object");
	shader_outline_fbo = compileShader("shader_outline_fbo");
	shader_background = compileShader("shader_background");
	shader_fxaa = compileShader("shader_fxaa");
}*/


////////////////////////////////////////////////////////////////////////////////
/// @brief
////////////////////////////////////////////////////////////////////////////////
void GLWidget::resizeGL(int width, int height)
{
	//m_GlView.setWinGLSize(width, height);
	//if (fbo_outline) delete fbo_outline;
	//if (fbo_selected_outline) delete fbo_selected_outline;
	//if (fbo_window) delete fbo_window;
	//fbo_outline = new QGLFramebufferObject(width,height,QGLFramebufferObject::Depth,GL_TEXTURE_2D,GL_RGBA8);
	//fbo_selected_outline = new QGLFramebufferObject(width,height,QGLFramebufferObject::Depth,GL_TEXTURE_2D,GL_RGBA8);
	//fbo_window = new QGLFramebufferObject(width,height,QGLFramebufferObject::Depth,GL_TEXTURE_2D,GL_RGBA8);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief
////////////////////////////////////////////////////////////////////////////////
/*void GLWidget::drawScreenQuad() {
	//Setup correct projection-view matrix (all views)
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1,1,-1,1, 1.0f, 500.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glTranslatef(0.0, 0.0, -1.0);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);

	float aspect = ((float)width())/((float)height());
	glBegin(GL_QUADS);
		glTexCoord2f( 0.0f, 1.0f);
		glVertex2f(-1,1);
		glTexCoord2f( 1.0f, 1.0f);
		glVertex2f(1,1);
		glTexCoord2f( 1.0f, 0.0f);
		glVertex2f(1,-1);
		glTexCoord2f( 0.0f, 0.0f);
		glVertex2f(-1,-1);
	glEnd();
}*/


////////////////////////////////////////////////////////////////////////////////
/// @brief
////////////////////////////////////////////////////////////////////////////////
void GLWidget::paintGL() {
}


////////////////////////////////////////////////////////////////////////////////
/// @brief
////////////////////////////////////////////////////////////////////////////////
void GLWidget::mousePressEvent(QMouseEvent* e) {
	last_pos = e->pos();	
}


////////////////////////////////////////////////////////////////////////////////
/// @brief
////////////////////////////////////////////////////////////////////////////////
void GLWidget::mouseMoveEvent(QMouseEvent* e) {
	int dx = e->x() - last_pos.x();
	int dy = e->y() - last_pos.y();

	/*if (event->buttons() & Qt::LeftButton) {
		if (event->modifiers() & Qt::ControlModifier) {
			EVDS_QUATERNION delta_quaternion;
			EVDS_Quaternion_SetEuler(&delta_quaternion,0,0,0,EVDS_RAD(0.2f*(dx+dy)));
			EVDS_Quaternion_Multiply(&current_quaternion,&current_quaternion,&delta_quaternion);
		} else {
			EVDS_QUATERNION delta_quaternion;
			EVDS_Quaternion_SetEuler(&delta_quaternion,0,0,-EVDS_RAD(0.2f*dx),0);
			EVDS_Quaternion_Multiply(&current_quaternion,&current_quaternion,&delta_quaternion);
			EVDS_Quaternion_SetEuler(&delta_quaternion,0,EVDS_RAD(0.2f*dy),0,0);
			EVDS_Quaternion_Multiply(&current_quaternion,&current_quaternion,&delta_quaternion);
		}

		updateGL();
	}
	if (event->buttons() & Qt::RightButton) {
		float scale = 2.0 / ((float)height());

		EVDS_VECTOR delta_vector;
		EVDS_Vector_Set(&delta_vector,0,0,dx*scale*zZoom,-dy*scale*zZoom,0);
		EVDS_Vector_Rotate(&delta_vector,&delta_vector,&current_quaternion);
		EVDS_Vector_Add(&current_offset,&current_offset,&delta_vector);

		updateGL();
	}*/
	last_pos = e->pos();
}


////////////////////////////////////////////////////////////////////////////////
/// @brief
////////////////////////////////////////////////////////////////////////////////
void GLWidget::wheelEvent(QWheelEvent *e) {
    float step = (e->modifiers() & Qt::ControlModifier) ? 5.0f : 1.0f;
	current_zoom += step * e->delta() / (150*4);
	if (current_zoom < 0.1f) current_zoom = 0.1f;

	updateGL();
}