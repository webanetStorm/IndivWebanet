#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <ctime>

using namespace std;


const int SIZE = 21;

void Run( vector<vector<int>>& current )
{
    srand( static_cast<unsigned int>( time( 0 ) ) );

    ofstream file( "work.dat" );

    for ( int i = 0; i < SIZE; i++ )
    {
        for ( int j = 0; j < SIZE; j++ )
        {
            if ( ( rand() % 2 ) + 1 == 1 )
            {
                file << 'X';
                current[i][j] = 1;
            }
            else
                file << '_';
        }
        file << '\n';
    }

    file.close();
}

void LogGeneration( vector<vector<int>> current )
{
    ofstream outputFile( "work.out" );

    for ( vector<int> row : current )
    {
        for ( int elem : row )
            elem == 1 ? outputFile << 'X' : outputFile << ' ';
        outputFile << endl;
    }

    outputFile.close();
}

int CountNeighbors( vector<vector<int>> current, int i, int j )
{
    int result = 0;

    for ( int dx = -1; dx <= 1; dx++ )
    {
        for ( int dy = -1; dy <= 1; dy++ )
        {
            if ( dx == 0 and dy == 0 ) 
                continue;

            int x = i + dx, y = j + dy;

            if ( x >= 0 and x < SIZE and y >= 0 and y < SIZE )
                result += current[x][y];
        }
    }

    return result;
}

void UpdateGeneration( vector<vector<int>> current, vector<vector<int>>& next )
{
    for ( int i = 0; i < SIZE; i++ )
    {
        for ( int j = 0; j < SIZE; j++ )
        {
            int neighbors = CountNeighbors( current, i, j );

            if ( current[i][j] >= 1 and current[i][j] <= 11 )
                neighbors == 2 or neighbors == 3 ? next[i][j] = current[i][j] + 1 : next[i][j] = 0;
            else if ( current[i][j] == 0 )
                neighbors == 3 ? next[i][j] = 1 : next[i][j] = 0;
            else if ( current[i][j] == 12 )
                next[i][j] = 0;
        }
    }
}

void DisplayGeneration( vector<vector<int>> current )
{
    for ( vector<int> row : current )
    {
        for ( int elem : row )
            elem == 1 ? cout << 'X' : cout << ' ';
        cout << endl;
    }

    cout << "\n----------------------\n";
}

int CountSurvivors( vector<vector<int>> current )
{
    int result = 0;

    for ( vector<int> row : current )
        for ( int elem : row )
            if ( elem > 0 )
                result++;

    return result;
}

int main()
{

    setlocale( LC_ALL, "Russian" );



    unsigned int numGenerations;

    cout << "Введите количество поколений: ";
    if ( !( cin >>numGenerations ) or numGenerations > 100 )
    {
        cout << "Некорректный ввод\n";

        return 1;
    }


    vector<vector<int>> current( SIZE, vector<int>( SIZE, 0 ) );
    vector<vector<int>> next( SIZE, vector<int>( SIZE, 0 ) );

    Run( current );

    for ( int generation = 1; generation <= numGenerations; generation++ )
    {
        cout << "\nПоколение " << generation << ":\n";
        
        DisplayGeneration( current );
        LogGeneration( current );
        UpdateGeneration( current, next );

        if ( CountSurvivors( current ) == 0 )
        {
            cout << "\nВсе микробы погибли, жизнь завершилась на " << generation << " поколении." << endl;

            return 0;
        }

        current = next;
    }



    return 0;

}
