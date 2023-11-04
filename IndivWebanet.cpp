#include <iostream> // Библиотека для ввода и вывода данных.
#include <cstdlib> // Библиотека для общих функций, включая генерацию случайных чисел.
#include <fstream> // Библиотека для работы с файлами.
#include <vector> // Библиотека для использования векторов.
#include <ctime> // Библиотека для работы со временем.

using namespace std; // Директива, которая позволяет использовать стандартное пространство имен для упрощения кода.


const int SIZE = 21; // Константа, определяющая размер поля (21x21).

void Run( vector<vector<int>>& current ) // Генерирует начальное состояние и записывает его в файл "work.dat"
{
    srand( static_cast<unsigned int>( time( 0 ) ) ); // Инициализация генератора случайных чисел.

    ofstream file( "work.dat" ); // Открытие файла для записи начального состояния.

    /* Генерация начального состояния поля. */
    for ( int i = 0; i < SIZE; i++ )
    {
        for ( int j = 0; j < SIZE; j++ )
        {
            if ( ( rand() % 2 ) + 1 == 1 ) // Случайным образом определяем, родится ли микроб в данной ячейке.
            {
                file << 'X'; // Если микроб родился, записываем 'X' в файл.
                current[i][j] = 1; // Записываем 1 в текущее поколение, чтобы отметить, что микроб родился.
            }
            else
                file << '_'; // Если микроб не родился, записываем пустое место ('_') в файл.
        }
        file << '\n'; // Переход на новую строку для создания поля.
    }

    file.close(); // Закрытие файла.
}

void DisplayGeneration( vector<vector<int>> current ) // Вывод текущего поколения на экран
{
    /* Перебор текущего поколения. */
    for ( vector<int> row : current )
    {
        for ( int elem : row )
            elem > 0 ? cout << 'X' : cout << ' '; // Вывод 'X' для живых микробов, пробела для мертвых.
        cout << endl; // Переход на новую строку для разделения строк поколения.
    }

    cout << "\n---------------------\n"; // Разделитель между поколениями.
}

void LogGeneration( vector<vector<int>> current ) // Запись текущего поколения в файл "work.out"
{
    ofstream outputFile( "work.out" ); // Открытие файла для записи текущего поколения.

    /* Перебор текущего поколения. */
    for ( vector<int> row : current )
    {
        for ( int elem : row )
            elem == 1 ? outputFile << 'X' : outputFile << ' '; // Запись 'X' для живых микробов, пробела для мертвых.
        outputFile << endl; // Переход на новую строку для записи каждой строки поколения.
    }

    outputFile.close(); // Закрытие файла.
}

int CountNeighbors( vector<vector<int>> current, int i, int j ) // Подсчет количества соседей у данной ячейки в текущем поколении
{
    int result = 0; // Количество соседей.

    /* Перебираем соседние ячейки вокруг текущей. */
    for ( int dx = -1; dx <= 1; dx++ )
    {
        for ( int dy = -1; dy <= 1; dy++ )
        {
            if ( dx == 0 and dy == 0 ) 
                continue; // Пропустить текущую ячейку.

            int x = i + dx, y = j + dy;

            /* Проверяем, находится ли сосед внутри поля. */
            if ( x >= 0 and x < SIZE and y >= 0 and y < SIZE )
                result += current[x][y]; // // Увеличиваем счетчик живых соседей.
        }
    }

    return result; // Возвращаем количество соседей.
}

void UpdateGeneration( vector<vector<int>> current, vector<vector<int>>& next ) // Обновление текущего поколения на основе правил
{
    /* Перебор поколения. */
    for ( int i = 0; i < SIZE; i++ )
    {
        for ( int j = 0; j < SIZE; j++ )
        {
            int neighbors = CountNeighbors( current, i, j ); // Подсчет соседей для текущей ячейки.

            if ( current[i][j] >= 1 and current[i][j] <= 11 ) // Если микроб жив и его возраст в диапазоне от 1 до 11,
                neighbors == 2 or neighbors == 3 ? next[i][j] = current[i][j] + 1 : next[i][j] = 0; // микроб продолжает жить или погибает.
            else if ( current[i][j] == 0 ) // Если ячейка пуста (микроб не существует),
                neighbors == 3 ? next[i][j] = 1 : next[i][j] = 0; // микроб рождается или остается мертвым.
            else if ( current[i][j] == 12 ) // Если микроб старше 11 поколений,
                next[i][j] = 0; // микроб погибает от старости.
        }
    }
}

int CountSurvivors( vector<vector<int>> current ) // Подсчет количества живых микробов в текущем поколении
{
    int result = 0; // Количество живых микробов.

    /* Перебор текущего поколения. */
    for ( vector<int> row : current )
        for ( int elem : row )
            if ( elem > 0 ) // Если в текущей ячейке массива находится микроб с уровнем больше нуля,
                result++; // то увеличиваем результат на едицину.

    return result; // Возвращаем количество живых микробов.
}

int main()
{

    setlocale( LC_ALL, "Russian" ); // Установка локали для корректного вывода на кириллице.



    unsigned int numGenerations; // Колиычество поколений.

    cout << "Введите количество поколений: "; // Вывод.
    if ( !( cin >> numGenerations ) or numGenerations > 100 ) // Валидация ввода.
    {
        cout << "Некорректный ввод\n"; // Вывод.

        return 1; // Завершение программы с ошибкой.
    }


    vector<vector<int>> current( SIZE, vector<int>( SIZE, 0 ) ); // Инициализация текущего поколения.
    vector<vector<int>> next( SIZE, vector<int>( SIZE, 0 ) ); // Инициализация следующего поколения.

    Run( current ); // Генерация начального поколения.

    for ( int generation = 1; generation <= numGenerations; generation++ ) // Пеоебор поколений.
    {
        cout << "\nПоколение " << generation << ":\n\n"; // Вывод номера поколения.
        
        DisplayGeneration( current ); // Вывод текущего поколения на экран.
        LogGeneration( current ); // Запись текущего поколения в файл.
        UpdateGeneration( current, next ); // Обновление поколения на основе правил.

        if ( CountSurvivors( current ) == 0 ) // Если все микробы умерли,
        {
            cout << "\nВсе микробы погибли, жизнь завершилась на " << generation - 1 << " поколении.\n"; // выводим сообщение

            return 0; // и завершаем программу.
        }

        current = next; // Обновляем текущее поколение на следующее.
    }



    return 0; // Возврат нуля в качестве кода завершения программы, что означает успешное выполнение.

}
