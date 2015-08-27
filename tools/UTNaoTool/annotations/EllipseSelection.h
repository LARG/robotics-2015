#ifndef ELLIPSE_SELECTION_H
#define ELLIPSE_SELECTION_H

#include "Selection.h"
#include <QWidget>
#include <vector>

class EllipseSelection : public Selection {
    private:
        int top_, left_, width_, height_;
        std::vector<Point> vertices;
    public:
        EllipseSelection(){}
        EllipseSelection(std::vector<Point> v) {
            vertices = std::vector<Point>(v);
        }
        EllipseSelection(std::vector<QPoint> v)  {
            for(uint16_t i = 0; i < v.size(); i++)
                vertices.push_back(Point(v[i].x(), v[i].y()));
        }
        void addVertex(Point p) {
            vertices.push_back(p);
        }
        void fixpoints() {
            top_ = vertices[0].y;
            left_ = vertices[0].x;
            width_ = 0;
            height_ = 0;
            if(vertices.size() > 1) {
                if(vertices[1].y < top_) {
                    top_ = vertices[1].y;
                    height_ = vertices[0].y - top_;
                }
                else
                    height_ = vertices[1].y - top_;
            }
            if(vertices.size() > 2){
                width_ = 2 * abs(vertices[2].x - vertices[0].x);
                left_ = vertices[0].x - width_ / 2.0;
            }
        }
        int x() {
            return left_ + getOffset().x;
        }
        int y() {
            return top_ + getOffset().y;
        }
        int width() {
            return width_;
        }
        int height() {
            return height_;
        }
        virtual void Serialize(YAML::Emitter& emitter) const {
            emitter << YAML::BeginMap;
            emitter << YAML::Key << "type" << YAML::Value << "Ellipse";
            emitter << YAML::Key << "top" << YAML::Value << top_;
            emitter << YAML::Key << "left" << YAML::Value << left_;
            emitter << YAML::Key << "width" << YAML::Value << width_;
            emitter << YAML::Key << "height" << YAML::Value << height_;
            emitter << YAML::EndMap;
        }
        virtual void Deserialize(const YAML::Node& node) {
            node["top"] >> top_;
            node["left"] >> left_;
            node["width"] >> width_;
            node["height"] >> height_;
        }
        virtual SelectionType getSelectionType(){
            return EllipseSelectionType;
        }
        virtual std::string getName(){
            return "Ellipse";
        }
        virtual std::vector<Point> getEnclosedPoints() {
            std::vector<Point> points;
            for(int i = left_; i <= left_ + width_; i++)
                for(int j = top_; j <= top_ + height_; j++)
                    if(enclosesPoint(i,j))
                        points.push_back(Point(i,j));
            return points;
        }
        virtual bool enclosesPoint(int x, int y){
            float xrad = width_ / 2.0, yrad = height_ / 2.0;
            float cx = left_ + xrad, cy = top_ + yrad;
            float value = (x - cx) * (x - cx) / (xrad * xrad) + (y - cy) * (y - cy) / (yrad * yrad);

            if(value <= 1) return true;

            return false;
        }
        virtual Selection* copy() {
            EllipseSelection* e = new EllipseSelection();
            e->top_ = this->top_;
            e->left_ = this->left_;
            e->width_ = this->width_;
            e->height_ = this->height_;
            return e;
        }
        virtual Point getCenter() {
            return Point(left_ + width_ / 2, top_ + height_ / 2);
        }
};


#endif
