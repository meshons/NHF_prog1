/*
Result * TXT_result_char(char * line,R_charset charset){
    int szorzo = 1;
    if(charset == utf8)szorzo=3;
    int pos;
    char cpos[szorzo*namesize],category[szorzo*namesize];
    char surname[szorzo*namesize],vorname[szorzo*namesize],club[szorzo*namesize];
    int t,h,m,s;
    char ct[namesize];
    int i = 0,k=0,check=0;
    while(line[i] != '\0'){
        if(line[i] == '\t'){
            check++;
            k=0;
        }
        if(k>=szorzo*namesize){
            error=true;
            break;
        }
        if(check>5){
            error=true;
            break;
        }
        switch(check){
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
    if(check!=5){
        //error
        error=true;
    }
    Result * rnew = malloc(sizeof(result));
    if(rnew == NULL){
        error = true;
    }
    sscanf(cpos,"%d",&pos);
    rnew->pos = (atomic_int)pos;
    if(sscanf(ct,"%d:%d:%d",&h,&m,&s) == 3)
        t = h*60*60+m*60+s;
    else if(sscanf(ct,"%d:%d",&m,&s) == 2)
        t = m*60+s;
    else t = -1;
    rnew->t = (atomic_int)t;
    char_2_char_a(category,rnew->category);            
    if(charset == ascii){
        ascii_2_unicode_a(surname,rnew->surname);
        ascii_2_unicode_a(vorname,rnew->vorname);
        ascii_2_unicode_a(club,rnew->club);
        return rnew;
    }else
    if(charset == utf8){
        utf8_2_unicode_a(surname,rnew->surname);
        utf8_2_unicode_a(vorname,rnew->vorname);
        utf8_2_unicode_a(club,rnew->club);
        return rnew;        
    }
    ascii_2_unicode_a(surname,rnew->surname);
    ascii_2_unicode_a(vorname,rnew->vorname);
    ascii_2_unicode_a(club,rnew->club);
    return rnew;
}*/

/*
Result * TXT_result_unsigned_char(unsigned char * line,R_charset charset){
    int pos;
    //LOG_str((char *)line);
    char cpos[namesize] = {0}, category[namesize] = {0};
    unsigned char surname[namesize] = {0},vorname[namesize] = {0},club[namesize]  = {0};
    int t,h,m,s;
    char ct[namesize]  = {0};
    int i = 0,k=0,check=0;
    while((char)line[i] != '\n'){
        if((char)line[i] == '\t'){
            check++;
            k=0;
        }else{
        if(k>=namesize){
            error=true;
            //break;
        }
        if(check>5){
            error=true;
            //break;
        }
        switch(check){
            case 0:
                category[k++] = (char)line[i];
                break;
            case 1:
                cpos[k++] = (char)line[i];
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
                ct[k++] = (char)line[i];
                break;
        }
        }
        i++;
    }
    if(check!=5){
        //error
        error=true;
    }
    
    Result * rnew = malloc(sizeof(Result));
    if(rnew == NULL){
        error = true;
    }
    sscanf(cpos,"%d",&pos);
    rnew->pos = (atomic_int)pos;
    if(sscanf(ct,"%d:%d:%d",&h,&m,&s) == 3)
        t = h*60*60+m*60+s;
    else if(sscanf(ct,"%d:%d",&m,&s) == 2)
        t = m*60+s;
    else t = -1;
    rnew->t = (atomic_int)t;
    
    char_2_char_a(category,rnew->category);
    
    if(charset == latin2){
        latin2_2_unicode_a(surname,rnew->surname);
        latin2_2_unicode_a(vorname,rnew->vorname);
        latin2_2_unicode_a(club,rnew->club);
        return rnew;        
    }else
    if(charset == win1250){
        win1250_2_unicode_a(surname,rnew->surname);
        win1250_2_unicode_a(vorname,rnew->vorname);
        win1250_2_unicode_a(club,rnew->club);
        return rnew;        
    }
    win1252_2_unicode_a(surname,rnew->surname);
    win1252_2_unicode_a(vorname,rnew->vorname);
    win1252_2_unicode_a(club,rnew->club);
    return rnew;
}

Result * TXT_result_unsigned_short(unsigned short * line){
    int pos;
    char cpos[namesize], category[namesize];
    unsigned short surname[namesize],vorname[namesize],club[namesize];
    int t,h,m,s;
    char ct[namesize];
    int i = 0,k=0,check=0;
    while(line[i] != 0){
        if(line[i] == '\t'){
            check++;
            k=0;
        }
        if(k>=namesize){
            error=true;
            break;
        }
        if(check>5){
            error=true;
            break;
        }
        switch(check){
            case 0:
                category[k++] = line[i]%256;
                break;
            case 1:
                cpos[k++] = line[i]%256;
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
                ct[k++] = line[i]%256;
                break;
        }
    }
    if(check!=5){
        //error
        error=true;
    }
    Result * rnew = malloc(sizeof(result));
    if(rnew == NULL){
        error = true;
    }
    sscanf(cpos,"%d",&pos);
    rnew->pos = (atomic_int)pos;
    if(sscanf(ct,"%d:%d:%d",&h,&m,&s) == 3)
        t = h*60*60+m*60+s;
    else if(sscanf(ct,"%d:%d",&m,&s) == 2)
        t = m*60+s;
    else t = -1;
    rnew->t = (atomic_int)t;
    char_2_char_a(category,rnew->category);            
    unicode_2_unicode_a(surname,rnew->surname);
    unicode_2_unicode_a(vorname,rnew->vorname);
    unicode_2_unicode_a(club,rnew->club);
    return rnew;
}
*/

