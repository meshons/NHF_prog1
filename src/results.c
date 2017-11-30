#include "results.h"

Result *R_result_new(Category *list, R_list listtype)
{
    Result *rnew = malloc(sizeof(Result));
    memset(rnew, 0, sizeof(Result));
    if (rnew == NULL)
    {
        error = true;
        return NULL;
    }
    rnew->next = NULL;

    Result *rlist = NULL;
    if (list->res == NULL)
    {
        list->res = rnew;
    }
    else
    {
        for (rlist = list->res; rlist->next != NULL; rlist = rlist->next)
            ;
        rlist->next = rnew;
    }
    if (listtype == start)
        list->all++;
    else if (listtype == result)
        list->in++;
    return rnew;
}
void R_result_add(Category *list, Result *r, R_list listtype)
{
    r->next = NULL;
    Result *rlist = NULL;
    if (list == NULL)
    {
        printf("he");
    }
    if (list->res == NULL)
    {
        list->res = r;
    }
    else
    {
        rlist = list->res;
        while (rlist->next != NULL)
        {
            rlist = rlist->next;
        }
        rlist->next = r;
    }
    if (listtype == start)
        list->all++;
    else if (listtype == result)
    {
        list->in++;
        list->all++;
    }
}

void R_result_delete(Category *list, Result *del)
{
    Result *rlist;
    list->all--;
    list->in--;
    if(list->res==del){
        list->res = del->next;
    }else{
    for (rlist = list->res; rlist->next != NULL; rlist = rlist->next)
        if (rlist->next == del)
        {
            rlist->next = del->next;
            break;
        }
    }
    free(del);
}

Category *R_category_new(Category **list)
{
    Category *cnew = malloc(sizeof(Category));
    memset(cnew, 0, sizeof(Category));
    if (cnew == NULL)
    {
        error = true;
        return NULL;
    }
    Category *c = NULL;
    if (*list == NULL)
    {
        *list = cnew;
    }
    else
    {
        for (c = *list; c->next != NULL; c = c->next)
            ;
        c->next = cnew;
    }
    cnew->in = 0;
    cnew->all = 0;
    cnew->next = NULL;
    cnew->res = NULL;
    return cnew;
}

Category *R_category_find(Category *list, char *name)
{
    Category *c;
    for (c = list; c->next != NULL; c = c->next)
        if (strcmp(name, c->name) == 0)
            return c;
    return NULL;
}

Category *R_category_find_a(Category *list, atomic_char *name)
{
    Category *c = NULL;
    int i, j;
    for (j = 0; name[j] != 0; j++)
        ;
    for (c = list; c != NULL; c = c->next)
    {
        for (i = 0; ((char)name[i] == c->name[i] && i < j); i++)
            ;
        if (i == j)
            return c;
    }
    return NULL;
}

void R_category_deleteresults(Category *list)
{
    Result *rdel = list->res, *rtmp;
    while (rdel != NULL)
    {
        rtmp = rdel->next;
        free(rdel);
        rdel = rtmp;
    }
    list->res = NULL;
}

void R_category_delete(Category **list, Category *del)
{
    Category *clist;
    R_category_deleteresults(del);
    if(*list == del){
        *list = (*list)->next;
    }{
    for (clist = *list; clist != NULL; clist = clist->next)
        if (clist->next == del)
        {
            clist->next = del->next;
            break;
        }
    }
    free(del);
}

void R_category_deleteall(Category *list)
{
    Category *tmp;
    while (list != NULL)
    {
        tmp = list->next;
        R_category_deleteresults(list);
        free(list);
        list = tmp;
    }
}

char *R_charset_ascii(R_charset charset)
{
    if (charset == uni)
        return "uni";
    if (charset == ascii)
        return "ascii";
    if (charset == win1250)
        return "windows-1250";
    if (charset == win1252)
        return "windows-1252";
    if (charset == utf8)
        return "utf-8";
    if (charset == latin2)
        return "latin2";
    return "undefined";
}

