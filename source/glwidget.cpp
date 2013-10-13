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
	current_pitch = 3.1415f*0.25f;
	current_yaw = 3.1415f*0.25f;
	current_zoom = 2.0;

	render_timer = new QTime();
	render_timer->start();
}


////////////////////////////////////////////////////////////////////////////////
/// @brief
////////////////////////////////////////////////////////////////////////////////
GLWidget::~GLWidget() {
}


////////////////////////////////////////////////////////////////////////////////
/// @brief
////////////////////////////////////////////////////////////////////////////////
QGLShaderProgram* GLWidget::compileShader(const QString& name) {
	if (!QGLShaderProgram::hasOpenGLShaderPrograms(context())) {
		return 0;
	}

	QGLShader fragment(QGLShader::Fragment);
	QGLShader vertex(QGLShader::Vertex);

	if (!fragment.compileSourceFile("../shaders/" + name + ".frag")) {
	//if (!fragment.compileSourceFile(":/" + name + ".frag")) {
		QMessageBox::warning(0, tr("Exhaust Editor"),tr("Shader error in [%1.frag]:\n%2.").arg(name).arg(fragment.log()));
		return 0;
	}
	if (!vertex.compileSourceFile("../shaders/" + name + ".vert")) {
	//if (!vertex.compileSourceFile(":/" + name + ".vert")) {
		QMessageBox::warning(0, tr("Exhaust Editor"),tr("Shader error in [%1.vert]:\n%2.").arg(name).arg(fragment.log()));
		return 0;
	}

	QGLShaderProgram* shader = new QGLShaderProgram(context());
	shader->addShader(&fragment);
	shader->addShader(&vertex);
	shader->link();
	return shader;	
}


////////////////////////////////////////////////////////////////////////////////
/// @brief
////////////////////////////////////////////////////////////////////////////////
void GLWidget::initializeGL() {
	shader_exhaust = compileShader("exhaust");
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
	//Setup viewport
	float aspect = ((float)width())/((float)height());
	glViewport(0,0,width(),height());

	//Setup camera
	QVector3D camera_pos = QVector3D(
		current_zoom*4.0*cos(current_yaw)*cos(current_pitch),
		current_zoom*4.0*sin(current_yaw)*cos(current_pitch),
		current_zoom*4.0*sin(current_pitch));
	QMatrix4x4 projection_matrix;
	projection_matrix.perspective(45.0f,aspect,1.0f,500.0f);
	QMatrix4x4 view_matrix;
	view_matrix.lookAt(camera_pos,QVector3D(0,0,0),QVector3D(0,0,1));

	//Load matrices
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(projection_matrix.data());
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(view_matrix.data());

	//Clear background
	glClearColor(0.00f,0.00f,0.00f,1.00f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	/*fw_render_get_camera(camPos,camUp);
	glUseProgram(exhaust_program);
	glUniform3f(glGetUniformLocation(exhaust_program,"v_cameraPos"), (float)camPos[0], (float)camPos[1], (float)camPos[2]);
	glUniform1f(glGetUniformLocation(exhaust_program,"time"), EVDS_Thread_GetTime());*/

	glEnable(GL_CULL_FACE);
	if (shader_exhaust) {
		shader_exhaust->bind();
		shader_exhaust->setUniformValue("time",(float)(render_timer->elapsed()/1000.0));
		shader_exhaust->setUniformValue("v_cameraPos",camera_pos);
	}

	//Draw rocket engine exhaust
	float dX  = 30.0f;
	float dYZ = 1.0f;
	glBegin(GL_QUADS);
		glColor3f(0.0f,1.0f,0.0f);
		glVertex3f( dX  , dYZ ,-dYZ );
		glVertex3f(-0.0f, dYZ ,-dYZ );
		glVertex3f(-0.0f, dYZ , dYZ );
		glVertex3f( dX  , dYZ , dYZ );
		glColor3f(1.0f,0.5f,0.0f);
		glVertex3f( dX  ,-dYZ , dYZ );
		glVertex3f(-0.0f,-dYZ , dYZ );
		glVertex3f(-0.0f,-dYZ ,-dYZ );
		glVertex3f( dX  ,-dYZ ,-dYZ );
		glColor3f(1.0f,0.0f,0.0f);
		glVertex3f( dX  , dYZ , dYZ );
		glVertex3f(-0.0f, dYZ , dYZ );
		glVertex3f(-0.0f,-dYZ , dYZ );
		glVertex3f( dX  ,-dYZ , dYZ );
		glColor3f(1.0f,1.0f,0.0f);
		glVertex3f( dX  ,-dYZ ,-dYZ );
		glVertex3f(-0.0f,-dYZ ,-dYZ );
		glVertex3f(-0.0f, dYZ ,-dYZ );
		glVertex3f( dX  , dYZ ,-dYZ );
		glColor3f(0.0f,0.0f,1.0f);
		glVertex3f(-0.0f, dYZ , dYZ );
		glVertex3f(-0.0f, dYZ ,-dYZ );
		glVertex3f(-0.0f,-dYZ ,-dYZ );
		glVertex3f(-0.0f,-dYZ , dYZ );
		glColor3f(1.0f,0.0f,1.0f);
		glVertex3f( dX  , dYZ ,-dYZ );
		glVertex3f( dX  , dYZ , dYZ );
		glVertex3f( dX  ,-dYZ , dYZ );
		glVertex3f( dX  ,-dYZ ,-dYZ );
	glEnd();
	glPopMatrix();

	if (shader_exhaust) {
		shader_exhaust->release();
	}
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

	if (e->buttons() & Qt::LeftButton) {
		current_pitch -= dy*0.0025f;
		current_yaw += dx*0.0025f;

		updateGL();
	}
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