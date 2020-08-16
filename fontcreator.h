#ifndef FONTCREATOR_H
#define FONTCREATOR_H

#include <map>

#include <QFont>
#include <QFontMetrics>

class FontCreator
{
private:
    struct FontParameters
    {
        int width;
        int height;
    };

    FontCreator()
    {
        const QChar base_char('W');
        QFont font(QString(""));
        for (int i = min_point_size_; i <= max_point_size_; ++i)
        {
            font.setPointSize(i);
            // fm - font metrics
            QFontMetrics fm(font);
            // fp - font parameters
            FontParameters fp{fm.horizontalAdvance(base_char), fm.height()};
            average_font_width_.insert(std::make_pair(i, fp));
        }
    }
    FontCreator(const FontCreator&);
    FontCreator& operator= (FontCreator&);

private:
    std::map<int, FontParameters> average_font_width_;
    const int min_point_size_{1};
    const int max_point_size_{50};

public:
    static FontCreator& getInstance()
    {
        static FontCreator instance;
        return instance;
    }

    QFont createFont(const int width, const int height,
                     const QString& text) const
    {
        QFont font(QString(""), average_font_width_.rbegin()->first);
        const int chars_amount = text.length();

        for (const auto& [point_size, parameters] : average_font_width_)
        {
            const int length = parameters.width * chars_amount;
            if (width < length || height < parameters.height)
            {
                font.setPointSize(point_size);
                break;
            }
        }

        return font;
    }

    static QRectF createRect(const float width, const float height,
                      const QFont& font, const QString& text)
    {
        // fm - font metrics
        const QFontMetrics fm(font);
        const float text_width = fm.horizontalAdvance(text);
        const float text_height = fm.height();
        const float rect_x = 0.5 * (width - text_width);
        const float rect_y = 0.5 * (height - text_height);
        return {rect_x, rect_y, width - rect_x, height - rect_y};
    }
};

#endif // FONTCREATOR_H
