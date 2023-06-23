#pragma once
#include <QMediaPlayer>
#include <QWidget>
#include <QAbstractButton>
#include <QSlider>
#include <QLabel>
#include <QUrl>

class VideoPlayer : public QWidget
{
	Q_OBJECT

public:

	VideoPlayer(QWidget* parent = nullptr);
	~VideoPlayer();

	void setUrl(const QUrl &url);

public slots:

	void openFile();
	void play();

private slots:

	void mediaStateChanged(QMediaPlayer::State state);
	void positionChanged(qint64 position);
	void durationChanged(qint64 duration);
	void setPosition(int position);

private:

	QMediaPlayer* player;
	QAbstractButton* playButton;
	QSlider* positionSlider;
	QLabel* errorLabel;
};
