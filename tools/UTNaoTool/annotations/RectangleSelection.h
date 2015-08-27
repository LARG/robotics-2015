#ifndef RECTANGLE_SELECTION_H
#define RECTANGLE_SELECTION_H

#include "Selection.h"
#include <QWidget>
#include <vector>

class RectangleSelection : public Selection {
    private:
        Point topleft, bottomright;
        void init(Point tl, Point br) {
            if(tl.x < br.x) {
                topleft.x = tl.x;
                bottomright.x = br.x;
            }
            else {
                topleft.x = br.x;
                bottomright.x = tl.x;
            }
            if(tl.y < br.y){
                topleft.y = tl.y;
                bottomright.y = br.y;
            }
            else {
                topleft.y = br.y;
                bottomright.y = tl.y;
            }
        }
    public:
        RectangleSelection(){}
        RectangleSelection(Point tl, Point br){
            init(tl,br);
        }
        RectangleSelection(QPoint qtl, QPoint qbr){
            Point tl(qtl.x(), qtl.y());
            Point br(qbr.x(), qbr.y());
            init(tl,br);
        }
        Point getTopLeft() {
            return topleft + getOffset();
        }
        Point getBottomRight() {
            return bottomright + getOffset();
        }
        virtual void Serialize(YAML::Emitter& emitter) const {
            emitter << YAML::BeginMap;
            emitter << YAML::Key << "type" << YAML::Value << "Rectangle";
            emitter << YAML::Key << "top" << YAML::Value << topleft.y;
            emitter << YAML::Key << "left" << YAML::Value << topleft.x;
            emitter << YAML::Key << "bottom" << YAML::Value << bottomright.y;
            emitter << YAML::Key << "right" << YAML::Value << bottomright.x;
            emitter << YAML::EndMap;
        }
        virtual void Deserialize(const YAML::Node& node) {
            int top, left, right, bottom;
            node["top"] >> top;
            node["left"] >> left;
            node["right"] >> right;
            node["bottom"] >> bottom;
            topleft = Point(left, top);
            bottomright = Point(right,bottom);
        }
        virtual SelectionType getSelectionType(){
            return RectangleSelectionType;
        }
        virtual std::string getName(){
            return "Rectangle";
        }
        virtual std::vector<Point> getEnclosedPoints() {
            std::vector<Point> points;
            for(int i = topleft.x; i <= bottomright.x; i++) {
                for(int j = topleft.y; j <= bottomright.y; j++) {
                    Point p(i,j);
                    points.push_back(p);
                }
            }
            return points;
        }
        virtual bool enclosesPoint(int x, int y){
            if(x >= topleft.x && y >= topleft.y && x < bottomright.x && y < bottomright.y)
                return true;
            return false;
        }
        virtual Selection* copy() {
            RectangleSelection* r = new RectangleSelection();
            r->topleft = this->topleft;
            r->bottomright = this->bottomright;
            return r;
        }
        virtual Point getCenter() {
            return Point( (this->bottomright.x - this->topleft.x) / 2, (this->bottomright.y - this->topleft.y) / 2 );
        }
};

#endif
