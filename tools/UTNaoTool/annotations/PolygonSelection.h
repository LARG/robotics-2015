#ifndef POLYGON_SELECTION_H
#define POLYGON_SELECTION_H

#include "Selection.h"
#include <QWidget>
#include <vector>
#include <math/Polygon.h>

class PolygonSelection : public Selection {
    private:
        Polygon* polygon_;
        void getPolygon() {
            int count = vertices.size();
            std::vector<Point> points;
            for(int i=0; i < count; i++)
                points.push_back(Point(vertices[i].x,vertices[i].y));
            polygon_ = new Polygon(points);
        }
        std::vector<Point> vertices;
    public:
        PolygonSelection() : polygon_(0) {};
        PolygonSelection(std::vector<Point> v) : polygon_(0) {
            vertices = std::vector<Point>(v);
        }
        PolygonSelection(std::vector<QPoint> v) : polygon_(0) {
            for(uint16_t i = 0; i < v.size(); i++)
                vertices.push_back(Point(v[i].x(), v[i].y()));
        }
        ~PolygonSelection() {
            if(polygon_) delete polygon_;
        }
        std::vector<Point> getVertices() {
            uint16_t count = vertices.size();
            std::vector<Point> offsetVertices;
            for(uint16_t i = 0; i < count; i++) {
                offsetVertices.push_back(vertices[i] + getOffset());
            }
            return offsetVertices;
        }
        void addVertex(Point p) {
            vertices.push_back(p);
        }
        virtual void Serialize(YAML::Emitter& emitter) const {
            emitter << YAML::BeginMap;
            emitter << YAML::Key << "type" << YAML::Value << "Polygon";
            emitter << YAML::Key << "vertices" << YAML::Value;

            emitter << YAML::BeginSeq;
            for(uint16_t i = 0; i < vertices.size(); i++) {
                emitter << YAML::BeginMap;
                emitter << YAML::Key << "x" << YAML::Value << vertices[i].x;
                emitter << YAML::Key << "y" << YAML::Value << vertices[i].y;
                emitter << YAML::EndMap;
            }
            emitter << YAML::EndSeq;
            emitter << YAML::EndMap;
        }
        virtual void Deserialize(const YAML::Node& node) {
            const YAML::Node& v = node["vertices"];
            for(uint16_t i = 0; i < v.size(); i++){
                int x, y;
                const YAML::Node& vertex = v[i];
                vertex["x"] >> x;
                vertex["y"] >> y;
                Point point(x,y);
                vertices.push_back(point);
            }
        }
        virtual SelectionType getSelectionType(){
            return PolygonSelectionType;
        }
        virtual std::string getName(){
            return "Polygon";
        }
        virtual std::vector<Point> getEnclosedPoints() {
            if(!polygon_) getPolygon();
            return polygon_->enclosedPoints();
        }
        virtual bool enclosesPoint(int x, int y){
            if(!polygon_) getPolygon();
            return polygon_->enclosesPoint(x,y);
        }
        virtual Selection* copy() {
            PolygonSelection* p = new PolygonSelection();
            int count = vertices.size();
            for(int i = 0; i < count; i++)
                p->vertices.push_back(vertices[i]);
            return p;
        }
        virtual Point getCenter() {
            Point average(0,0);
            for(uint16_t i = 0; i < vertices.size(); i++) {
                average.x += vertices[i].x;
                average.y += vertices[i].y;
            }
            average.x /= vertices.size();
            average.y /= vertices.size();
            return average;
        }
};

#endif
