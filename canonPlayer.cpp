#include<bits/stdc++.h>
using namespace std;
using namespace std::chrono; 
const short ARMY = 1;                               // identifier for army
const short TOWNHALL = 2;
const short WHITE_ARMY = ARMY;
const short WHITE_TOWNHALL = TOWNHALL;
const short BLACK_ARMY = -1*ARMY;
const short BLACK_TOWNHALL = -1*TOWNHALL;
const short EMPTY_PLACE = 0;
const short TOWNHALL_MULTIPLIER = 1000;
const short ARMY_MULTIPLIER = 1;
short BOARD_SIZE_X = 8;
short BOARD_SIZE_Y = 8;
short MAX_DEPTH = 3;                              //max depth for minimax algorithm
short SELF_PLAYER = -1;                            //identifier for our player
int MAX_TIME = 90;                                    //max time allowed
int TIME_USED = 0;
short total_moves = 0;
const string MOVES_FILE = "moves";
map<pair<string,string>,vector<struct move>> black_moves;
map<pair<string,string>,vector<struct move>> white_moves;
map<pair<string,string>,struct move> best_move;

// struct that stores the moves and an evaluation value for states
struct move{
    char type;
    short init_xpos;
    short init_ypos;
    char moveType;
    short final_xpos;
    short final_ypos;
    short evaluation;
};

// function to read the moves already stored from previous plays
void readMoves(){
    string input;
    string line;
    ifstream bestmoves(MOVES_FILE);
    if(SELF_PLAYER==1){
        while(getline(bestmoves,line)){
        istringstream ss(line);
        struct move m;
        string black_soldiers;
        string white_soldiers;
        ss >> black_soldiers;
        ss >> white_soldiers;
        ss >> m.type;
        ss >> m.init_xpos;
        ss >> m.init_ypos;
        ss >> m.moveType;
        ss >> m.final_xpos;
        ss >> m.final_ypos;
        best_move[{black_soldiers,white_soldiers}] = m;
        }
    }else{
        while(getline(bestmoves,line)){
        istringstream ss(line);
        struct move m;
        string black_soldiers;
        string white_soldiers;
        ss >> black_soldiers;
        ss >> white_soldiers;
        ss >> m.type;
        ss >> m.init_xpos;
        m.init_xpos = BOARD_SIZE_X-1-m.init_xpos;
        ss >> m.init_ypos;
        m.init_ypos = BOARD_SIZE_Y-1-m.init_ypos;
        ss >> m.moveType;
        ss >> m.final_xpos;
        m.final_xpos = BOARD_SIZE_X-1-m.final_xpos;
        ss >> m.final_ypos;
        m.final_ypos = BOARD_SIZE_Y-1-m.final_ypos;
        reverse(white_soldiers.begin(),white_soldiers.end());
        reverse(black_soldiers.begin(),black_soldiers.end());
        best_move[{white_soldiers,black_soldiers}] = m;
        }
    }
}

// fuction to represent the state of the board using bitset
pair<string,string> getBitState(vector<vector<short>> board){
    if(BOARD_SIZE_X==8 && BOARD_SIZE_Y==8){
        bitset<64> black_soldiers;
        bitset<64> white_soldiers;
        for(short i=0;i<BOARD_SIZE_Y;i++){
            for(short j=0;j<BOARD_SIZE_X;j++){
                short boxnum = i*BOARD_SIZE_X + j;
                if(board[i][j]==1*ARMY){
                    white_soldiers.set(boxnum);
                }else if(board[i][j]==-1*ARMY){
                    black_soldiers.set(boxnum);
                }
            }
        }
        return make_pair(black_soldiers.to_string(),white_soldiers.to_string());
    }else if(BOARD_SIZE_X==10 && BOARD_SIZE_Y==10){
        bitset<100> black_soldiers;
        bitset<100> white_soldiers;
        for(short i=0;i<BOARD_SIZE_Y;i++){
            for(short j=0;j<BOARD_SIZE_X;j++){
                short boxnum = i*BOARD_SIZE_X + j;
                if(board[i][j]==1*ARMY){
                    white_soldiers.set(boxnum);
                }else if(board[i][j]==-1*ARMY){
                    black_soldiers.set(boxnum);
                }
            }
        }
        return make_pair(black_soldiers.to_string(),white_soldiers.to_string());
    }else{
        bitset<80> black_soldiers;
        bitset<80> white_soldiers;
        for(short i=0;i<BOARD_SIZE_Y;i++){
            for(short j=0;j<BOARD_SIZE_X;j++){
                short boxnum = i*BOARD_SIZE_X + j;
                if(board[i][j]==1*ARMY){
                    white_soldiers.set(boxnum);
                }else if(board[i][j]==-1*ARMY){
                    black_soldiers.set(boxnum);
                }
            }
        }
        return make_pair(black_soldiers.to_string(),white_soldiers.to_string());
    }
}

