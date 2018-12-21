#include "inotifier.h"
#include <errno.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <libgen.h>
#include <string.h>
#include <iostream>

#include <QFileInfo>
#include <QDir>

#include <QStringList>

using namespace std;

INotifier::INotifier()
    : m_fd(-1)
{ }

INotifier::~INotifier()
{
    if (m_fd >= 0)
        close(m_fd);
}

bool
INotifier::monitor(const QStringList& files)
{
    Q_ASSERT(m_fd == -1);

    m_fd = inotify_init();
    Q_ASSERT(m_fd > 0);

    for (const QString& f:files) {
        int ret = inotify_add_watch(m_fd, f.toStdString().c_str(), IN_CLOSE_WRITE);
        if (ret == -1) {
            cerr << "Unable to monitor file " << f.toStdString() << ": "
                 << strerror(errno) << " (" << errno << ")" << endl;
            return false;
        }
        wd2Path[ret] = f;
    }


    return true;
}

void
INotifier::run()
{
    Q_ASSERT(m_fd != -1);

    /* Emit initial file change so we load the file for the first time */
    //    emit fileChange(m_filename);
    char * p;
    char buf[BUF_LEN] __attribute__((aligned(8)));

    ssize_t numRead;

    for (;;) { /* Read events forever */
        numRead = read(m_fd, buf, BUF_LEN);
        if (numRead == 0) {
            qDebug() << "read() from inotify fd returned 0!";
            Q_ASSERT(numRead == 0);
        }
        if (numRead == -1) {
            qDebug() << "read";
            Q_ASSERT(numRead == -1);
        }
        qDebug() << "Read %ld bytes from inotify fd\n" << numRead;

        /* Process all of the events in buffer returned by read() */

        for (p = buf; p < buf + numRead;) {
            struct inotify_event * event = reinterpret_cast<inotify_event *>(p);

            emit fileChange(wd2Path[event->wd] + "/" + event->name);

            p += sizeof(struct inotify_event) + event->len;
        }
    }
} // INotifier::run

void
INotifier::quit()
{
    this->terminate();
    this->wait();
}
