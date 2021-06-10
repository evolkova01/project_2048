#include "board.h"
#include <iostream>
using namespace std;

Board::Board(QObject *parent) : QObject(parent)
{
    dimension = 4;
    score = 0;
    bestscore = 0;
    nb_rounds = 1;
    status = 'P';
    fusion_possible = false;
    verify_win = false;
    verify_lost = false;
    BlockIndexX = 0;
    BlockIndexY = 0;
    BlockIndexColor = 0;
    BlockIndexNumber = 0;
    BlockIndexTextColor = 0;
    allocateActualBoard();
    createNewBlock(true);
    savegame();
    boardSignal();
}

void Board::newGame()
{
    setround(1);
    BlockIndexX = 0;
    BlockIndexY = 0;
    BlockIndexColor = 0;
    BlockIndexNumber = 0;
    BlockIndexTextColor = 0;
    verify_win = false;
    verify_lost = false;
    fusion_possible = false;
    setstatus('P');
    for (int i=0;i<dimension*dimension;i++)
    {
        Act[i]->setvalue(0);
    }
    createNewBlock(true);
    savegame();
    boardSignal();
}

void Board::endGame(){
    bool verify_zeros = true;
    for(int i=0;i<dimension*dimension;i++) {
        if(Act[i]->getvalue()==0){
            verify_zeros = false;
        }
    }
    if(verify_zeros){
        for (int i=1;i<5;i++) {
            verifyCommand(i);
        }
        if(fusion_possible==false){
            setstatus('L');
            cout << "You lost." << endl;
            verify_lost = true;
        }
    }
    boardSignal();
}

void Board::winGame(){
    for(int i=0;i<dimension*dimension;i++) {
        if(Act[i]->getvalue()==2048){
            setstatus('L');
            cout << "You won !" << endl;
            verify_win = true;
        }
    }
    boardSignal();
}

void Board::createNewBlock(bool flag_start)
{
    vector<int> indexNullBlocs;
    int nblock_index, random_integer, some_score = 0;
    srand(time(NULL));
    for (int i=0;i<dimension*dimension;i++)
    {
        if(Act[i]->getvalue()==0)
        {
            indexNullBlocs.push_back(i);
        }
    }
    if(indexNullBlocs.size()>0){
        random_integer = rand()% indexNullBlocs.size();
        nblock_index = indexNullBlocs[random_integer];

        if(flag_start==false)
        {
           vector<int> values = {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,4,4,4};
           random_integer = rand() % values.size();
           Act[nblock_index]->setvalue(values[random_integer]);
        }
        else
        {
            Act[nblock_index]->setvalue(2);
        }
    }

    for (int i =0;i<dimension*dimension;i++) // Обновить счет
    {
        some_score += Act[i]->getvalue();
    }
    setscore(some_score);

    if (bestscore < some_score) // Обновить рекорд
        setbestscore(some_score);
}

