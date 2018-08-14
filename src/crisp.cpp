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

#include "crisp.h"

#include <QAbstractListModel>
#include <QDebug>
#include <QDropEvent>
#include <QFileInfo>
#include <QMimeData>
#include <QMimeDatabase>
#include <QModelIndex>
#include <QQuickTextDocument>
#include <QTextDocument>

#include <KFileItem>
#include <KFileMetaData/Extractor>
#include <KFileMetaData/ExtractorCollection>
#include <KFileMetaData/SimpleExtractionResult>
#include <KFileMetaData/UserMetaData>
#include <KLocalizedString>

#include <iterator>
#include <algorithm>

#include "EditorHighlight.h"
#include "AttachmentModel.h"
#include "utils/MimeDataIterator.h"

class Crisp::Private {
public:
    Crisp* const q;

    struct NoteData {
        QString title;
        QString description;

        QVector<AttachmentData> attachments;
    };


    QString title;
    QString description;
    AttachmentModel attachmentsModel;
    QMimeDatabase mimeDatabase;
    QPixmap preview;

    KFileMetaData::ExtractorCollection allExtractors;

    Private(Crisp* parent)
        : q(parent)
    {
    }

    AttachmentData extractFileData(const QUrl& url) const
    {
        AttachmentData result;

        result.url = url;

        qDebug() << "Inspecting File/URL: " << url;

        const auto file = url.toLocalFile();
        const auto mimetype = mimeDatabase.mimeTypeForFile(file).name();

        auto extractors = allExtractors.fetchExtractors(mimetype);

        qDebug() << "Extractors for " << mimetype << " - count " << extractors.size();

        if (!extractors.isEmpty()) {
            auto extractor = extractors.first();
            KFileMetaData::SimpleExtractionResult extracted(
                    file,
                    mimetype,
                    KFileMetaData::ExtractionResult::ExtractMetaData);

            extractor->extract(&extracted);

            const auto properties = extracted.properties();

            qDebug() << "Extracted properties: " << properties;

            result.title
                = properties[KFileMetaData::Property::Title].toString();
        }

        if (result.title.isEmpty()) {
            QFileInfo fileInfo(file);
            result.title = fileInfo.baseName();
        }

        if (result.icon.isEmpty()) {
            KFileItem fileItem(url, mimetype);
            result.icon  = fileItem.iconName();
        }

        return result;
    }

    QStringList splitLines(const QString& in) const
    {
        return in.split(QRegExp("(\r\n|\r|\n)"),
                          QString::SplitBehavior::SkipEmptyParts);
    }


    NoteData readNoteData(const QVariantList& mimeData) const
    {
        NoteData result;

        QStringList urls;
        QString plainText;
        QString htmlText;

        // Extracting the basic information from the dropped mime data
        for (const auto& [format, data]: MimeData(mimeData)) {
            if (format == "text/uri-list") {
                urls = splitLines(data);
                qDebug() << "Found urls: " << urls;

            } else if (format == "text/plain") {
                plainText = data;
                qDebug() << "Found plain text: " << plainText;

            } else if (format == "text/html") {
                htmlText = data;
                qDebug() << "Found html: " << htmlText;

            } else {
                qDebug() << "Ignored mime type: " << format;
            }
        }

        // If the plain text contains urls, ignore it
        if (!urls.isEmpty()) {
            auto plainTextUrls = splitLines(plainText);

            // Overdoing the cleanup
            if (plainTextUrls.size() == urls.size()) {
                plainText.clear();
                qDebug() << "Erased plain text because it contains urls";
            }
        }

        // Reading the URL data
        result.attachments.resize(urls.size());
        std::transform(std::cbegin(urls), std::cend(urls),
                       std::begin(result.attachments),
                       [this] (const QUrl url) {
                           return url.isLocalFile() ? extractFileData(url)
                                : AttachmentData{};
                       }
                    );

        // Taking the first url to be the title and description source
        if (!result.attachments.isEmpty()) {
            const auto firstAttachment = result.attachments.first();

            result.title = firstAttachment.title;
        }

        result.description = plainText;

        qDebug() << "Note data:";
        qDebug() << "Title: " << result.title;
        qDebug() << "Description: " << result.description;

        return result;
    }
};


Crisp::Crisp(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args)
    , d(new Private(this))
{
}

Crisp::~Crisp()
{
}


void Crisp::createNoteFromData(const QVariantList& mimeData)
{
    qDebug() << "-- Creating/Updating the note with new data --";

    auto data = d->readNoteData(mimeData);

    if (d->title.isEmpty()) {
        setTitle(data.title);
    }

    setDescription(
            description() + (d->description.isEmpty() ? "" : "\n\n") + data.description);

    for (const auto& attachment: data.attachments) {
        qDebug() << "Attachment: " << attachment.url;
        d->attachmentsModel.addItem(attachment);
    }
}

bool Crisp::loadingInfo() const
{
    return true;
}

void Crisp::registerDocument(QQuickTextDocument* document)
{
    new EditorHighlight(document->textDocument());
}

void Crisp::setTitle(const QString &title)
{
    if (d->title == title) return;

    d->title = title;

    emit titleChanged(d->title);
}

QString Crisp::title() const
{
    return d->title;
}

void Crisp::setDescription(const QString &description)
{
    if (d->description == description) return;

    d->description = description;

    emit descriptionChanged(d->description);
}

QString Crisp::description() const
{
    return d->description;
}

void Crisp::setPreview(const QPixmap& preview)
{
    d->preview = preview;
    previewChanged(preview);
}

QPixmap Crisp::preview() const
{
    return d->preview;
}

QObject* Crisp::attachments()
{
    return &d->attachmentsModel;
}

void Crisp::addAttachment(const QString &url)
{
    // TODO
}

void Crisp::removeAttachment(const QString &url)
{
    qDebug() << "Url to remove: " << url;
    d->attachmentsModel.removeItem(url);
}


K_EXPORT_PLASMA_APPLET_WITH_JSON(crisp, Crisp, "metadata.json")

#include "crisp.moc"
