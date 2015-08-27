#ifndef ANNOTATION_GROUP_H
#define ANNOTATION_GROUP_H

#include <common/YamlConfig.h>
#include <memory/Log.h>
#include <map>
#include <stdint.h>

class Annotation;
class LocalizationAnnotation;

class AnnotationGroup : public YamlConfig {

    private:
        std::vector<Annotation*> vannotations_;
        std::vector<LocalizationAnnotation*> lannotations_;

    public:
        AnnotationGroup();
        AnnotationGroup(std::vector<Annotation*> vannotations);
        AnnotationGroup(const AnnotationGroup& other);
        ~AnnotationGroup();
        void serialize(YAML::Emitter& emitter) const;
        void deserialize(const YAML::Node& node);
        std::vector<Annotation*> getAnnotations();
        std::vector<LocalizationAnnotation*> getLocalizationAnnotations();
        void save(Log* log);
        void save(std::string directory);
        bool load(Log* log);
        void clear();
        void mergeAnnotations(std::vector<Annotation*> annotations,int sourceMinFrame, int targetMinFrame, int targetRange);
        void deleteFromFrames(std::vector<int> frames);
        void deleteFromFrame(int frame);
        void addLocalizationAnnotation(LocalizationAnnotation* la);
};

#endif
