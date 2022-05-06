#ifndef ABOUTWINDOW_HPP
#define ABOUTWINDOW_HPP

#include <QWidget>

namespace Ui {
class AboutWindow;
}

class AboutWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AboutWindow(QWidget *parent = nullptr);
    ~AboutWindow();

private:
    Ui::AboutWindow *ui;
};

#endif // ABOUTWINDOW_HPP
