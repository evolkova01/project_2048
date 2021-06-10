#include "brick.h"

Block::Block(int i, int j)
{
    row = i;
    col = j;
    value = 0;
    setpredefBlock();
    setpredefText();
    setcolors();
}
void Block::setcol(int col_val)
{
    col = col_val;
}
void Block::setrow(int row_val)
{
    row = row_val;
}
void Block::setvalue(int val)
{
    value = val;
    setcolors();
}
void Block::setpredefBlock()
{
    predefColorsBlock << "#00000000" << "#FCF5DE" << "#FAEBB8" <<
                         "#FFC675" << "#FFA175" << "#F88368" <<
                         "#F8687A" << "#F1DE7E" << "#FDE253" <<
                         "#FAB647" << "#E48F07" << "#ffffff";
}
void Block::setpredefText()
{
    predefColorsText << "#00000000" << "#0a4403" << "#0a4403"
                     << "#0a4403"   << "#0a4403" << "#0a4403"
                     << "#0a4403"   << "#0a4403" << "#0a4403"
                     << "#0a4403"   << "#0a4403" << "#0a4403";
}
void Block::setcolors()
{
    switch (value) {
    case 0 :
        color_block = predefColorsBlock.at(0);
        color_text = predefColorsText.at(0);
        break;
    case 2 :
        color_block = predefColorsBlock.at(1);
        color_text = predefColorsText.at(1);
        break;
    case 4 :
        color_block = predefColorsBlock.at(2);
        color_text = predefColorsText.at(2);
        break;
    case 8 :
        color_block = predefColorsBlock.at(3);
        color_text = predefColorsText.at(3);
        break;
    case 16 :
        color_block = predefColorsBlock.at(4);
        color_text = predefColorsText.at(4);
        break;
    case 32 :
        color_block = predefColorsBlock.at(5);
        color_text = predefColorsText.at(5);
        break;
    case 64 :
        color_block = predefColorsBlock.at(6);
        color_text = predefColorsText.at(6);
        break;
    case 128 :
        color_block = predefColorsBlock.at(7);
        color_text = predefColorsText.at(7);
        break;
    case 256 :
        color_block = predefColorsBlock.at(8);
        color_text = predefColorsText.at(8);
        break;
    case 512 :
        color_block = predefColorsBlock.at(9);
        color_text = predefColorsText.at(9);
        break;
    case 1024 :
        color_block = predefColorsBlock.at(10);
        color_text = predefColorsText.at(10);
        break;
    case 2048 :
        color_block = predefColorsBlock.at(11);
        color_text = predefColorsText.at(11);
        break;
    }
}
int Block::getcol()
{
    return col;
}
int Block::getrow()
{
    return row;
}
int Block::getvalue()
{
    return value;
}
QString Block::getcolorblock()
{
    return color_block;
}
QString Block::getcolortext()
{
    return color_text;
}