//function returns a boolean for if the given co-ordinates exist in the board
bool isInBoard(short x,short y){
    if(x>=0 && x <BOARD_SIZE_X && y>=0 && y <BOARD_SIZE_Y){
        return true;
    }else{
        return false;
    }
}

// returns the sign of the given piece
short sign(short piece){
    if(piece < 0){
        return -1;
    }else if(piece > 0){
        return 1;
    }else{
        return 0;
    }
}

// function to print the board in an output file
void printBoard(vector<vector<short>> board){
    for(short i=0;i<BOARD_SIZE_Y;i++){
        for(short j=0;j<BOARD_SIZE_X;j++){
            short x = board[i][j];
            if(x<0){
                cout << x << " ";
            }else{
                cout << " " << x << " ";
            }
        }
        printf("\n");
    }
    cout << "------------------------\n";
}

// function to print the board in an output file
void printMoves(pair<string,string> bit_board, struct move best){
    string output = MOVES_FILE;
    ofstream ost (output, ios::app);
    if(SELF_PLAYER==1){
        ost << bit_board.first << " " << bit_board.second << " " << best.type << " " << best.init_xpos << " " << best.init_ypos << " " << best.moveType << " " << best.final_xpos << " " << best.final_ypos;        
    }else{
        string black_soldiers = bit_board.first;
        string white_soldiers = bit_board.second;
        reverse(white_soldiers.begin(),white_soldiers.end());
        reverse(black_soldiers.begin(),black_soldiers.end());
        ost << white_soldiers << " " << black_soldiers << " " << best.type << " " << BOARD_SIZE_X-1-best.init_xpos << " " << BOARD_SIZE_Y-1-best.init_ypos << " " << best.moveType << " " << BOARD_SIZE_X-1-best.final_xpos << " " << BOARD_SIZE_Y-1-best.final_ypos;
    }
    ost << "\n";
    ost.close();
}

