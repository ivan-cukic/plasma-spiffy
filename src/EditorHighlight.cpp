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

#include "EditorHighlight.h"

#include <QRegExp>
#include <QDebug>

EditorHighlight::EditorHighlight(QTextDocument* parent)
    : QSyntaxHighlighter(parent)
{
    qreal fontScale = 1.4;

    QRegExp tagMatcher("#[^# ]+");
    QTextCharFormat tagFormat;
    tagFormat.setBackground(Qt::darkCyan);
    tagFormat.setForeground(Qt::white);

    m_rules << HighlightRule { tagMatcher, tagFormat, 0 };

    auto font = tagFormat.font();
    font.setBold(true);

    qDebug() << "font" << font;

    QRegExp heading3Matcher("^### .*$");
    QTextCharFormat heading3Format;
    heading3Format.setFont(font);

    m_rules << HighlightRule { heading3Matcher, heading3Format, 4};

    QRegExp heading2Matcher("^## .*$");
    QTextCharFormat heading2Format;
    font.setPixelSize(font.pixelSize() * fontScale);
    font.setPointSize(font.pointSize() * fontScale);
    heading2Format.setFont(font);

    m_rules << HighlightRule { heading2Matcher, heading2Format, 3};

    QRegExp heading1Matcher("^# .*$");
    QTextCharFormat heading1Format;
    font.setPixelSize(font.pixelSize() * fontScale);
    font.setPointSize(font.pointSize() * fontScale);
    heading1Format.setFont(font);

    m_rules << HighlightRule { heading1Matcher, heading1Format, 2 };

}

void EditorHighlight::highlightBlock(const QString &text)
{
    for (const auto& [matcher, format, prefixSkip]: m_rules) {
        auto index = matcher.indexIn(text);
        while (index >= 0) {
            auto length = matcher.matchedLength();
            setFormat(index + prefixSkip, length - prefixSkip, format);
            index = matcher.indexIn(text, index + length);
        }
    }
}

