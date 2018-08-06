/*
 *   Copyright (C) %{CURRENT_YEAR} by %{AUTHOR} <%{EMAIL}>
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef TAGGER_H
#define TAGGER_H


#include <Plasma/Applet>

class Tagger : public Plasma::Applet
{
    Q_OBJECT

    Q_PROPERTY(bool loadingInfo READ loadingInfo NOTIFY loadingInfoChanged)
    Q_PROPERTY(QString nativeText READ nativeText CONSTANT)

public:
    Tagger(QObject *parent, const QVariantList &args);
    ~Tagger();

    QString nativeText() const;
    bool loadingInfo() const;

public Q_SLOTS:
    void dropData();

Q_SIGNALS:
    void loadingInfoChanged(bool value);

private:
    QString m_nativeText;
};

#endif
