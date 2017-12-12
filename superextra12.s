DEF LD  0x80                ; LED adatregiszter (�rhat�/olvashat�) 

CODE
start:
    mov r0, #70 ; A kisz�moland� faktori�lis, max 146
    mov r1, #0 ; Faktori�lis sz�ml�l� 0-�z�sa
    mov r2, #255 ; Az utols� sz�mjegyre mutat� regiszter
    mov r6, #0 ; Az als� carry byte null�z�sa
    mov r7, #0 ; A felso carry byte null�z�sa
    mov r15, #127 ; A legutols� mem�riahelyre mutat� regiszter
    mov r14, #1 ; A bet�ltendo 1 �rt�k az utols� helyre
    mov (r15), r14 ; 1 bet�lt�se az utols� helyre
    cmp r0, r1 ; Ha a kisz�moland� faktori�lis 1
    jz end ; akkor v�ge a sz�ml�l�snak
    
factorial:
    add r1, #1 ; A faktori�lis sz�ml�l� n�vel�e
    mov r3, #0 ; A (-1)-edik felso mem�riahelyre
    ;mutat� regiszter

digit:
    sub r3, #1 ; A mem�riahelyre mutat� pointer cs�kkent�se
    mov r13, r3 ; �s m�solat k�sz�t�se r13-ba
    sr0 r13 ; Mem�ri�ra mutat� regiszter �s 
            ; als� vagy felso BCD-re utal� 
            ; carry bit k�pz�se
    mov r5, (r13) ; r13-ban l�vo mem�ria bet�lt�se r5-be
    jc firstload ; Ha nem az als� BCD-re van sz�ks�g
    swp r5 ; akkor megcser�li az als� �s felso 4 bitet
firstload:
    and r5, #0x0F ; Als� 4 bit maszkol�sa
    mov r12, r1 ; A szorz�s egyik t�nyezoj�nek m�sol�sa
    cmp r5, #0x00 ; Ellenorzi, hogy a szorzand� 0-a  *
    jz multipl_end ; ha igen akkor v�ge a szorz�snak   *
multipl_loop:
    add r6, r5 ; Als� carry byte n�vel�se r5-el
    adc r7, #0 ; Felso carry byte n�vel�se a carry flag-el
    sub r12, #1 ; A szorz�t�nyezo cs�kkent�se
    jnz multipl_loop ; Am�g nem nulla �jra
multipl_end:
    jsr divmod ; Oszt� subrutin megh�v�sa
    mov r13, r3 ; A mem�ri�b�l bet�lt�tt regiszter 
    ; m�sol�sa r13-ba
    sr0 r13 ; Mem�ri�ra mutat� regiszter �s 
            ; als� vagy felso BCD-re utal� 
            ; carry bit k�pz�se
    mov r5, (r13) ; r13 �ltal mutatott mem�ria bet�lt�se r5-be
    jc firstsave ; Ha az als� 4 bitre utal� carry
    swp r10 ; nem 1, akkor megcser�li az als� �s felso
            ; 4 bitet az �j �rt�kben amit a mem�ri�ba
            ; akarunk helyezni
    and r5, #0x0F ; Maszkol�ssal 0-�zza r10 hely�t, hogy a m�sik
                  ; 4 biten l�vo �rt�k ne v�ltozzon
    jmp secondsave ; Ez csak az�rt kell, hogy ne maszkolja a
                    ; a felso 4 bitet is
firstsave: 
    and r5, #0xF0 ; Maszkol�ssal 0-�zza r10 hely�t, hogy a m�sik
                  ; 4 biten l�vo �rt�k ne v�ltozzon
secondsave:
    add r5, r10 ; Az �j �rt�k hozz�ad�sa r5-h�z, amivel nem v�ltoztatja
                ; a nem az adott BCD-re mutatott �rt�ket
    mov (r13), r5 ; Az �j �rt�k ment�se a mem�ri�ba
    cmp r3, r2 ; Ellenorzi, hogy az utols� sz�mjegyn�l tart-e
    jnz digit ; �s ha nem akkor ism�tli
