#pragma once

#include <QWidget>

namespace Ui {

class DeckCardWidget;

}

namespace durak {

class DeckCardWidget : public QWidget {
  Q_OBJECT

  Ui::DeckCardWidget *ui;
};
} // namespace durak