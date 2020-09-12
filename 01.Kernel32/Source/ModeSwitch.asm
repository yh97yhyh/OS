# file      ModeSwitch.asm
# date      2009/01/01
# author    kkamagui 
#           Copyright(c)2008 All rights reserved by kkamagui
# brief     ��� ��ȯ�� ���õ� �ҽ� ����

[BITS 32]               ; ������ �ڵ�� 32��Ʈ �ڵ�� ����

; C ���� ȣ���� �� �ֵ��� �̸��� ������(Export)
global kReadCPUID, kSwitchAndExecute64bitKernel

SECTION .text       ; text ����(���׸�Ʈ)�� ����

; CPUID�� ��ȯ
;   PARAM: DWORD dwEAX, DWORD* pdwEAX,* pdwEBX,* pdwECX,* pdwEDX
kReadCPUID:
    push ebp        ; ���̽� ������ ��������(EBP)�� ���ÿ� ����
    mov ebp, esp    ; ���̽� ������ ��������(EBP)�� ���� ������ ��������(ESP)�� ���� ����
    push eax        ; �Լ����� �ӽ÷� ����ϴ� �������ͷ� �Լ��� ������ �κп���
    push ebx        ; ���ÿ� ���Ե� ���� ���� ���� ������ ����
    push ecx
    push edx
    push esi

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; EAX ���������� ������ CPUID ���ɾ� ����
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    mov eax, dword [ ebp + 8 ]  ; �Ķ���� 1(dwEAX)�� EAX �������Ϳ� ����
    cpuid                       ; CPUID ���ɾ� ����
    
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; ��ȯ�� ���� �Ķ���Ϳ� ����
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; *pdwEAX
    mov esi, dword [ ebp + 12 ] ; �Ķ���� 2(pdwEAX)�� ESI �������Ϳ� ����
    mov dword [ esi ], eax      ; pdwEAX�� �������̹Ƿ� �����Ͱ� ����Ű�� ��巹����
                                ; EAX ���������� ���� ����
    ; *pdwEBX
    mov esi, dword [ ebp + 16 ] ; �Ķ���� 3(pdwEBX)�� ESI �������Ϳ� ����
    mov dword [ esi ], ebx      ; pdwEBX�� �������̹Ƿ� �����Ͱ� ����Ű�� ��巹����
                                ; EBX ���������� ���� ����

    ; *pdwECX
    mov esi, dword [ ebp + 20 ] ; �Ķ���� 4(pdwECX)�� ESI �������Ϳ� ����
    mov dword [ esi ], ecx      ; pdwECX�� �������̹Ƿ� �����Ͱ� ����Ű�� ��巹����
                                ; ECX ���������� ���� ����
                                
    ; *pdwEDX
    mov esi, dword [ ebp + 24 ] ; �Ķ���� 5(pdwEDX)�� ESI �������Ϳ� ����
    mov dword [ esi ], edx      ; pdwEDX�� �������̹Ƿ� �����Ͱ� ����Ű�� ��巹����
                                ; EDX ���������� ���� ����

    pop esi     ; �Լ����� ����� ���� ESI �������ͺ��� EBP �������ͱ����� ���ÿ�
    pop edx     ; ���Ե� ���� �̿��ؼ� ����
    pop ecx     ; ������ ���� �������� �� �����Ͱ� ���� ���� ������
    pop ebx     ; �ڷᱸ��(Last-In, First-Out)�̹Ƿ� ����(push)�� ��������
    pop eax     ; ����(pop) �ؾ� ��
    pop ebp     ; ���̽� ������ ��������(EBP) ����
    ret         ; �Լ��� ȣ���� ���� �ڵ��� ��ġ�� ����
    
; IA-32e ���� ��ȯ�ϰ� 64��Ʈ Ŀ���� ����
;   PARAM: ����
kSwitchAndExecute64bitKernel:
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; CR4 ��Ʈ�� ���������� PAE ��Ʈ�� 1�� ����
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    mov eax, cr4    ; CR4 ��Ʈ�� ���������� ���� EAX �������Ϳ� ����
    or eax, 0x20    ; PAE ��Ʈ(��Ʈ 5)�� 1�� ����
    mov cr4, eax    ; PAE ��Ʈ�� 1�� ������ ���� CR4 ��Ʈ�� �������Ϳ� ����
    
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; CR3 ��Ʈ�� �������Ϳ� PML4 ���̺��� ��巹�� �� ĳ�� Ȱ��ȭ
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    mov eax, 0x100000   ; EAX �������Ϳ� PML4 ���̺��� �����ϴ� 0x100000(1MB)�� ����
    mov cr3, eax        ; CR3 ��Ʈ�� �������Ϳ� 0x100000(1MB)�� ����
    
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; IA32_EFER.LME�� 1�� �����Ͽ� IA-32e ��带 Ȱ��ȭ
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    mov ecx, 0xC0000080 ; IA32_EFER MSR ���������� ��巹���� ����
    rdmsr               ; MSR �������͸� �б�
    
    or eax, 0x0100      ; EAX �������Ϳ� ����� IA32_EFER MSR�� ���� 32��Ʈ���� 
                        ; LME ��Ʈ(��Ʈ 8)�� 1�� ����
    wrmsr               ; MSR �������Ϳ� ����
        
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; CR0 ��Ʈ�� �������͸� NW ��Ʈ(��Ʈ 29) = 0, CD ��Ʈ(��Ʈ 30) = 0, PG ��Ʈ(��Ʈ 31) = 1��
    ; �����Ͽ� ĳ�� ��ɰ� ����¡ ����� Ȱ��ȭ
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    mov eax, cr0            ; EAX �������Ϳ� CR0 ��Ʈ�� �������͸� ����
    or eax, 0xE0000000      ; NW ��Ʈ(��Ʈ 29), CD ��Ʈ(��Ʈ 30), PG ��Ʈ(��Ʈ 31)�� ��� 1�� ���
    xor eax, 0x60000000     ; NW ��Ʈ(��Ʈ 29)�� CD ��Ʈ(��Ʈ 30)�� XOR�Ͽ� 0���� ����
   or eax, 0x10000
    mov cr0, eax            ; NW ��Ʈ = 0, CD ��Ʈ = 0, PG ��Ʈ = 1�� ������ ���� �ٽ� 
                            ; CR0 ��Ʈ�� �������Ϳ� ����
    
    jmp 0x08:0x200000   ; CS ���׸�Ʈ �����͸� IA-32e ���� �ڵ� ���׸�Ʈ ��ũ���ͷ�
                        ; ��ü�ϰ� 0x200000(2MB) ��巹���� �̵�
                            
    ; ����� ������� ����
    jmp $