R_charset R_ascii_charset(char *str)
{
    if (strcmp(str, "uni") == 0)
        return uni;
    if (strcmp(str, "ascii") == 0)
        return ascii;
    if (strcmp(str, "windows-1250") == 0)
        return win1250;
    if (strcmp(str, "windows-1252") == 0)
        return win1252;
    if (strcmp(str, "utf-8") == 0)
        return utf8;
    if (strcmp(str, "latin2") == 0)
        return latin2;
    return undefined_charset;
}

char *R_format_ascii(R_format format)
{
    if (format == txt)
        return "txt";
    if (format == xml)
        return "iof";
    if (format == csv)
        return "csv";
    return "undefined";
}

R_format R_ascii_format(char *str)
{
    if (strcmp(str, "txt") == 0)
        return txt;
    if (strcmp(str, "iof") == 0)
        return xml;
    if (strcmp(str, "csv") == 0)
        return csv;
    return undefined_format;
}

Result *R_result_search(Category *list, Result *rel, char level)
{
    if (level == 0)
    {
        Category *c = NULL;
        c = R_category_find_a(list, rel->category);
        if (c == NULL)
            return NULL;
        Result *r;
        for (r = c->res; r != NULL; r = r->next)
        {
            if (cmp_a_ushort(rel->surname, r->surname) && cmp_a_ushort(rel->vorname, r->vorname) && cmp_a_ushort(rel->club, r->club))
            {
                return r;
            }
        }
    }
    return NULL;
}

void R_result_sort(Category *list)
{
    Category *c = NULL;
    for (c = list; c != NULL; c = c->next)
    {
        insertionSort(&(c->res));
    }
}

void insertionSort(Result **head_ref)
{
    Result *sorted = NULL;
    Result *current = *head_ref;
    while (current != NULL)
    {
        Result *next = current->next;
        sortedInsert(&sorted, current);
        current = next;
    }

    *head_ref = sorted;
}

void sortedInsert(Result **head_ref, Result *new_node)
{
    Result *current;
    if (*head_ref == NULL || (*head_ref)->pos >= new_node->pos)
    {
        new_node->next = *head_ref;
        *head_ref = new_node;
    }
    else
    {
        current = *head_ref;
        if (new_node->pos > 997)
        {
            while (current->next != NULL)
            {
                current = current->next;
            }
            new_node->next = current->next;
            current->next = new_node;
        }
        else
        {
            while (current->next != NULL &&
                   current->next->pos <= new_node->pos)
            {
                current = current->next;
            }
            new_node->next = current->next;
            current->next = new_node;
        }
    }
}

void deleteAFK(Category *list){
    Category *c = NULL;
    time_t now = time(0);
    for (c = list; c != NULL; c = c->next)
    {
        Result *r;
        for(r=c->res;r!=NULL;r=r->next){
            if((now - r->updated > 360 && r->pos!=996) || (now - r->updated > 36000 && r->pos==996)){
                R_result_delete(c,r);
            }
        }
    }
}

Result *R_new()
{
    Result *r = malloc(sizeof(Result));
    if (r == NULL)
        return NULL;

    memset(r, 0, sizeof(Result));
    return r;
}

char *R_load_cats(char *filename, int *size)
{
    int lines = 0;
    FILE *fp = fopen(filename, "rt");
    char ch;
    while (!feof(fp))
    {
        ch = fgetc(fp);
        if (ch == '\n')
        {
            lines++;
        }
    }
    lines++;
    printf("%d", lines);
    fclose(fp);
    char *cats = malloc(sizeof(char) * lines * namesize);
    memset(cats, 0, sizeof(char) * lines * namesize);
    if (cats == NULL)
    {
        return NULL;
    }
    fp = fopen(filename, "rt");

    if (fp == NULL)
        return NULL;
    int i = 0;
    bool run = true;
    while (run)
    {
        if (fgets(cats + i * namesize, namesize, fp) != NULL)
        {
            i++;
        }
        if (lines == i + 1)
            run = false;
    }
    for (i = 0; i < lines * namesize; i++)
        if (*(cats + i) == '\n')
            *(cats + i) = 0;

    *size = lines;
    fclose(fp);
    return cats;
}

int pow_dec(int n)
{
    int i, sum = 1;
    for (i = 0; i < n; i++)
        sum = sum * 10;
    return sum;
}
