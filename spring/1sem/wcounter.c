#include <stdio.h>
#include "wcounter.h"
#include "parser.h"
#include "hashtable.h"
#include "array.h"

void word_count(const char * file)
{
    // Инициализация
    parser * psr = parser_ctor(file);
    if (!psr) return;
    hashtable * ht = hashtable_ctor();
    if (!ht) 
    {
        parser_dtor(psr);
        return;
    }

    // Добавление слов в хеш-таблицу
    char word[WORD_LEN] = "";
    while (get_word(psr, word))
    {
        ht_add_word(ht, word);
    }
    
    // Преобразование хеш-таблицы в массив для удобной сортировки 
    array * arr = array_ctor(ht);
    if (!arr)
    {
        hashtable_dtor(ht);
        parser_dtor(psr);
        return;
    }
    sort(arr, ht);

    // Вывод данных на консоль
    unsigned count = 0;
    for (int i = 0; i < arr->array_len; i++)
    {
        if (count = ht_get_count(ht, arr->arr[i]))
        {
            printf("%s: %d\n", ht_get_word(ht, arr->arr[i]), count);
        }
    }

    array_dtor(arr);
    parser_dtor(psr);
    hashtable_dtor(ht);
}

