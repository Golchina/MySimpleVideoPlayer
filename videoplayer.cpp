#include "videoplayer.h"
#include <QtWidgets>
#include <QVideoWidget>

VideoPlayer::VideoPlayer(QWidget* parent) : QWidget(parent)
{
	player = new QMediaPlayer(this, QMediaPlayer::VideoSurface);
	QVideoWidget* videoWidget = new QVideoWidget;

	QPushButton* openButton = new QPushButton(tr("Open..."));
	connect(openButton, &QPushButton::clicked, this, &VideoPlayer::openFile);

	playButton = new QPushButton;
	playButton->setEnabled(false);
	playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

	connect(playButton, &QPushButton::clicked, this, &VideoPlayer::play);

	positionSlider = new QSlider(Qt::Horizontal);
	positionSlider->setRange(0, 0);

	connect(positionSlider, &QSlider::sliderMoved, this, &VideoPlayer::setPosition);

	QBoxLayout* controlLayout = new QHBoxLayout;
	controlLayout->setMargin(0);
	controlLayout->addWidget(openButton);
	controlLayout->addWidget(playButton);
	controlLayout->addWidget(positionSlider);

	QBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(videoWidget);
	layout->addLayout(controlLayout);

	setLayout(layout);

	player->setVideoOutput(videoWidget);
	connect(player, &QMediaPlayer::stateChanged, this, &VideoPlayer::mediaStateChanged);
	connect(player, &QMediaPlayer::positionChanged, this, &VideoPlayer::positionChanged);
	connect(player, &QMediaPlayer::durationChanged, this, &VideoPlayer::durationChanged);
}

VideoPlayer::~VideoPlayer()
{
}

void VideoPlayer::openFile()
{
	QFileDialog fileDialog(this);
	fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
	fileDialog.setWindowTitle(tr("Open Movie"));
	QStringList supportedMimeTypes = player->supportedMimeTypes();
	if (!supportedMimeTypes.isEmpty())
		fileDialog.setMimeTypeFilters(supportedMimeTypes);
	fileDialog.setDirectory(QStandardPaths::standardLocations(QStandardPaths::MoviesLocation).value(0, QDir::homePath()));
	if (fileDialog.exec() == QDialog::Accepted)	
		setUrl(fileDialog.selectedUrls().constFirst());

	/*QString selectedFile = QFileDialog::getOpenFileName(this, "Open Movie", "", "All");
	listWidget->addItem(selectedFile);
	QMediaContent file = selectedFile;*/
}

void VideoPlayer::setUrl(const QUrl &url)
{
	setWindowFilePath(url.isLocalFile() ? url.toLocalFile() : QString());
	player->setMedia(url);
	playButton->setEnabled(true);
}

void VideoPlayer::play()
{
	switch (player->state()) {
	case QMediaPlayer::PlayingState:
		player->pause();
		break;
	default:
		player->play();
		break;
	}
}

void VideoPlayer::mediaStateChanged(QMediaPlayer::State state)
{
	switch (state) {
	case QMediaPlayer::PlayingState:
		playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
		break;
	default:
		playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
		break;
	}
}

void VideoPlayer::positionChanged(qint64 position)
{
	positionSlider->setValue(position);
}

void VideoPlayer::durationChanged(qint64 duration)
{
	positionSlider->setRange(0, duration);
}

void VideoPlayer::setPosition(int position)
{
	player->setPosition(position);
}
