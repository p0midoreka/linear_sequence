//
// Created by Ekaterina on 15.11.2021.
//
#include "linear_sequence.h"
#include <string.h>

struct LSQ{
    LSQ_BaseTypeT *head;
    LSQ_IntegerIndexT cap;
    LSQ_IntegerIndexT size;
};

struct LSQ_Iterator{
    LSQ_IntegerIndexT index;
    LSQ *handle;
};

/* Функция, создающая пустой контейнер. Возвращает назначенный ему дескриптор */
LSQ_HandleT LSQ_CreateSequence(void){
     LSQ *c = (LSQ*)malloc(sizeof(LSQ));
     c->head = NULL;
     c->cap = 0;
     c->size = 0;
     return (LSQ_HandleT)c;

}

/* Функция, уничтожающая контейнер с заданным дескриптором. Освобождает принадлежащую ему память */
void LSQ_DestroySequence(LSQ_HandleT handle){
    if(handle ==  LSQ_HandleInvalid ) return;
    free(((LSQ*)(handle))->head);
    free(handle);
}

/* Функция, возвращающая текущее количество элементов в контейнере */
LSQ_IntegerIndexT LSQ_GetSize(LSQ_HandleT handle){
    if(handle ==  LSQ_HandleInvalid ) return 0;
    return ((LSQ*)(handle))->size;
}

/* Функция, определяющая, может ли данный итератор быть разыменован */
int LSQ_IsIteratorDereferencable(LSQ_IteratorT iterator){ ///Итераторы
    if( iterator != NULL && (((LSQ_Iterator*)(iterator))->index) > 0 &&
    (((LSQ_Iterator*)(iterator))->index)<= (((LSQ_Iterator*)(iterator))->handle)->size)
        return 1;
    else
        return 0;
}

/* Функция, определяющая, указывает ли данный итератор на элемент, следующий за последним в контейнере */
int LSQ_IsIteratorPastRear(LSQ_IteratorT iterator){
    if( iterator != NULL && (((LSQ_Iterator*)(iterator))->index) >= (((LSQ_Iterator*)(iterator))->handle)->size)
        return 1;
    else
        return 0;
}

/* Функция, определяющая, указывает ли данный итератор на элемент, предшествующий первому в контейнере */
int LSQ_IsIteratorBeforeFirst(LSQ_IteratorT iterator){
    if( iterator != NULL && (((LSQ_Iterator*)(iterator))->index) < 0)
        return 1;
    else
        return 0;
}


/* Функция, разыменовывающая итератор. Возвращает указатель на элемент, на который ссылается данный итератор */
LSQ_BaseTypeT* LSQ_DereferenceIterator(LSQ_IteratorT iterator){
    if( iterator == NULL) return NULL;
    return ((LSQ_Iterator*)(iterator))->handle->head+((LSQ_Iterator*)(iterator))->index;
}

/// Следующие три функции создают итератор в памяти и возвращают его дескриптор
/* Функция, возвращающая итератор, ссылающийся на элемент с указанным индексом */
LSQ_IteratorT LSQ_GetElementByIndex(LSQ_HandleT handle, LSQ_IntegerIndexT index){
    if(handle == LSQ_HandleInvalid) return NULL;
    if(index > ((LSQ*)(handle))->size-1) return NULL;
    LSQ_Iterator *iterator = (LSQ_Iterator*)malloc(sizeof(LSQ_Iterator));
    iterator->index = index;
    iterator->handle = (LSQ*)handle;
    return iterator;
}
/* Функция, возвращающая итератор, ссылающийся на первый элемент контейнера */
LSQ_IteratorT LSQ_GetFrontElement(LSQ_HandleT handle){
    if(handle == LSQ_HandleInvalid) return NULL;
    if( ((LSQ*)handle)->size > 0) return NULL;
    LSQ_Iterator *iterator = (LSQ_Iterator*)malloc(sizeof(LSQ_Iterator));
    iterator->index = 0;
    iterator->handle = (LSQ*)handle;
    return iterator;
}
/* Функция, возвращающая итератор, ссылающийся на элемент контейнера следующий за последним */
LSQ_IteratorT LSQ_GetPastRearElement(LSQ_HandleT handle){
    if(handle == LSQ_HandleInvalid) return NULL;
    LSQ_Iterator *iterator = (LSQ_Iterator*)malloc(sizeof(LSQ_Iterator));
    iterator->index = ((LSQ*)handle)->size ;
    iterator->handle = (LSQ*)handle;
    return iterator;
}
/* Функция, уничтожающая итератор с заданным дескриптором и освобождающая принадлежащую ему память */
void LSQ_DestroyIterator(LSQ_IteratorT iterator){
    if(iterator ==  NULL ) return;
    free(iterator);
}
/* Функция, перемещающая итератор на один элемент вперед */
void LSQ_AdvanceOneElement(LSQ_IteratorT iterator){
    if(iterator ==  NULL ) return;
    ((LSQ_Iterator*)(iterator))->index++;
}
/* Функция, перемещающая итератор на один элемент назад */
void LSQ_RewindOneElement(LSQ_IteratorT iterator){
    if(iterator ==  NULL ) return;
    ((LSQ_Iterator*)(iterator))->index--;
}
/* Функция, перемещающая итератор на заданное смещение со знаком */
void LSQ_ShiftPosition(LSQ_IteratorT iterator, LSQ_IntegerIndexT shift){
    if(iterator ==  NULL ) return;
    ((LSQ_Iterator*)(iterator))->index += shift;

}
/* Функция, устанавливающая итератор на элемент с указанным номером */
void LSQ_SetPosition(LSQ_IteratorT iterator, LSQ_IntegerIndexT pos){///Конец итератарам
    if(iterator ==  NULL ) return;
    ((LSQ_Iterator*)(iterator))->index = pos;
}

