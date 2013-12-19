#ifndef DW_RAYTRACER_GUI_WINDOW_H
#define DW_RAYTRACER_GUI_WINDOW_H

#include <QMainWindow>
#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QLayout>
#include <QScrollArea>
#include <QDockWidget>
#include "Raytracer.h"
#include "gui/CanvasWidget.h"

namespace raytracer { namespace gui {

static const unsigned int WINDOW_WIDTH = 1200;
static const unsigned int WINDOW_HEIGHT = 900;
static const unsigned int INITIAL_CANVAS_WIDTH = 1000;
static const unsigned int INITIAL_CANVAS_HEIGHT = 1000;

class RaytracerWindow : public QMainWindow
{

	Q_OBJECT

public:
	RaytracerWindow(Raytracer* renderer);
	virtual ~RaytracerWindow();

	/* Called when the window is closed. */
	virtual void closeEvent(QCloseEvent* event);
	
	/* Widget hierarchy is made public so the controller 
	 * can easily access the widgets and connect their
	 * signals to controller slots. */
	QMenu* fileMenu;
		QAction* quitAction;
		QAction* saveAction;
	QScrollArea* canvasScrollArea;
		CanvasWidget* canvasWidget;
	QDockWidget* toolboxDock;
		QWidget* toolboxWidget;
		QBoxLayout* toolboxLayout;
			QGroupBox* raytracerSettings;
				QBoxLayout* raytracerSettingsLayout;
					QBoxLayout* rayRowOneLayout;
						QLabel* sampMethodLabel;
						QComboBox* sampMethod;
					QBoxLayout* rayRowTwoLayout;
						QLabel* numSamplesLabel;
						QSpinBox* numSamples;
					QBoxLayout* rayRowThreeLayout;
						QLabel* sizeLabel;
						QSpinBox* widthBox;
						QLabel* xLabel;
						QSpinBox* heightBox;
			QGroupBox* effectsSettings;
				QBoxLayout* effectsSettingsLayout;
					QCheckBox* localIlluminationSwitch;
					QCheckBox* reflectRefractSwitch;
					QCheckBox* shadowsSwitch;
			QGroupBox* sceneSettings;
				QBoxLayout* sceneSettingsLayout;
					QCheckBox* lightOneSwitch;
					QCheckBox* lightTwoSwitch;
					QBoxLayout* sceneRowOneLayout;
						QLabel* terrainLabel;
						QComboBox* terrainHeightmap;
					QBoxLayout* sceneRowTwoLayout;
						QLabel* viewpointLabel;
						QComboBox* viewpoint;
			QGroupBox* geometricOptSettings;
				QBoxLayout* geometricOptSettingsLayout;
					QCheckBox* useOctree;
					QCheckBox* showOctree;
			QPushButton* renderButton;
	
signals:
	/* Signal emitted when the window is closed. */
	void closed();
	
private:
	Raytracer* renderer;

};

} }

#endif