/*
Result * TXT_start_char(char * line,R_charset charset){
    int szorzo = 1;
    if(charset == utf8)szorzo=3;
    char category[szorzo*namesize];
    char surname[szorzo*namesize],vorname[szorzo*namesize],club[szorzo*namesize];
    int t,h,m,s;
    char ct[namesize];
    int i = 0,k=0,check=0;
    while(line[i] != '\0'){
        if(line[i] == '\t'){
            check++;
            k=0;
        }
        if(k>=szorzo*namesize){
            error=true;
            break;
        }
        if(check>4){
            error=true;
            break;
        }
        switch(check){
            case 0:
                category[k++] = line[i];
                break;
            case 1:
                surname[k++] = line[i];
                break;
            case 2:
                vorname[k++] = line[i];
                break;
            case 3:
                club[k++] = line[i];
                break;
            case 4:
                ct[k++] = line[i];
                break;
        }
    }
    if(check!=4){
        //error
        error=true;
    }
    Result * rnew = malloc(sizeof(result));
    if(rnew == NULL){
        error = true;
    }
    if(sscanf(ct,"%d:%d:%d",&h,&m,&s) == 3)
        t = h*60*60+m*60+s;
    else if(sscanf(ct,"%d:%d",&m,&s) == 2)
        t = m*60+s;
    else t = -1;
    rnew->starttime = (atomic_int)t;
    char_2_char_a(category,rnew->category);            
    if(charset == ascii){
        ascii_2_unicode_a(surname,rnew->surname);
        ascii_2_unicode_a(vorname,rnew->vorname);
        ascii_2_unicode_a(club,rnew->club);
        return rnew;
    }else
    if(charset == utf8){
        utf8_2_unicode_a(surname,rnew->surname);
        utf8_2_unicode_a(vorname,rnew->vorname);
        utf8_2_unicode_a(club,rnew->club);
        return rnew;        
    }
    ascii_2_unicode_a(surname,rnew->surname);
    ascii_2_unicode_a(vorname,rnew->vorname);
    ascii_2_unicode_a(club,rnew->club);
    return rnew;
}

Result * TXT_start_unsigned_char(unsigned char * line,R_charset charset){
    char category[namesize];
    unsigned char surname[namesize],vorname[namesize],club[namesize];
    int t,h,m,s;
    char ct[namesize];
    int i = 0,k=0,check=0;
    while(line[i] != '\0'){
        if(line[i] == '\t'){
            check++;
            k=0;
        }
        if(k>=namesize){
            error=true;
            break;
        }
        if(check>4){
            error=true;
            break;
        }
        switch(check){
            case 0:
                category[k++] = line[i];
                break;
            case 1:
                surname[k++] = line[i];
                break;
            case 2:
                vorname[k++] = line[i];
                break;
            case 3:
                club[k++] = line[i];
                break;
            case 4:
                ct[k++] = line[i];
                break;
        }
    }
    if(check!=4){
        //error
        error=true;
    }
    Result * rnew = malloc(sizeof(result));
    if(rnew == NULL){
        error = true;
    }
    if(sscanf(ct,"%d:%d:%d",&h,&m,&s) == 3)
        t = h*60*60+m*60+s;
    else if(sscanf(ct,"%d:%d",&m,&s) == 2)
        t = m*60+s;
    else t = -1;
    rnew->starttime = (atomic_int)t;
    char_2_char_a(category,rnew->category);            
    if(charset == latin2){
        latin2_2_unicode_a(surname,rnew->surname);
        latin2_2_unicode_a(vorname,rnew->vorname);
        latin2_2_unicode_a(club,rnew->club);
        return rnew;        
    }else
    if(charset == win1250){
        win1250_2_unicode_a(surname,rnew->surname);
        win1250_2_unicode_a(vorname,rnew->vorname);
        win1250_2_unicode_a(club,rnew->club);
        return rnew;        
    }else
    win1252_2_unicode_a(surname,rnew->surname);
    win1252_2_unicode_a(vorname,rnew->vorname);
    win1252_2_unicode_a(club,rnew->club);
    return rnew;
}

Result * TXT_start_unsigned_short(unsigned short * line){
    char category[namesize];
    unsigned short surname[namesize],vorname[namesize],club[namesize];
    int t,h,m,s;
    char ct[namesize];
    int i = 0,k=0,check=0;
    while(line[i] != 0){
        if(line[i] == '\t'){
            check++;
            k=0;
        }
        if(k>=namesize){
            error=true;
            break;
        }
        if(check>4){
            error=true;
            break;
        }
        switch(check){
            case 0:
                category[k++] = line[i]%256;
                break;
            case 1:
                surname[k++] = line[i];
                break;
            case 2:
                vorname[k++] = line[i];
                break;
            case 3:
                club[k++] = line[i];
                break;
            case 4:
                ct[k++] = line[i]%256;
                break;
        }
    }
    if(check!=4){
        //error
        error=true;
    }
    Result * rnew = malloc(sizeof(result));
    if(rnew == NULL){
        error = true;
    }
    if(sscanf(ct,"%d:%d:%d",&h,&m,&s) == 3)
        t = h*60*60+m*60+s;
    else if(sscanf(ct,"%d:%d",&m,&s) == 2)
        t = m*60+s;
    else t = -1;
    rnew->starttime = (atomic_int)t;
    char_2_char_a(category,rnew->category);            
    unicode_2_unicode_a(surname,rnew->surname);
    unicode_2_unicode_a(vorname,rnew->vorname);
    unicode_2_unicode_a(club,rnew->club);
    return rnew;
}

void TXT_read_startlist(FILE * f,Category ** list){
    int size = 5*namesize+5;
    R_charset charset = TXT_getcharset(f);
    Result * r, *r_exist;
    int i;    
    char line[size];
    unsigned short lineuni[size];
    ascii_2_unicode(line,lineuni);
    while(fgets(line,size,f)!=NULL){
        if(charset == ascii || charset==utf8)
            r = TXT_result_char(line,charset);
        else if(charset == win1250 || charset == win1252 || charset==latin2)
            r = TXT_result_unsigned_char((unsigned char *)line,charset);
        else if(charset == uni)
            r = TXT_result_unsigned_short(lineuni);
        else
            r = TXT_result_char(line,charset);
        r_exist = R_result_search(*list,r,0);
        if(r_exist!=NULL){
            r_exist->starttime = r->t;
            free(r);
        }else{
            Category * c = R_category_find_a(*list,r->category);
            if(c==NULL)
                c = R_category_new(list);
                for(i=0;r->category[i] != 0;i++)
                    c->name[i] = r->category[i];
                R_result_new(c,start);
        }
    }
}
*/

