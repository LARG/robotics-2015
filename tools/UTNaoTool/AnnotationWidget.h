#ifndef ANNOTATION_WIDGET_H
#define ANNOTATION_WIDGET_H

#include <QtGui>
#include <QWidget>

#include <map>
#include <vector>
#include <fstream>

#include "annotations/AnnotationGroup.h"
#include "annotations/Annotation.h"
#include "annotations/RectangleSelection.h"
#include "annotations/PolygonSelection.h"
#include "annotations/EllipseSelection.h"
#include "annotations/SelectionType.h"
#include "annotations/AnnotationAnalyzer.h"

#include "ui_AnnotationWidget.h"

#include <vision/VisionModule.h>
#include <vision/ColorTableMethods.h>
#include <math/Point.h>

class AnnotationListWidgetItem : public QListWidgetItem {
    private:
        Annotation* annotation_;
        void init(Annotation* annotation){
            annotation_ = annotation;
            QString camera(annotation->getCamera() == Camera::TOP ? "Top: " : "Bottom: ");
            setText(camera + QString::fromStdString(annotation->getName()));
        }
    public:
        AnnotationListWidgetItem(Annotation* annotation) {
            init(annotation);
        }
        AnnotationListWidgetItem(const AnnotationListWidgetItem& other) : QListWidgetItem(other) {
            init(other.annotation_);
        }
        Annotation* getAnnotation() {
            return annotation_;
        }
        void resetText(){
            init(annotation_);
        }
};

class SelectionListWidgetItem : public QListWidgetItem {
    private:
        Selection* selection_;
        void init(Selection* selection){
            selection_ = selection;
            setText(QString::fromStdString(selection->getName()));
        }
    public:
        SelectionListWidgetItem(Selection* selection) {
            init(selection);
        }
        SelectionListWidgetItem(const SelectionListWidgetItem& other) : QListWidgetItem(other) {
            init(other.selection_);
        }
        Selection* getSelection() {
            return selection_;
        }
        void resetText(){
            init(selection_);
        }
};

class AnnotationWidget : public QWidget, public Ui_UTAnnotationWidget {
    Q_OBJECT

    private:
        int maxFrames_, currentFrame_;
        Color selectedColor_;
        std::map<std::string, Annotation*> annotations_;
        Annotation *selectedAnnotation_;
        void clearSelections();
        void loadSelections(Annotation*);
        void loadChoices(Annotation*);
        bool filterAnnotation(Annotation*);
        Camera::Type selectedCamera_, currentCamera_;
        ImageProcessor* topProcessor_, *bottomProcessor_;
        AnnotationAnalyzer analyzer;
        Log* log_;
        QString colorStrings[NUM_COLORS], selectionStrings[NumSelectionTypes], cameraStrings[2];
        bool definingMovements_;

    public:
        AnnotationWidget(QWidget* parent);
        void setImageProcessors(ImageProcessor*,ImageProcessor*);
        void setMaxFrames(int);
        std::vector<Annotation*> getAnnotations();
        std::vector<Selection*> getSelections();
    public slots:
        void selected(Selection*);
        void annotationSelected();
        void selectionBoxIndexChanged(const QString&);
        void colorBoxIndexChanged(const QString&);
        void cameraBoxIndexChanged(const QString&);
        void annotationModeChanged(int state);
        void handleNewLogFrame(int);
        void setCurrentCamera(Camera::Type);
        void handleNewLogLoaded(Log*);
        void handleClick(int,int,int);
        void toggleDefiningMovements();
        void clearMovements();

        void insert();
        void update();
        void deleteAnnotation();
        void deleteSelection();
        void redrawCurrentSelections();

        void saveToFile();
    signals:
        void selectionTypeChanged(SelectionType);
        void selectionEnabled(bool);
        void setCurrentSelections(std::vector<Selection*>);
        void setCurrentAnnotations(std::vector<Annotation*>);
};

#endif
