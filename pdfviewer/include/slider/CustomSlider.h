#include <QtWidgets/QSlider>

class CustomSlider : public QSlider {
 public:
  explicit CustomSlider(QWidget* parent = nullptr);
  ~CustomSlider();

 protected:
  void mousePressEvent(QMouseEvent* event);
};
