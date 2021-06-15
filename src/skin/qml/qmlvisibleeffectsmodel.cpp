#include "skin/qml/qmlvisibleeffectsmodel.h"

#include <QModelIndex>

#include "effects/effectmanifest.h"
#include "effects/effectsmanager.h"

namespace mixxx {
namespace skin {
namespace qml {
namespace {
const QHash<int, QByteArray> kRoleNames = {
        {Qt::DisplayRole, "display"},
        {Qt::ToolTipRole, "tooltip"},
        {QmlVisibleEffectsModel::EffectIdRole, "effectId"},
};
}

QmlVisibleEffectsModel::QmlVisibleEffectsModel(
        std::shared_ptr<EffectsManager> pEffectsManager,
        QObject* parent)
        : QAbstractListModel(parent), m_pEffectsManager(pEffectsManager) {
    m_visibleEffectManifests = m_pEffectsManager->getVisibleEffectManifests();
    connect(m_pEffectsManager.get(),
            &EffectsManager::visibleEffectsUpdated,
            this,
            &QmlVisibleEffectsModel::slotVisibleEffectsUpdated);
}

void QmlVisibleEffectsModel::slotVisibleEffectsUpdated() {
    const QList<EffectManifestPointer> visibleEffectManifests =
            m_pEffectsManager->getVisibleEffectManifests();
    const auto firstMismatch = std::mismatch(visibleEffectManifests.begin(),
            visibleEffectManifests.end(),
            m_visibleEffectManifests.begin(),
            [](const auto a, const auto b) { return a->id() == b->id(); });
    Q_UNUSED(firstMismatch);
    qWarning() << "first mismatched elements";
}

QVariant QmlVisibleEffectsModel::data(const QModelIndex& index, int role) const {
    if (index.row() == 0) {
        switch (role) {
        case Qt::DisplayRole:
            return EffectsManager::kNoEffectString;
        case Qt::ToolTipRole:
            return tr("No effect loaded.");
        default:
            return QVariant();
        }
    }

    if (index.row() > m_visibleEffectManifests.size()) {
        return QVariant();
    }

    const EffectManifestPointer pManifest = m_visibleEffectManifests.at(index.row() - 1);
    switch (role) {
    case Qt::DisplayRole:
        return pManifest->displayName();
    case Qt::ToolTipRole:
        return pManifest->description();
    case QmlVisibleEffectsModel::EffectIdRole:
        return pManifest->id();
    default:
        return QVariant();
    }
}

int QmlVisibleEffectsModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid()) {
        return 0;
    }

    // Add +1 because we also include "no effect" in the model
    return m_visibleEffectManifests.size() + 1;
}

QHash<int, QByteArray> QmlVisibleEffectsModel::roleNames() const {
    return kRoleNames;
}

} // namespace qml
} // namespace skin
} // namespace mixxx
