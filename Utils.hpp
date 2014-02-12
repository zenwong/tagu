#ifndef UTILS_HPP
#define UTILS_HPP
#include <QPixmap>
#include <QPainter>
#include <QString>
#include <string>

static QPixmap pixmapWithText(const QPixmap &pix, std::string title) {
    QPixmap result = pix;
    QPainter painter(&result);
    //painter.setPen(QColor(Qt::red));
    painter.setFont(QFont("Arial", 12, QFont::Bold));
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);

    QRect rect(130, 190, 250, 25);

    painter.drawRect(rect);
    painter.fillRect(rect, QColor(Qt::white));
    painter.drawText(rect, Qt::AlignRight | Qt::AlignVCenter, QString::fromStdString(title));

    //painter.drawText(result.rect(), Qt::AlignBottom | Qt::AlignRight, QString::fromStdString(title));
    return result;
}

#endif // UTILS_HPP
