#include "file.h"

R_charset TXT_getcharset(FILE *f)
{
    char line[30];
    if (fgets(line, 30, f) == NULL)
    {
        error = true;
    }
    if (strcmp(line, "ascii\n") == 0)
        return ascii;
    if (strcmp(line, "windows-1252\n") == 0)
        return win1252;
    if (strcmp(line, "windows-1250\n") == 0)
        return win1250;
    if (strcmp(line, "latin2\n") == 0)
        return latin2;
    if (strcmp(line, "utf8\n") == 0)
        return utf8;
    return undefined_charset;
}

Result *TXT_line(char *line, R_charset charset, R_list listtype)
{
    int i = 0, k = 0, check = 0;

    int multiplier = 1;
    if (charset == utf8)
        multiplier = 3;
    char cpos[namesize] = {0};
    char category[namesize] = {0};
    char surname[namesize * 3] = {0};
    char vorname[namesize * 3] = {0};
    char club[namesize * 3] = {0};
    char ct[namesize] = {0};
    int pos;
    int t, h, m, s;

    while (line[i] != '\n' && line[i] != 0)
    {
        if (line[i] == '\t')
        {
            check++;
            if (listtype == start && check == 1)
                check++;
            k = 0;
        }
        else
        {
            if (k >= namesize * multiplier)
            {
                error = true;
            }
            if (check > 5)
            {
                error = true;
            }
            switch (check)
            {
            case 0:
                category[k++] = line[i];
                break;
            case 1:
                cpos[k++] = line[i];
                break;
            case 2:
                surname[k++] = line[i];
                break;
            case 3:
                vorname[k++] = line[i];
                break;
            case 4:
                club[k++] = line[i];
                break;
            case 5:
                ct[k++] = line[i];
                break;
            }
        }
        i++;
    }
    if (check != 5)
    {
        error = true;
    }

    Result *rnew = malloc(sizeof(Result));
    if (rnew == NULL)
    {
        error = true;
    }
    if (listtype == result)
    {
        sscanf(cpos, "%d", &pos);
        rnew->pos = (atomic_int)pos;
    }
    else
    {
        rnew->pos = (atomic_int)998;
    }
    if (sscanf(ct, "%d:%d:%d", &h, &m, &s) == 3)
        t = h * 60 * 60 + m * 60 + s;
    else if (sscanf(ct, "%d:%d", &m, &s) == 2)
        t = m * 60 + s;
    else
        t = -1;
    if (listtype == result)
        rnew->t = (atomic_int)t;
    else if (listtype == start)
        rnew->starttime = (atomic_int)t;
    char_2_char_a(category, rnew->category);

    if (charset == latin2)
    {
        latin2_2_unicode_a((unsigned char *)surname, rnew->surname);
        latin2_2_unicode_a((unsigned char *)vorname, rnew->vorname);
        latin2_2_unicode_a((unsigned char *)club, rnew->club);
        return rnew;
    }
    else if (charset == win1250)
    {
        win1250_2_unicode_a((unsigned char *)surname, rnew->surname);
        win1250_2_unicode_a((unsigned char *)vorname, rnew->vorname);
        win1250_2_unicode_a((unsigned char *)club, rnew->club);
        return rnew;
    }
    else if (charset == utf8)
    {
        utf8_2_unicode_a(surname, rnew->surname);
        utf8_2_unicode_a(vorname, rnew->vorname);
        utf8_2_unicode_a(club, rnew->club);
        return rnew;
    }
    else if (charset == win1252)
    {
        win1252_2_unicode_a((unsigned char *)surname, rnew->surname);
        win1252_2_unicode_a((unsigned char *)vorname, rnew->vorname);
        win1252_2_unicode_a((unsigned char *)club, rnew->club);
        return rnew;
    }
    ascii_2_unicode_a(surname, rnew->surname);
    ascii_2_unicode_a(vorname, rnew->vorname);
    ascii_2_unicode_a(club, rnew->club);
    return rnew;
}

