#include "Annotation.h"

Annotation::Annotation(){
}

Annotation::Annotation(std::string name){
    name_ = name;
}

void Annotation::addSelection(Selection* selection){
    selections_.push_back(selection);
}

void Annotation::removeSelection(Selection* selection){
    selections_.erase(std::remove(selections_.begin(), selections_.end(), selection), selections_.end());
}

std::string Annotation::getName() {
    return name_;
}

void Annotation::setName(std::string name) {
    name_ = name;
}

const std::vector<Selection*> Annotation::getSelections() const {
    return selections_;
}

bool Annotation::isInFrame(int frame){
    return frame >= minFrame_ && frame <= maxFrame_;
}
void Annotation::setMinFrame(int frame){
    minFrame_ = frame;
}
void Annotation::setMaxFrame(int frame){
    maxFrame_ = frame;
}

int Annotation::getMinFrame() {
    return minFrame_;
}

int Annotation::getMaxFrame() {
    return maxFrame_;
}

void Annotation::setColor(Color c){
    color_ = c;
}

Color Annotation::getColor() {
    return color_;
}

Camera::Type Annotation::getCamera() {
    return camera_;
}

void Annotation::setCamera(Camera::Type camera) {
    camera_ = camera;
}

std::vector<Point> Annotation::getEnclosedPoints() {
    uint16_t count = selections_.size();
    std::vector<Point> points;
    for(uint16_t i = 0; i < count; i++) {
        Selection* s = selections_[i];
        std::vector<Point> spoints = s->getEnclosedPoints();
        uint16_t scount = spoints.size();
        for(uint16_t j=0; j< scount; j++) {
            points.push_back(spoints[j]);
        }
    }
    return points;
}

std::vector<Point> Annotation::getEnclosedPoints(int frame) {
    if(centerPoints_.empty()) return getEnclosedPoints();
    uint16_t count = selections_.size();
    std::vector<Point> points;
    for(uint16_t i = 0; i < count; i++) {
        Selection* s = selections_[i];
        std::vector<Point> spoints = s->getEnclosedPoints(frame);
        uint16_t scount = spoints.size();
        for(uint16_t j=0; j< scount; j++) {
            points.push_back(spoints[j]);
        }
    }
    return points;
}

bool Annotation::enclosesPoint(int x, int y){
    uint16_t count = selections_.size();
    for(uint16_t i = 0; i < count; i++)
        if(selections_[i]->enclosesPoint(x,y))
            return true;
    return false;
}

bool Annotation::enclosesPoint(int frame, int x, int y) {
    if(centerPoints_.empty()) return enclosesPoint(x,y);
    uint16_t count = selections_.size();
    for(uint16_t i = 0; i < count; i++)
        if(selections_[i]->enclosesPoint(frame,x,y))
            return true;
    return false;
}

void Annotation::serialize(YAML::Emitter& emitter) const {
    emitter << YAML::BeginMap;
    emitter << YAML::Key << "name" << YAML::Value << name_;
    emitter << YAML::Key << "minFrame" << YAML::Value << minFrame_;
    emitter << YAML::Key << "maxFrame" << YAML::Value << maxFrame_;
    emitter << YAML::Key << "color" << YAML::Value << color_;
    emitter << YAML::Key << "camera" << YAML::Value << camera_;
    emitter << YAML::Key << "isSample" << YAML::Value << isSample_;
    emitter << YAML::Key << "centerPoints" << YAML::Value;
    emitter << YAML::BeginSeq;
    for(std::map<int,Point>::const_iterator it = centerPoints_.begin(); it != centerPoints_.end(); it++) {
        int frame = (*it).first;
        Point newCenter = (*it).second;
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "frame" << YAML::Value << frame;
        emitter << YAML::Key << "x" << YAML::Value << newCenter.x;
        emitter << YAML::Key << "y" << YAML::Value << newCenter.y;
        emitter << YAML::EndMap;
    }
    emitter << YAML::EndSeq;
    emitter << YAML::Key << "selections" << YAML::Value;
    emitter << YAML::BeginSeq;
    for(uint16_t i = 0; i < selections_.size(); i++) {
        emitter << *selections_[i];
    }
    emitter << YAML::EndSeq;
    emitter << YAML::EndMap;
}

