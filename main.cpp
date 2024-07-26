#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <stdexcept>

using namespace std;

const int BOARD_SIZE = 8;
const int SQUARE_SIZE = 60;

int blue_pieces = 12;
int red_pieces = 12;

class Piece
{
private:
    bool is_red = false;
    sf::CircleShape shape;
    bool possible_moves[BOARD_SIZE][BOARD_SIZE] = {false};
    pair<int,int> position;

public:
    Piece(bool is_red, int x, int y) : is_red(is_red), position(x,y) 
    {
        // Define Piece with sfml methods
        shape.setRadius(28);
        shape.setPointCount(100);
        shape.setFillColor(is_red ? sf::Color::Red : sf::Color::Blue);
        shape.setPosition(SQUARE_SIZE * x + (SQUARE_SIZE/2 - 28), SQUARE_SIZE * y + (SQUARE_SIZE/2 - 28));
    }

    void setPosition(int x, int y)
    {
        this->position = {x,y};
        shape.setPosition(x * SQUARE_SIZE + (SQUARE_SIZE / 2 - 28), y * SQUARE_SIZE + (SQUARE_SIZE / 2 - 28));
    }

    pair<int,int> getPosition()
    {
        return this->position;
    }

    bool getIsRed()
    {
        return is_red
;
    }

    sf::CircleShape getShape()
    {
        return this->shape;
    }

};


enum class MoveType {
    None,   // No move
    Normal, // Regular move
    Kill    // Kill move
};

class Board
{
private:
    sf::RenderWindow& window;
    sf::RectangleShape board[BOARD_SIZE][BOARD_SIZE];
    Piece* pieces[BOARD_SIZE][BOARD_SIZE] = {nullptr};
    MoveType possible_moves[BOARD_SIZE][BOARD_SIZE] = {MoveType::None};

public:
    Board(sf::RenderWindow& window) : window(window)
    {
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            for (int y = 0; y < BOARD_SIZE; y++)
            {
                board[x][y].setSize(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
                board[x][y].setPosition(x * SQUARE_SIZE, y * SQUARE_SIZE);

                if ((x + y) % 2 == 0) 
                {
                    board[x][y].setFillColor(sf::Color::White);
                } 
                else 
                {
                    board[x][y].setFillColor(sf::Color::Black);
                }
            }
        }
    }

    MoveType getPossibleMoveType(int x, int y)
    {
        return possible_moves[x][y];
    }

    void addPiece(int x, int y, Piece* piece) 
    {
        piece->setPosition(x,y);
        pieces[x][y] = piece;
    }

    void removePiece(int x, int y)
    {
        pieces[x][y] = nullptr;
    }

    Piece* getPiece(int x, int y)
    {
        return pieces[x][y];
    }

    void movePiece(int prev_x, int prev_y, int x, int y)
    {
        Piece* piece = pieces[prev_x][prev_y];
        removePiece(prev_x,prev_y);
        addPiece(x,y,piece);
    }

