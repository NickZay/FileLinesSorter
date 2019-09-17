/************************************************************
* @file                                                     *
*           Этот файл считывает Ваш текстовый файл txt      *
*           и сортирует его по строкам, убирая все нулевые  *
*           строчки при этом.                               *
*           Можно сортировать как сначала, так и с конца    *
* @date     14 сентября 2019 года                           *
* @author   Никита Зайцев                                   *
* email:    zaitsev.ni@phystech.edu                         *
************************************************************/

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#define BEGIN     0
#define END       1
#define NOTSTRICT 0
#define STRICT    1


/// @brief  Структура, содержащая указатели на начала
///         и концы строк Вашего файла
struct arr;

/// @brief  Считывает файл в массив и переводит его
///         в нужный формат
/// @note   Требуется очистка динамической памяти
/// @return Массив из файла в правильном формате
char* read ();

/// @brief  Показывает результат работы копировщика
///         через массив с текстом
void showText (char *text);

/// @brief  Получает размер для массива из указателей
/// @param[in] text Массив с текстом
/// @return Размер целевого массива структур
int  getIndexSize (char *text);

/// @brief  Создает массив из указателей для нашего текста
/// @note   Требуется очистка динамической памяти
/// @param[in] indexSize Размер массива
/// @param[in] text Массив с текстом
/// @return Массив из структур с указателями
arr* makeIndex (char *text, int indexSize);

/// @brief  Функция разделения массива указателей по пивоту
///         Не рандомная, потому что отлаживать тяжело
/// @param[in] index Массив из структур
/// @param[in] n Количество строк в массиве
/// @param[in] version : BEGIN=0 - с прямым компаратором, END=1 - с обраным
/// @return Номер элемента разделения
int  Partition (arr *index, int n, int version);

/// @brief  Функция сортировки массива из структур размера n
/// @param[in] index Массив из структур
/// @param[in] n Размер массива
/// @param[in] version : BEGIN=0 - с прямым компаратором, END=1 - с обраным
void qsort (arr *index, int n, int version);

/// @brief Функция сравнения двух строк
/// @param[in] first  Первая строка
/// @param[in] second Вторая строка
/// @param[in] mode: NOTSTRICT=0 - нестрого, STRICT=1 - строго
/// @param[in] version : BEGIN=0 - с прямым компаратором, END=1 - с обраным
/// @return true, если первая меньше; false, если первая больше
bool comp (arr first, arr second, int mode, int version);

/// @brief  Показывает результат работы через массив
///         из структур
/// @param[in] index Массив структур
/// @param[in] indexSize Размер массива
void showResult (arr *index, int indexSize);

/// @brief  Меняет местами два объекта из структур,
///         пришедших по ссылке
/// @param[in] first  Указатель на первую строку
/// @param[in] second Указатель на вторую строку
void swap (arr *first, arr *second);

/// @brief Проверяет символ на код буквы
/// @param[in] c Входящий символ
/// @return true, если c буква
bool isLetter (char c);

/// @brief Меняет букву на строчную, если она была заглавной
/// @param[in] c Входящий символ
/// @return Нужный символ
char makeItSmall (char c);

/// @brief Проверка моего компаратора
void testComp();

/// @brief Проверка отсортированности массива
/// @param[in] index Массив структур
/// @param[in] indexSize Размер массива
/// @param[in] version : BEGIN=0 - с прямым компаратором, END=1 - с обраным
void testArray (arr *index, int indexSize, int version);

/// @brief Запись отсортированного массива в созданный до этого файл
/// @param[in] index Массив структур
/// @param[in] indexSize Размер массива
void record (arr *index, int indexSize);

/// @brief  Считывает букву и сравнивает ее с y Y
/// @return true, если символ = y Y, иначе false
bool ifY ();

int main()
{
    testComp();

    char *text = read();                      assert (text);
    int indexSize = getIndexSize (text);      assert (indexSize >= 0);
    arr* index = makeIndex (text, indexSize); assert (index);

    qsort (index, indexSize, BEGIN);
    showResult (index, indexSize);

    testArray (index, indexSize, BEGIN);
    record (index, indexSize);

    {// ОБРАТНАЯ СОРТИРОВКА
        printf ("Do you want to get text sorted from ends? [Y/N] ");
        if ( ifY() )
        {
            qsort (index, indexSize, END);
            showResult (index, indexSize);
            putchar ('\n');

            testArray (index, indexSize, END);
            record (index, indexSize);
        }
    }
    {// ИЗНАЧАЛЬНЫЙ ТЕКСТ
        printf ("Do you want to see original text? [Y/N] ");
        if ( ifY() )
            showText (text);
    }

    free (index); index = nullptr;
    free (text); //!!!

    printf("\n\n  Thank you for chosing our product\n  (C)NickZay\n");
    return 0;
}