void Annotation::deserialize(const YAML::Node& node) {
    node["name"] >> name_;
    node["minFrame"] >> minFrame_;
    node["maxFrame"] >> maxFrame_;
    int color;
    node["color"] >> color;
    color_ = (Color)color;
    int camera;
    node["camera"] >> camera;
    camera_ = (Camera::Type)camera;
    if(const YAML::Node *pSample = node.FindValue("isSample"))
        *pSample >> isSample_;
    if(const YAML::Node *pCenterPoints = node.FindValue("centerPoints")) {
        const YAML::Node& centerPoints = *pCenterPoints;
        for(uint16_t i = 0; i < centerPoints.size(); i++) {
            const YAML::Node& centerPoint = centerPoints[i];
            int frame, x, y;
            centerPoint["frame"] >> frame;
            centerPoint["x"] >> x;
            centerPoint["y"] >> y;
            setCenterPoint(frame,x,y);
        }
    }
    const YAML::Node& selections = node["selections"];
    for(uint16_t i = 0; i < selections.size(); i++) {
        const YAML::Node& selection = selections[i];
        std::string type;
        selection["type"] >> type;
        if(type == "Polygon") {
            PolygonSelection* polygon = new PolygonSelection();
            selection >> *polygon;
            if(polygon->getVertices().size() == 0) continue;
            selections_.push_back(polygon);
        }
        else if (type == "Ellipse") {
            EllipseSelection* ellipse = new EllipseSelection();
            selection >> *ellipse;
            selections_.push_back(ellipse);
        }
        else if (type == "Rectangle") {
            RectangleSelection* rectangle = new RectangleSelection();
            selection >> *rectangle;
            selections_.push_back(rectangle);
        }
    }
    updateSelectionOffsets();
}

Annotation* Annotation::copy() {
    Annotation* annotation = new Annotation();
    annotation->name_ = this->name_;
    annotation->minFrame_ = this->minFrame_;
    annotation->maxFrame_ = this->maxFrame_;
    annotation->color_ = this->color_;
    annotation->camera_ = this->camera_;
    annotation->centerPoints_ = this->centerPoints_;
    annotation->isSample_ = this->isSample_;
    for(uint16_t i = 0; i < selections_.size(); i++)
        annotation->selections_.push_back(selections_[i]->copy());
    return annotation;
}

bool Annotation::isSample() {
    return isSample_;
}

void Annotation::setSample(bool value) {
    isSample_ = value;
}

void Annotation::setCenterPoint(int frame, int x, int y) {
    centerPoints_[frame] = Point(x,y);
    updateSelectionOffsets();
}

Point Annotation::getCenter() {
    Point average(0,0);
    if(selections_.size() == 0)
        return average;
    for(uint16_t i = 0; i < selections_.size(); i++) {
        Selection* selection = selections_[i];
        Point center = selection->getCenter();
        average.x += center.x;
        average.y += center.y;
    }
    average.x /= selections_.size();
    average.y /= selections_.size();
    return average;
}

void Annotation::updateSelectionOffsets() {
    Point origCenter = getCenter();
    for(uint16_t i = 0; i < selections_.size(); i++) {
        Selection* selection = selections_[i];
        for(std::map<int,Point>::iterator it = centerPoints_.begin(); it != centerPoints_.end(); it++) {
            int frame = (*it).first;
            Point newCenter = (*it).second;
            Point offset = newCenter - origCenter;
            selection->setOffset(frame, offset);
        }
    }
}

void Annotation::setCurrentFrame(int frame) {
    for(uint16_t i = 0; i < selections_.size(); i++)
        selections_[i]->setCurrentFrame(frame);
}

void Annotation::clearCenterPoints() {
    centerPoints_.clear();
    for(uint16_t i = 0; i < selections_.size(); i++) {
        Selection* selection = selections_[i];
        selection->clearOffsets();
    }
}

void Annotation::remapCenterPoints(int offset) {
    std::map<int,Point> original = centerPoints_;
    centerPoints_.clear();
    for(std::map<int,Point>::iterator it = original.begin(); it != original.end(); it++) {
        int frame = (*it).first;
        Point point = (*it).second;
        centerPoints_[frame + offset] = point;
    }
}
