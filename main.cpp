#include<iostream>
#include <SFML/Graphics.hpp>

using namespace std;

sf::RenderWindow window(sf::VideoMode(480, 520), "Checkers"); // WINDOW

const int BOARD_SIZE = 8;
const int SQUARE_SIZE = 60; 


sf::CircleShape Pieces[BOARD_SIZE][BOARD_SIZE]; //DRAWS PIECES
sf::RectangleShape board[BOARD_SIZE][BOARD_SIZE]; //DRAWS BOARD
sf::RectangleShape PossibleBoard[BOARD_SIZE][BOARD_SIZE]; //DRAWS HELPER SQUARES

bool selected = false;
bool RED[BOARD_SIZE][BOARD_SIZE]; //To Decide where to draw a piece for RED
bool BLUE[BOARD_SIZE][BOARD_SIZE]; //To Decide where to draw a piece for BLUE
bool POSSIBLE[BOARD_SIZE][BOARD_SIZE]; //To Decide where to draw helper Squares for current team


bool turn_RED = true;


bool keyPressed = false;
bool mouseButtonPressed = false;



pair<int,int> pos; // For saving selected Element
void drawBoard()
{
        // Initialize the board
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                // Set the size and position of the square
                board[i][j].setSize(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
                board[i][j].setPosition(i * SQUARE_SIZE, j * SQUARE_SIZE);

                // Alternate the color of the squares
                if ((i + j) % 2 == 0) {
                    board[i][j].setFillColor(sf::Color::White);
                } else {
                    board[i][j].setFillColor(sf::Color::Black);
                }
            }
        }

        // In your game loop, draw the board
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                window.draw(board[i][j]);
            }
        }

            // In your game loop, draw the board
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                window.draw(Pieces[i][j]);
                if (POSSIBLE[i][j]) //Trying to draw helper squares here
                {
                    sf::RectangleShape Square;
                    Square.setSize(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
                    Square.setPosition(SQUARE_SIZE * i, SQUARE_SIZE * j);
                    sf::Color lightgreen(100, 255, 100, 255 * 0.4);
                    Square.setFillColor(lightgreen);
                    window.draw(Square);
                }
            }
        }
}

void drawPieces()
{
    int radius = 28;
    int pointCount = 100;
    //clear
    for (int i = 0; i < BOARD_SIZE; ++i) 
    {
        for (int j = 0; j < BOARD_SIZE; ++j) 
        {
            Pieces[i][j] = sf::CircleShape(); // Assign a new CircleShape with default properties
        }
    }

    for (int i = 0; i < BOARD_SIZE; ++i) 
    {
        for (int j = 0; j < BOARD_SIZE; ++j) 
        {
            if (RED[i][j] == true)
            { 
                Pieces[i][j].setRadius(radius);
                Pieces[i][j].setPointCount(pointCount);
                Pieces[i][j].setPosition(SQUARE_SIZE * i + (SQUARE_SIZE/2 - radius), SQUARE_SIZE * j + (SQUARE_SIZE/2 - radius));
                Pieces[i][j].setFillColor(sf::Color::Red);
            }

            if (BLUE[i][j] == true)
            {
                Pieces[i][j].setRadius(radius);
                Pieces[i][j].setPointCount(pointCount);
                Pieces[i][j].setPosition(SQUARE_SIZE * i + (SQUARE_SIZE/2 - radius), SQUARE_SIZE * j + (SQUARE_SIZE/2 - radius));
                Pieces[i][j].setFillColor(sf::Color::Blue);
            }
        }
    }

}

void drawWhoseTurn()
{
    sf::Text text;
    sf::Font font;
    if (!font.loadFromFile("game_over.ttf")) // replace with path to your font file
    {
        throw runtime_error ("not loaded properly");
    }
    text.setPosition(10,465);
    text.setFont(font);
    text.setCharacterSize(45);

    if (turn_RED)
    {
        text.setString("IT IS REDS TURN");
        text.setColor(sf::Color::Red);
    }
    
    else
    {
        text.setString("IT IS BLUES TURN");
        text.setColor(sf::Color::Blue);
    }

    window.draw(text);
}
//i == X Koordinate
//j == Y Koordinate

void clearHelper()
{
    //clear Possible
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            POSSIBLE[i][j] = false;
        }
    }
}

bool valid(int& i, int& j, bool red)
{
    if (red)
    {
        if (RED[i][j] == false && BLUE[i][j] == false)
        { 
            if (pos.first == i && pos.second == j-1) //Infront Move
                return true;

            if (pos.first == i-2 && pos.second == j-2) //Diagonal right move
                if (BLUE[i-1][j-1] == true)
                {
                    BLUE[i-1][j-1] = false;
                    return true;
                }

            if (pos.first == i+2 && pos.second == j-2) //Diagonal left move
                if (BLUE[i+1][j-1] == true)
                {
                    BLUE[i+1][j-1] = false;
                    return true;
                }
        }
        
    }
    else // Handle blue pieces
    {
        if (BLUE[i][j] == false && RED[i][j] == false) // Infront Move
        { 
            if (pos.first == i && pos.second == j+1)
                return true;

            if (pos.first == i-2 && pos.second == j+2) // Diagonal right move
                if (RED[i-1][j+1] == true)
                {
                    RED[i-1][j+1] = false;
                    return true;
                }

            if (pos.first == i+2 && pos.second == j+2) // Diagonal left move
                if (RED[i+1][j+1] == true)
                {
                    RED[i+1][j+1] = false;
                    return true;
                }
        }
    }
    
    cout << "False move" << endl;
    return false;
}