vector<int> Board::checkMovLimits(int id_mov)
{
    vector<int> index_mobileBlocks;
    index_mobileBlocks.clear();
    switch (id_mov)
    {
    case 1: //вверх
        for (int i=0;i<dimension*dimension;i++)
        {
            if((Act[i]->getrow()!=0)&&(Act[i]->getvalue()!=0))
            {
                index_mobileBlocks.push_back(i);
            }

        }
        break;
    case 2: //вниз
        for (int i=(dimension*dimension)-1;i>=0;i=i-1)
        {
            if((Act[i]->getrow()!=dimension-1)&&(Act[i]->getvalue()!=0))
            {
                index_mobileBlocks.push_back(i);
            }

        }
        break;
    case 3: //влево
        for (int i=0;i<dimension*dimension;i++)
        {
            if((Act[i]->getcol()!=0)&&(Act[i]->getvalue()!=0))
            {
                index_mobileBlocks.push_back(i);
            }

        }
        break;
    case 4: //вправо
        for (int i=(dimension*dimension)-1;i>=0;i=i-1)
        {
            if((Act[i]->getcol()!=dimension-1)&&(Act[i]->getvalue()!=0))
            {
                index_mobileBlocks.push_back(i);
            }
        }
        break;
    }
    return index_mobileBlocks;
}
void Board::fusionBlocks(int indexreceiver, int indexsender)
{
    Act[indexreceiver]->setvalue(2*(Act[indexsender]->getvalue()));;
    Act[indexsender]->setvalue(0);
    fusion_possible = false;
}
void Board::moveBlocks(int indexreceiver, int indexsender)
{
    Act[indexreceiver]->setvalue((Act[indexsender]->getvalue()));;
    Act[indexsender]->setvalue(0);

}
bool Board::verifyCommand(int id_move){
    checkforPairs(id_move,false);
    return fusion_possible;
}
void Board::checkforPairs(int id_mov, bool command_fusion)
{
    int aux_index1,aux_index2,aux_count,lim_inf;
        switch (id_mov)
        {
        case 1: //вверх
            aux_index1 = dimension*(dimension-2);
            for (int i =0;i<dimension;i++)
            {
                for (int j=i;j<=aux_index1;j = j+dimension)
                {
                    if((Act[j]->getvalue()==0))
                    {
                        continue;// не ищет слияния
                    }
                    else // в поиске соединения
                    {
                        aux_count = j+dimension;
                        while(aux_count<=aux_index1+dimension)
                        {
                            if(Act[j]->getvalue()==Act[aux_count]->getvalue())
                            {
                                fusion_possible = true;
                                if(command_fusion){
                                    fusionBlocks(j,aux_count); //соединение
                                }

                                aux_count = aux_index1+dimension+1; //заканчивается поиск соединения
                            }
                            else if(Act[aux_count]->getvalue()!=0)
                            {
                                aux_count = aux_index1+dimension+1;// если элемент не соединяется, то прекратить поиск
                            }
                            else {
                                aux_count = aux_count+dimension;
                            }
                        }
                    }
                }
                aux_index1 = aux_index1+1;
            }
            break;
        case 2: //вниз
            aux_index2 = dimension*(dimension-1);
            lim_inf = dimension;
            for (int i=aux_index2;i<dimension*dimension;i++)
            {
                for (int j=i;j>=lim_inf;j=j-dimension)
                {
                    if((Act[j]->getvalue()==0))
                    {
                        continue;// не ищет слияния
                    }
                    else // в поиске соединения
                    {
                        aux_count = j-dimension;
                        while(aux_count>=lim_inf-dimension)
                        {
                            if(Act[j]->getvalue()==Act[aux_count]->getvalue())
                            {
                                fusion_possible = true;
                                if(command_fusion){
                                    fusionBlocks(j,aux_count);
                                }
                                aux_count = lim_inf-dimension-1; //заканчивается поиск соединения
                            }
                            else if(Act[aux_count]->getvalue()!=0)
                            {
                                aux_count = lim_inf-dimension-1; //если элемент не соединяется, то прекратить поиск
                            }
                            else
                            {
                                aux_count = aux_count-dimension;
                            }
                        }
                    }
                }
                lim_inf = lim_inf+1;
            }
            break;
        case 3://влево
            aux_index1 = dimension-2;
            aux_index2 = dimension*(dimension-1);
            for (int i = 0;i<=aux_index2;i=i+dimension)
            {
                for (int j=i;j<=aux_index1;j++)
                {
                    if((Act[j]->getvalue()==0))
                    {
                        continue;
                    }
                    else
                    {
                        aux_count = j+1;
                        while(aux_count<=aux_index1+1)
                        {
                            if(Act[j]->getvalue()==Act[aux_count]->getvalue())
                            {
                                fusion_possible = true;
                                if(command_fusion){
                                    fusionBlocks(j,aux_count);
                                }
                                aux_count = aux_index1+2 ;
                            }
                            else if(Act[aux_count]->getvalue()!=0)
                            {
                                aux_count = aux_index1+2;
                            }
                            else {
                                aux_count = aux_count+1;
                            }
                        }
                    }
                }
                aux_index1 = aux_index1+dimension;
            }
            break;
        case 4: //вправо
            lim_inf = 1;
            for (int i=dimension-1;i<dimension*dimension;i = i+dimension)
            {
                for (int j =i;j>=lim_inf;j=j-1)
                {
                    if((Act[j]->getvalue()==0))
                    {
                        continue;
                    }
                    else
                    {
                        aux_count = j-1;
                        while(aux_count>=lim_inf-1)
                        {
                            if(Act[j]->getvalue()==Act[aux_count]->getvalue())
                            {
                                fusion_possible = true;
                                if(command_fusion){
                                    fusionBlocks(j,aux_count);
                                }
                                aux_count = lim_inf-2 ;
                            }
                            else if(Act[aux_count]->getvalue()!=0)
                            {
                                aux_count = lim_inf-2;
                            }
                            else {
                                aux_count = aux_count-1;
                            }
                        }
                    }
                }
                lim_inf = lim_inf +dimension;
            }
            break;
        }
}
void Board::goUP()
{
    winGame();
    endGame();
    if(status == 'P'){
        vector<int> index_mobileBlocks;
        int col_aux;
        bool command_fusion = verifyCommand(1);
        checkforPairs(1,command_fusion); //соединение элементов
        index_mobileBlocks = checkMovLimits(1); //получение индексов элементов
        for (auto & index : index_mobileBlocks)
        {
            col_aux = Act[index]->getcol();
            for (int i=col_aux;i<index;i=i+dimension)
            {
                if(Act[i]->getvalue()==0)
                {
                    moveBlocks(i,index);//найдена позиция для перемещения
                    break;
                }
            }
        }
        createNewBlock(false);
        savegame();
        boardSignal();
    }
    winGame();
}
void Board::goDown()
{
    winGame();
    endGame();
    if(status == 'P'){
        vector<int> index_mobileBlocks;
        int col_aux,aux_index1;
        bool command_fusion = verifyCommand(2);
        checkforPairs(2,command_fusion);
        index_mobileBlocks = checkMovLimits(2);
        for (auto & index : index_mobileBlocks)
        {
            col_aux = Act[index]->getcol();
            aux_index1 = dimension*(dimension-1);
            for (int i=col_aux+aux_index1;i>index;i=i-dimension)
            {
                if(Act[i]->getvalue()==0)
                {
                    moveBlocks(i,index);
                    break;
                }
            }
        }
        createNewBlock(false);
        savegame();
        boardSignal();
    }
    winGame();
}
void Board::goLeft()
{
    winGame();
    endGame();
    if(status == 'P'){
        vector<int> index_mobileBlocks;
        int row_aux,index_row;
        bool command_fusion = verifyCommand(3);
        checkforPairs(3,command_fusion);
        index_mobileBlocks = checkMovLimits(3);
        for (auto & index : index_mobileBlocks)
        {
            row_aux = Act[index]->getrow();
            index_row = row_aux*dimension;
            for (int i=index_row;i<index;i++)
            {
                if(Act[i]->getvalue()==0)
                {
                    moveBlocks(i,index);
                    break;
                }
            }
        }
        createNewBlock(false);
        savegame();
        boardSignal();
    }
    winGame();
}
void Board::goRight()
{
    winGame();
    endGame();
    if(status == 'P'){
        vector<int> index_mobileBlocks;
        int row_aux,index_end_row;
        bool command_fusion = verifyCommand(4);
        checkforPairs(4,command_fusion);
        index_mobileBlocks = checkMovLimits(4);
        for (auto & index : index_mobileBlocks)
        {
            row_aux = Act[index]->getrow();
            index_end_row = (row_aux+1)*dimension - 1;
            for (int i=index_end_row;i>index;i=i-1)
            {
                if(Act[i]->getvalue()==0)
                {
                    moveBlocks(i,index);
                    break;
                }
            }
        }
        createNewBlock(false);
        savegame();
        boardSignal();
    }
    winGame();
}

