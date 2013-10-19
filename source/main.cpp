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
#include <QtGui>
#include <QIcon>

#include "main.h"
#include "glwidget.h"


////////////////////////////////////////////////////////////////////////////////
/// @brief
////////////////////////////////////////////////////////////////////////////////
void fw_editor_message(QtMsgType type, const char *msg) {
	switch (type) {
		case QtDebugMsg:
			fprintf(stderr, "[ ] %s\n", msg);
			break;
		case QtWarningMsg:
			fprintf(stderr, "[!] %s\n", msg);
			break;
		case QtCriticalMsg:
			fprintf(stderr, "Critical: %s\n", msg);
//#ifdef _DEBUG
//			_asm { int 3 };
//#endif
			break;
		case QtFatalMsg:
			fprintf(stderr, "Fatal: %s\n", msg);
#ifdef _DEBUG
			_asm { int 3 };
#endif
			abort();
	}
}


////////////////////////////////////////////////////////////////////////////////
/// @brief
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[]) {
	QApplication* application = new QApplication(argc,argv);
	//Q_INIT_RESOURCE(resources);

	qInstallMsgHandler(fw_editor_message);
	MainWindow* mainWindow = new MainWindow(); //Show main window
	mainWindow->show();

	application->exec();
}


////////////////////////////////////////////////////////////////////////////////
/// @brief
////////////////////////////////////////////////////////////////////////////////
MainWindow::MainWindow() {
	//Setup form with layout
	QVBoxLayout* layout = new QVBoxLayout;
	QWidget* form = new QWidget;
	setCentralWidget(form);
	form->setLayout(layout);

	//Add settings panel
	settings_layout = new QFormLayout;
	QWidget* settings = new QWidget;
	settings->setLayout(settings_layout);
	layout->addWidget(settings);

	//Add widgets to layout
	glwidget = new GLWidget(this);
	layout->addWidget(glwidget,1);

	//Add control
	addSlider("core_ef", 4.0,	"Exponential falloff (radial)",		-5.0, 8.0);
	addSlider("core_lf", 0.0,	"Exponential falloff (length)",		-5.0, 8.0);
	addSlider("core_cef", 4.0,	"Cutoff falloff (radial)",			-5.0, 8.0);
	addSlider("core_clf", 0.0,	"Cutoff falloff (length)",			-5.0, 8.0);


	//Set default title and size
	/*QIcon foxworks_icon = QIcon();
	foxworks_icon.addFile(":/icon/foxworks.png");
	foxworks_icon.addFile(":/icon/foxworks256.png",QSize(32,32));
	foxworks_icon.addFile(":/icon/foxworks256.png",QSize(64,64));
	foxworks_icon.addFile(":/icon/foxworks256.png",QSize(128,128));
	foxworks_icon.addFile(":/icon/foxworks256.png",QSize(256,256));
	foxworks_icon.addFile(":/icon/foxworks1024.png");*/

	//setWindowIcon(foxworks_icon);
	//setWindowTitle(tr("FoxWorks Editor"));
	//setUnifiedTitleAndToolBarOnMac(true);
	resize(1024,640);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief
////////////////////////////////////////////////////////////////////////////////
void MainWindow::closeEvent(QCloseEvent *event) {
	//...
}


////////////////////////////////////////////////////////////////////////////////
/// @brief
////////////////////////////////////////////////////////////////////////////////
void MainWindow::valueChanged(int i_value) {
	glwidget->parameter[sender()->objectName()] = i_value*0.01f;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief
////////////////////////////////////////////////////////////////////////////////
void MainWindow::addSlider(QString param, double value, QString name, double min, double max) {
	QSlider* slider = new QSlider(Qt::Horizontal);
	slider->setObjectName(param);
	slider->setMinimum(min*100.0);
	slider->setMaximum(max*100.0);
	slider->setValue(value*100.0);
	connect(slider,SIGNAL(valueChanged(int)),this,SLOT(valueChanged(int)));
	settings_layout->addRow(name+":",slider);
}