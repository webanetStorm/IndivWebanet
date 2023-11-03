#include <iostream>
#include <fstream>
#include <vector>

using namespace std;


const int SIZE = 21;

void displayGeneration( vector<vector<int>>& generation )
{
    for ( int i = 0; i < SIZE; i++ )
    {
        for ( int j = 0; j < SIZE; j++ )
            generation[i][j] == 0 ? cout << ' ' : cout << '~';
        cout << endl;
    }
}

void updateGeneration( vector<vector<int>>& current, vector<vector<int>>& next )
{
    for ( int i = 0; i < SIZE; i++ )
    {
        for ( int j = 0; j < SIZE; j++ )
        {
            int neighbors = 0;

            for ( int dx = -1; dx <= 1; dx++ )
            {
                for ( int dy = -1; dy <= 1; dy++ )
                {
                    if ( dx == 0 and dy == 0 ) 
                        continue;

                    int x = i + dx;
                    int y = j + dy;

                    if ( x >= 0 and x < SIZE and y >= 0 and y < SIZE and current[x][y] > 0 )
                        neighbors++;
                }
            }

            if ( current[i][j] > 0 )
                neighbors == 2 or neighbors == 3 ? next[i][j] = current[i][j] + 1 : next[i][j] = 0;
            else if ( current[i][j] == 0 )
                if ( neighbors == 3 )
                    next[i][j] = 1;
            else if ( current[i][j] == 12 )
                next[i][j] = 0;
        }
    }
}

int main()
{

    setlocale( LC_ALL, "Russian" );


    vector<vector<int>> current( SIZE, vector<int>( SIZE, 0 ) );
    vector<vector<int>> next( SIZE, vector<int>( SIZE, 0 ) );

    ifstream input( "work.dat" );
    char c;

    for ( int i = 0; i < SIZE; i++ )
    {
        for ( int j = 0; j < SIZE; j++ )
        {
            input >> c;
            if ( c == '~' )
                current[i][j] = 1;
        }
    }PWeba
    input.close();


    unsigned int generations;

    cout << "Кол-во поколений: ";
    if ( !( cin >> generations ) or generations > 100 )
    {
        cout << "Некорректный ввод";

        return 1;
    }

    ofstream output( "work.out" );
    for ( int i = 0; i < generations; i++ )
    {
        displayGeneration( current );
        updateGeneration( current, next );
        swap( current, next );
    }

    output.close();


    return 0;

}