void playingTime(int& i, int& j)
{
    std::cout << "SPALTE: " << i << std::endl;
    std::cout << "ZEILE:  " << j << std::endl;
    std::cout << "GRID NUMBER: " << j * 8 + i << std::endl;
    if (turn_RED)
    {
        if (!selected)
        {
            pos = {i, j};
            if (RED[i][j] == false)
            {
                cout << "Invalid Checker chosen RED" << endl;
            }
            else
            {
                // Here you display helper squares
                if (j + 1 < BOARD_SIZE)
                {
                    if (j + 1 < BOARD_SIZE)
                    {
                        if (RED[i][j + 1] == false && BLUE[i][j + 1] == false) // If there is no piece Infrong
                        {
                            cout << "Called Infront";
                            POSSIBLE[i][j + 1] = true;
                        }

                        // KILL Helpers
                        if (i + 1 < BOARD_SIZE)
                        {
                            if (BLUE[i+1][j+1] == true && RED[i+2][j+2] == false && BLUE[i+2][j+2] == false)
                            {
                                cout << "Called diagonal right" << endl;
                                POSSIBLE[i+2][j+2] = true;
                            }
                        }
                        if (i - 1 >= 0)
                        {
                            if (BLUE[i-1][j+1] == true && RED[i-2][j+2] == false && BLUE[i-2][j+2] == false)
                            {
                                POSSIBLE[i-2][j+2] = true;
                                cout << "Called diagonal right" << endl;
                            }
                        }
                    }

                }
                selected = true;
            }
        }
        else
        {
            if (valid(i,j, true))
            {
                RED[i][j] = true;
                RED[pos.first][pos.second] = false;
                turn_RED = false;
                selected = false;
                pos = {i, j};
                clearHelper(); // Update pos after moving the piece
            }
            else
            {
                cout << "Invalid Move" << endl;
            }
        }
    }
    else
    {
        if (!selected)
        {
            pos = {i, j};
            if (BLUE[i][j] == false)
            {
                cout << "Invalid Checker chosen BLUE" << endl;
            }
            else
            {
                // Here you display helper squares
                if (j - 1 >= 0)
                {
                    if (RED[i][j - 1] == false && BLUE[i][j - 1] == false)
                    {
                        POSSIBLE[i][j - 1] = true;
                    }


                    //KILL Move Diagonal Helpers
                    if (i + 1 < BOARD_SIZE)
                    {
                        if (RED[i+1][j-1] == true && BLUE[i+2][j-2] == false && RED[i+2][j-2] == false)
                        {
                            cout << "Called diagonal right" << endl;
                            POSSIBLE[i+2][j-2] = true;
                        }
                    }
                    if (i - 1 >= 0)
                    {
                        if (RED[i-1][j-1] == true && BLUE[i-2][j-2] == false && RED[i-2][j-2] == false)
                        {
                            POSSIBLE[i-2][j-2] = true;
                            cout << "Called diagonal right" << endl;
                        }
                    }
                }
                selected = true;
            }
        }
        else
        {
            if (valid(i,j,false))
            {
                BLUE[i][j] = true;
                BLUE[pos.first][pos.second] = false;
                turn_RED = true;
                selected = false;
                pos = {i, j}; // Update pos after moving the piece
                clearHelper();
            }
            else
            {
                cout << "Invalid Move" << endl;

            }
        }
    }
}




void WindowMain()
{
    window.setFramerateLimit(30);
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    mouseButtonPressed = true;
                }
            }

            if (event.type == sf::Event::MouseButtonReleased)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    mouseButtonPressed = false;
                }
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.scancode == sf::Keyboard::Scan::Escape)
                    keyPressed = true;
            }

            if (event.type == sf::Event::KeyReleased)
            {
                if (event.key.scancode == sf::Keyboard::Scan::Escape)
                    keyPressed = false;
            }
        }
        if (keyPressed)
        {
            selected = false;
            cout << "TEST" << endl;
            clearHelper();
        }
        if (mouseButtonPressed)
        {
            int i = sf::Mouse::getPosition(window).x / SQUARE_SIZE;
            int j = sf::Mouse::getPosition(window).y / SQUARE_SIZE;
            playingTime(i, j);
            mouseButtonPressed = false;
        }

        // clear the window with black color
        window.clear(sf::Color::Black);
        drawBoard();
        drawPieces();
        drawWhoseTurn();
        // end the current frame
        window.display();
    }
}






int main()
{

    // Initialize all elements to false
for(int i = 0; i < BOARD_SIZE; ++i) {
    for(int j = 0; j < BOARD_SIZE; ++j) {
        RED[j][i] = false;
        BLUE[j][i] = false;
        POSSIBLE[j][i] = false;
    }
}

// Set the first two columns to true
for(int i = 0; i < 3; ++i) {
    for(int j = 0; j < BOARD_SIZE; ++j) {
        if ((i+j) % 2 == 0)
            RED[j][i] = true;
    }
}

// Set the last two columns to true
for(int i = BOARD_SIZE - 3; i < BOARD_SIZE; ++i) {
    for(int j = 0; j < BOARD_SIZE; ++j) 
    {
        cout<<"Called";
        if ((i+j) % 2 == 0)
        BLUE[j][i] = true;
    }
}

    WindowMain();
    return 0;
}