carry:
    mov r14, r6 ; als� carry byte m�sol�sa r14-be
    add r14, r7 ; felso carry byte hozz�ad�sa r14-hez
    jn carry_c ; Ha t�lcsordul folytassa a folyamatot
    jz carryend ; Ha nem csordul t�l �s 0 akkor v�ge a folyamatnak
carry_c:
    sub r2, #1 ; Az utols� sz�mjegy �rt�k�nek cs�kkent�se
    jsr divmod ; Az oszt� subrutin megh�v�sa
    mov r13, r2 ; r2 m�sol�sa r13-ba
    sr0 r13 ; Mem�ri�ra mutat� regiszter �s 
            ; als� vagy felso BCD-re utal� 
            ; carry bit k�pz�se
    mov r5, (r13) ; Bet�lt�s a mem�ri�b�l r5-be
    jc carryfsave ; Ha az als� 4 bitre utal� carry
    swp r10 ; nem 1, akkor megcser�li az als� �s felso
            ; 4 bitet az �j �rt�kben amit a mem�ri�ba
            ; akarunk helyezni
    and r5, #0x0F ; Maszkol�ssal 0-�zza r10 hely�t, hogy a m�sik
                  ; 4 biten l�vo �rt�k ne v�ltozzon
    jmp carryssave ; Ez csak az�rt kell, hogy ne maszkolja a
                    ; a felso 4 bitet is
carryfsave:
    and r5, #0xF0 ; Maszkol�ssal 0-�zza r10 hely�t, hogy a m�sik
                  ; 4 biten l�vo �rt�k ne v�ltozzon
carryssave:
    add r5, r10 ; Az �j �rt�k hozz�ad�sa r5-h�z, amivel nem v�ltoztatja
                ; a nem az adott BCD-re mutatott �rt�ket
    mov (r13), r5 ; Az �j �rt�k ment�se a mem�ri�ba
    jmp carry ; Elol ellenoriz, �gy sima visszaugr�s az elej�re
carryend:
    cmp r0, r1 ; A faktor�lis sz�ml�l� megegyezik e a k�rt
    jnz factorial ; faktori�lissal, ha nem akkor �jra
end:
    mov LD, r0 ; A ki�rand� faktori�lis kijelz�se a LED-ekre
    jmp end ; End-loop
    

divmod:
    mov r8, #0 ; Oszt�s eredm�ny�nek als� byte-j�nak �s
    mov r9, #0 ; az oszt�s eredm�ny�nek felso byte-j�nak
                ; elok�sz�t�se, felt�lt�se 0-val
    mov r10, r6 ; Als� carry byte m�solata r10-be
    mov r11, r7 ; Felso carry byte m�solata r11-be
divmod_loop:
    add r8, #1 ; Az oszt�s eredm�ny�nek n�vel�se
    adc r9, #0 ; Carry flag hozz�ad�sa a felso byte-hoz
    sub r10, #10 ; 10 levon�s az osztand�b�l, 10 az oszt�
    jnv divmod_loop ; Ha nem csordul t�l �jra
divmod_end:
    sub r11, #1 ; Ha t�lcsordul cs�kkenti a felso carry byte-ot
    jnv divmod_loop ; Ha nem csordul t�l �jra
    add r10, #10 ; Mivel biztosan t�lcsordul �gy egyel vissza
    sub r8, #1 ; �s az eredm�ny cs�kkent�se 1-el
    sbc r9, #0 ; figyelembe v�ve a felso byte-ot is
    mov r6, r8 ; az oszt�s eredm�ny�nek bet�lt�se az als�
    mov r7, r9 ; �s felso carry byte-ba, az oszt�s k�sz
               ; r10 a carry modulos 10, vagyis a kello BCD
    rts ; Visszat�r�s subrutinb�l
    
