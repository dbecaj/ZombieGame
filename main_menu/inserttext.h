#ifndef INSERTTEXT_H
#define INSERTTEXT_H

#include <QGraphicsItemGroup>
#include <QObject>
#include <QTimer>

enum class InputType
{
    ALL,
    NUMBERS_ONLY,
    IPV4
};

class InsertText : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT

    InputType type; // Limits us what we can and cannot input
    int textLimit;
    int textDisplayLimit;
    float cursorDisplayLimit;
    QTimer* cursorTimer;
    QGraphicsLineItem* cursorLine; // The blinking cursor line
    QGraphicsTextItem* input; // Displays the text to the scene
    QString text; // Stores the inputed text

private slots:
    void blinkCursor(); // Hides/Showes the cursor line
public:
    // All text info(font-family, font-size...) is carryed by the QFont
    explicit InsertText(float x, float y, float cursorWidth, QFont font, int textLimit = -1, InputType type = InputType::ALL, QGraphicsItem* parent = nullptr);
    ~InsertText();

    void changeTextColor(QColor color);
    void clearText(); // Clears the text from the InputText and resets the cursor position
    void setTextDisplayLimit(int limit) { textDisplayLimit = limit; }
    void setCursorDisplayLimit(float limit) { cursorDisplayLimit = limit; }
    void showCursor(bool show = true);
    bool isCursorVisible() { return cursorTimer->isActive(); }

    void keyPressEvent(QKeyEvent *event);

    QString getText() { return text; }
};

#endif // INSERTTEXT_H