// function to initialise a canon board to keep track of the moves
vector<vector<short>> initBoard(){
    if(BOARD_SIZE_X==8 && BOARD_SIZE_Y ==8){
        vector<vector<short>> BOARD = {{WHITE_TOWNHALL,WHITE_ARMY,WHITE_TOWNHALL,WHITE_ARMY,WHITE_TOWNHALL,WHITE_ARMY,WHITE_TOWNHALL,WHITE_ARMY}
                                    ,{EMPTY_PLACE,WHITE_ARMY,EMPTY_PLACE,WHITE_ARMY,EMPTY_PLACE,WHITE_ARMY,EMPTY_PLACE,WHITE_ARMY}
                                    ,{EMPTY_PLACE,WHITE_ARMY,EMPTY_PLACE,WHITE_ARMY,EMPTY_PLACE,WHITE_ARMY,EMPTY_PLACE,WHITE_ARMY}
                                    ,{EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE}
                                    ,{EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE}
                                    ,{BLACK_ARMY,EMPTY_PLACE,BLACK_ARMY,EMPTY_PLACE,BLACK_ARMY,EMPTY_PLACE,BLACK_ARMY,EMPTY_PLACE}
                                    ,{BLACK_ARMY,EMPTY_PLACE,BLACK_ARMY,EMPTY_PLACE,BLACK_ARMY,EMPTY_PLACE,BLACK_ARMY,EMPTY_PLACE}
                                    ,{BLACK_ARMY,BLACK_TOWNHALL,BLACK_ARMY,BLACK_TOWNHALL,BLACK_ARMY,BLACK_TOWNHALL,BLACK_ARMY,BLACK_TOWNHALL}};
        return BOARD;
    }else if(BOARD_SIZE_X==10 && BOARD_SIZE_Y==10){
        vector<vector<short>> BOARD = {{WHITE_TOWNHALL,WHITE_ARMY,WHITE_TOWNHALL,WHITE_ARMY,WHITE_TOWNHALL,WHITE_ARMY,WHITE_TOWNHALL,WHITE_ARMY,WHITE_TOWNHALL,WHITE_ARMY}
                                    ,{EMPTY_PLACE,WHITE_ARMY,EMPTY_PLACE,WHITE_ARMY,EMPTY_PLACE,WHITE_ARMY,EMPTY_PLACE,WHITE_ARMY,EMPTY_PLACE,WHITE_ARMY}
                                    ,{EMPTY_PLACE,WHITE_ARMY,EMPTY_PLACE,WHITE_ARMY,EMPTY_PLACE,WHITE_ARMY,EMPTY_PLACE,WHITE_ARMY,EMPTY_PLACE,WHITE_ARMY}
                                    ,{EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE}
                                    ,{EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE}
                                    ,{EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE}
                                    ,{EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE}
                                    ,{BLACK_ARMY,EMPTY_PLACE,BLACK_ARMY,EMPTY_PLACE,BLACK_ARMY,EMPTY_PLACE,BLACK_ARMY,EMPTY_PLACE,BLACK_ARMY,EMPTY_PLACE}
                                    ,{BLACK_ARMY,EMPTY_PLACE,BLACK_ARMY,EMPTY_PLACE,BLACK_ARMY,EMPTY_PLACE,BLACK_ARMY,EMPTY_PLACE,BLACK_ARMY,EMPTY_PLACE}
                                    ,{BLACK_ARMY,BLACK_TOWNHALL,BLACK_ARMY,BLACK_TOWNHALL,BLACK_ARMY,BLACK_TOWNHALL,BLACK_ARMY,BLACK_TOWNHALL,BLACK_ARMY,BLACK_TOWNHALL}};
        return BOARD;
    }else{
        vector<vector<short>> BOARD = {{WHITE_TOWNHALL,WHITE_ARMY,WHITE_TOWNHALL,WHITE_ARMY,WHITE_TOWNHALL,WHITE_ARMY,WHITE_TOWNHALL,WHITE_ARMY}
                                    ,{EMPTY_PLACE,WHITE_ARMY,EMPTY_PLACE,WHITE_ARMY,EMPTY_PLACE,WHITE_ARMY,EMPTY_PLACE,WHITE_ARMY}
                                    ,{EMPTY_PLACE,WHITE_ARMY,EMPTY_PLACE,WHITE_ARMY,EMPTY_PLACE,WHITE_ARMY,EMPTY_PLACE,WHITE_ARMY}
                                    ,{EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE}
                                    ,{EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE}
                                    ,{EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE}
                                    ,{EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE,EMPTY_PLACE}
                                    ,{BLACK_ARMY,EMPTY_PLACE,BLACK_ARMY,EMPTY_PLACE,BLACK_ARMY,EMPTY_PLACE,BLACK_ARMY,EMPTY_PLACE}
                                    ,{BLACK_ARMY,EMPTY_PLACE,BLACK_ARMY,EMPTY_PLACE,BLACK_ARMY,EMPTY_PLACE,BLACK_ARMY,EMPTY_PLACE}
                                    ,{BLACK_ARMY,BLACK_TOWNHALL,BLACK_ARMY,BLACK_TOWNHALL,BLACK_ARMY,BLACK_TOWNHALL,BLACK_ARMY,BLACK_TOWNHALL}};
        return BOARD;
    }
}

// function to make a move on the board
void makeMove(char type,short init_xpos,short init_ypos,char moveType,short final_xpos,short final_ypos,vector<vector<short>> &board){
    if(type=='S'){
        if(moveType=='M'){
            board[final_ypos][final_xpos] = board[init_ypos][init_xpos];
            board[init_ypos][init_xpos] = EMPTY_PLACE;
        }else if(moveType=='B'){
            board[final_ypos][final_xpos] = EMPTY_PLACE;
        }
    }    
}


