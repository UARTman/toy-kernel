// Check A20 line
// Returns to caller if A20 gate is cleared.
// Continues to A20_on if A20 line is set.
// Written by Elad Ashkcenazi, modified by Anton Gusev

.section .text
.global check_a20
.type check_a20, @function
check_a20:

pusha
mov $0x112345, %edi  //odd megabyte address.
mov $0x012345, %esi  //even megabyte address.
mov %esi, (%esi)     //making sure that both addresses contain diffrent values.
mov %edi, (%edi)     //(if A20 line is cleared the two pointers would point to the address 0x012345 that would contain 0x112345 (edi))
cmpsd             //compare addresses to see if the're equivalent.
popa
jne A20_on        //if not equivalent , A20 line is set.
mov $0, %al
ret               //if equivalent , the A20 line is cleared.

A20_on:
mov $1, %al
ret