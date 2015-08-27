#ifndef SELECTION_H
#define SELECTION_H

#include "yaml-cpp/yaml.h"
#include "SelectionType.h"
#include <map>

#include <math/Point.h>
#include <stdint.h>

class Selection {
    private:
        std::map<int,Point> offsets_;
        int currentFrame_;
    protected:
        Point getOffset(int frame) {
            if(offsets_.find(frame) != offsets_.end())
                return offsets_[frame];
            return Point(0,0);
        }
        Point getOffset() {
            return getOffset(currentFrame_);
        }
        bool hasOffsets() {
            return !offsets_.empty();
        }
    public:
        virtual void Serialize(YAML::Emitter&) const = 0;
        virtual void Deserialize(const YAML::Node&) = 0;
        virtual SelectionType getSelectionType() = 0;
        virtual std::string getName() = 0;
        virtual std::vector<Point> getEnclosedPoints() = 0;
        virtual std::vector<Point> getEnclosedPoints(int frame) {
            if(!hasOffsets()) return getEnclosedPoints();
            std::vector<Point> originals = getEnclosedPoints();
            std::vector<Point> offsets;
            Point offset = getOffset(frame);
            uint16_t count = originals.size();
            for(uint16_t i = 0; i < count; i++)
                offsets.push_back(originals[i] + offset);
            return offsets;
        }
        virtual bool enclosesPoint(int,int) = 0;
        virtual bool enclosesPoint(int frame, int x, int y) {
            if(!hasOffsets()) return enclosesPoint(x,y);
            Point offset = getOffset(frame);
            return enclosesPoint(x - offset.x, y - offset.y);
        }
        virtual Selection* copy() = 0;
        virtual Point getCenter() = 0;

        void setOffset(int frame, Point offset) {
            offsets_[frame] = offset;
        }
        void setCurrentFrame(int frame) {
            currentFrame_ = frame;
        }
        void clearOffsets() {
            offsets_.clear();
        }
};

YAML::Emitter& operator<< (YAML::Emitter& out, const Selection& selection);
void operator>> (const YAML::Node& node, Selection& selection);

#endif
