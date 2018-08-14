/*
 *   Copyright (C) 2018 Ivan Čukić <ivan.cukic(at)kde.org>
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) version 3, or any
 *   later version accepted by the membership of KDE e.V. (or its
 *   successor approved by the membership of KDE e.V.), which shall
 *   act as a proxy defined in Section 6 of version 3 of the license.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library.
 *   If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ATTACHMENTMODEL_H
#define ATTACHMENTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QVector>
#include <QUrl>
#include <QPixmap>

namespace KIO {
    class PreviewJob;
}

struct AttachmentData {
    QUrl url;
    QString title;
    QString icon;
    QPixmap preview;
};

class AttachmentModel: public QAbstractListModel {
    Q_OBJECT

    Q_PROPERTY(QObject* previewsModel READ previewsModel CONSTANT)

public:
    enum AttachmentModelRoles {
        AttachmentTitle = Qt::UserRole,
        AttachmentIcon,
        AttachmentPreview,
        AttachmentUrl
    };

    AttachmentModel();
    ~AttachmentModel();

    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex& index) const override;
    QVariant data(const QModelIndex& index, int role) const override;

    void addItem(const AttachmentData& attachment);
    void removeItem(const QString &url);

    Q_SCRIPTABLE QAbstractItemModel* previewsModel();

private:
    void removePreviewJob(const QUrl &url);
    void registerPreviewResult(const QUrl& url, const QPixmap& pixmap);

    auto findItem(const QUrl &url) const;

    QVector<AttachmentData> m_attachments;
    QList<KIO::PreviewJob*> m_previewJobs;
    QAbstractItemModel* m_previewsModel = nullptr;
};

#endif // include guard