void TXT_read(FILE *f, Category **list, R_list listtype)
{
    int size = 6 * (namesize + 1);
    if (f == NULL)
    {
        error = true;
    }
    R_charset charset = TXT_getcharset(f);
    Result *r, *r_exist;
    int i;
    char line[6 * (namesize + 1)];
    while (fgets(line, size, f) != NULL)
    {
        r = TXT_line(line, charset, listtype);
        r_exist = R_result_search(*(list), r, 0);
        if (r_exist != NULL)
        {
            if (listtype == result)
            {
                r_exist->pos = r->pos;
                r_exist->t = r->t;
            }
            else if (listtype == start)
            {
                r_exist->starttime = r->starttime;
            }
            free(r);
            r_exist->updated = time(0);
        }
        else
        {
            Category *c = NULL;
            c = R_category_find_a(*list, r->category);
            if (c == NULL)
            {
                c = R_category_new(list);
                for (i = 0; r->category[i] != 0; i++)
                    c->name[i] = r->category[i];
            }
            r->updated = time(0);
            R_result_add(c, r, listtype);
        }
    }
}

//IOF
void IOF_read(FILE *f, Category **list, R_list listtype, R_charset charset)
{
    int size = 6 * (namesize + 1);
    if (f == NULL)
    {
        error = true;
    }
    char line[6 * (namesize + 1)];
    char ShortName[12] = "<ShortName>";
    int SN_l = 11;
    char Family[9] = "<Family>";
    int F_l = 8;
    char Given[8] = "<Given>";
    int G_l = 7;
    //char Name[7] = "<Name>";
    //int N_l = 6;
    char StartTime[12] = "<StartTime>";
    int ST_l = 11;
    char Time[7] = "<Time>";
    int T_l = 6;
    char Position[11] = "<Position>";
    int P_l = 10;
    char Status[9] = "<Status>";
    int S_l = 8;
    //char Organisation[15] = "<Organisation>";
    //char Organisation_end[15] = "</Organisation>";
    bool find = false;
    bool results = false;

    char cpos[namesize] = {0};
    char category[namesize] = {0};
    char surname[namesize * 3] = {0};
    char vorname[namesize * 3] = {0};
    char club[namesize * 3] = {0};
    char ct[namesize] = {0};
    char cstatus[namesize] = {0};
    int pos;
    int t, h, m, s;

    while (fgets(line, size, f) != NULL)
    {
        int i;
        if (strncmp(line, ShortName, SN_l) == 0)
        {
            if (results)
            {
                for (i = 0; (line[SN_l + i] != '<' && i < namesize - 1); i++)
                    club[i] = line[SN_l + i];
            }
            else
            {
                memset(category, 0, namesize);
                for (i = 0; (line[SN_l + i] != '<' && i < namesize - 1); i++)
                    category[i] = line[SN_l + i];
            }
        }
        else if (strncmp(line, Family, F_l) == 0)
        {
            for (i = 0; (line[F_l + i] != '<' && i < namesize - 1); i++)
                surname[i] = line[F_l + i];
            results = true;
        }
        else if (strncmp(line, Given, G_l) == 0)
        {
            for (i = 0; (line[G_l + i] != '<' && i < namesize - 1); i++)
                vorname[i] = line[G_l + i];
        }
        else if (listtype == result && strncmp(line, Time, T_l) == 0)
        {
            for (i = 0; (line[T_l + i] != '<' && line[T_l + i] != '.' && i < namesize - 1); i++)
                ct[i] = line[T_l + i];
        }
        else if (listtype == start && strncmp(line, StartTime, ST_l) == 0)
        {
            bool T = false;
            for (i = 0; (line[ST_l + i] != '<' && line[ST_l + i] != '.' && i < namesize - 1); i++)
            {
                if (T)
                    ct[i] = line[ST_l + i];
                if (line[ST_l + i] == 'T')
                    T = true;
            }
        }
        else if (strncmp(line, Position, P_l) == 0)
        {
            for (i = 0; (line[P_l + i] != '<' && i < namesize - 1); i++)
                cpos[i] = line[P_l + i];
        }
        else if (strncmp(line, Status, S_l) == 0)
        {
            for (i = 0; (line[S_l + i] != '<' && i < namesize - 1); i++)
                cstatus[i] = line[S_l + i];
            find = true;
            results = false;
        }

        if (find)
        {
            Result *rnew = R_new();
            if (rnew == NULL)
            {
                error = true;
            }
            if(listtype==start){
            if (sscanf(ct, "%d:%d:%d", &h, &m, &s) == 3)
                t = h * 60 * 60 + m * 60 + s;
            else if (sscanf(ct, "%d:%d", &m, &s) == 2)
                t = m * 60 + s;
            else
                t = -1;
            }else{
                sscanf(ct,"%d",&t);
            }

            if (listtype == start)
            {
                rnew->pos = 996;
                rnew->starttime = (atomic_int)t;
            }
            else if (listtype == result)
            {
                rnew->t = (atomic_int)t;
                if (cpos[0] == 0 && cstatus[0] != 'O')
                    pos = 998;
                else if (cpos[0] == 0 && cstatus[0] == 'O')
                    pos = 999;
                else
                    sscanf(cpos, "%d", &pos);
                if (cpos[0] == 'V' && cpos[1] == 'k')
                    pos = 997;
                rnew->pos = (atomic_int)pos;
            }

            char_2_char_a(category, rnew->category);
            if (charset == latin2)
            {
                latin2_2_unicode_a((unsigned char *)surname, rnew->surname);
                latin2_2_unicode_a((unsigned char *)vorname, rnew->vorname);
                latin2_2_unicode_a((unsigned char *)club, rnew->club);
            }
            else if (charset == win1250)
            {
                win1250_2_unicode_a((unsigned char *)surname, rnew->surname);
                win1250_2_unicode_a((unsigned char *)vorname, rnew->vorname);
                win1250_2_unicode_a((unsigned char *)club, rnew->club);
            }
            else if (charset == utf8)
            {
                utf8_2_unicode_a(surname, rnew->surname);
                utf8_2_unicode_a(vorname, rnew->vorname);
                utf8_2_unicode_a(club, rnew->club);
            }
            else if (charset == win1252)
            {
                win1252_2_unicode_a((unsigned char *)surname, rnew->surname);
                win1252_2_unicode_a((unsigned char *)vorname, rnew->vorname);
                win1252_2_unicode_a((unsigned char *)club, rnew->club);
            }
            else
            {
                ascii_2_unicode_a(surname, rnew->surname);
                ascii_2_unicode_a(vorname, rnew->vorname);
                ascii_2_unicode_a(club, rnew->club);
            }

            Result *r_exist = R_result_search(*(list), rnew, 0);
            if (r_exist != NULL)
            {
                if (listtype == start)
                {
                    r_exist->starttime = rnew->starttime;
                }
                else if (listtype == result)
                {
                    r_exist->pos = rnew->pos;
                    r_exist->t = rnew->t;
                }
                r_exist->updated = time(0);
                free(rnew);
            }
            else
            {
                Category *c = NULL;
                c = R_category_find_a(*list, rnew->category);
                if (c == NULL)
                {
                    c = R_category_new(list);
                    for (int i = 0; rnew->category[i] != 0; i++)
                        c->name[i] = rnew->category[i];
                }
                rnew->updated = time(0);
                R_result_add(c, rnew, result);
            }

            memset(cpos, 0, namesize);
            memset(surname, 0, namesize * 3);
            memset(vorname, 0, namesize * 3);
            memset(club, 0, namesize * 3);
            memset(ct, 0, namesize);
            memset(cstatus, 0, namesize);
            pos = 0;
            t = h = m = s = 0;
            find = false;
        }
    }
}