    void draw()
    {
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            for (int y = 0; y < BOARD_SIZE; y++)
            {
                //draw Board
                window.draw(board[x][y]);

                //draw possible Moves
                if (possible_moves[x][y] == MoveType::Normal || possible_moves[x][y] == MoveType::Kill) {
                    drawPossibleMoves(x,y);
                }

                //draw Pieces
                if (pieces[x][y] != nullptr)
                    window.draw(pieces[x][y]->getShape());
            }
        }
    }


    void setPossibleMoves(int x, int y)
    {
        setPossibleDiagonalMoves(x,y);
        setPossibleKillMoves(x,y);
    }

    void setPossibleDiagonalMoves(int x, int y)
    {
        Piece* piece = pieces[x][y]; //check if piece is valid
        if (!piece) return;

        bool is_red = piece->getIsRed(); //get color
        int direction = is_red ? -1 : 1;
        
        int offsets[2][2] = {{1, direction}, {-1, direction}}; //set offsets

        for (auto& offset : offsets)
        {
            int dx = offset[0];
            int dy = offset[1];
            
            int new_x = x + dx;
            int new_y = y + dy;
        
            //check if move is in bounds
            if (new_x >= 0 && new_x < BOARD_SIZE && new_y >= 0 && new_y < BOARD_SIZE)
            {
                if (pieces[new_x][new_y] == nullptr)
                {
                    possible_moves[new_x][new_y] = MoveType::Normal;
                }
            }
        }
    }

    void setPossibleKillMoves(int x, int y)
    {
        
        Piece* piece = pieces[x][y]; //check if piece is valid
        if (!piece) return;

        bool is_red = piece->getIsRed(); //get color
        int offsets[4][2] = {{1, -1}, {-1, -1},{1, 1},{-1, 1}}; //set offsets 

        for (auto& offset : offsets) //check offsets for possible moves
        {
            int dx = offset[0];
            int dy = offset[1];
            
            int new_x = x + dx;
            int new_y = y + dy;

            if (new_x >= 0 && new_x < BOARD_SIZE && new_y >= 0 && new_y < BOARD_SIZE)
            {
                Piece* target = pieces[new_x][new_y]; // find target piece
                if (target && target->getIsRed() != is_red) //check if piece to be captured is of the opposite color
                {
                    int capture_x = new_x + dx;
                    int capture_y = new_y + dy;

                    if (capture_x >= 0 && capture_x < BOARD_SIZE && capture_y >= 0 && capture_y < BOARD_SIZE && pieces[capture_x][capture_y] == nullptr)
                    {
                        possible_moves[capture_x][capture_y] = MoveType::Kill;
                    }
                }
            }
        }
    }



    bool isPossibleMove(int x, int y)
    {
        return (possible_moves[x][y] == MoveType::Normal || possible_moves[x][y] == MoveType::Kill);
    }

    void drawPossibleMoves(int x, int y)
    {
        sf::RectangleShape highlight(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
        highlight.setPosition(x * SQUARE_SIZE, y * SQUARE_SIZE);
        highlight.setFillColor(sf::Color(100, 255, 100, 102));
        window.draw(highlight);
    }

    void clearPossibleMoves()
    {
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            for (int y = 0; y < BOARD_SIZE; y++)
            {
                possible_moves[x][y] = MoveType::None;
            }
        }
    }

    bool checkNextKillMove(pair<int,int> piece_pos)
    {
        int x = piece_pos.first;
        int y = piece_pos.second;

        clearPossibleMoves();
        setPossibleKillMoves(x, y);

        for (int i = 0; i < BOARD_SIZE; i++)
        {
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                if (possible_moves[i][j] == MoveType::Kill)
                {
                    return true;
                }
            }
        }

        return false;
    }
};

class Game
{
private:
    Board board;
    sf::RenderWindow window;
    bool red_turn;
    Piece* selected_piece;
    sf::Font font;
    sf::Text turn_text;
    sf::Text blue_text;
    sf::Text red_text;

public:
    Game(): window(sf::VideoMode(480,520), "Checkers"), board(window), red_turn(true), selected_piece{nullptr}
    {
        initText();
        initPieces();
        run();
    }

private:

    void initText()
    {
        if (!font.loadFromFile("game_over.ttf")) {
            throw runtime_error("Font not loaded properly");
        }
        turn_text.setFont(font);
        turn_text.setCharacterSize(45);
        turn_text.setPosition(10, 465);

        blue_text.setFont(font);
        blue_text.setCharacterSize(35);
        blue_text.setPosition(360, 460);

        red_text.setFont(font);
        red_text.setCharacterSize(35);
        red_text.setPosition(360, 475);
    }

    void run()
    {
        window.setFramerateLimit(60);
        while(window.isOpen())
        {
            handleClick();
            updateGame();
            render();
        }
    }

