#ifndef CLASSIFICATION_WIDGET_H
#define CLASSIFICATION_WIDGET_H

#include <QtGui>
#include <QWidget>

#include <vision/VisionModule.h>
#include <vision/ColorTableMethods.h>
#include <memory/Log.h>

#include "annotations/Annotation.h"
#include "ui_ClassificationWidget.h"

class ClassificationWidget : public QWidget, public Ui_UTClassificationWidget {
    Q_OBJECT
    private:
        QString strCol[NUM_COLORS];
        Camera::Type currentCamera_;
        ImageProcessor *topProcessor_, *bottomProcessor_;
        Log* log_;
        vector<Annotation*> annotations_;
        int maxFrames_;

        int getColorFlags();
    public:
        ClassificationWidget(QWidget*);
    public slots:
        void setUndef();
        void setGreen();
        void setWhite();
        void setOrange();
        void setGoalBlue();
        void setGoalYellow();
        void setRobotPink();
        void setRobotWhite();
        void generateColorTable();
        void handleNewLogLoaded(Log*);
        void setCurrentCamera(Camera::Type);
        void setImageProcessors(ImageProcessor*,ImageProcessor*);
        void setAnnotations(std::vector<Annotation*>);
    signals:
        void colorTableGenerated();
};

#endif