struct arr
{
    arr (char *a, char *b):
        first (a),
        second(b)
        {}

    char *first  = nullptr;
    char *second = nullptr;
};

char* read ()
{
    printf ("Please, enter your filename: ");
    char name[20];
    fflush (stdin);
    char* Check = gets (name);
    assert (Check != 0);
    printf ("\n");

    //printf("Please");
    FILE *filename = fopen (name, "r");
    assert (filename);

    // Узнаем размер
    bool foo = fseek (filename, 0, SEEK_END);
    assert (!foo);

    int Size = ftell (filename);
    assert (Size != -1);

    foo = fseek (filename, 0, SEEK_SET);
    assert (!foo);

    // Создаем массив текста и переводим его в нужный формат
    char *text = (char*) calloc (Size + 3, sizeof (char));
    assert (text);

    text[0] = '\0';
    // Без ассерта, потому что fread не передает \r
    // И у нас результат зависел бы от операционки
    Size = fread (text + 1, sizeof (char), Size, filename);

    for (int i = 1; i < Size + 1; ++i)
        text[i] = ((text[i] == '\n') ? '\0' : text[i]);
    (text[Size] == '\0') ? text[Size+1] = '\n' : (text[Size+1] = '\0', text[Size+2] = '\n');

    int check = fclose (filename);
    assert (check != EOF);

    return text;
}

void showText (char *text)
{
    for (int i = 0; text[i] != '\n'; ++i)
        (text[i] == '\0') ? printf ("\n"): putchar (text[i]);
}

int getIndexSize (char *text)
{
    int indexSize = 0;
    for (int i = 0; text[i] != '\n'; ++i)
        if (text[i] == '\0' && text[i+1] != '\0')
            ++indexSize;
    return --indexSize;
}

arr* makeIndex (char *text, int indexSize)
{
    arr *index = (arr*) calloc (indexSize, sizeof (arr));
    assert (index);

    for (int i = 0, j = 0; (text[i] != '\n') && (j < indexSize); ++i)
    {
        if (text[i] == '\0' && text[i+1] != '\0')
            index[j].first = &text[i+1];
        if (text[i] != '\0' && text[i+1] == '\0')
            index[j++].second = &text[i];
    }
    return index;
}

void showResult (arr *index, int indexSize)
{
    for (int i = 0, check = EOF; i < indexSize; ++i)
    {
        check = puts (index[i].first);
        assert (check != EOF);
    }
    putchar ('\n');
}

bool comp (arr fir, arr sec, int mode, int version)
{
    int i = 0, j = 0;
    if (version == BEGIN)
    {
        while (fir.first[i] != '\0' && sec.first[j] != '\0')
        {
            if (!isLetter (fir.first[i]))
              { ++i; continue; }
            if (!isLetter (sec.first[j]))
              { ++j; continue; }
            if (makeItSmall (fir.first[i]) < makeItSmall (sec.first[j]))
                return true;
            else if (makeItSmall (fir.first[i]) > makeItSmall (sec.first[j]))
                return false;
            else
              { ++i; ++j; }
        }
        if ((fir.first[i] == '\0') && (sec.first[j] != '\0'))
            return true;
        else if (fir.first[i] != '\0' && sec.first[j] == '\0')
            return false;
    }
    else
    {
        while (fir.second[i] != '\0' && sec.second[j] != '\0')
        {
            if (!isLetter (fir.second[i]))
              { --i; continue; }
            if (!isLetter (sec.second[j]))
              { --j; continue; }
            if (makeItSmall (fir.second[i]) < makeItSmall (sec.second[j]))
                return true;
            else if (makeItSmall (fir.second[i]) > makeItSmall (sec.second[j]))
                return false;
            else
              { --i; --j; }
        }
        if ((fir.second[i] == '\0') && (sec.second[j] != '\0'))
            return true;
        else if (fir.second[i] != '\0' && sec.second[j] == '\0')
            return false;
    }
    if (mode == NOTSTRICT)
        return true;
    else
        return false;
}

inline bool isLetter (char c)
{
    if ((('A' <= c) && (c <= 'Z')) || (('a' <= c) && (c <='z')))
        return true;
    else
        return false;
}