void Board::savegame(){
    bool verify_change = false;
    if(nb_rounds>1)
    {
        for (int i=0;i<dimension;i++)
        {
            for (int j=0;j<dimension;j++)
            {
                if(histAct[nb_rounds-1][i][j]!=Act[i+dimension*j]->getvalue())
                    verify_change = true;
            }
        }
    }
    else {
        verify_change = true;
    }
    if(verify_change)
    {
        int*** aux_int;
        aux_int = new int**[nb_rounds+1];
        for(int i=0;i<nb_rounds+1;i++){
            aux_int[i] = new int*[dimension];
        }
        for(int i=0;i<nb_rounds+1;i++){
            for(int j=0;j<dimension;j++){
                (aux_int[i])[j] = new int[dimension];
            }
        }
        for(int i=0;i<nb_rounds;i++){
            for(int j=0;j<dimension;j++){
                for(int k=0; k<dimension; k++){
                    ((aux_int[i])[j])[k] = ((histAct[i])[j])[k];
                }
            }
        }
        for(int i=0;i<dimension;i++){
            for(int j=0; j<dimension; j++){
                ((aux_int[nb_rounds])[i])[j] = Act[i+dimension*j]->getvalue();
            }
        }

        for(int i=0;i<nb_rounds;i++){
            for(int j=0;j<dimension;j++){
                delete (histAct[i])[j];
            }
            delete histAct[i];
        }
        delete [] histAct;
        histAct = aux_int;
        countnewround();
    }
}
bool Board::sendwin()
{
    return verify_win;
}
bool Board::sendlost()
{
    return verify_lost;
}
int Board::getscore()
{
    return score;
}
int Board::getbestscore()
{
    return bestscore;
}
int Board::getdimension()
{
    return dimension;
}
char Board::getstatus()
{
    return status;
}
int Board::getrounds()
{
    return nb_rounds;
}
Block** Board::getTAB()
{
    return Act;
}

