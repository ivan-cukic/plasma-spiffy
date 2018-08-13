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

#ifndef MIMEDATAITERATOR_H
#define MIMEDATAITERATOR_H


class MimeDataIterator {
    using host_iterator = QVariantList::const_iterator;

public:
    MimeDataIterator(const host_iterator& it)
    : it(it)
    {
    }

    using iterator_category = std::forward_iterator_tag;
    using difference_type = host_iterator::difference_type;
    using value_type = const std::pair<QString, QString>;
    using pointer = value_type*;
    using reference = value_type&;

    bool operator!=(const MimeDataIterator& other) const
    {
        return it != other.it;
    }

    bool operator==(const MimeDataIterator& other) const
    {
        return it == other.it;
    }

    value_type operator* () const
    {
        const auto& list = it->toList();
        return std::make_pair(
                list[0].toString(),
                list[1].toString());

    }

    auto operator++()
    {
        ++it;
        return *this;
    }

private:
    host_iterator it;
};

class MimeData {
public:
    MimeData(const QVariantList &data)
        : data(data)
    {
    }

    auto begin() const
    {
        return MimeDataIterator(data.begin());
    }

    auto end() const
    {
        return MimeDataIterator(data.end());
    }


private:
    const QVariantList &data;
};


#endif // include guard

