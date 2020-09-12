[ORG 0x00]          
[BITS 16]          

SECTION .text       ;

jmp 0x07C0:START    ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;   MINT64 OS
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
TOTALSECTORCOUNT:   dw  0x02    ; ��Ʈ �δ��� ������ MINT64 OS �̹����� ũ��
                                ; �ִ� 1152 ����(0x90000byte)���� ����
KERNEL32SECTORCOUNT: dw 0x02    ; ��ȣ ��� Ŀ���� �� ���� ��

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
START:
    mov ax, 0x07C0  
    mov ds, ax      
    mov ax, 0xB800  
    mov es, ax      
    mov ax, 0x0000  
    mov ss, ax      
    mov sp, 0xFFFE  
    mov bp, 0xFFFE  

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.SCREENCLEARLOOP:                   
    mov byte [ es: si ], 0          
                                   
    mov byte [ es: si + 1 ], 0x0A   

   add si, 2                       

    cmp si, 80 * 25 * 2     
                            
    jl .SCREENCLEARLOOP     
                         
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    push IMAGELOADINGMESSAGE              
    push 0                                      
    push 0                                        
    call PRINTMESSAGE                                  
    add  sp, 6                                        
        
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
RESETDISK:                        
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; BIOS Reset Function 
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    mov ax, 0
    mov dl, 0              
    int 0x13     
 
    jc  HANDLEDISKERROR
        
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   
    mov si, 0x1000                 
                                    
    mov es, si                      
    mov bx, 0x0000                 

    mov di, word [ TOTALSECTORCOUNT ] 

READDATA:                           
    cmp di, 0               
    je  READEND            
    sub di, 0x1            
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; BIOS Read Function ȣ��
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    mov ah, 0x02                        
    mov al, 0x1                         
    mov ch, byte [ TRACKNUMBER ]       
    mov cl, byte [ SECTORNUMBER ]       
    mov dh, byte [ HEADNUMBER ]         
    mov dl, 0x00                        
    int 0x13                            
    jc HANDLEDISKERROR               
    
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    add si, 0x0020     
    mov es, si          
    mov al, byte [ SECTORNUMBER ]       
    add al, 0x01                        
    mov byte [ SECTORNUMBER ], al       
    cmp al, 19                        
    jl READDATA                 
        
    xor byte [ HEADNUMBER ], 0x01       
    mov byte [ SECTORNUMBER ], 0x01     
    cmp byte [ HEADNUMBER ], 0x00       
    jne READDATA                        
    
    add byte [ TRACKNUMBER ], 0x01      
    jmp READDATA                        
READEND:
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    push LOADINGCOMPLETEMESSAGE     
    push 0                         
    push 20                        
    call PRINTMESSAGE             
    add  sp, 6                    

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  call do_e820
  mov ecx , ebp
  push ecx

    ;jmp $
  jmp 0x1000:0x0000

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

  
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


HANDLEDISKERROR:
    push DISKERRORMESSAGE  
    push 0                 
    push 20                
    call PRINTMESSAGE       
    jmp $                  
PRINTMESSAGE:
    push bp         
    mov bp, sp      
    push es        
    push si        
    push di         
    push ax
    push cx
    push dx
    
   
    mov ax, 0xB800            
                               
    mov es, ax                 
    
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   
    mov ax, word [ bp + 6 ]    
    mov si, 160                
    mul si                     
    mov di, ax                 
    mov ax, word [ bp + 4 ]     
    mov si, 2                   
    mul si                     
    add di, ax                  
        
    mov si, word [ bp + 8 ]     

.MESSAGELOOP:               
    mov cl, byte [ si ]     
    
    cmp cl, 0               
    je .MESSAGEEND          

    mov byte [ es: di ], cl 
    
    add si, 1              
    add di, 2               
                            

    jmp .MESSAGELOOP       

.MESSAGEEND:
    pop dx      
    pop cx     
    pop ax      
    pop di     
    pop si      
    pop es
    pop bp      
    ret         


 do_e820:
  mov di,0x8004
  xor ebx, ebx    ; ebx must be 0 to start
  xor bp, bp    ; keep an entry count in bp
  mov edx, 0x0534D4150  ; Place "SMAP" into edx
  mov eax, 0xe820
  mov [es:di + 20], dword 1  ; force a valid ACPI 3.X entry
  mov ecx, 24    ; ask for 24 bytes
  int 0x15
  jc short .failed  ; carry set on first call means "unsupported function"
  mov edx, 0x0534D4150  ; Some BIOSes apparently trash this register?
  cmp eax, edx    ; on success, eax must have been reset to "SMAP"
  jne short .failed
  test ebx, ebx    ; ebx = 0 implies list is only 1 entry long (worthless)
  je short .failed
  jmp short .jmpin
.e820lp:
  mov eax, 0xe820    ; eax, ecx get trashed on every int 0x15 call
  mov [es:di + 20], dword 1  ; force a valid ACPI 3.X entry
  mov ecx, 24    ; ask for 24 bytes again
  int 0x15
  jc short .e820f    ; carry set means "end of list already reached"
  mov edx, 0x0534D4150  ; repair potentially trashed register
.jmpin:
  jcxz .skipent    ; skip any 0 length entries
  cmp cl, 20    ; got a 24 byte ACPI 3.X response?
  jbe short .notext
  test byte [es:di + 20], 1  ; if so: is the "ignore this data" bit clear?
  je short .skipent
.notext:
  mov ecx, [es:di + 8]  ; get lower uint32_t of memory region length
  or ecx, [es:di + 12]  ; "or" it with upper uint32_t to test for zero
  jz .skipent    ; if length uint64_t is 0, skip entry
  add ebp,[es:di + 8]
  add di, 24
.skipent:
  test ebx, ebx    ; if ebx resets to 0, list is complete
  jne short .e820lp
.e820f:
  clc      ; there is "jc" on end of list to this point, so the carry must be cleared
  ret
.failed:
  stc      ; "function unsupported" error exit
  ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

DISKERRORMESSAGE:       db  'DISK Error~!!', 0
IMAGELOADINGMESSAGE:    db  'OS Image Loading...', 0
LOADINGCOMPLETEMESSAGE: db  'Complete~!!', 0


SECTORNUMBER:           db  0x02    
HEADNUMBER:             db  0x00    
TRACKNUMBER:            db  0x00    

     
times 510 - ( $ - $$ )    db    0x00    

db 0x55             
db 0xAA            
                 
