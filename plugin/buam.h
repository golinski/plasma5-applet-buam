/* plasma5-applet-buam -- Books from your university libray
 * Copyright © 2016 Michał Goliński
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef BUAM_H
#define BUAM_H

#include <QtQml>
#include <QObject>
#include <QList>
#include <QNetworkCookie>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QCoreApplication>

class Buam;

class BuamPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri) {
        Q_ASSERT(uri == QLatin1String("org.kde.plasma.private.buam"));
        qmlRegisterType<Buam>(uri, 1, 0, "Buam");
    }
};

class Book {
public:
    Book() { }
    Book(const Book& b) {
        title = b.title;
        link = b.link;
        diff = b.diff;
        dueDate = b.dueDate;
    }
    Book(Book&& b) {
        title.swap(b.title);
        link.swap(b.link);
        diff = b.diff;
        dueDate.swap(b.dueDate);
    }
    Book& operator=(const Book& b) {
        title = b.title;
        link = b.link;
        diff = b.diff;
        dueDate = b.dueDate;
        return *this;
    }

    QString title;
    QString link;
    qint64 diff;
    QDateTime dueDate;
};

//Q_DECLARE_METATYPE(Book)
//Q_DECLARE_METATYPE(QList<Book>)

class Buam: public QObject
{
    Q_OBJECT
public:
    explicit Buam(QObject *parent = 0);
    QList<Book> booksList;
    Q_INVOKABLE QVariantList getBooksList() {
        QVariantList l;
        for (auto i: booksList) {
            QVariantMap m;
            m["title"] = i.title;
            m["link"] = i.link;
            m["diff"] = i.diff;
            m["date"] = i.dueDate;
            l.append(m);
        }
        return l;
    }

    Q_INVOKABLE void setPeselNumber(QString s) {
        peselNumber = s;
    }
    Q_INVOKABLE void setCardNumber(QString s) {
        cardNumber = s;
    }
signals:
    void booksDownloaded();

public slots:
    void updateBookList();
    void startPageDownloaded(QNetworkReply* pReply);
    void loginFormDownloaded(QNetworkReply* pReply);
    void afterLoginDownloaded(QNetworkReply* pReply);
    void bookTableDownloaded(QNetworkReply* pReply);

private:
    QNetworkAccessManager accessManager;
    QVariant cookies;
    QString peselNumber;
    QString cardNumber;
};

#endif // BUAM_H
