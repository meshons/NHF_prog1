DEF LD  0x80                ; LED adatregiszter (írható/olvasható) 

CODE
start:
    mov r0, #70 ; A kiszámolandó faktoriális, max 146
    mov r1, #0 ; Faktoriális számláló 0-ázása
    mov r2, #255 ; Az utolsó számjegyre mutató regiszter
    mov r6, #0 ; Az alsó carry byte nullázása
    mov r7, #0 ; A felso carry byte nullázása
    mov r15, #127 ; A legutolsó memóriahelyre mutató regiszter
    mov r14, #1 ; A betöltendo 1 érték az utolsó helyre
    mov (r15), r14 ; 1 betöltése az utolsó helyre
    cmp r0, r1 ; Ha a kiszámolandó faktoriális 1
    jz end ; akkor vége a számlálásnak
    
factorial:
    add r1, #1 ; A faktoriális számláló növelée
    mov r3, #0 ; A (-1)-edik felso memóriahelyre
    ;mutató regiszter

digit:
    sub r3, #1 ; A memóriahelyre mutató pointer csökkentése
    mov r13, r3 ; és másolat készítése r13-ba
    sr0 r13 ; Memóriára mutató regiszter és 
            ; alsó vagy felso BCD-re utaló 
            ; carry bit képzése
    mov r5, (r13) ; r13-ban lévo memória betöltése r5-be
    jc firstload ; Ha nem az alsó BCD-re van szükség
    swp r5 ; akkor megcseréli az alsó és felso 4 bitet
firstload:
    and r5, #0x0F ; Alsó 4 bit maszkolása
    mov r12, r1 ; A szorzás egyik tényezojének másolása
    cmp r5, #0x00 ; Ellenorzi, hogy a szorzandó 0-a  *
    jz multipl_end ; ha igen akkor vége a szorzásnak   *
multipl_loop:
    add r6, r5 ; Alsó carry byte növelése r5-el
    adc r7, #0 ; Felso carry byte növelése a carry flag-el
    sub r12, #1 ; A szorzótényezo csökkentése
    jnz multipl_loop ; Amíg nem nulla újra
multipl_end:
    jsr divmod ; Osztó subrutin meghívása
    mov r13, r3 ; A memóriából betöltött regiszter 
    ; másolása r13-ba
    sr0 r13 ; Memóriára mutató regiszter és 
            ; alsó vagy felso BCD-re utaló 
            ; carry bit képzése
    mov r5, (r13) ; r13 által mutatott memória betöltése r5-be
    jc firstsave ; Ha az alsó 4 bitre utaló carry
    swp r10 ; nem 1, akkor megcseréli az alsó és felso
            ; 4 bitet az új értékben amit a memóriába
            ; akarunk helyezni
    and r5, #0x0F ; Maszkolással 0-ázza r10 helyét, hogy a másik
                  ; 4 biten lévo érték ne változzon
    jmp secondsave ; Ez csak azért kell, hogy ne maszkolja a
                    ; a felso 4 bitet is
firstsave: 
    and r5, #0xF0 ; Maszkolással 0-ázza r10 helyét, hogy a másik
                  ; 4 biten lévo érték ne változzon
secondsave:
    add r5, r10 ; Az új érték hozzáadása r5-höz, amivel nem változtatja
                ; a nem az adott BCD-re mutatott értéket
    mov (r13), r5 ; Az új érték mentése a memóriába
    cmp r3, r2 ; Ellenorzi, hogy az utolsó számjegynél tart-e
    jnz digit ; és ha nem akkor ismétli
carry:
    mov r14, r6 ; alsó carry byte másolása r14-be
    add r14, r7 ; felso carry byte hozzáadása r14-hez
    jn carry_c ; Ha túlcsordul folytassa a folyamatot
    jz carryend ; Ha nem csordul túl és 0 akkor vége a folyamatnak
carry_c:
    sub r2, #1 ; Az utolsó számjegy értékének csökkentése
    jsr divmod ; Az osztó subrutin meghívása
    mov r13, r2 ; r2 másolása r13-ba
    sr0 r13 ; Memóriára mutató regiszter és 
            ; alsó vagy felso BCD-re utaló 
            ; carry bit képzése
    mov r5, (r13) ; Betöltés a memóriából r5-be
    jc carryfsave ; Ha az alsó 4 bitre utaló carry
    swp r10 ; nem 1, akkor megcseréli az alsó és felso
            ; 4 bitet az új értékben amit a memóriába
            ; akarunk helyezni
    and r5, #0x0F ; Maszkolással 0-ázza r10 helyét, hogy a másik
                  ; 4 biten lévo érték ne változzon
    jmp carryssave ; Ez csak azért kell, hogy ne maszkolja a
                    ; a felso 4 bitet is
carryfsave:
    and r5, #0xF0 ; Maszkolással 0-ázza r10 helyét, hogy a másik
                  ; 4 biten lévo érték ne változzon
carryssave:
    add r5, r10 ; Az új érték hozzáadása r5-höz, amivel nem változtatja
                ; a nem az adott BCD-re mutatott értéket
    mov (r13), r5 ; Az új érték mentése a memóriába
    jmp carry ; Elol ellenoriz, így sima visszaugrás az elejére
carryend:
    cmp r0, r1 ; A faktorális számláló megegyezik e a kért
    jnz factorial ; faktoriálissal, ha nem akkor újra
end:
    mov LD, r0 ; A kiírandó faktoriális kijelzése a LED-ekre
    jmp end ; End-loop
    

divmod:
    mov r8, #0 ; Osztás eredményének alsó byte-jának és
    mov r9, #0 ; az osztás eredményének felso byte-jának
                ; elokészítése, feltöltése 0-val
    mov r10, r6 ; Alsó carry byte másolata r10-be
    mov r11, r7 ; Felso carry byte másolata r11-be
divmod_loop:
    add r8, #1 ; Az osztás eredményének növelése
    adc r9, #0 ; Carry flag hozzáadása a felso byte-hoz
    sub r10, #10 ; 10 levonás az osztandóból, 10 az osztó
    jnv divmod_loop ; Ha nem csordul túl újra
divmod_end:
    sub r11, #1 ; Ha túlcsordul csökkenti a felso carry byte-ot
    jnv divmod_loop ; Ha nem csordul túl újra
    add r10, #10 ; Mivel biztosan túlcsordul így egyel vissza
    sub r8, #1 ; és az eredmény csökkentése 1-el
    sbc r9, #0 ; figyelembe véve a felso byte-ot is
    mov r6, r8 ; az osztás eredményének betöltése az alsó
    mov r7, r9 ; és felso carry byte-ba, az osztás kész
               ; r10 a carry modulos 10, vagyis a kello BCD
    rts ; Visszatérés subrutinból
    