Result *CSV_line(char *line, R_charset charset, R_list listtype)
{
    int i = 0, j = 0, k = 0;

    char cpos[namesize] = {0};
    char category[namesize] = {0};
    char surname[namesize * 3] = {0};
    char vorname[namesize * 3] = {0};
    char club[namesize * 3] = {0};
    char ct[namesize] = {0};
    int pos = 0;
    char ce[namesize] = {0};
    int t, h, m, s;

    while (line[i] != '\n' && line[i] != 0)
    {
        if (line[i] == ';')
        {
            k++;
            if (k == 20 && club[0] != 0)
            {
                club[j++] = ' ';
            }
            else
            {
                j = 0;
            }
        }
        else
        {
            switch (k)
            {
            case 5:
                if (line[i] != '\"')
                    surname[j++] = line[i];
                break;
            case 6:
                if (line[i] != '\"')
                    vorname[j++] = line[i];
                break;
            case 19:
                if (line[i] != '\"')
                    club[j++] = line[i];
                break;
            case 20:
                if (line[i] != '\"')
                    club[j++] = line[i];
                break;
            case 13:
                if (line[i] != '\"' && listtype == result)
                    ct[j++] = line[i];
                break;
            case 14:
                if (line[i] != '\"' && listtype == result)
                    ce[j++] = line[i];
                break;
            case 11:
                if (line[i] != '\"' && listtype == start)
                    ct[j++] = line[i];
                break;
            case 25:
                if (line[i] != '\"')
                    category[j++] = line[i];
                break;
            case 57:
                if (line[i] != '\"' && listtype == result)
                    cpos[j++] = line[i];
                break;
            default:
                break;
            }
        }
        i++;
    }
    Result *rnew = R_new();
    if (rnew == NULL)
    {
        error = true;
        return NULL;
    }

    if (sscanf(ct, "%d:%d:%d", &h, &m, &s) == 3)
        t = h * 60 * 60 + m * 60 + s;
    else if (sscanf(ct, "%d:%d", &m, &s) == 2)
        t = m * 60 + s;
    else
        t = -1;

    if (listtype == start)
    {
        rnew->pos = 996;
        rnew->starttime = (atomic_int)t;
    }
    else if (listtype == result)
    {
        rnew->t = (atomic_int)t;
        if (cpos[0] == 0 && ce[0] == 0)
            pos = 998;
        else if (cpos[0] == 0 && ce[0] != 0)
            pos = 999;
        else
            sscanf(cpos, "%d", &pos);
        if (cpos[0] == 'V' && cpos[1] == 'k')
            pos = 997;
        rnew->pos = (atomic_int)pos;
    }

    char_2_char_a(category, rnew->category);

    if (charset == latin2)
    {
        latin2_2_unicode_a((unsigned char *)surname, rnew->surname);
        latin2_2_unicode_a((unsigned char *)vorname, rnew->vorname);
        latin2_2_unicode_a((unsigned char *)club, rnew->club);
        return rnew;
    }
    else if (charset == win1250)
    {
        win1250_2_unicode_a((unsigned char *)surname, rnew->surname);
        win1250_2_unicode_a((unsigned char *)vorname, rnew->vorname);
        win1250_2_unicode_a((unsigned char *)club, rnew->club);
        return rnew;
    }
    else if (charset == utf8)
    {
        utf8_2_unicode_a(surname, rnew->surname);
        utf8_2_unicode_a(vorname, rnew->vorname);
        utf8_2_unicode_a(club, rnew->club);
        return rnew;
    }
    else if (charset == win1252)
    {
        win1252_2_unicode_a((unsigned char *)surname, rnew->surname);
        win1252_2_unicode_a((unsigned char *)vorname, rnew->vorname);
        win1252_2_unicode_a((unsigned char *)club, rnew->club);
        return rnew;
    }
    ascii_2_unicode_a(surname, rnew->surname);
    ascii_2_unicode_a(vorname, rnew->vorname);
    ascii_2_unicode_a(club, rnew->club);
    return rnew;
}

