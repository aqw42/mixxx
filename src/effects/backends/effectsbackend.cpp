#include "effects/backends/effectsbackend.h"

namespace {
const QString backendTypeNameLV2 = QStringLiteral("LV2");
const QString backendTypeNameBuiltIn = QStringLiteral("BuiltIn");
const QString backendTypeNameUnknown = QStringLiteral("Unknown");
} // anonymous namespace

EffectBackendType EffectsBackend::backendTypeFromString(const QString& typeName) {
    if (typeName == backendTypeNameLV2) {
        return EffectBackendType::LV2;
    } else {
        return EffectBackendType::BuiltIn;
    }
}

QString EffectsBackend::backendTypeToString(EffectBackendType backendType) {
    if (backendType == EffectBackendType::BuiltIn) {
        return backendTypeNameBuiltIn;
    } else if (backendType == EffectBackendType::LV2) {
        return backendTypeNameLV2;
    } else {
        return backendTypeNameUnknown;
    }
}
