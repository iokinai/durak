#pragma once

#include <QLayout>
#include <QWidget>

namespace durak {

inline void clearLayout( QLayout *layout ) {
  if ( !layout )
    return;

  while ( QLayoutItem *item = layout->takeAt( 0 ) ) {
    if ( QWidget *widget = item->widget() ) {
      widget->hide();
      widget->deleteLater(); // Важно: безопасное удаление
    }
    delete item;
  }
}

} // namespace durak