void CSV_read(FILE *f, Category **list, R_list listtype, R_charset charset)
{
    const int size = 2000;
    if (f == NULL)
    {
        error = true;
    }
    Result *r = NULL, *r_exist = NULL;
    int i;
    char *line = calloc(size, sizeof(char));
    fgets(line, size, f);
    while (fgets(line, size, f) != NULL)
    {
        r = CSV_line(line, charset, listtype);
        r_exist = R_result_search(*(list), r, 0);
        if (r_exist != NULL)
        {
            if (listtype == start)
            {
                r_exist->starttime = r->starttime;
            }
            else if (listtype == result)
            {
                r_exist->pos = r->pos;
                r_exist->t = r->t;
            }
            r_exist->updated = time(0);
            free(r);
        }
        else
        {
            Category *c = NULL;
            c = R_category_find_a(*list, r->category);
            if (c == NULL)
            {
                c = R_category_new(list);
                for (i = 0; r->category[i] != 0; i++)
                    c->name[i] = r->category[i];
            }
            r->updated = time(0);
            R_result_add(c, r, result);
        }
    }
    free(line);
}

void *readerthread(void *arg)
{
    struct thread_arg *set = arg;
    FILE *fp;
    clock_t timestart;
    //bool first = false;
    while (true)
    {
        timestart = clock();
        fp = fopen(set->filename, "rt");
        if (fp != NULL)
        {
            if (set->format == txt)
            {
                TXT_read(fp, set->list, set->type);
            }
            else if (set->format == csv)
            {
                CSV_read(fp, set->list, set->type, set->charset);
            }
            else if (set->format == xml)
            {
                IOF_read(fp, set->list, set->type, set->charset);
            }
            fclose(fp);
        }
        deleteAFK(*(set->list));
        if (set->type == result)
            R_result_sort(*(set->list));

        timestart = set->refreshrate - (clock() - timestart) / CLOCKS_PER_SEC;
        if (timestart > 0) // && first)
        {
#ifdef _WIN32
            Sleep(timestart * 1000);
#else
            sleep(timestart * 1000);
#endif
        }
        //if(first == false)first = true;
    }
    pthread_exit(NULL);
}