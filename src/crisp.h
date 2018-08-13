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

#ifndef CRISP_H
#define CRISP_H

class QQuickTextDocument;

#include <Plasma/Applet>

#include <QStandardItemModel>
#include <QScopedPointer>

class Crisp : public Plasma::Applet
{
    Q_OBJECT

    Q_PROPERTY(bool loadingInfo READ loadingInfo NOTIFY loadingInfoChanged)

    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QPixmap preview READ preview WRITE setPreview NOTIFY previewChanged)

    Q_PROPERTY(QObject* attachments READ attachments CONSTANT)


public:
    Crisp(QObject *parent, const QVariantList &args);
    ~Crisp();

    bool loadingInfo() const;

    QString title() const;
    QString description() const;
    QPixmap preview() const;

    QObject* attachments();

public Q_SLOTS:
    void createNoteFromData(const QVariantList& data);

    void registerDocument(QQuickTextDocument* document);

    void setTitle(const QString &title);
    void setDescription(const QString &title);
    void setPreview(const QPixmap& preview);

    void addAttachment(const QString &url);
    void removeAttachment(const QString &url);

Q_SIGNALS:
    void loadingInfoChanged(bool value);

    void titleChanged(const QString &title);
    void descriptionChanged(const QString &description);
    void previewChanged(const QPixmap& preview);

private:
    class Private;
    const QScopedPointer<Private> d;
};

#endif
