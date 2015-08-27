#ifndef ANNOTATION_H
#define ANNOTATION_H

#include <vector>
#include <map>
#include "Selection.h"
#include "PolygonSelection.h"
#include "EllipseSelection.h"
#include "RectangleSelection.h"
#include "BaseAnnotation.h"
#include <vision/enums/Colors.h>
#include <math/Point.h>
#include <common/RobotInfo.h>
#include <algorithm>

class Annotation : public BaseAnnotation {
    private:
        std::vector<Selection*> selections_;
        std::string name_;
        int minFrame_, maxFrame_;
        Color color_;
        Camera::Type camera_;
        bool isSample_;
        std::map<int,Point> centerPoints_;
    public:
        Annotation();
        Annotation(std::string);
        void addSelection(Selection*);
        void removeSelection(Selection*);
        std::string getName();
        void setName(std::string);
        const std::vector<Selection*> getSelections() const;
        bool isInFrame(int);
        void setMaxFrame(int);
        void setMinFrame(int);
        int getMaxFrame();
        int getMinFrame();
        Color getColor();
        void setColor(Color);
        Camera::Type getCamera();
        void setCamera(Camera::Type);
        bool isSample();
        void setSample(bool);
        std::vector<Point> getEnclosedPoints();
        std::vector<Point> getEnclosedPoints(int);
        bool enclosesPoint(int,int);
        bool enclosesPoint(int,int,int);

        void serialize(YAML::Emitter& emitter) const;
        void deserialize(const YAML::Node& node);

        Annotation* copy();

        void setCenterPoint(int,int,int);
        Point getCenter();
        void updateSelectionOffsets();
        void setCurrentFrame(int);
        void clearCenterPoints();
        void remapCenterPoints(int);
};

#endif
