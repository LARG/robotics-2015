#include "Selection.h"

YAML::Emitter& operator<< (YAML::Emitter& out, const Selection& selection) {
    selection.Serialize(out);
    return out;
}

void operator>> (const YAML::Node& node, Selection& selection) {
    selection.Deserialize(node);
}