char makeItSmall (char c)
{
    if ((('A' <= c) && (c <= 'Z')) || (('a' <= c) && (c <='z')))
        {
            if (('A' <= c) && (c <= 'Z'))
                return c + 'a'-'A';
            else
                return c;
        }
    assert (0); // You tried change not a letter
}

int Partition (arr *index, int n, int version)
{
    if (n <= 1) // ОЧЕВИДНО
        return 0;
    arr pivot = index[n-1];
    int i = 0, j = n - 2;
    while (i <= j)
    {
        for ( ; i < n - 1 && comp (index[i], pivot, STRICT, version); ++i) {}
        for ( ; j >= 0    && comp (pivot, index[j], STRICT, version); --j) {}
        if (i < j)
        {
            swap (index + i, index + j);
            ++i;
            --j;
        }
    }
    swap (index + i, index + (n - 1));
    return i;
}

void qsort (arr *index, int n, int version)
{
    if (n <= 1) // ОЧЕВИДНО
        return;

    if (n == 2) // ТОЖЕ ВЕРНО
        if (version == BEGIN)
            if (comp (index[0], index[1], NOTSTRICT, version))
                return;
            else
              { swap (index, index + 1); return; }
        else
            if (comp (index[0], index[1], NOTSTRICT, version))
                return;
            else
              { swap (index, index + 1); return; }

    if (n == 3) // ТОЧНО ПРАВИЛЬНО!!!
        if (comp (index[0], index[1], NOTSTRICT, version))
            if (comp (index[1], index[2], NOTSTRICT, version))       // 0 1 2
                return;
            else
                if (comp (index[0], index[2], NOTSTRICT, version))  // 0 2 1
                  { swap (index + 1, index + 2); return; }
                else                                                // 1 2 0
                  { swap (index, index + 2);
                    swap (index + 1, index + 2);
                    return;
                  }
        else
            if (comp (index[2], index[1], NOTSTRICT, version))       // 2 1 0
              { swap (index, index + 2); return; }
            else
                if (comp (index[0], index[2], NOTSTRICT, version))  // 1 0 2
                  { swap (index, index + 1); return; }
                else                                    // 2 0 1
                {   swap (index, index + 2);
                    swap (index, index + 1);
                    return; }
    int part = Partition (index, n, version);
    if (part > 0)
        qsort (index, part, version);
    if (part < n - 1)
        qsort (index + part + 1, n - (1 + part), version);
}

void swap (arr *first, arr *second)
{
    arr t = *first;
    *first = *second;
    *second = t;
}

void testComp()
{
    printf ("Do you want to test my compare function? [Y/N]: ");
    char c = 'n';
    scanf ("%c", &c);
    putchar ('\n');
    if (c == 'y' || c == 'Y')
    {
        arr fir ("aa b", "a bb"), sec ("a bb", "aa b");
        if (      (comp (fir, sec, STRICT,    BEGIN) == true)
                ||(comp (fir, sec, NOTSTRICT, BEGIN) == true)
                ||(comp (sec, fir, STRICT,    BEGIN) == false)
                ||(comp (sec, fir, NOTSTRICT, BEGIN) == false)  )
            printf ("\n\nEVERYTHING is OKEY\n\n");
        else
            printf ("\n\nSOMETHING went WRONG\n\n");
    }
}

void testArray (arr *index, int indexSize, int version)
{
    if (version == BEGIN)
        printf ("Do you want to check STRAIGHT qsort? [Y/N]: ");
    else
        printf ("Do you want to check REVERSE qsort? [Y/N]: ");
    if ( ifY() )
    {
        for (int i = 0; i < indexSize - 1; ++i)
            if (comp (index[i], index[i+1], NOTSTRICT, version) == false)
            {
                printf ("SOMETHING went WRONG\n");
            }
        printf ("\nEVERYTHING is OKEY\n\n");
    }
}

void record (arr *index, int indexSize)
{
    printf ("\nDo you want to record it? [Y/N]: ");
    if ( ifY() )
    {
        printf ("\nPlease, point me the file of record: ");
        char name[20];
        fflush (stdin);
        char* Check = gets (name);
        assert (Check != 0);
        printf ("\n");

        FILE *filename = fopen (name, "w");
        assert (filename);

        for (int i = 0, check = 0; i < indexSize; ++i)
        {
            check = fputs (index[i].first, filename);
            assert (check != EOF);
            check = fputc ('\n', filename);
            assert (check != EOF);
        }
    }
}

bool ifY()
{
    char c = 'n';
    fflush (stdin);
    scanf ("%c", &c);
    putchar ('\n');
    if (c == 'y' || c == 'Y')
        return true;
    else
        return false;
}