/*
Result *TXT_start(char *line, R_charset charset)
{
    int i = 0, k = 0, check = 0;

    int multiplier = 1;
    if (charset == utf8)
        multiplier = 3;
    char category[namesize] = {0};
    char surname[namesize * 3] = {0};
    char vorname[namesize * 3] = {0};
    char club[namesize * 3] = {0};
    char ct[namesize] = {0};
    int t, h, m, s;

    while (line[i] != '\n' && line[i] != 0)
    {
        if (line[i] == '\t')
        {
            check++;
            k = 0;
        }
        else
        {
            if (k >= namesize * multiplier)
            {
                error = true;
                //break;
            }
            if (check > 4)
            {
                error = true;
                //break;
            }
            switch (check)
            {
            case 0:
                category[k++] = line[i];
                break;
            case 1:
                surname[k++] = line[i];
                break;
            case 2:
                vorname[k++] = line[i];
                break;
            case 3:
                club[k++] = line[i];
                break;
            case 4:
                ct[k++] = line[i];
                break;
            }
        }
        i++;
    }
    if (check != 4)
    {
        //error
        error = true;
    }

    Result *rnew = malloc(sizeof(Result));
    if (rnew == NULL)
    {
        error = true;
    }
    if (sscanf(ct, "%d:%d:%d", &h, &m, &s) == 3)
        t = h * 60 * 60 + m * 60 + s;
    else if (sscanf(ct, "%d:%d", &m, &s) == 2)
        t = m * 60 + s;
    else
        t = -1;
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
}*/
//Result * TXT_start(char * line,R_charset charset);
//void TXT_read_start(FILE * f,Category ** list);

/*void TXT_read_start(FILE *f, Category **list)
{
    int size = 6 * (namesize + 1);
    if (f == NULL)
    {
        LOG_str("No start file");
        error = true;
    }
    R_charset charset = TXT_getcharset(f);
    LOG_str(R_charset_ascii(charset));
    Result *r, *r_exist;
    int i;
    char line[size];
    while (fgets(line, size, f) != NULL)
    {
        r = TXT_start(line, charset);
        r_exist = R_result_search(*(list), r, 0);
        if (r_exist != NULL)
        {
            r_exist->starttime = r->starttime;
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
            R_result_add(c, r, result);
        }
    }
}*/
