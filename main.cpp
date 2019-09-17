/************************************************************
* @file                                                     *
*           This file read your text file.txt               *
*           and sort it by lines, deleting all zero         *
*           lines by doing this                             *
*           You can sort text by beginnings or by ends      *
* @date     14th of September 2019 year                     *
* @author   Nickita Zaitsiev                                *
* email:    zaitsev.ni@phystech.edu                         *
************************************************************/

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

const int BEGIN     = 0;
const int END       = 1;
const int NOTSTRICT = 0;
const int STRICT    = 1;


/// @brief  Structure which contains pointers on beginnings
///         and ends lines of your file
struct arr;

/// @brief  Read file in array and translate it in needful format
/// @param[in] name Address of file, which should be read
/// @note   You need clean your data after usage
/// @return Array from file in the correct format
char* read (char *name);

/// @brief  Show result of work of copier by using array of text
void showText (char *text);

/// @brief  Get size of array of pointers
/// @param[in] text Array with text
/// @return Size of needful array of structs
int  getIndexSize (char *text);

/// @brief  Create array of pointers for our text
/// @note   You need clean your data after usage
/// @param[in] indexSize Size of your array
/// @param[in] text Array of text
/// @return Array of structs with pointers
arr* makeIndex (char *text, int indexSize);

/// @brief  Function of devision array of pointer by pivot
/// @param[in] index Array of structs
/// @param[in] n Quantity of lines in array
/// @param[in] version : BEGIN=0 - with straight, END=1 - with back
/// @return Number of element of devision
int  Partition (arr *index, int n, int version);

/// @brief  Function of sorting array of structs with size n
/// @param[in] index Array of structs
/// @param[in] n Quantity of lines in array
/// @param[in] version : BEGIN = 0 - with straight, END = 1 - with back
void qsort (arr *index, int n, int version);

/// @brief Function of sorting two lines
/// @param[in] first  First line
/// @param[in] second Second line
/// @param[in] mode: NOTSTRICT = 0, STRICT = 1
/// @param[in] version : BEGIN = 0 - with straight, END = 1 - with back
/// @return true, if first is less; false, if first is greater
bool comp (arr first, arr second, int mode, int version);

/// @brief  Show result of work with array of structs
/// @param[in] index Array of structs
/// @param[in] indexSize Size of your array
void showResult (arr *index, int indexSize);

/// @brief  Swap two objects of structs, which came with address
/// @param[in] first  Pointer on first line
/// @param[in] second Pointer on second line
void swap (arr *first, arr *second);

/// @brief  Check symbol on letter
/// @param[in] c Input symbol
/// @return true, if c is a letter
inline bool isLetter (char c);

/// @brief  Swap letter lowercase
/// @param[in] c Input symbol
/// @return Needful symbol
inline char makeItSmall (char c);

/// @brief  Check my Comparator
void testComp();

/// @brief  Check sorting array
/// @param[in] index Array of structs
/// @param[in] indexSize Size of your array
/// @param[in] version : BEGIN = 0 - with straight, END = 1 - with back
void testArray (arr *index, int indexSize, int version);

/// @brief  Recording sorted array in your created file
/// @param[in] index Array of structs
/// @param[in] indexSize Size of your array
void record (arr *index, int indexSize);

/// @brief  Read letter and compare it with y and Y
/// @return true, if symbol = y Y, else false
bool ifY ();

/// @brief  Sorting of array of structs with size 1, 2 or 3
/// @param[in] index Array of structs
/// @param[in] n Quantity of lines in array
/// @param[in] version : BEGIN = 0 - with straight, END = 1 - with back
void sort123 (arr* index, int n, int version);

/// @brief  Read line
/// @note   need a free
/// @return Return it
char* readName ();

/// @brief  Give you the size of the file, which you gave
/// @param[in] filename
/// @return Size
int getSize (FILE* filename);

/// @brief  Sort from beginnings
/// @param[in] first  First line
/// @param[in] second Second line
/// @param[in] mode: NOTSTRICT = 0, STRICT = 1
/// @return true, if first is less; false, if first is greater
bool compHelpFirst (arr fir, arr sec, int mode);

/// @brief  Sort from endings
/// @param[in] first  First line
/// @param[in] second Second line
/// @param[in] mode: NOTSTRICT = 0, STRICT = 1
/// @return true, if first is less; false, if first is greater
bool compHelpSecond (arr fir, arr sec, int mode);