void LSQ_UpdateCapacity(LSQ_HandleT handle){
    ((LSQ*)handle)->size++;
    if(((LSQ*)handle)->cap < ((LSQ*)handle)->size){
        if(((LSQ*)handle)->cap == 0){
            ((LSQ*)handle)->cap = 1;
        }
        ((LSQ*)handle)->head = (LSQ_BaseTypeT*)(realloc(((LSQ*)handle)->head,sizeof(LSQ_BaseTypeT) * 2 * ((LSQ*)handle)->cap));
        ((LSQ*)handle)->cap *= 2;
    }
}
/* Функция, добавляющая элемент в начало контейнера */
void LSQ_InsertFrontElement(LSQ_HandleT handle, LSQ_BaseTypeT element){
    if(handle == LSQ_HandleInvalid) return;
    LSQ_UpdateCapacity(handle);
    memmove( ((LSQ*)handle)->head+1, ((LSQ*)handle)->head,((LSQ*)handle)->size * sizeof(LSQ_BaseTypeT));
    *(((LSQ*)(handle))->head) = element;
}
/* Функция, добавляющая элемент в конец контейнера */
void LSQ_InsertRearElement(LSQ_HandleT handle, LSQ_BaseTypeT element){
    if(handle == LSQ_HandleInvalid) return;
    LSQ_UpdateCapacity(handle);
    *(((LSQ*)(handle))->head + ((LSQ*)handle)->size - 1 ) = element;
}
/* Функция, добавляющая элемент в контейнер на позицию, указываемую в данный момент итератором.         */
/* Элемент, на который указывает итератор, а также все последующие, сдвигается на одну позицию в конец. */
/* Заданный итератор продолжает указывать на элемент последовательности с тем же индексом.              */
void LSQ_InsertElementBeforeGiven(LSQ_IteratorT iterator, LSQ_BaseTypeT newElement){
    if(iterator ==  NULL ) return;
    LSQ_UpdateCapacity(((LSQ_Iterator*)iterator)->handle);
    memmove( ((LSQ_Iterator*)(iterator))->handle->head+((LSQ_Iterator*)(iterator))->index+1,
            ((LSQ_Iterator*)(iterator))->handle->head+((LSQ_Iterator*)(iterator))->index,
            (((LSQ_Iterator*)iterator)->handle->size - ((LSQ_Iterator*)(iterator))->index - 1)* sizeof (LSQ_BaseTypeT));

    *(((LSQ_Iterator*)iterator)->handle->head + ((LSQ_Iterator*)(iterator))->index) = newElement;


}

/* Функция, удаляющая первый элемент контейнера */
void LSQ_DeleteFrontElement(LSQ_HandleT handle){
    if(handle == LSQ_HandleInvalid) return;
    if(((LSQ*)handle)->size < 1) return;
    ((LSQ*)(handle))->size--;
    memmove( ((LSQ*)handle)->head+1, ((LSQ*)handle)->head,((LSQ*)handle)->size * sizeof(LSQ_BaseTypeT));


}
/* Функция, удаляющая последний элемент контейнера */
void LSQ_DeleteRearElement(LSQ_HandleT handle){
    if(handle == LSQ_HandleInvalid) return;
    if(((LSQ*)handle)->size < 1) return;
    ((LSQ*)(handle))->size--;

}
/* Функция, удаляющая элемент контейнера, указываемый заданным итератором.                 */
/* Все последующие элементы смещаются на одну позицию в сторону начала.                    */
/* Заданный итератор продолжает указывать на элемент последовательности с тем же индексом. */
void LSQ_DeleteGivenElement(LSQ_IteratorT iterator){
    if(iterator ==  NULL ) return;
    if(((LSQ_Iterator*)iterator)->handle->size < 1) return;
    ((LSQ_Iterator*)iterator)->handle->size--;
    LSQ_UpdateCapacity(((LSQ_Iterator*)iterator)->handle);
    memmove( ((LSQ_Iterator*)(iterator))->handle->head+((LSQ_Iterator*)(iterator))->index,
             ((LSQ_Iterator*)(iterator))->handle->head+((LSQ_Iterator*)(iterator))->index+1,
             (((LSQ_Iterator*)iterator)->handle->size - ((LSQ_Iterator*)(iterator))->index - 1)* sizeof (LSQ_BaseTypeT));



}