    void initPieces()
    {
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            for (int y = 0; y < 3; y++)
            {
                if ((x + y) % 2 != 0)
                {
                    board.addPiece(x, y, new Piece(false,x,y));
                }
            }
            for (int y = 5; y < BOARD_SIZE; y++)
            {
                if ((x + y) % 2 != 0)
                {
                    board.addPiece(x, y, new Piece(true,x,y));
                }
            }
        }
    }

    void handleClick()
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed)
            {
                cout << endl << "ITS: " << (red_turn ? "REDS TURN" : "BLUES TURN") << endl << endl;
                if (selected_piece == nullptr) //select Piece
                    selectPiece(event.mouseButton.x / SQUARE_SIZE, event.mouseButton.y / SQUARE_SIZE);

                else //check if move is valid, if yes, make it
                    checkMove(event.mouseButton.x / SQUARE_SIZE, event.mouseButton.y / SQUARE_SIZE);
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) 
            {
                selected_piece = nullptr;
                board.clearPossibleMoves();
            }

        }
    }

    void selectPiece(int x, int y)
    {
        board.clearPossibleMoves();

        if (board.getPiece(x,y) == nullptr)
        {
            selected_piece = nullptr;
            board.clearPossibleMoves();
            cout << "Empty spot clicked" << endl;
            return;
        }
        if (red_turn && board.getPiece(x,y)->getIsRed())
        {
            selected_piece = board.getPiece(x,y);
            cout << "Red piece Clicked" << endl;
            board.setPossibleMoves(x,y);
        }

        else if (!red_turn && !board.getPiece(x,y)->getIsRed())
        {
            selected_piece = board.getPiece(x,y);
            cout << "Blue piece Clicked" << endl;
            board.setPossibleMoves(x,y);
        }

        else
        {
            cout << "wrong piece clicked" << endl;
        }
    }

    void checkMove(int x, int y)
    {
        if (board.isPossibleMove(x,y))
        {
            cout << "Valid Move" << endl;
            makeMove(x,y);
        }
        else
        {
            board.clearPossibleMoves();
            cout << "Move to: " << x << " " << y << "not possible" << endl;
            selected_piece = nullptr;
        }
    }

    void makeMove(int x, int y)
    {
        pair<int, int> movePosition = {x, y};
        cout << "Piece moved from: " << selected_piece->getPosition().first << " " << selected_piece->getPosition().second << ", to: " << movePosition.first << " " << movePosition.second << endl;

        Piece* piece = selected_piece;
        int prev_x = piece->getPosition().first;
        int prev_y = piece->getPosition().second;

        if (board.getPossibleMoveType(x, y) == MoveType::Normal) // Normal move
        {
            board.movePiece(prev_x, prev_y, x, y);
            board.clearPossibleMoves();
            red_turn = !red_turn;
            selected_piece = nullptr;
        }
        else if (board.getPossibleMoveType(x, y) == MoveType::Kill) // Capture move
        {

            int dx = x - prev_x;
            int dy = y - prev_y;

            int capture_x = prev_x + dx / 2;
            int capture_y = prev_y + dy / 2;
            board.getPiece(capture_x,capture_y)->getIsRed() ? --red_pieces : --blue_pieces;
            board.removePiece(capture_x, capture_y); // Remove captured piece
            board.movePiece(prev_x, prev_y, x, y);
            piece->setPosition(x, y); // Update piece's position

            // Check if next kill move is available
            if (board.checkNextKillMove(piece->getPosition()))
            {
                // Allow the same player to continue making captures
                return;
            }
            else
            {
                board.clearPossibleMoves();
                red_turn = !red_turn;
                selected_piece = nullptr;
            }
        }
    }

    void updateGame()
    {
        if (red_turn) 
        {
            turn_text.setString("IT IS RED'S TURN");
            turn_text.setFillColor(sf::Color::Red);
        } else 
        {
            turn_text.setString("IT IS BLUE'S TURN");
            turn_text.setFillColor(sf::Color::Blue);
        }

        blue_text.setString(std::to_string(blue_pieces) + " / 12 REMAINING");
        blue_text.setFillColor(sf::Color::Blue);

        red_text.setString(std::to_string(red_pieces) + " / 12 REMAINING");
        red_text.setFillColor(sf::Color::Red);
    }

    void render()
    {
        window.clear();
        board.draw();
        window.draw(turn_text);
        window.draw(red_text);
        window.draw(blue_text);
        window.display();
    }

};

int main()
{
    try
    {
        Game game;
    }
    catch (const exception& err)
    {
        cerr << err.what() << endl;
        return 1;
    }
    return 0;
    
}
