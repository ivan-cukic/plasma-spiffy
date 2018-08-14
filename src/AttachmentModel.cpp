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

#include "AttachmentModel.h"

#include <QUrl>
#include <QSortFilterProxyModel>
#include <QDebug>

#include <KIO/PreviewJob>


class AttachmentsWithPreviews: public QSortFilterProxyModel {
public:
    AttachmentsWithPreviews(AttachmentModel* parent)
        : QSortFilterProxyModel(parent)
    {
        setSourceModel(parent);
    }

    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override
    {
        /* return true; */
        const auto index = sourceModel()->index(source_row, 0, source_parent);
        return index.data(AttachmentModel::AttachmentPreview).isValid();
    }

};


AttachmentModel::AttachmentModel()
{
}

AttachmentModel::~AttachmentModel()
{
}

QHash<int, QByteArray> AttachmentModel::roleNames() const
{
    return {
        {AttachmentTitle,   "title"},
        {AttachmentIcon,    "icon"},
        {AttachmentPreview, "preview"},
        {AttachmentUrl,     "url"}
    };
}

int AttachmentModel::rowCount(const QModelIndex& index) const
{
    if (index.isValid()) return 0;

    return m_attachments.count();
}

QVariant AttachmentModel::data(const QModelIndex& index, int role) const
{
    const auto row = index.row();

    if (row < 0 || row >= m_attachments.count()) return {};

    const auto& attachment = m_attachments[row];

    return role == Qt::DisplayRole    ? attachment.title
         : role == Qt::DecorationRole ? attachment.icon
         : role == AttachmentTitle    ? attachment.title
         : role == AttachmentIcon     ? attachment.icon
         : role == AttachmentPreview  ? (
                 attachment.preview.isNull() ? QVariant() : attachment.preview
             )
         : role == AttachmentUrl      ? attachment.url
         : QVariant();

}

auto AttachmentModel::findItem(const QUrl &url) const
{
    return std::find_if(
            std::cbegin(m_attachments),
            std::cend(m_attachments),
            [&] (const auto &item) {
                return item.url == url;
            });
}

void AttachmentModel::removePreviewJob(const QUrl &url)
{
    Q_UNUSED(url);
    m_previewJobs.removeAll(static_cast<KIO::PreviewJob*>(sender()));
}

void AttachmentModel::registerPreviewResult(const QUrl& url, const QPixmap& pixmap)
{
    removePreviewJob(url);

    auto itemToUpdate = findItem(url);
    if (itemToUpdate == std::cend(m_attachments)) {
        return;
    }

    auto rowToUpdate = std::distance(std::cbegin(m_attachments), itemToUpdate);
    auto indexToUpdate = index(rowToUpdate);

    m_attachments[rowToUpdate].preview = pixmap;
    dataChanged(indexToUpdate, indexToUpdate, {AttachmentPreview});
}

void AttachmentModel::addItem(const AttachmentData &attachment)
{
    if (findItem(attachment.url) != std::cend(m_attachments)) {
        return;
    }

    beginInsertRows(QModelIndex(), m_attachments.count(),
                                   m_attachments.count());

    m_attachments << attachment;

    auto job = KIO::filePreview({attachment.url}, 400);
    connect(job, &KIO::PreviewJob::gotPreview,
            this, [this] (const KFileItem &file, const QPixmap &pixmap) {
                qDebug() << "Got the preview";
                registerPreviewResult(file.mostLocalUrl(), pixmap);
            });
    connect(job, &KIO::PreviewJob::failed,
            this, [this] (const KFileItem &file) {
                qDebug() << "Lost the preview";
                removePreviewJob(file.mostLocalUrl());
            });

    endInsertRows();
}

void AttachmentModel::removeItem(const QString &url)
{
    auto itemToRemove = findItem(url);
    if (itemToRemove == std::cend(m_attachments)) {
        return;
    }

    auto rowToRemove = std::distance(std::cbegin(m_attachments), itemToRemove);

    beginRemoveRows(QModelIndex(), rowToRemove, rowToRemove);
    m_attachments.remove(rowToRemove);
    endRemoveRows();
}


QAbstractItemModel* AttachmentModel::previewsModel()
{
    if (!m_previewsModel) {
        m_previewsModel = new AttachmentsWithPreviews(this);
    }
    return m_previewsModel;
}
