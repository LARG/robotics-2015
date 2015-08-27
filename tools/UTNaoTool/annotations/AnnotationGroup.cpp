#include "AnnotationGroup.h"
#include "Annotation.h"
#include "LocalizationAnnotation.h"

AnnotationGroup::AnnotationGroup(){
}

AnnotationGroup::AnnotationGroup(std::vector<Annotation*> annotations) {
  for(auto va : annotations)
    vannotations_.push_back(va);
}

AnnotationGroup::AnnotationGroup(const AnnotationGroup& other) {
  for(auto la : other.lannotations_)
    lannotations_.push_back(new LocalizationAnnotation(*la));
  for(auto va : other.vannotations_)
    vannotations_.push_back(new Annotation(*va));
}

AnnotationGroup::~AnnotationGroup() {
  // TODO: modify calls that copy pointers rather than objects so that
  // we can always delete annotations that have been added. - JM 4/16/15
  //for(auto la : lannotations_)
    //delete la;
  //for(auto va : vannotations_)
    //delete va;
}

void AnnotationGroup::serialize(YAML::Emitter& emitter) const {
  emitter << YAML::Key << "vision_annotations" << YAML::Value;
  emitter << YAML::BeginSeq;
  for(auto a : vannotations_) {
    emitter << *a;
  }
  emitter << YAML::EndSeq;

  emitter << YAML::Key << "localization_annotations" << YAML::Value;
  emitter << YAML::BeginSeq;
  for(auto a : lannotations_) {
    emitter << *a;
  }
  emitter << YAML::EndSeq;
}

void AnnotationGroup::deserialize(const YAML::Node& node){
  const auto& vnode = node["vision_annotations"];
  for(uint16_t i=0; i < vnode.size(); i++) {
    Annotation* annotation = new Annotation();
    vnode[i] >> *annotation;
    vannotations_.push_back(annotation);
  }
  const auto& lnode = node["localization_annotations"];
  for(uint16_t i=0; i < lnode.size(); i++) {
    LocalizationAnnotation* annotation = new LocalizationAnnotation();
    lnode[i] >> *annotation;
    lannotations_.push_back(annotation);
  }
}

std::vector<Annotation*> AnnotationGroup::getAnnotations() {
  return vannotations_;
}

std::vector<LocalizationAnnotation*> AnnotationGroup::getLocalizationAnnotations() {
  return lannotations_;
}

bool AnnotationGroup::load(Log* log) {
  clear();
  std::string directory = log->directory();
  std::string path = directory + "/annotations.yaml";
  return loadFromFile(path);
}

void AnnotationGroup::save(Log* log) {
  std::string directory = log->directory();
  save(directory);
}

void AnnotationGroup::save(std::string directory) {
  std::string path = directory + "/annotations.yaml";
  saveToFile(path);
}

void AnnotationGroup::clear() {
  vannotations_.clear();
  lannotations_.clear();
}

void AnnotationGroup::mergeAnnotations(std::vector<Annotation*> annotations, int sourceMinFrame, int targetMinFrame, int targetRange) {
  std::vector<Annotation*> toMerge;
  for(unsigned int i = 0; i < annotations.size(); i++) {
    Annotation* annotation = annotations[i];
    if(annotation->getMinFrame() <= sourceMinFrame + targetRange && annotation->getMaxFrame() >= sourceMinFrame)
      toMerge.push_back(annotation->copy());
  }
  for(unsigned int i = 0; i < toMerge.size(); i++) {
    Annotation* annotation = toMerge[i];

    // Remap frames
    int frameOffset = targetMinFrame - sourceMinFrame;
    int newMinFrame = std::max(sourceMinFrame, annotation->getMinFrame());
    int annotationRange = annotation->getMaxFrame() - annotation->getMinFrame();
    int range = std::min(annotationRange,targetRange);
    annotation->setMinFrame(newMinFrame + frameOffset);
    annotation->setMaxFrame(newMinFrame + frameOffset + range);
    annotation->remapCenterPoints(frameOffset);
    vannotations_.push_back(annotation);
  }
}

void AnnotationGroup::deleteFromFrames(std::vector<int> frames) {
  for(unsigned int i = 0; i < frames.size(); i++) {
    int frame = frames[i];
    deleteFromFrame(frame);
  }
}

void AnnotationGroup::deleteFromFrame(int frame) {
  for(unsigned int j = 0; j < vannotations_.size(); j++) {
    Annotation* annotation = vannotations_[j];
    if(annotation->getMaxFrame() >= frame)
      annotation->setMaxFrame(annotation->getMaxFrame() - 1);
    if(annotation->getMinFrame() >= frame)
      annotation->setMinFrame(annotation->getMinFrame() - 1);
  }
}
        
void AnnotationGroup::addLocalizationAnnotation(LocalizationAnnotation* la) {
  lannotations_.push_back(la);
}