// function to test the move, it doesn't make change to the input board but returns a changed board
vector<vector<short>> testMove(char type,short init_xpos,short init_ypos,char moveType,short final_xpos,short final_ypos,vector<vector<short>> board){
    if(type=='S'){
        if(moveType=='M'){
            board[final_ypos][final_xpos] = board[init_ypos][init_xpos];
            board[init_ypos][init_xpos] = EMPTY_PLACE;
        }else if(moveType=='B'){
            board[final_ypos][final_xpos] = EMPTY_PLACE;
        }
    }  
    return board;  
}

bool gameOver(vector<vector<short>> board){
    short black_army=0,white_army=0,black_townhall=0,white_townhall=0;
    for(short i=0;i<BOARD_SIZE_Y;i++){
        for(short j=0;j<BOARD_SIZE_X;j++){
            if(board[i][j]==WHITE_ARMY){
                white_army++;
            }else if(board[i][j]==BLACK_ARMY){
                black_army++;
            }else if(board[i][j]==WHITE_TOWNHALL){
                white_townhall++;
            }else if(board[i][j]==BLACK_TOWNHALL){
                black_townhall++;
            }
        }
    }

    if(black_army == 0 || white_army == 0 || black_townhall <3 || white_townhall < 3){
        return true;
    }else{
        return false;
    }
}


