#pragma once

#include <QThread>
#include <linux/limits.h>
#include <qdebug.h>

#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

class INotifier : public QThread
{
    Q_OBJECT

public:
    INotifier();
    ~INotifier();

    bool monitor(const QStringList &files);

public slots:
    void quit();

signals:
    void fileChange(const QString &filename);

private:
    int m_fd;
    QString m_filename;
    QMap<int, QString> wd2Path;
    virtual void run();
};
