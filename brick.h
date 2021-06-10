#ifndef BRICK_H
#define BRICK_H

#include <QString>
#include <QDebug>
#include <vector>
#include <QStringList>

class Block
{

public:
    Block(int row,int col);

    void setrow(int row_val);
    void setcol(int col_val);
    void setvalue(int val);
    void setcolors();
    void setpredefBlock();
    void setpredefText();

    int getrow();
    int getcol();
    int getvalue();
    QString getcolorblock();
    QString getcolortext();

private:
    int row;
    int col;
    int value;
    QString color_block;
    QString color_text;
    QStringList predefColorsBlock, predefColorsText;
};

#endif // BRICK_H
