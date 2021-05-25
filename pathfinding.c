#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <windows.h>


typedef struct vector2 {
    int x;
    int y;
} *vec2;

typedef struct vector2_list {
    vec2 val;
    struct vector2_list* next;
} *vec2l;


float dist(int x1, int y1, int x2, int y2) {
    return sqrt((double)pow(x1-x2,2)+pow(y1-y2,2));
}

int** init_tab(int size) {
    int** tab = malloc(sizeof(int*)*size);
    for(int i = 0 ; i < size ; i++)
        tab[i]=malloc(sizeof(int)*size);
    return tab;
}

void display_tab(int** tab, int size) {
    for(int i = 0 ; i < size ; i++) {
        for(int j = 0 ; j < size ; j++) {
            switch(tab[j][i]) {
                case 0: printf("%c ",176); break;
                case 1: printf("D "); break;
                case 2: printf("A "); break;
                case 3: printf("o "); break;
                case -1: printf("%c ", 178); break;
            }
        }
        printf("\n");
    }
}

void slow_display(vec2l path, int** board, vec2 start, vec2 end, int size, int delta_t) {
    while(path!=NULL) {
        board[path->val->x][path->val->y]=3;
        board[start->x][start->y]=1;
        board[end->x][end->y]=2;
        path = path->next;
        Sleep(delta_t);
        system("cls");
        display_tab(board, size);
    }
}

vec2 vec2i(int x, int y) {
    vec2 v = malloc(sizeof(struct vector2));
    v->x=x;
    v->y=y;
    return v;
}

bool veq(vec2 v1, vec2 v2) {
    if(v1==NULL&&v2==NULL)
        return true;
    else if(v1==NULL||v2==NULL)
        return false;
    else return (v1->x==v2->x&&v1->y==v2->y);
}

vec2 verr() {
    return vec2i(-1,-1);
}

vec2l vdisp(vec2l arr) {
    vec2l tmp = arr;
    while(tmp!=NULL) {
        printf("(%d,%d) ",tmp->val->x, tmp->val->y);
        tmp=tmp->next;
    }
    printf("\n");
    return arr;
}

int vlen(vec2l arr) {
    int len = 0;
    while(arr!=NULL) {
        arr=arr->next;
        len++;
    }
    return len;
}

bool vcontains(vec2l arr, vec2 val) {
    while(arr!=NULL) {
        if(veq(arr->val, val))
            return true;
        arr=arr->next;
    }
    return false;
}


vec2l nth(vec2l arr, int n) {
    while(n>0&&arr!=NULL) {
        arr=arr->next;
        n++;
    }
    return arr;
}
vec2l addF(vec2l arr, vec2 val) {
    vec2l first = malloc(sizeof(vec2l));
    first->val=val;
    first->next=arr;
    return first;
}
vec2l remf(vec2l arr, int length) {
    vec2l first = arr->next;
    free(arr);
    return first;
}




vec2* sortBy(vec2* vec_arr, float* d_arr, int size) {
    for(int i = 0 ; i < size ; i++)  {
        float min=d_arr[i];
        int min_ind=i;
        for(int j = i ; j < size ; j++) {
            if(d_arr[j]<min) {
                min=d_arr[j];
                min_ind = j;
            }
        }
        vec2 tmp_v=vec_arr[min_ind];
        vec_arr[min_ind]=vec_arr[i];
        vec_arr[i]=tmp_v;
        float tmp_f=d_arr[min_ind];
        d_arr[min_ind]=d_arr[i];
        d_arr[i]=tmp_f;
    }
    return vec_arr;
}

vec2l compute(int** board, vec2 pos, vec2 prev, vec2 target, vec2l visited, int brds) {            // brds: taille du tableau 
    //printf("%d %d\n",pos->x,pos->y);
    if(vcontains(visited, pos))
        return NULL;
    if(veq(pos, target))
        return addF(NULL,pos);
    if(veq(pos, verr()))
        return NULL;

    vec2* udrl = malloc(sizeof(struct vector2)*4);        // les vecteurs up, down, right, left t'as capté
    udrl[0]=pos->x>0&&board[pos->x-1][pos->y]!=-1?vec2i(pos->x-1, pos->y):vec2i(-1,-1);
    udrl[1]=pos->x<brds-1&&board[pos->x+1][pos->y]!=-1?vec2i(pos->x+1, pos->y):vec2i(-1,-1);
    udrl[2]=pos->y>0&&board[pos->x][pos->y-1]!=-1?vec2i(pos->x, pos->y-1):vec2i(-1,-1);
    udrl[3]=pos->y<brds-1&&board[pos->x][pos->y+1]!=-1?vec2i(pos->x, pos->y+1):vec2i(-1,-1);

    float* udrld = malloc(sizeof(float)*4);             // les distances des vecteurs au target
    for(int i = 0 ; i < 4 ; i++)
        udrld[i]=udrl[i]->x!=-1?dist(udrl[i]->x, udrl[i]->y, target->x, target->y):-1;

    udrl = sortBy(udrl, udrld, 4);

    
    for(int i = 0 ; i < 4 ; i++) {
        if(udrld[i]==-1||veq(udrl[i],prev))
            continue;
        vec2l res = compute(board, udrl[i], pos, target, addF(visited, pos), brds);
        if(res!=NULL)
            return addF(res,pos);
    }

    return NULL;
}


void main() {

    int size = 10;
    int** board = init_tab(size);
    for(int i = 0 ; i < size ; i++)
        for(int j = 0 ; j < size ; j++)
            board[i][j] = 0;
    for(int i = 0 ; i < 9 ; i++) {
        board[i][4]=-1;
        board[i+1][7]=-1;
    }

    vec2 start_pos = vec2i(0,0);
    vec2 target = vec2i(9,9);
    vec2l path = compute(board, start_pos, NULL, target, NULL, size);

    slow_display(path, board, start_pos, target, size, 200);

}