void Board::setround(int round)
{
    nb_rounds = round;
}
void Board::countnewround()
{
    nb_rounds++;
}
void Board::setscore(int val_score)
{
    score = val_score;
}
void Board::setstatus(char newstatus)
{
    status = newstatus;
}
void Board::setbestscore(int best_score)
{
    bestscore = best_score;
}
void Board::setdimension(int dim)
{
    if(dim>2)
    {
        destroyActualBoard();
        dimension = dim;
        allocateActualBoard();
        newGame();
    }
}
QString Board::sendBlocknumber()
{
    int blockNumber;
    if(BlockIndexNumber == dimension*dimension)
    {
        BlockIndexNumber = 0;
    }
    blockNumber = Act[BlockIndexNumber]->getvalue();
    BlockIndexNumber++;
    return QString::number(blockNumber);
}
QString Board::sendBlockColor()
{
    QString blockColor;
    if(BlockIndexColor == dimension*dimension)
    {
        BlockIndexColor = 0;
    }
    blockColor = Act[BlockIndexColor]->getcolorblock();
    BlockIndexColor++;
    return blockColor;
}
QString Board::sendBlockTextColor()
{
    QString textColor;
    if(BlockIndexTextColor == dimension*dimension)
    {
        BlockIndexTextColor = 0;
    }
    textColor = Act[BlockIndexTextColor]->getcolortext();
    BlockIndexTextColor++;
    return textColor;

}
int Board::sendBlockX()
{
    int xBlock,row_block;
    if(BlockIndexX == dimension*dimension)
    {
        BlockIndexX = 0;
    }
    row_block = Act[BlockIndexX]->getrow();
    BlockIndexX++;
    xBlock = (row_block*85)+10;
    return xBlock;

}
int Board::sendBlockY()
{
    int yBlock,col_block;
    if(BlockIndexY == dimension*dimension)
    {
        BlockIndexY = 0;
    }
    col_block = Act[BlockIndexY]->getcol();
    BlockIndexY++;
    yBlock = (col_block*85)+10;
    return yBlock;
}

void Board::allocateActualBoard()
{
    Act = new Block*[dimension*dimension];
    int row_count = 0;
    int col_count = -1;
    int aux = dimension;
    for (int i=0;i<dimension*dimension;i++)
    {
        if(i == aux)
        {
            row_count++;
            aux = aux+dimension;
            col_count = -1;
        }
        col_count++;
        Act[i] = new Block(row_count,col_count);
    }
    histAct = new int**[1];
    histAct[0] = new int*[dimension];
    for(int i=0;i<dimension;i++){
        (histAct[0])[i] = new int[dimension];
    }
    for(int i=0;i<dimension;i++){
        for(int j=0; j<dimension; j++){
            ((histAct[0])[i])[j] = Act[j+dimension*i]->getvalue();
        }
    }
    setscore(nb_rounds);
}
void Board::destroyActualBoard()
{
    if (Act!=NULL)
    {
        for (int i=0;i<dimension*dimension;i++)
        {
            delete Act[i];
        }
        delete [] Act;
        for(int i=0;i<nb_rounds;i++){
            for(int j=0;j<dimension;j++){
                delete (histAct[i])[j];
            }
            delete histAct[i];
        }
        delete [] histAct;
        histAct = NULL;
        Act = NULL;
    }
}
Board::~Board()
{
    destroyActualBoard();
}
