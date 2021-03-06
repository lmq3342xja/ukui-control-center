#ifndef RESOLUTIONSLIDER_H
#define RESOLUTIONSLIDER_H

#include <QWidget>
#include <QSet>

#include <KF5/KScreen/kscreen/output.h>

class QSlider;
class QLabel;
class QComboBox;
class QStyledItemDelegate;

class ResolutionSlider : public QWidget
{
    Q_OBJECT

  public:
    explicit ResolutionSlider(const KScreen::OutputPtr &output, QWidget *parent = nullptr);
    ~ResolutionSlider() override;

    QSize currentResolution() const;

  Q_SIGNALS:
    void resolutionChanged(const QSize &size);

  private Q_SLOTS:
    void slotValueChanged(int);
    void slotOutputModeChanged();

  private:
    void init();

    KScreen::OutputPtr mOutput;

    QList<QSize> mModes;

    QLabel *mSmallestLabel = nullptr;
    QLabel *mBiggestLabel = nullptr;
    QLabel *mCurrentLabel = nullptr;
    QSlider *mSlider = nullptr;
    QComboBox *mComboBox = nullptr;

    QString qss;

    QStyledItemDelegate *itemDelege;
};

#endif // RESOLUTIONSLIDER_H
