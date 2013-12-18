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
#include "Raytracer.h"
#include "gui/CanvasWidget.h"

namespace raytracer { namespace gui {

class RaytracerWindow : public QMainWindow
{

	Q_OBJECT

public:
	RaytracerWindow(Raytracer* renderer);
	virtual ~RaytracerWindow();

	/* Called when the window is closed. */
	virtual void closeEvent(QCloseEvent* event);

	/* Allows controller to connect signals produced by
	 * these widgets to event handlers. */
	QAction* getSaveAction();	
	QAction* getQuitAction();
	CanvasWidget* getCanvasWidget();
	
signals:
	/* Signal emitted when the window is closed. */
	void closed();
	
private:
	Raytracer* renderer;
	
	QMenu* fileMenu;
		QAction* quitAction;
		QAction* saveAction;
		
	CanvasWidget* canvasWidget;		
		
	QDockWidget* toolboxDock;
		QWidget* toolboxWidget;
		QLayout* toolboxLayout;
			QGroupBox* raytracerSettings;
				QLayout* rayRowOneLayout;
					QLabel* sampMethodLabel;
					QComboBox* sampMethod;
				QLayout* rayRowTwoLayout;
					QLabel* numSamplesLabel;
					QSpinBox* numSamples;
				QLayout* rayRowThreeLayout;
					QLabel* sizeLabel;
					QSpinBox* widthBox;
					QLabel* xLabel;
					QSpinBox* heightBox;
			QGroupBox* effectsSettings;
				QCheckBox* localIlluminationSwitch;
				QCheckBox* reflectRefractSwitch;
				QCheckBox* shadowsSwitch;
			QGroupBox* sceneSettings;
				QLayout* sceneRowOneLayout;
					QLabel* terrainLabel;
					QComboBox* terrainHeightmap;
				QLayout* sceneRowTwoLayout;
					QLabel* viewpointLabel;
					QComboBox* viewpoint;
			QGroupBox* geometricOptSettings;
				QCheckBox* useOctree;
				QCheckBox* showOctree;
			QPushButton* renderButton;

};

} }

#endif
