#ifndef RICKMORTY_VIEWMODELS_VIEW_STATE_H
#define RICKMORTY_VIEWMODELS_VIEW_STATE_H

#include <QObject>
#include <QtQml/qqmlregistration.h>

namespace rickmorty::viewmodels {

class ViewState {
    Q_GADGET
    QML_ELEMENT
    QML_UNCREATABLE("ViewState is an enum container")

public:
    enum State {
        Idle,
        Loading,
        Success,
        Error
    };
    Q_ENUM(State)
};

}  // namespace rickmorty::viewmodels

#endif  // RICKMORTY_VIEWMODELS_VIEW_STATE_H