//function to get all possible moves and bombs
vector<struct move> possibleMoves(vector<vector<short>> board, short player){
    pair<string,string> bit_board = getBitState(board);
    map<pair<string,string>,vector<struct move>>::iterator board_it;
    if(player == -1){
        board_it = black_moves.find(bit_board);
        if(board_it!=black_moves.end()){
            return board_it->second;
        }
    }else if(player == 1){
        board_it = white_moves.find(bit_board);
        if(board_it!=white_moves.end()){
            return board_it->second;
        }
    }
    map<pair<short,short>,vector<pair<short,short>>> playerMoves;
    map<pair<short,short>,pair<short,short>> canonBombs;
    vector<struct move> allPossibleMoves;
    map<pair<short,short>,vector<pair<short,short>>>::iterator mit;
    map<pair<short,short>,pair<short,short>>::iterator bit;
    //constants for moves
    short b[5] = {1, 1, 1, 0, 0},dx[5] = {-1, 0, 1, -1, 1},dy[5] = {-1, -1, -1, 0, 0};
    short adjx[5] = {-1, -1, 0, 1, 1} ,adjy[5] = {0, -1, -1, -1, 0};
    //constants for canon
    short dxc[4][3][4] = {{{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, {{-3, -2, 4, 5}, {-4, -3, 3, 4}, {-5, -4, 2, 3}}, {{-3, -2, 4, 5}, {-4, -3, 3, 4}, {-5, -4, 2, 3}}, {{-3, -2, 4, 5}, {-4, -3, 3, 4}, {-5, -4, 2, 3}}};
    short dyc[4][3][4] = {{{-3, -2, 4, 5}, {-4, -3, 3, 4}, {-5, -4, 2, 3}}, {{-3, -2, 4, 5}, {-4, -3, 3, 4}, {-5, -4, 2, 3}}, {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, {{3, 2, -4, -5}, {4, 3, -3, -4}, {5, 4, -2, -3}}};
    short valid_dx[4][3][2] = {{{0, 0}, {0, 0}, {0, 0}}, {{-1, 3}, {-2, 2}, {-3, 1}}, {{-1, 3}, {-2, 2}, {-3, 1}}, {{-1, 3}, {-2, 2}, {-3, 1}}};
    short valid_dy[4][3][2] = {{{-1, 3}, {-2, 2}, {-3, 1}}, {{-1, 3}, {-2, 2}, {-3, 1}}, {{0, 0}, {0, 0}, {0, 0}}, {{1, -3}, {2, -2}, {3, -1}}};
    short cdx[4][3][1] = {{{0}, {}, {0}}, {{3}, {}, {-3}}, {{3}, {}, {-3}}, {{3}, {}, {-3}}};
    short cdy[4][3][1] = {{{3}, {}, {-3}}, {{3}, {}, {-3}}, {{0}, {}, {0}}, {{-3}, {}, {3}}};
    short soldierx[4][3][2] = {{{0, 0}, {0, 0}, {0, 0}}, {{1, 2}, {-1, 1}, {-2, -1}}, {{1, 2}, {-1, 1}, {-2, -1}}, {{1, 2}, {-1, 1}, {-2, -1}}};
    short soldiery[4][3][2] = {{{1, 2}, {-1, 1}, {-2, -1}}, {{1, 2}, {-1, 1}, {-2, -1}}, {{0, 0}, {0, 0}, {0, 0}}, {{-1, -2}, {1, -1}, {2, 1}}};

    for(short x=0; x< BOARD_SIZE_X;x++){
        for(short y=0; y<BOARD_SIZE_Y; y++){
            if(board[y][x]==ARMY*player){
                vector<pair<short,short>> moves;
                int tx,ty;           

                short direction = player*-1;

                // Forward moves
                for (short i = 0; i < 5; i++){
                        tx = x + dx[i];
                        ty = y + dy[i] * direction;

                        if(!isInBoard(tx, ty) || (sign(board[ty][tx]) == player) || (b[i] == 0 && sign(board[ty][tx]) != player*-1)){
                            continue;
                        }                            
                        moves.push_back(make_pair(tx,ty));                        
                }

                // Backward moves
                bool check = false;
                for(short i = 0; i < 5 && !check; i++){
                        tx = x + adjx[i];
                        ty = y + adjy[i] * direction;
                        if(isInBoard(tx, ty) && board[ty][tx] == ARMY*player*-1)
                                check = true;
                }
                short rx[3] = {-2, 0, 2}, ry[3] = {2, 2, 2};

                if(check){
                    for (short i = 0; i < 3; i++){
                                tx = x + rx[i];
                                ty = y + ry[i] * direction;
                                if(!isInBoard(tx, ty) || sign(board[ty][tx]) == player){
                                    continue;
                                }else{
                                    moves.push_back(make_pair(tx,ty));
                                }                                
                        }
                }

                //Cannon
                for(short i = 0; i < 4; i++){
                    for(short j = 0; j < 3; j++){
                        check = true;
                        int s[2][2] = {{x + valid_dx[i][j][0], y + valid_dy[i][j][0] * direction}, {x + valid_dx[i][j][1], y + valid_dy[i][j][1] * direction}};
                        if((isInBoard(s[0][0], s[0][1]) && (board[s[0][1]][s[0][0]] == 0)) || (isInBoard(s[1][0], s[1][1]) && (board[s[1][1]][s[1][0]] == 0))){
                            for(short k = 0; k < 2; k++){
                                tx = x + soldierx[i][j][k];
                                ty = y + soldiery[i][j][k] * direction;
                                if(!isInBoard(tx, ty) || board[ty][tx] != player*ARMY){
                                    check = false;
                                }
                            }

                            if(check){
                                for(short k = 0; k < 4; k++){
                                    if((isInBoard(s[k/2][0], s[k/2][1]) && (board[s[k/2][1]][s[k/2][0]] == 0))){
                                        tx = x + dxc[i][j][k];
                                        ty = y + dyc[i][j][k] * direction;
                                        if(isInBoard(tx, ty) && sign(board[ty][tx]) != player){
                                            canonBombs.insert({make_pair(tx,ty),make_pair(x,y)});
                                        }
                                    }
                                }

                                for(short k = 0; k < 1; k++){
                                    tx = x + cdx[i][j][k];
                                    ty = y + cdy[i][j][k] * direction;
                                    if(!isInBoard(tx, ty) || board[ty][tx] != EMPTY_PLACE){
                                        continue;
                                    }else{
                                        moves.push_back(make_pair(tx,ty));
                                    }
                                }
                            }
                        }               
                    }
                }
                if(moves.size()>0){
                    playerMoves.insert({make_pair(x,y),moves});
                } 
            }
        }
    }

    for(bit = canonBombs.begin();bit!=canonBombs.end();bit++){
        pair<short,short> origin = (*bit).second;
        pair<short,short> target = (*bit).first;
        struct move m;
        m.type = 'S';
        m.moveType = 'B';
        m.init_xpos = origin.first;
        m.init_ypos = origin.second;
        m.final_xpos = target.first;
        m.final_ypos = target.second;
        allPossibleMoves.push_back(m);
    }

    for(mit = playerMoves.begin();mit!=playerMoves.end();mit++){
        pair<short,short> origin = (*mit).first;
        vector<pair<short,short>> possibleMoves = (*mit).second;
        for(short i=0; i<possibleMoves.size();i++){
            struct move m;
            m.type = 'S';
            m.moveType = 'M';
            m.init_xpos = origin.first;
            m.init_ypos = origin.second;
            m.final_xpos = possibleMoves[i].first;
            m.final_ypos = possibleMoves[i].second;
            allPossibleMoves.push_back(m);
        }
    }

    if(player == -1){
        black_moves[bit_board] = allPossibleMoves;
    }else if(player == 1){
        white_moves[bit_board] = allPossibleMoves;
    }

    return allPossibleMoves;
}

// prints the move vector passed to it
void printMoves(vector<struct move> mb){
    vector<struct move>::iterator it;
    for(it=mb.begin();it!=mb.end();it++){
        cout << (*it).type << ", " << (*it).init_xpos << ", " << (*it).init_ypos << ", " << (*it).moveType << ", " << (*it).final_xpos << ", " << (*it).final_ypos << "\n";
    }
}

// function to evaluate the given board value
short eval(vector<vector<short>> board){
    short scoreo = 0, scored = 0, maxw = 0, maxb = 0;
    for(short i=0;i<BOARD_SIZE_Y;i++){
        for(short j=0;j<BOARD_SIZE_X;j++){
            short x = board[i][j];
            if(abs(x)==ARMY){
                if(x>0){
                    scoreo+=((i+1))*x + x*ARMY_MULTIPLIER;
                    scored+=((BOARD_SIZE_Y-i))*x + x*ARMY_MULTIPLIER;
                    if((i+1)>maxw){
                        maxw=i+1;
                    }
                }else{
                    scoreo+=((BOARD_SIZE_Y-i))*x + x*ARMY_MULTIPLIER;
                    scored+=((i+1))*x + x*ARMY_MULTIPLIER;
                    if((BOARD_SIZE_Y-i)>maxb){
                        maxb=(BOARD_SIZE_Y-i);
                    }
                }
            }else if(abs(x)==TOWNHALL){
                scoreo+=x*TOWNHALL_MULTIPLIER;
                scored+=x*TOWNHALL_MULTIPLIER;
            }
        }
    }
    
    if((SELF_PLAYER==1 && maxb>=BOARD_SIZE_Y/2)||(SELF_PLAYER==-1 && maxw>=BOARD_SIZE_Y/2)){
        return scoreo*SELF_PLAYER;
    }else{
        return scored*SELF_PLAYER;
    }
}

//minimax function to find the best move
struct move minimax(vector<vector<short>> board, short depth,short player,struct move alpha,struct move beta){
    struct move best;
    struct move score;
    srand(time(0));
    vector<struct move> mb = possibleMoves(board,player);
    if(depth == MAX_DEPTH || mb.size()<1 || gameOver(board)){
        best.evaluation = eval(board);
        return best;
    }
    vector<struct move>::iterator it;

    if(player==SELF_PLAYER){
        best.evaluation = SHRT_MIN;
        vector<struct move>::iterator it;
        for(it=mb.begin();it!=mb.end();it++){
            vector<vector<short>> nextboard = testMove((*it).type,(*it).init_xpos,(*it).init_ypos,(*it).moveType,(*it).final_xpos,(*it).final_ypos,board);
            struct move score;
            score = minimax(nextboard, depth+1, -1*player,alpha,beta);
            short value = score.evaluation;
            score = (*it);
            score.evaluation = value;
            if(score.evaluation > best.evaluation){
                best = score;
            }else if(score.evaluation==best.evaluation){
                if((rand()%10)<8 && best.moveType =='M' && score.moveType=='B'){
                    best = score;
                }else if((rand()%10)<5){
                    best = score;
                }
            }
            if(best.evaluation > alpha.evaluation){
                alpha = best;
            }
            if(beta.evaluation < alpha.evaluation){
                break;
            }
        }
        return best;
    }else{
        best.evaluation = SHRT_MAX;
        vector<struct move>::iterator it;
        for(it=mb.begin();it!=mb.end();it++){
            vector<vector<short>> nextboard = testMove((*it).type,(*it).init_xpos,(*it).init_ypos,(*it).moveType,(*it).final_xpos,(*it).final_ypos,board);
            struct move score;
            score = minimax(nextboard, depth+1, -1*player,alpha,beta);
            short value = score.evaluation;
            score = (*it);
            score.evaluation = value;
            if(score.evaluation < best.evaluation){
                best = score;
            }
            if(best.evaluation < beta.evaluation){
                beta = best;
            }
            if(beta.evaluation < alpha.evaluation){
                break;
            }
        }
        return best;
    }
}

//function to call the original minimax function
struct move minimaxCall(vector<vector<short>> board, short depth,short player){
    struct move alpha,beta;
    alpha.evaluation = SHRT_MIN;
    beta.evaluation = SHRT_MAX;
    return minimax(board,depth,player,alpha,beta);
}

//set the max depth value
void setMaxDepth(vector<vector<short>> board){
    short mb = possibleMoves(board,-1).size()+possibleMoves(board,1).size();
    if(mb>70){
        MAX_DEPTH = 4;
    }else if(mb>30){
        MAX_DEPTH = 5;
    }else{
        MAX_DEPTH = 6;
    }
    short time_remaining = MAX_TIME - TIME_USED/1000;
    if(time_remaining < (3*MAX_TIME)/20){
        if(mb<40){
            MAX_DEPTH = 4;
        }else{
            MAX_DEPTH = 3;
        }
    }
    if(total_moves<3){
        MAX_DEPTH = 4;
    }
    
}


int main(){
    short playerTurn, sizeBoardX, sizeBoardY, maxTime;
    cin >> playerTurn;
    if(playerTurn == 2){
        SELF_PLAYER = 1;
    }
    cin >> sizeBoardY;
    cin >> sizeBoardX;
    BOARD_SIZE_X = sizeBoardX;
    BOARD_SIZE_Y = sizeBoardY;
    cin >> MAX_TIME;
    TIME_USED = 0;
    vector<vector<short>> board;
    board = initBoard();
    //readMoves();
    if(SELF_PLAYER == -1){
        auto start = high_resolution_clock::now();
        struct move m;
        pair<string,string> bit_board = getBitState(board);
        if(best_move.find(bit_board)!=best_move.end()){
            m = best_move[bit_board];
        }else{
            m = minimaxCall(board,0,SELF_PLAYER);
            best_move[bit_board] = m;
            printMoves(bit_board,m);   
        } 
        cout << m.type << " " << m.init_xpos << " " << m.init_ypos << " " << m.moveType << " " << m.final_xpos << " " << m.final_ypos << "\n";
        makeMove(m.type,m.init_xpos,m.init_ypos,m.moveType,m.final_xpos,m.final_ypos,board);
        auto stop = high_resolution_clock::now();
        TIME_USED += duration_cast<microseconds>(stop - start).count()/1000;
        total_moves++;
    }
    while(true){
        struct move o;
        cin >> o.type;
        cin >> o.init_xpos;
        cin >> o.init_ypos;
        cin >> o.moveType;
        cin >> o.final_xpos;
        cin >> o.final_ypos;
        auto start = high_resolution_clock::now();
        makeMove(o.type,o.init_xpos,o.init_ypos,o.moveType,o.final_xpos,o.final_ypos,board);
        setMaxDepth(board);
        struct move m;
        pair<string,string> bit_board = getBitState(board);
        if(best_move.find(bit_board)!=best_move.end()){
            m = best_move[bit_board];
        }else{
            m = minimaxCall(board,0,SELF_PLAYER);
            best_move[bit_board] = m;
            printMoves(bit_board,m);   
        }
        makeMove(m.type,m.init_xpos,m.init_ypos,m.moveType,m.final_xpos,m.final_ypos,board);
        cout << m.type << " " << m.init_xpos << " " << m.init_ypos << " " << m.moveType << " " << m.final_xpos << " " << m.final_ypos << "\n";
        auto stop = high_resolution_clock::now();
        TIME_USED += duration_cast<microseconds>(stop - start).count()/1000;
        total_moves++;
    }
    
}