int main()
{
    testComp();

    char *name = readName();                    assert (name);
    char *text = read (name);                   assert (text);
    free (name);                                name = nullptr;

    int indexSize = getIndexSize (text);        assert (indexSize >= 0);
    arr* index = makeIndex (text, indexSize);   assert (index);

    qsort (index, indexSize, BEGIN);
    showResult (index, indexSize);

    testArray (index, indexSize, BEGIN);
    record (index, indexSize);

    {// Œ¡–¿“Õ¿ﬂ —Œ–“»–Œ¬ ¿
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
    {// »«Õ¿◊¿À‹Õ€… “≈ —“
        printf ("Do you want to see original text? [Y/N] ");
        if ( ifY() )
            showText (text);
    }

    free (index);   index = nullptr;
    free (text);    text  = nullptr;

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

char* read (char* name)
{
    //printf ("Where?\nhey");
    FILE *filename = fopen (name, "r"); assert (filename);
    //printf ("Where?\nallo");
    int Size = getSize (filename);

    printf ("Where?\n");
    // Why 3? My text will be "\0text\n\0" => I need +3
    char *text = (char*) calloc (Size + 3, sizeof (char));  assert (text);
    Size = fread (text + 1, sizeof (char), Size, filename); assert (Size >= 0);

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
    if (version == BEGIN)
        return compHelpFirst  (fir, sec, mode);
    else
        return compHelpSecond (fir, sec, mode);
}

bool compHelpFirst (arr fir, arr sec, int mode)
{
    int i = 0, j = 0;
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

    if (mode == NOTSTRICT)
        return true;
    else
        return false;
}

bool compHelpSecond (arr fir, arr sec, int mode)
{
    int i = 0, j = 0;
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

inline char makeItSmall (char c)
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
    if (n <= 1) // Œ◊≈¬»ƒÕŒ
        return 0;
    arr pivot = index[n-1];
    int left = 0, right = n - 2;
    while (left <= right)
    {
        for ( ; left < n - 1 && comp (index[left], pivot,  STRICT, version); ++left) {}
        for ( ; right >= 0   && comp (pivot, index[right], STRICT, version); --right) {}
        if (left < right)
        {
            swap (index + left, index + right);
            ++left;
            --right;
        }
    }
    swap (index + left, index + (n - 1));
    return left;
}

void qsort (arr *index, int n, int version)
{
    if (n <= 1) // Œ◊≈¬»ƒÕŒ
        return;
    sort123 (index, n, version);

    int part = Partition (index, n, version);
    if (part > 0)
        qsort (index, part, version);
    if (part < n - 1)
        qsort (index + part + 1, n - (1 + part), version);
}

void sort123 (arr* index, int n, int version)
{
    if (n <= 1 || n >= 4)
        return;
    if (n == 2) // ¬≈–ÕŒ
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

    if (n == 3) // “Œ◊ÕŒ œ–¿¬»À‹ÕŒ!!!
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
                  { swap (index, index + 2);
                    swap (index, index + 1);
                    return;
                  }
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
        if (    (comp (fir, sec, STRICT,    BEGIN) == true) ||
                (comp (fir, sec, NOTSTRICT, BEGIN) == true) ||
                (comp (sec, fir, STRICT,    BEGIN) == false)||
                (comp (sec, fir, NOTSTRICT, BEGIN) == false))
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
        {
            assert (i >= 0 && i < indexSize);
            if (comp (index[i], index[i+1], NOTSTRICT, version) == false)
            {
                printf ("SOMETHING went WRONG\n");
                assert (0);
            }
        }
        printf ("\nEVERYTHING is OKEY\n\n");
    }
}

void record (arr *index, int indexSize)
{
    printf ("\nDo you want to record it? [Y/N]: ");
    if ( ifY() )
    {
        char *name = readName();            assert (name);
        FILE *filename = fopen (name, "w"); assert (filename);
        free (name);                        name = nullptr;

        for (int i = 0, check = 0; i < indexSize; ++i)
        {
            check = fputs (index[i].first, filename);   assert (check != EOF);
            check = fputc ('\n', filename);             assert (check != EOF);
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

char* readName ()
{
    printf ("\nPlease, enter your filename: ");
    char *name = (char*) calloc (255, sizeof (char));  assert (name);
    fflush (stdin);
    char* Check = gets (name);  assert (Check);
    printf ("\n%s", name);
    printf ("\n");
    return name;
}

int getSize (FILE* filename)
{
    assert (filename);
    bool foo = fseek (filename, 0, SEEK_END);   assert (!foo);
    int Size = ftell (filename);                assert (Size != -1);
    foo = fseek (filename, 0, SEEK_SET);        assert (!foo);
    return Size;
}



