#ifndef GRAPHICSTYPES_H
#define GRAPHICSTYPES_H

#include <QObject>

namespace Graphics
{
    Q_NAMESPACE

    enum class QualityLevel
    {
        Low = 0,
        Medium,
        High,
        Ultra
    };

    Q_ENUM_NS(QualityLevel)
}

#endif
