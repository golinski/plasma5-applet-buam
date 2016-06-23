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

#include "buam.h"
#include <QByteArray>
#include <QTextStream>
#include <QNetworkRequest>
#include <QDebug>
#include <QUrlQuery>
#include <algorithm>

void Buam::updateBookList() {
    if (peselNumber.length() == 0 || cardNumber.length() == 0) {
        return;
    }
    connect(&accessManager, &QNetworkAccessManager::finished,
            this, &Buam::startPageDownloaded);

    QNetworkRequest request(QUrl("http://uam-hip.pfsl.poznan.pl/ipac20/ipac.jsp?profile="));
    accessManager.get(request);
}

Buam::Buam(QObject *parent) : QObject(parent) {
    //updateBookList();
}

void Buam::startPageDownloaded(QNetworkReply* reply) {
    //cookies = qvariant_cast<QList<QNetworkCookie>>(reply->header(QNetworkRequest::SetCookieHeader));
    cookies = reply->header(QNetworkRequest::SetCookieHeader);
    //qInfo() << qvariant_cast<QList<QNetworkCookie>>(cookies);
    reply->deleteLater();
    accessManager.disconnect();

    connect(&accessManager, &QNetworkAccessManager::finished,
            this,           &Buam::loginFormDownloaded);
    QNetworkRequest request(QUrl("http://uam-hip.pfsl.poznan.pl/ipac20/ipac.jsp?profile=bu-uam&menu=account"));
    request.setHeader(QNetworkRequest::SetCookieHeader, cookies);
    accessManager.get(request);
}

void Buam::loginFormDownloaded(QNetworkReply* reply) {
    QUrlQuery postData;

    QTextStream ts(reply->readAll());
    QString s;

    QRegExp r(R"__(<input xmlns:sstrings="http://www.dynix.com/2001/sstrings" xmlns:URLEncoder="/com.dynix.util.IpacURLEncoder" type="hidden" name="([^"]*)" value="([^"]*)">)__");
    while(ts.readLineInto(&s)) {
        int pos = 0;
        while ((pos = r.indexIn(s, pos)) != -1) {
            //qInfo() << r.cap(1) << " " << r.cap(2);
            postData.addQueryItem(r.cap(1), r.cap(2));
            pos += r.matchedLength();
        }
    }

    reply->deleteLater();
    accessManager.disconnect();

    //postData.addQueryItem("sec1", "156747");
    //postData.addQueryItem("sec2", "85020407233");
    postData.addQueryItem("sec1", cardNumber);
    postData.addQueryItem("sec2", peselNumber);
    QNetworkRequest request(QUrl("http://uam-hip.pfsl.poznan.pl/ipac20/ipac.jsp"));
    request.setHeader(QNetworkRequest::CookieHeader, cookies);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    connect(&accessManager, &QNetworkAccessManager::finished,
            this,           &Buam::afterLoginDownloaded);
    accessManager.post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
}

void Buam::afterLoginDownloaded(QNetworkReply* reply) {
    QTextStream ts(reply->readAll());
    QString s;

    QRegExp r(R"__(<a class="navBarAnchor" title="Wypo.*yczenia" alt="Wypo.*yczenia" href="([^"]*)">Wypo.*yczenia</a>)__");
    while (ts.readLineInto(&s)) {
        if (r.indexIn(s) != -1) {
            //qInfo() << r.cap(1);
            s = r.cap(1);
            break;
        }
    }

    reply->deleteLater();
    accessManager.disconnect();

    s.replace(QString("&amp;"), QString("&"));
    QNetworkRequest request(QUrl::fromEncoded(s.toLatin1()));
    //qInfo() << request.url().url();
    request.setHeader(QNetworkRequest::CookieHeader, cookies);
    connect(&accessManager, &QNetworkAccessManager::finished,
            this,           &Buam::bookTableDownloaded);
    request.setHeader(QNetworkRequest::SetCookieHeader, cookies);
    accessManager.get(request);
}

void Buam::bookTableDownloaded(QNetworkReply* reply) {
    //qDeleteAll(booksList);
    booksList.clear();

    QTextStream ts(reply->readAll());
    QString s;
    QRegExp bookTitle(R"__(<a class="mediumBoldAnchor" href="([^"]*)">([^<]*).</a>)__");
    QRegExp bookDate(R"__(<a class="normalBlackFont2">(../../....)</a>)__");
    while(ts.readLineInto(&s)) {
        int pos = 0;
        while ((pos = bookTitle.indexIn(s, pos)) != -1) {
            //qInfo() << bookTitle.cap(1) << "\n  " << bookTitle.cap(2);
            Book book;

            book.link = bookTitle.cap(1);
            book.link.replace(QString("&amp;"), QString("&"));
            book.title = bookTitle.cap(2);

            pos += bookTitle.matchedLength();
            pos = bookDate.indexIn(s, pos);
            pos += bookDate.matchedLength();
            pos = bookDate.indexIn(s, pos);
            pos += bookDate.matchedLength();
            QDateTime dueDate = QDateTime::fromString(bookDate.cap(1), QString("dd/MM/yyyy"));
            book.dueDate = dueDate;

            qint64 diff = QDateTime::currentDateTime().daysTo(dueDate);
            book.diff = diff;
            //qInfo() << "    " << diff;
            booksList.append(book);
        }
    }
    //std::sort(booksList.begin(), booksList.end(), [](QObject *a, QObject *b) { return static_cast<Book*>(a)->diff < static_cast<Book*>(b)->diff; });
    std::sort(booksList.begin(), booksList.end(), [](const Book &a, const Book &b) { return a.diff < b.diff; });
    emit booksDownloaded();
    //for(auto i: booksList) {
        //Book *book = static_cast<Book*>(i);
        //qInfo() << book->title << "\ni " << book->link << "\ni  " << book->dueDate << "\ni    " << book->diff;
        //Book &book = i;
        //qInfo() << book.title << "\ni " << book.link << "\ni  " << book.dueDate << "\ni    " << book.diff;
    //}
}
